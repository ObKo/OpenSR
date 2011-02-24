#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    QGraphicsPixmapItem item;

    QList<QPixmap> frames;
    QTimer animationTimer;
    int currentFrame;

    void loadFile(const QString& fileName);

private Q_SLOTS:
    void openFile();
    void nextFrame();
};

#endif // MAINWINDOW_H
