#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <fstream>
#include <libRanger.h>

using namespace Rangers;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);
    scene.addItem(&item);
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(&animationTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));

    if(qApp->arguments().count() > 1)
        loadFile(qApp->arguments().at(1));
}

MainWindow::~MainWindow()
{
    delete ui;
}

unsigned char* revert(const unsigned char *rgba, int width, int height)
{
    unsigned char *result = new unsigned char[width*height*4];
    for(int i = 0; i < width*height*4; i+=4)
    {
        result[i] = rgba[i + 1];
        result[i + 1] = rgba[i + 2];
        result[i + 2] = rgba[i + 3];
        result[i + 3] = rgba[i];
    }
    return result;
}

void extractPKG(Rangers::PKGItem *root, QString path, std::ifstream& f)
{
    if(!root)
        root = Rangers::loadPKG(f);
    
    if(root->dataType == 3)
    {
	if(qstrlen(root->name) > 0)
	{
	    path += QString("/") + QString::fromAscii(root->name);
	    QDir().mkpath(path);
	}
	
	for(int i = 0; i < root->childCount; i++)
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
}

void MainWindow::loadFile(const QString& fileName)
{
        QFileInfo fileInfo(fileName);
        if(!fileInfo.exists())
            return;

		std::ifstream f(fileName.toLocal8Bit().data(), std::ios::binary);

        if(!frames.isEmpty())
        {
            frames.clear();
            animationTimer.stop();
        }


        if(fileInfo.suffix().toLower() == "gi")
        {
            Rangers::GIFrame frame = Rangers::loadGIFile(f);
            unsigned char *data = revert(frame.data, frame.width, frame.height);
            item.setPixmap(QPixmap::fromImage(QImage(data, frame.width, frame.height, QImage::Format_ARGB32)));
            delete data;
            delete frame.data;
            scene.setSceneRect(0, 0, frame.width, frame.height);
            scene.invalidate();
        }
        else if(fileInfo.suffix().toLower() == "gai")
        {
            f.seekg(0, std::ios_base::end);
	    size_t fileSize = f.tellg();
	    f.seekg(0, std::ios_base::beg);
	    char *fileData = new char[fileSize];
	    f.read(fileData, fileSize);
	    f.close();
            Rangers::GAIAnimation anim = Rangers::loadGAIAnimation(fileData);
            for(int i = 0; i < anim.frameCount; i++)
            {
                unsigned char *data = revert(anim.frames[i].data, anim.frames[i].width, anim.frames[i].height);
                frames.append(QPixmap::fromImage(QImage(data, anim.frames[i].width, anim.frames[i].height, QImage::Format_ARGB32)));
                delete[] data;
                delete[] anim.frames[i].data;
            }
            delete[] anim.frames;
	    delete[] fileData;
            currentFrame = 0;
            item.setPixmap(frames.at(currentFrame));
            animationTimer.setInterval(60);
            animationTimer.setSingleShot(false);
            animationTimer.start();
            scene.setSceneRect(0, 0, anim.width, anim.height);
            scene.invalidate();
        }
        else if(fileInfo.suffix().toLower() == "hai")
        {
            Rangers::HAIAnimation anim = Rangers::loadHAI(f);
            for(int i = 0; i < anim.frameCount; i++)
            {
                unsigned char *data = revert(anim.frames + i * anim.width * anim.height * 4, anim.width, anim.height);
                frames.append(QPixmap::fromImage(QImage(data, anim.width, anim.height, QImage::Format_ARGB32)));
                delete data;
            }
            delete anim.frames;
            currentFrame = 0;
            item.setPixmap(frames.at(currentFrame));
            animationTimer.setInterval(60);
            animationTimer.setSingleShot(false);
            animationTimer.start();
            scene.setSceneRect(0, 0, anim.width, anim.height);
            scene.invalidate();
        }
        else if(fileInfo.suffix().toLower() == "pkg")
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
    if(fileName.isNull())
        return;

    loadFile(fileName);
}
