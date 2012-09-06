#include "EditorWindow.h"
#include <QPixmap>
#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QFileDialog>
#include "ui_EditorWindow.h"
#include "BackgroundItem.h"
#include "EditorView.h"
#include "PatchItem.h"
#include "NinePatchItem.h"

namespace QNPE
{
EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::EditorWindow)
{
    m_ui->setupUi(this);

    m_patch = new PatchItem(QRectF());
    m_previewItem = new NinePatchItem(QPixmap(), QRectF());
    m_pixmap = new QGraphicsPixmapItem(QPixmap());
    m_editorBackground = new BackgroundItem(QRectF());

    m_editorBackground->setZValue(-1);
    m_patch->setZValue(1);

    m_editorScene.addItem(m_editorBackground);
    m_editorScene.addItem(m_patch);
    m_editorScene.addItem(m_pixmap);

    m_previewScene.addItem(m_previewItem);

    m_ui->previewView->setScene(&m_previewScene);
    m_ui->editorView->setScene(&m_editorScene);

    connect(&m_editorScene, SIGNAL(changed(QList<QRectF>)), this, SLOT(updateScenes(QList<QRectF>)));
    connect(&m_previewScene, SIGNAL(changed(QList<QRectF>)), this, SLOT(updatePreview(QList<QRectF>)));
    connect(m_ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(m_ui->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));

    restoreParemeters();
}

EditorWindow::~EditorWindow()
{
    delete m_ui;
}

void EditorWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open image"), QString(),
                       "Images (*.bmp *.png *.xpm *.jpg *.jpeg);;Android 9-patch (*.9.png)");
    if (fileName.isEmpty())
        return;

    QPixmap pixmap(fileName);
    QFileInfo file(fileName);

    if (file.completeSuffix() == "9.png")
    {
        QImage image = pixmap.toImage();
        qreal width = image.width() - 2;
        qreal height = image.height() - 2;
        bool black = false;
        QVector<qreal> rows, columns;
        QRgb pixel;
        for (int i = 1; i < image.width() - 1; i++)
        {
            pixel = image.pixel(i, 0);
            if (qAlpha(pixel) != 255)
                pixel = 0xffffffff;
            if ((qGray(pixel) > 127) && black)
            {
                columns << (i - 1) / width;
                black = false;
            }
            if ((qGray(pixel) < 128) && !black)
            {
                columns << (i - 1) / width;
                black = true;
            }
        }
        black = false;
        for (int i = 1; i < image.height() - 1; i++)
        {
            pixel = image.pixel(0, i);
            if (qAlpha(pixel) != 255)
                pixel = 0xffffffff;
            if ((qGray(pixel) > 127) && black)
            {
                rows << (i - 1) / height;
                black = false;
            }
            if ((qGray(pixel) < 128) && !black)
            {
                rows << (i - 1) / height;
                black = true;
            }
        }
        //qDebug() << rows << columns;
        pixmap = QPixmap::fromImage(image.copy(1, 1, image.width() - 2, image.height() - 2));
        m_pixmap->setPixmap(pixmap);
        m_patch->reset(m_pixmap->boundingRect(), rows, columns);
        m_previewItem->reset(pixmap, m_pixmap->boundingRect());
        m_editorBackground->setRect(m_pixmap->boundingRect());
    }
    else
    {
        m_pixmap->setPixmap(pixmap);
        m_patch->reset(m_pixmap->boundingRect());
        m_previewItem->reset(pixmap, m_pixmap->boundingRect());
        m_editorBackground->setRect(m_pixmap->boundingRect());
    }
}

void EditorWindow::saveFile()
{
    QStringList filters = QStringList() << tr("Android 9-patch (*.9.png)");
    QString filterString;
    foreach(QString filter, filters)
    {
        filterString.append(";;").append(filter);
    }
    filterString.remove(0, 2);

    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save image"), QString(),
                       filterString, &selectedFilter);
    if (fileName.isEmpty())
        return;

    QFileInfo file(fileName);

    if (selectedFilter == filters.at(0))
    {
        bool black = false;
        QImage image = m_pixmap->pixmap().toImage();
        image = image.copy(-1, -1, image.width() + 2, image.height() + 2);

        qreal step = 0.0;
        int nextIndex = 0;
        QVector<qreal> rows = m_patch->rows();
        QVector<qreal> columns = m_patch->columns();

        for (int i = 1; i < image.width() - 1; i++)
        {
            if ((nextIndex < columns.count()) && (step >= columns[nextIndex]))
            {
                nextIndex++;
                black = !black;
            }
            if (black)
                image.setPixel(i, 0, qRgba(0, 0, 0, 255));
            step += 1.0 / (image.width() - 2);
        }

        step = 0.0f;
        nextIndex = 0;
        black = false;

        for (int i = 1; i < image.height() - 1; i++)
        {
            if ((nextIndex < rows.count()) && (step >= rows[nextIndex]))
            {
                nextIndex++;
                black = !black;
            }
            if (black)
                image.setPixel(0, i, qRgba(0, 0, 0, 255));
            step += 1.0 / (image.height() - 2);
        }

        QPixmap::fromImage(image).save(fileName, "PNG");
    }
}

void EditorWindow::closeEvent(QCloseEvent *event)
{
    saveParameters();
}

void EditorWindow::updateScenes(const QList<QRectF> &region)
{
    bool needUpdate = false;
    QRectF patchRect = m_patch->mapRectToScene(m_patch->boundingRect());
    foreach(QRectF r, region)
    {
        if (patchRect.intersects(r))
        {
            needUpdate = true;
            break;
        }
    }
    if (!needUpdate)
        return;
    m_previewItem->updateRects(m_patch->rows(), m_patch->columns());
}

void EditorWindow::updatePreview(const QList<QRectF> &region)
{
    QRectF itemRect = m_previewItem->mapRectToScene(m_previewItem->boundingRect());
    m_previewScene.setSceneRect(itemRect);
}

void EditorWindow::saveParameters()
{
    QSettings settings;
    settings.beginGroup("state");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("window", saveState());
    settings.setValue("editorPreviewSplitter", m_ui->editorPreviewSplitter->saveState());
    settings.setValue("previewInfoSplitter", m_ui->previewInfoSplitter->saveState());
    settings.setValue("editorZoom", m_ui->editorView->zoom());
    settings.endGroup();
}

void EditorWindow::restoreParemeters()
{
    QSettings settings;
    settings.beginGroup("state");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("window").toByteArray());
    m_ui->editorPreviewSplitter->restoreState(settings.value("editorPreviewSplitter").toByteArray());
    m_ui->previewInfoSplitter->restoreState(settings.value("previewInfoSplitter").toByteArray());
    m_ui->editorView->setZoom(settings.value("editorZoom").toReal());
    settings.endGroup();
}
}
