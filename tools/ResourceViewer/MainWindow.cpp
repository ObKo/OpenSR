/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2014 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <fstream>
#include <OpenSR/libRangerQt.h>
#include <QProgressDialog>
#include <QBuffer>
#include <QMovie>
#include <QGraphicsProxyWidget>
#include "ExtractDialog.h"

namespace OpenSR
{
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    animLoaded(false),
    currentFrame(0),
    animStarted(false),
    speed(1.0)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);
    ui->graphicsView->setInteractive(true);

    extractDialog = new ExtractDialog(this);

    scene.setBackgroundBrush(QBrush(Qt::black));
    scene.addItem(&item);

    ui->fileTreeView->setModel(&model);
    ui->fileTreeView->setHeaderHidden(true);

    timer.setSingleShot(true);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    connect(ui->speedSpinBox, SIGNAL(valueChanged(double)), this, SLOT(speedChanged(double)));
    connect(ui->fileTreeView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(openContextMenu(const QPoint&)));
    connect(ui->fileTreeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(treeDoubleClicked(const QModelIndex&)));
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(startAnimation()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopAnimation()));
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetAnimation()));
    connect(ui->frameSlider, SIGNAL(sliderPressed()), this, SLOT(stopAnimation()));
    connect(ui->frameSlider, SIGNAL(valueChanged(int)), this, SLOT(setFrame(int)));

    QSettings settings("OpenSR", "ResourceViewer");
    restoreGeometry(settings.value("mainWindow/geometry").toByteArray());
    restoreState(settings.value("mainWindow/state").toByteArray());
    ui->vertSplitter->restoreGeometry(settings.value("mainWindow/vertSplitterGeometry").toByteArray());
    ui->vertSplitter->restoreState(settings.value("mainWindow/vertSplitterState").toByteArray());
    ui->horizSplitter->restoreGeometry(settings.value("mainWindow/horizSplitterGeometry").toByteArray());
    ui->horizSplitter->restoreState(settings.value("mainWindow/horizSplitterState").toByteArray());

    if (qApp->arguments().count() > 1)
        loadFile(qApp->arguments().at(1));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete extractDialog;
}

void MainWindow::nextFrame()
{
    if (!animLoaded || !animStarted)
        return;

    currentFrame = (currentFrame + 1) % frames.size();

    setFrame(currentFrame);
    timer.setInterval(times[currentFrame] / speed);
    timer.start();
}

void MainWindow::setFrame(int frame)
{
    if ((frame >= frames.count()) || (frame < 0))
        return;

    currentFrame = frame;
    item.setPixmap(frames.at(frame));
    ui->frameLabel->setText(QString::number(frame + 1) + "/" + QString::number(frames.count()));
    ui->frameSlider->setValue(frame);
}

void MainWindow::loadFile(const QString& fileName)
{
    QFileInfo fileInfo(fileName);
    if (!fileInfo.exists())
        return;

    if (fileInfo.suffix().toLower() == "pkg")
    {
        model.addPKG(fileInfo);
    }
    else
    {
        loadResource(model.addFile(fileInfo));
    }
}

void MainWindow::openFile()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open file"), QString(),
                            tr("All suported files (*.gai *.hai *.gi *.pkg)"));
    if (fileNames.isEmpty())
        return;

    foreach(QString fileName, fileNames)
    {
        loadFile(fileName);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings("OpenSR", "ResourceViewer");
    settings.setValue("mainWindow/geometry", saveGeometry());
    settings.setValue("mainWindow/state", saveState());
    settings.setValue("mainWindow/vertSplitterGeometry", ui->vertSplitter->saveGeometry());
    settings.setValue("mainWindow/vertSplitterState", ui->vertSplitter->saveState());
    settings.setValue("mainWindow/horizSplitterGeometry", ui->horizSplitter->saveGeometry());
    settings.setValue("mainWindow/horizSplitterState", ui->horizSplitter->saveState());
    QMainWindow::closeEvent(event);
}

void MainWindow::speedChanged(double value)
{
    if (speed < 0.01)
        return;

    speed = value;
}

void MainWindow::loadResource(FileNode *node)
{
    QString fileName = node->name;
    QByteArray resData = model.getData(node);
    QFileInfo fileInfo(fileName);
    QBuffer dev(&resData);
    dev.open(QIODevice::ReadOnly);
    QImageReader reader(&dev, fileInfo.suffix().toLower().toLatin1());

    if (!reader.canRead())
        return;

    int frameCount = reader.imageCount();

    animLoaded = false;
    animStarted = true;
    frames.clear();
    times.clear();
    currentFrame = 0;
    timer.stop();
    speed = 1.0;

    if (frameCount > 1)
    {
        frames.resize(frameCount);
        times.resize(frameCount);

        for (int i = 0; i < frameCount; i++)
        {
            frames[i] = QPixmap::fromImage(reader.read());
            times[i] = reader.nextImageDelay();
        }
        animationLoaded();
    }
    else
    {
        item.setPixmap(QPixmap::fromImageReader(&reader));
        imageLoaded();
    }
}

void MainWindow::animationLoaded()
{
    if (!frames.count())
        return;

    animLoaded = true;

    scene.setSceneRect(0, 0, item.pixmap().width(), item.pixmap().height());
    scene.invalidate();

    ui->frameSlider->setMaximum(frames.count() - 1);
    ui->frameSlider->setMinimum(0);
    ui->frameSlider->setSingleStep(1);

    ui->paramGroupBox->setEnabled(true);
    ui->frameSlider->setEnabled(true);

    resetAnimation();
    startAnimation();
}

