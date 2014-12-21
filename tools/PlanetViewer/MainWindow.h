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

#ifndef OPENSR_PV_MAINWINDOW_H
#define OPENSR_PV_MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "PlanetModel.h"

namespace Ui
{
class MainWindow;
}

namespace OpenSR
{
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private Q_SLOTS:
    void updateParams();

    void setColor();
    void openCloudTexture();
    void openRingTexture();
    void openRingBackgroundTexture();
    void openTexture();

    void addPlanet();
    void removePlanet();
    void setDataDir();
    void loadPlanet(const QModelIndex& index);

    void save();
    void open();

    void showError(const QString& title, const QString& text);

private:
    Ui::MainWindow *ui;
    PlanetModel m_model;
    QSettings m_settings;
};
}

#endif // OPENSR_PV_MAINWINDOW_H
