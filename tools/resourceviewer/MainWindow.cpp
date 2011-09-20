#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QDebug>
#include <fstream>
#include <libRanger.h>

using namespace Rangers;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);
    ui->graphicsView->setInteractive(true);

    scene.setBackgroundBrush(QBrush(Qt::black));
    scene.addItem(&item);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(&animationTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    connect(ui->framerateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(framerateChanged(double)));
    connect(ui->durationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(durationChanged(double)));

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
}

void extractPKG(Rangers::PKGItem *root, QString path, std::ifstream& f)
{
    if (!root)
        root = Rangers::loadPKG(f);

    if (root->dataType == 3)
    {
        if (qstrlen(root->name) > 0)
        {
            path += QString("/") + QString::fromAscii(root->name);
            QDir().mkpath(path);
        }

        for (int i = 0; i < root->childCount; i++)
            extractPKG(&root->childs[i], path, f);
    }
    else
    {
        QFile out(path + "/" + QString::fromAscii(root->name));
        qDebug("Extracting file %s ...", (path + "/" + QString::fromAscii(root->name)).toLocal8Bit().data());
        out.open(QIODevice::WriteOnly);
        char *data = (char*)Rangers::extractFile(*root, f);
        out.write(data, root->size);
        out.close();
        delete data;
    }
}

void MainWindow::nextFrame()
{
    currentFrame = (currentFrame + 1) % frames.count();
    item.setPixmap(frames.at(currentFrame));
    ui->frameLabel->setText(QString::number(currentFrame) + "/" + QString::number(frames.count()));
}

void MainWindow::loadFile(const QString& fileName)
{
    ui->paramGroupBox->setEnabled(false);
    QFileInfo fileInfo(fileName);
    if (!fileInfo.exists())
        return;

    std::ifstream f(fileName.toLocal8Bit().data(), std::ios::binary);

    if (!frames.isEmpty())
    {
        frames.clear();
        animationTimer.stop();
    }


    if (fileInfo.suffix().toLower() == "gi")
    {
        Rangers::GIFrame frame = Rangers::loadGIFile(f);
        item.setPixmap(QPixmap::fromImage(QImage(frame.data, frame.width, frame.height, QImage::Format_ARGB32)));
        delete frame.data;
        scene.setSceneRect(0, 0, frame.width, frame.height);
        scene.invalidate();
    }
    else if (fileInfo.suffix().toLower() == "gai")
    {
        size_t offset = 0;
        Rangers::GAIHeader h = Rangers::loadGAIHeader(f, offset);
        Rangers::GIFrame *bg = 0;
        if (h.haveBackground)
        {
            QFileInfo bgFileInfo((fileInfo.absoluteDir().canonicalPath() + "/" + fileInfo.completeBaseName() + ".gi"));
            if (!bgFileInfo.exists())
                bgFileInfo = QFileInfo((fileInfo.absoluteDir().canonicalPath() + "/" + fileInfo.completeBaseName() + ".GI"));

            if (!bgFileInfo.exists())
                qCritical() << "Could not found background frame!";
            else
            {
                std::ifstream bgFile(bgFileInfo.absoluteFilePath().toLocal8Bit().data(), std::ios::in | std::ios::binary);
                bg = new Rangers::GIFrame();
                *bg = loadGIFile(bgFile);
                bgFile.close();
            }
        }
        offset = 0;
        Rangers::GAIAnimation anim = Rangers::loadGAIAnimation(f, offset, bg);
        for (int i = 0; i < anim.frameCount; i++)
        {
            frames.append(QPixmap::fromImage(QImage(anim.frames[i].data, anim.frames[i].width, anim.frames[i].height, QImage::Format_ARGB32)));
            delete[] anim.frames[i].data;
        }
        delete[] anim.frames;
        if (bg)
            delete bg->data;
        delete bg;
        currentFrame = 0;
        item.setPixmap(frames.at(currentFrame));
        animationTimer.setInterval(1000 / 15);
        animationTimer.setSingleShot(false);
        animationTimer.start();
        scene.setSceneRect(0, 0, anim.width, anim.height);
        scene.invalidate();

        ui->seekLabel->setText(QString::number(anim.waitSeek));
        ui->waitLabel->setText(QString::number(anim.waitSize));
        ui->framerateSpinBox->setValue(15);
        ui->durationSpinBox->setValue(1000.0 / 15);
        ui->paramGroupBox->setEnabled(true);
    }
    else if (fileInfo.suffix().toLower() == "hai")
    {
        Rangers::HAIAnimation anim = Rangers::loadHAI(f);
        for (int i = 0; i < anim.frameCount; i++)
        {
            frames.append(QPixmap::fromImage(QImage(anim.frames + i * anim.width * anim.height * 4, anim.width, anim.height, QImage::Format_ARGB32)));
        }
        delete anim.frames;
        currentFrame = 0;
        item.setPixmap(frames.at(currentFrame));
        animationTimer.setInterval(1000 / 15);
        animationTimer.setSingleShot(false);
        animationTimer.start();

        ui->seekLabel->setText("");
        ui->waitLabel->setText("");
        ui->framerateSpinBox->setValue(15);
        ui->durationSpinBox->setValue(1000.0 / 15);
        ui->paramGroupBox->setEnabled(true);

        scene.setSceneRect(0, 0, anim.width, anim.height);
        scene.invalidate();
    }
    else if (fileInfo.suffix().toLower() == "pkg")
    {
        PKGItem *root = Rangers::loadPKG(f);
        model = new PKGModel(root, this);
        ui->fileTreeView->setModel(model);
        //QString outDir = QFileDialog::getExistingDirectory(this, tr("Select output folder"));
        //extractPKG(0, outDir, f);
    }
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open image file"), QString(), tr("All suported files (*.gai *.hai *.gi *.pkg)"));
    if (fileName.isNull())
        return;

    loadFile(fileName);
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

void MainWindow::framerateChanged(double value)
{
    animationTimer.setInterval(int(1000 / value));
    ui->durationSpinBox->setValue(1000.0 / value);
}

void MainWindow::durationChanged(double value)
{
    animationTimer.setInterval(int(value));
    ui->framerateSpinBox->setValue(1000.0 / value);
}
