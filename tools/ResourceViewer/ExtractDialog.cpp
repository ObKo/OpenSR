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

#include "ExtractDialog.h"
#include "ui_ExtractDialog.h"

#include <QFileDialog>

namespace OpenSR
{
ExtractDialog::ExtractDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ExtractDialog),
    m_qrcName("resource.qrc")
{
    m_ui->setupUi(this);
    m_ui->qrcNameEdit->setText(m_qrcName);

    updateQRCForm(false);

    connect(this, SIGNAL(accepted()), this, SLOT(saveValues()));
    connect(m_ui->browseButton, SIGNAL(clicked()), this, SLOT(selectDir()));
    connect(m_ui->qrcCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateQRCForm(bool)));
}

ExtractDialog::~ExtractDialog()
{
    delete m_ui;
}

void ExtractDialog::saveValues()
{
    m_createQRC = m_ui->qrcCheckBox->isChecked();
    m_qrcName = m_ui->qrcNameEdit->text();
    m_dir = m_ui->dirEdit->text();
}

void ExtractDialog::selectDir()
{
    QString d = QFileDialog::getExistingDirectory(this, tr("Select dir to extract"));
    if (d.isEmpty())
        return;
    m_ui->dirEdit->setText(d);
}

bool ExtractDialog::createQRC()
{
    return m_createQRC;
}

QString ExtractDialog::qrcName()
{
    return m_qrcName;
}

QString ExtractDialog::directory()
{
    return m_dir;
}

void ExtractDialog::updateQRCForm(bool enabled)
{
    m_ui->qrcNameEdit->setEnabled(enabled);
    m_ui->qrcLabel->setEnabled(enabled);
}
}
