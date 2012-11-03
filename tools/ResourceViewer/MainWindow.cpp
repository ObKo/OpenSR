#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <fstream>
#include <libRanger.h>
#include <squish.h>
#include <QProgressDialog>

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

    ui->fileTreeView->setModel(&model);

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(&animationTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    connect(ui->framerateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(framerateChanged(double)));
    connect(ui->durationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(durationChanged(double)));
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
}

void MainWindow::nextFrame()
{
    setFrame((currentFrame + 1) % frames.count());
}

void MainWindow::setFrame(int frame)
{
    if ((frame >= frames.count()) || (frame < 0))
        return;

    currentFrame = frame;
    item.setPixmap(frames.at(currentFrame));
    ui->frameLabel->setText(QString::number(currentFrame + 1) + "/" + QString::number(frames.count()));
    ui->frameSlider->setValue(currentFrame);
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
    else if (fileInfo.suffix().toLower() == "rpkg")
    {
        model.addRPKG(fileInfo);
    }
    else
    {
        loadResource(model.addFile(fileInfo));
    }
}

void MainWindow::loadHAI(HAIAnimation anim)
{
    for (int i = 0; i < anim.frameCount; i++)
    {
        frames.append(QPixmap::fromImage(QImage(anim.frames + i * anim.width * anim.height * 4, anim.width, anim.height, QImage::Format_ARGB32)));
    }
    delete anim.frames;
    animationLoaded(15, 0, 0);
}

void MainWindow::loadGI(GIFrame frame)
{
    item.setPixmap(QPixmap::fromImage(QImage(frame.data, frame.width, frame.height, QImage::Format_ARGB32)));
    delete frame.data;
    imageLoaded();
}

void MainWindow::loadGAI(GAIAnimation anim)
{
    for (int i = 0; i < anim.frameCount; i++)
    {
        frames.append(QPixmap::fromImage(QImage(anim.frames[i].data, anim.frames[i].width, anim.frames[i].height, QImage::Format_ARGB32)));
        delete[] anim.frames[i].data;
    }
    delete[] anim.frames;
    animationLoaded(15, anim.waitSeek, anim.waitSize);
}

void MainWindow::openFile()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open file"), QString(),
                            tr("All suported files (*.gai *.hai *.gi *.pkg *.dds *.rpkg)"));
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

void MainWindow::loadResource(FileNode *node)
{
    if (!frames.isEmpty())
    {
        frames.clear();
        animationTimer.stop();
    }

    QString fileName = node->name;
    QFileInfo fileInfo(fileName);
    if (fileInfo.suffix().toLower() == "gi")
    {
        Rangers::GIFrame frame = Rangers::loadGIFile(model.getData(node).data());
        loadGI(frame);
    }
    else if (fileInfo.suffix().toLower() == "gai")
    {
        Rangers::GAIHeader h = Rangers::loadGAIHeader(model.getData(node).data());
        Rangers::GIFrame *bg = 0;
        if (h.haveBackground)
        {
            FileNode *bgNode = model.getSiblingNode(node, fileInfo.completeBaseName() + ".gi");
            if (!bgNode)
                bgNode = model.getSiblingNode(node, fileInfo.completeBaseName() + ".GI");

            if (!bgNode)
                qCritical() << "Could not found background frame!";
            else
            {
                bg = new Rangers::GIFrame;
                *bg = Rangers::loadGIFile(model.getData(bgNode).data());
            }
        }
        Rangers::GAIAnimation anim = Rangers::loadGAIAnimation(model.getData(node).data(), bg);
        loadGAI(anim);
        /*if (bg)
            delete bg->data;
        delete bg;*/
    }
    else if (fileInfo.suffix().toLower() == "hai")
    {
        Rangers::HAIAnimation anim = Rangers::loadHAI(model.getData(node).data());
        loadHAI(anim);
    }
    else if (fileInfo.suffix().toLower() == "dds")
    {
        loadDDS(model.getData(node).data());
    }
}

void convertRGBAToBGRA(unsigned char *rgba, int width, int height)
{
    for (int i = 0; i < width * height; i++)
    {
        unsigned char tmp = rgba[i * 4];
        rgba[i * 4] = rgba[i * 4 + 2];
        rgba[i * 4 + 2] = tmp;
    }
}