void MainWindow::imageLoaded()
{
    scene.setSceneRect(0, 0, item.pixmap().width(), item.pixmap().height());
    scene.invalidate();

    ui->paramGroupBox->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->resetButton->setEnabled(false);
    ui->frameSlider->setEnabled(false);
    ui->frameSlider->setValue(0);
    ui->frameSlider->setMaximum(0);
    ui->frameSlider->setMinimum(0);
}


void MainWindow::treeDoubleClicked(const QModelIndex& index)
{
    FileNode *node = static_cast<FileNode *>(index.internalPointer());
    loadResource(node);
}

namespace
{
void addFileToList(QList<FileNode*>& files, FileNode* node)
{
    //FIXME: Possible errors
    if ((node->parent) && (node->parent->parent))
        files.append(node);
    foreach(FileNode * child, node->childs)
    {
        addFileToList(files, child);
    }
}
}

void MainWindow::openContextMenu(const QPoint & pos)
{
    FileNode *node = static_cast<FileNode *>(ui->fileTreeView->indexAt(pos).internalPointer());
    if (!node)
        return;
    if (node->type == NODE_PKG)
    {
        QMenu menu(ui->fileTreeView);
        menu.addAction(tr("Extract..."));
        QAction *selectedAction = menu.exec(ui->fileTreeView->mapToGlobal(pos));
        if (!selectedAction)
            return;
        QList<FileNode*> files;
        addFileToList(files, node);
        QString baseDir = QFileInfo(node->fullName).dir().path();

        if (extractDialog->exec() != QDialog::Accepted)
            return;
        QDir outDir(extractDialog->directory());
        if (!outDir.exists())
            QMessageBox::critical(this, tr("Error extracting"), tr("Invalid extraction directory"));
        bool createQRC = extractDialog->createQRC();
        QFileInfo qrcFileInfo;
        QFile *qrcFile = 0;
        if (createQRC)
        {
            QString fn = extractDialog->qrcName();
            if (!fn.isEmpty())
                fn = "resources.qrc";
            qrcFileInfo = QFileInfo(outDir.canonicalPath() + '/' + extractDialog->qrcName());
            if (qrcFileInfo.suffix().isEmpty())
                qrcFileInfo = QFileInfo(outDir.canonicalPath() + '/' + extractDialog->qrcName() + ".qrc");
            qrcFile = new QFile(qrcFileInfo.absoluteFilePath());
            qrcFile->open(QIODevice::WriteOnly);
        }
        QTextStream qrc(qrcFile);
        if (createQRC)
            qrc << "<!DOCTYPE RCC><RCC version=\"1.0\"><qresource>\n";

        QProgressDialog progress(tr("Extracting files..."), tr("Cancel"), 0, files.count(), this);
        progress.setWindowModality(Qt::WindowModal);
        int value = 0;
        foreach(FileNode * child, files)
        {
            if (progress.wasCanceled())
                break;
            QString fullName = child->fullName;
            QString fullNameWithoutBase = fullName;
            if ((!baseDir.isNull()) && (baseDir != ".") && (fullName.startsWith(baseDir)))
                fullNameWithoutBase = fullName.right(fullName.length() - baseDir.length() - 1);
            progress.setValue(value);
            progress.setLabelText(fullNameWithoutBase);
            if (child->childs.count())
            {
                if (!outDir.mkpath(fullNameWithoutBase))
                {
                    qCritical() << "Cannot create out dir:" << outDir.canonicalPath() + "/" + fullNameWithoutBase;
                    QMessageBox::critical(this, tr("Error extracting files"), tr("Cannot create out dir: %1").arg(outDir.canonicalPath() + "/" + fullNameWithoutBase));
                    return;
                }
            }
            else
            {
                QFile out(outDir.canonicalPath() + "/" + fullNameWithoutBase);
                if (!out.open(QIODevice::WriteOnly))
                {
                    qCritical() << "Cannot open file " << outDir.canonicalPath() + "/" + fullNameWithoutBase << ":" << out.errorString();
                    QMessageBox::critical(this, tr("Error extracting files"),
                                          tr("Cannot open file %1: %2").arg(outDir.canonicalPath() + "/" + fullNameWithoutBase, out.errorString()));
                    return;
                }
                out.write(model.getData(child));
                out.close();
                if (createQRC)
                    qrc << "<file>" << fullNameWithoutBase << "</file>\n";
            }
            value++;
        }
        if (createQRC)
        {
            qrc << "</qresource></RCC>\n";
            if (qrcFile)
            {
                qrcFile->close();
                delete qrcFile;
            }
        }
        progress.setValue(files.count());
    }
}

void MainWindow::startAnimation()
{
    if (!animLoaded || !frames.count())
        return;

    animStarted = true;
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->resetButton->setEnabled(true);

    timer.setInterval(times[currentFrame] / speed);
    timer.start();
}

void MainWindow::stopAnimation()
{
    animStarted = false;

    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->resetButton->setEnabled(true);

    timer.stop();
}

void MainWindow::resetAnimation()
{
    if (!frames.count())
        return;

    stopAnimation();
    setFrame(0);

    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->resetButton->setEnabled(true);
}
}
