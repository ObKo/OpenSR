/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#define _USE_MATH_DEFINES
#include <math.h>
#include <QFileDialog>
#include <QInputDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    restoreGeometry(m_settings.value("gui/windowGeometry", QByteArray()).toByteArray());
    restoreState(m_settings.value("gui/windowState", QByteArray()).toByteArray());

    ui->planetSpeed->setValue(ui->planetWidget->planetSpeed());
    ui->cloudSpeed->setValue(ui->planetWidget->cloudSpeed());
    ui->planetSize->setValue(ui->planetWidget->planetSize());
    ui->cloud->setChecked(ui->planetWidget->cloudEnabled());
    ui->colorCorrection->setChecked(ui->planetWidget->colorCorrection());
    ui->solarAngle->setValue((int)(ui->planetWidget->solarAngle() * 180 / M_PI) - 90);
    ui->ring->setChecked(ui->planetWidget->ringEnabled());
    ui->ringBg->setChecked(ui->planetWidget->ringBackgroundEnabled());

    QColor c = ui->planetWidget->ambientColor();
    QString colorText = QString("#%1%2%3").arg((ushort)((c.rgba() >> 16) & 0xff), 2, 16, QChar('0'))
                        .arg((ushort)((c.rgba() >> 8) & 0xff), 2, 16, QChar('0'))
                        .arg((ushort)((c.rgba() >> 0) & 0xff), 2, 16, QChar('0')).toUpper();
    ui->colorLabel->setStyleSheet("background-color: " + colorText + ";");
    ui->colorLabel->setText(colorText);

    ui->planetListView->setModel(&m_model);

    connect(ui->planetSpeed, SIGNAL(valueChanged(double)), this, SLOT(updateParams()));
    connect(ui->cloudSpeed, SIGNAL(valueChanged(double)), this, SLOT(updateParams()));
    connect(ui->planetSize, SIGNAL(valueChanged(int)), this, SLOT(updateParams()));
    connect(ui->cloud, SIGNAL(toggled(bool)), this, SLOT(updateParams()));
    connect(ui->ring, SIGNAL(toggled(bool)), this, SLOT(updateParams()));
    connect(ui->ringBg, SIGNAL(toggled(bool)), this, SLOT(updateParams()));
    connect(ui->colorCorrection, SIGNAL(toggled(bool)), this, SLOT(updateParams()));
    connect(ui->solarAngle, SIGNAL(valueChanged(int)), this, SLOT(updateParams()));

    connect(ui->ringBgBrowse, SIGNAL(clicked()), this, SLOT(openRingBackgroundTexture()));
    connect(ui->ringBrowse, SIGNAL(clicked()), this, SLOT(openRingTexture()));
    connect(ui->textureBrowse, SIGNAL(clicked()), this, SLOT(openTexture()));
    connect(ui->cloudBrowse, SIGNAL(clicked()), this, SLOT(openCloudTexture()));
    connect(ui->colorButton, SIGNAL(clicked()), this, SLOT(setColor()));
    connect(ui->savePlanetButton, SIGNAL(clicked()), this, SLOT(addPlanet()));
    connect(ui->actionSet_data_dir, SIGNAL(triggered()), this, SLOT(setDataDir()));
    connect(ui->planetListView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(loadPlanet(QModelIndex)));
    connect(ui->removePlanetButton, SIGNAL(clicked()), this, SLOT(removePlanet()));
    connect(ui->actionSave_JSON, SIGNAL(triggered()), this, SLOT(save()));
    connect(ui->actionOpen_JSON, SIGNAL(triggered()), this, SLOT(open()));
}

void MainWindow::openCloudTexture()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Open texture"), m_settings.value("data/dataDir", "").toString());
    if (name.isEmpty())
        return;
    ui->cloudTexture->setText(QDir(m_settings.value("data/dataDir", "").toString()).relativeFilePath(name));
    ui->planetWidget->setCloudTexture(name);
}

void MainWindow::openRingTexture()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Open texture"), m_settings.value("data/dataDir", "").toString());
    if (name.isEmpty())
        return;

    ui->planetWidget->setRingTexture(name);
    ui->ringTexture->setText(QDir(m_settings.value("data/dataDir", "").toString()).relativeFilePath(name));
}

void MainWindow::openRingBackgroundTexture()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Open texture"), m_settings.value("data/dataDir", "").toString());
    if (name.isEmpty())
        return;

    ui->planetWidget->setRingBackground(name);
    ui->ringBgTexture->setText(QDir(m_settings.value("data/dataDir", "").toString()).relativeFilePath(name));
}

void MainWindow::openTexture()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Open texture"), m_settings.value("data/dataDir", "").toString());
    if (name.isEmpty())
        return;
    ui->texture->setText(QDir(m_settings.value("data/dataDir", "").toString()).relativeFilePath(name));
    ui->planetWidget->setTexture(name);
}

void MainWindow::setDataDir()
{
    bool ok;
    QString dir = QInputDialog::getText(this, tr("Set data dir"), "Data directory:",
                                        QLineEdit::Normal, m_settings.value("data/dataDir", "").toString(), &ok);

    if (!ok)
        return;

    if (!QDir(dir).exists())
    {
        QMessageBox::critical(this, tr("Error"), tr("No such directory"));
        return;
    }
    m_settings.setValue("data/dataDir", dir);
}