void MainWindow::loadDDS(const char *data)
{
    if (*(uint32_t*)(data) != 0x20534444)
    {
        QMessageBox::warning(this, tr("Unsupported file"), tr("Not a DDS file."));
        return;
    }

    DDSHeader header = *((DDSHeader*)(data + 4));

    switch (header.ddspf.fourCC)
    {
    case 0x31545844:
    case 0x33545844:
    case 0x35545844:
        break;
    default:
        QMessageBox::warning(this, tr("Unsupported file"), tr("Unsupported DDS compression."));
        return;

    }

    if ((header.caps & DDSCAPS_COMPLEX) && (header.caps2 & DDSCAPS2_VOLUME)
            && (header.flags & DDSD_LINEARSIZE) && (header.flags & DDSD_LINEARSIZE) && (header.ddspf.flags & DDPF_FOURCC))
    {
        unsigned char *rgbaData = new unsigned char[header.height * header.width * 4];
        for (int i = 0; i < header.depth; i++)
        {
            unsigned char *dxt = ((unsigned char *)data) + 4 + sizeof(DDSHeader) + header.pitchOrLinearSize * header.height * i;
            int squishFlags;

            switch (header.ddspf.fourCC)
            {
            case 0x31545844:
                squishFlags = squish::kDxt1;
                break;
            case 0x33545844:
                squishFlags = squish::kDxt3;
                break;
            case 0x35545844:
                squishFlags = squish::kDxt5;
                break;
            }
            squish::DecompressImage(rgbaData, header.width, header.height, dxt, squishFlags);
            convertRGBAToBGRA(rgbaData, header.width, header.height);
            frames.append(QPixmap::fromImage(QImage(rgbaData, header.width, header.height, QImage::Format_ARGB32)));
        }
        delete rgbaData;
        animationLoaded(15, header.reserved1[0], header.reserved1[1]);

    }
    else if (header.ddspf.flags & DDPF_FOURCC)
    {
        unsigned char *rgbaData = new unsigned char[header.height * header.width * 4];
        unsigned char *dxt = ((unsigned char *)data) + 4 + sizeof(DDSHeader);
        int squishFlags;

        switch (header.ddspf.fourCC)
        {
        case 0x31545844:
            squishFlags = squish::kDxt1;
            break;
        case 0x33545844:
            squishFlags = squish::kDxt3;
            break;
        case 0x35545844:
            squishFlags = squish::kDxt5;
            break;
        }
        squish::DecompressImage(rgbaData, header.width, header.height, dxt, squishFlags);
        convertRGBAToBGRA(rgbaData, header.width, header.height);
        item.setPixmap(QPixmap::fromImage(QImage(rgbaData, header.width, header.height, QImage::Format_ARGB32)));
        delete rgbaData;
        imageLoaded();
    }
    else
    {
        QMessageBox::warning(this, tr("Unsupported file"), tr("Unsupported DDS format."));
        return;
    }
}


void MainWindow::animationLoaded(int framerate, int waitSeek, int waitSize)
{
    if (!frames.count())
        return;

    item.setPixmap(frames.at(0));
    animationTimer.setInterval(1000 / framerate);
    scene.setSceneRect(0, 0, item.pixmap().width(), item.pixmap().height());
    scene.invalidate();

    ui->seekLabel->setText(QString::number(waitSeek));
    ui->waitLabel->setText(QString::number(waitSize));
    ui->framerateSpinBox->setValue(framerate);
    ui->durationSpinBox->setValue(1000 / framerate);
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
    animationTimer.stop();
    scene.setSceneRect(0, 0, item.pixmap().width(), item.pixmap().height());
    scene.invalidate();

    ui->seekLabel->setText("");
    ui->waitLabel->setText("");
    ui->framerateSpinBox->setValue(15.0);
    ui->durationSpinBox->setValue(1000.0 / 15.0);

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
    if (node->type == NODE_RPKG || node->type == NODE_PKG)
    {
        QMenu menu(ui->fileTreeView);
        menu.addAction(tr("Extract..."));
        QAction *selectedAction = menu.exec(ui->fileTreeView->mapToGlobal(pos));
        if (!selectedAction)
            return;
        QList<FileNode*> files;
        addFileToList(files, node);
        QString baseDir = QFileInfo(node->fullName).dir().path();
        qDebug() << "Base dir: " << baseDir;
        QString outDirectory = QFileDialog::getExistingDirectory(this, tr("Select folder to extract"));
        if (outDirectory.isNull())
            return;
        qDebug() << "Output dir: " << outDirectory;
        QDir outDir(outDirectory);
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
            qDebug() << "Name:" << fullName << "name w/o base: " << fullNameWithoutBase;
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
                qDebug() << "Saving to " << outDir.canonicalPath() + "/" + fullNameWithoutBase;
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
            }
            value++;
        }
        progress.setValue(files.count());
    }
}

void MainWindow::startAnimation()
{
    if (animationTimer.isActive())
        return;
    if (!frames.count())
        return;

    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->resetButton->setEnabled(true);
    animationTimer.setSingleShot(false);
    animationTimer.start();
}

void MainWindow::stopAnimation()
{
    if (!animationTimer.isActive())
        return;

    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->resetButton->setEnabled(true);
    animationTimer.stop();
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
