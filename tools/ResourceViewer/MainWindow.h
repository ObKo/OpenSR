#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "FileModel.h"

namespace Ui
{
class MainWindow;
}

namespace Rangers
{
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    QGraphicsPixmapItem item;

    QList<QPixmap> frames;
    QTimer animationTimer;
    int currentFrame;

    void loadFile(const QString& fileName);
    FileModel model;

    void loadGAI(GAIAnimation animation);
    void loadGI(GIFrame frame);
    void loadHAI(HAIAnimation animation);

    void loadResource(FileNode *node);

    void animationLoaded(int framerate, int waitSeek, int waitSize);
    void imageLoaded();

private Q_SLOTS:
    void openFile();
    void nextFrame();
    void framerateChanged(double value);
    void durationChanged(double value);
    void treeDoubleClicked(const QModelIndex& index);
    void openContextMenu(const QPoint & pos);
    void stopAnimation();
    void startAnimation();
    void resetAnimation();
    void setFrame(int frame);
};
}
#endif // MAINWINDOW_H
