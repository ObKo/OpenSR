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

#ifndef OPENSR_RV_FILEMODEL_H
#define OPENSR_RV_FILEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QList>
#include <QFileInfo>
#include <QMap>
#include <QFileIconProvider>

namespace OpenSR
{
enum NodeType {NODE_FILE, NODE_PKG};
struct FileNode
{
    QString name;
    QString fullName;
    FileNode *parent;
    QList<FileNode*> childs;
    NodeType type;
    void *userData;
};
class FileModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    FileModel(QObject *parent = 0);

    FileNode* addPKG(const QFileInfo& file);
    FileNode* addFile(const QFileInfo& file);

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QByteArray getData(const QModelIndex &index);
    QByteArray getData(FileNode *node);
    FileNode* getSiblingNode(FileNode* node, const QString& name);

private:
    FileNode *rootItem;
    FileNode *getItem(const QModelIndex &index) const;
    QMap<FileNode*, QFileInfo> archives;
    QFileIconProvider iconProvider;
};
}

#endif // OPENSR_RV_FILEMODEL_H
