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

#ifndef OPENSR_RV_EXTRACTDIALOG_H
#define OPENSR_RV_EXTRACTDIALOG_H

#include <QDialog>

namespace Ui
{
class ExtractDialog;
}

namespace OpenSR
{
class ExtractDialog : public QDialog
{
    Q_OBJECT

public:
    Q_PROPERTY(QString directory READ directory)
    Q_PROPERTY(QString qrcName READ qrcName)
    Q_PROPERTY(bool createQRC READ createQRC)

    explicit ExtractDialog(QWidget *parent = 0);
    ~ExtractDialog();

    bool createQRC();
    QString qrcName();
    QString directory();

private:
    Ui::ExtractDialog *m_ui;

    QString m_dir;
    QString m_qrcName;
    bool m_createQRC;

private Q_SLOTS:
    void saveValues();
    void selectDir();
    void updateQRCForm(bool enabled);
};
}


#endif // OPENSR_RV_EXTRACTDIALOG_H
