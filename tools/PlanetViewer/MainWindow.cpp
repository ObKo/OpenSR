#include "MainWindow.h"
#include "ui_MainWindow.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->planetSpeed->setValue(ui->planetWidget->planetSpeed());
    ui->cloudSpeed->setValue(ui->planetWidget->cloudSpeed());
    ui->planetSize->setValue(ui->planetWidget->planetSize());
    ui->cloud->setChecked(ui->planetWidget->cloudEnabled());
    ui->colorCorrection->setChecked(ui->planetWidget->colorCorrection());
    ui->solarAngle->setValue((int)(ui->planetWidget->solarAngle() * 180 / M_PI) - 90);

    connect(ui->planetSpeed, SIGNAL(valueChanged(double)), this, SLOT(updateParams()));
    connect(ui->cloudSpeed, SIGNAL(valueChanged(double)), this, SLOT(updateParams()));
    connect(ui->planetSize, SIGNAL(valueChanged(int)), this, SLOT(updateParams()));
    connect(ui->cloud, SIGNAL(toggled(bool)), this, SLOT(updateParams()));
    connect(ui->colorCorrection, SIGNAL(toggled(bool)), this, SLOT(updateParams()));
    connect(ui->solarAngle, SIGNAL(valueChanged(int)), this, SLOT(updateParams()));

    connect(ui->textureBrowse, SIGNAL(clicked()), this, SLOT(openTexture()));
    connect(ui->cloudBrowse, SIGNAL(clicked()), this, SLOT(openCloudTexture()));
}

void MainWindow::openCloudTexture()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Open texture"));
    if (name.isEmpty())
        return;
    ui->cloudTexture->setText(name);
    ui->planetWidget->setCloudTexture(QPixmap(name));
}

void MainWindow::openTexture()
{
    QString name = QFileDialog::getOpenFileName(this, tr("Open texture"));
    if (name.isEmpty())
        return;
    ui->texture->setText(name);
    ui->planetWidget->setTexture(QPixmap(name));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateParams()
{
    ui->planetWidget->setPlanetSpeed(ui->planetSpeed->value());
    ui->planetWidget->setCloudSpeed(ui->cloudSpeed->value());
    ui->planetWidget->setPlanetSize(ui->planetSize->value());
    ui->planetWidget->setCloudEnabled(ui->cloud->isChecked());
    ui->planetWidget->setColorCorrection(ui->colorCorrection->isChecked());
    ui->planetWidget->setSolarAngle((ui->solarAngle->value() + 90) / 180.0 * M_PI);
}