void MainWindow::loadPlanet(const QModelIndex& index)
{
    Planet p = m_model.getPlanet(index);

    QString colorText = QString("#%1%2%3").arg((ushort)((p.ambientColor.rgba() >> 16) & 0xff), 2, 16, QChar('0'))
                        .arg((ushort)((p.ambientColor.rgba() >> 8) & 0xff), 2, 16, QChar('0'))
                        .arg((ushort)((p.ambientColor.rgba() >> 0) & 0xff), 2, 16, QChar('0')).toUpper();
    ui->colorLabel->setStyleSheet("background-color: " + colorText + ";");
    ui->colorLabel->setText(colorText);
    ui->cloudTexture->setText(p.cloud);
    ui->cloudSpeed->setValue(p.cloudSpeed);
    ui->cloud->setChecked(p.hasCloud);
    ui->ID->setText(p.id);
    ui->planetSize->setValue(p.size);
    ui->planetSpeed->setValue(p.speed);
    ui->texture->setText(p.texture);
    ui->ring->setChecked(p.hasRing);
    ui->ringTexture->setText(p.ring);
    ui->ringBg->setChecked(p.hasRingBackground);
    ui->ringBgTexture->setText(p.ringBackground);

    ui->planetWidget->setAmbientColor(p.ambientColor);
    ui->planetWidget->setPlanetSpeed(p.speed);
    ui->planetWidget->setCloudSpeed(p.cloudSpeed);
    ui->planetWidget->setPlanetSize(p.size);
    ui->planetWidget->setCloudEnabled(p.hasCloud);
    ui->planetWidget->setRingEnabled(p.hasRing);
    ui->planetWidget->setRingBackgroundEnabled(p.hasRingBackground);
    ui->planetWidget->setTexture(QDir(m_settings.value("data/dataDir", "").toString()).absoluteFilePath(p.texture));
    ui->planetWidget->setCloudTexture(QDir(m_settings.value("data/dataDir", "").toString()).absoluteFilePath(p.cloud));
    ui->planetWidget->setRingTexture(QDir(m_settings.value("data/dataDir", "").toString()).absoluteFilePath(p.ring));
    ui->planetWidget->setRingBackground(QDir(m_settings.value("data/dataDir", "").toString()).absoluteFilePath(p.ringBackground));
}

void MainWindow::save()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Save JSON"), QString(), "JSON files (*.json)");
    if (file.isEmpty())
        return;

    m_model.saveJSON(file);
}

void MainWindow::open()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open JSON"), QString(), "JSON files (*.json)");
    if (file.isEmpty())
        return;

    m_model.loadJSON(file);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::removePlanet()
{
    QModelIndexList planetIndexes = ui->planetListView->selectionModel()->selectedRows();
    m_model.removePlanets(planetIndexes);
}

void MainWindow::addPlanet()
{
    Planet p;
    p.ambientColor = ui->planetWidget->ambientColor();
    p.cloud = ui->cloudTexture->text();
    p.cloudSpeed = ui->cloudSpeed->value();
    p.hasCloud = ui->cloud->isChecked();
    p.id = ui->ID->text();
    p.size = ui->planetSize->value();
    p.speed = ui->planetSpeed->value();
    p.texture = ui->texture->text();
    p.ring = ui->ringTexture->text();
    p.hasRing = ui->ring->isChecked();
    p.hasRingBackground = ui->ringBg->isChecked();
    p.ringBackground = ui->ringBgTexture->text();
    m_model.addPlanet(p);
}

void MainWindow::setColor()
{
    QColorDialog d(this);

    d.setCurrentColor(ui->planetWidget->ambientColor());

    if (!d.exec())
        return;

    QColor c = d.selectedColor();
    ui->planetWidget->setAmbientColor(c);
    QString colorText = QString("#%1%2%3").arg((ushort)((c.rgba() >> 16) & 0xff), 2, 16, QChar('0'))
                        .arg((ushort)((c.rgba() >> 8) & 0xff), 2, 16, QChar('0'))
                        .arg((ushort)((c.rgba() >> 0) & 0xff), 2, 16, QChar('0')).toUpper();
    ui->colorLabel->setStyleSheet("background-color: " + colorText + ";");
    ui->colorLabel->setText(colorText);
}

void MainWindow::updateParams()
{
    ui->planetWidget->setPlanetSpeed(ui->planetSpeed->value());
    ui->planetWidget->setCloudSpeed(ui->cloudSpeed->value());
    ui->planetWidget->setPlanetSize(ui->planetSize->value());
    ui->planetWidget->setCloudEnabled(ui->cloud->isChecked());
    ui->planetWidget->setColorCorrection(ui->colorCorrection->isChecked());
    ui->planetWidget->setSolarAngle((ui->solarAngle->value() + 90) / 180.0 * M_PI);
    ui->planetWidget->setRingEnabled(ui->ring->isChecked());
    ui->planetWidget->setRingBackgroundEnabled(ui->ringBg->isChecked());
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    m_settings.setValue("gui/windowGeometry", saveGeometry());
    m_settings.setValue("gui/windowState", saveState());
    QWidget::closeEvent(event);
}
