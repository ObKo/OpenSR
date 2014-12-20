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

#ifndef OPENSR_RV_MAINWINDOW_H
#define OPENSR_RV_MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QLabel>
#include "FileModel.h"

namespace Ui
{
class MainWindow;
}

namespace OpenSR
{
class ExtractDialog;
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
    QGraphicsProxyWidget *widget;
    QGraphicsPixmapItem item;
    QTimer timer;

    QVector<QPixmap> frames;
    QVector<int> times;
    bool animLoaded;
    bool animStarted;
    int currentFrame;
    double speed;

    void loadFile(const QString& fileName);
    FileModel model;

    ExtractDialog *extractDialog;

    void loadResource(FileNode *node);

    void animationLoaded();
    void imageLoaded();

private Q_SLOTS:
    void openFile();
    void nextFrame();
    void speedChanged(double value);
    void treeDoubleClicked(const QModelIndex& index);
    void openContextMenu(const QPoint & pos);
    void stopAnimation();
    void startAnimation();
    void resetAnimation();
    void setFrame(int frame);
};
}
#endif // OPENSR_RV_MAINWINDOW_H
