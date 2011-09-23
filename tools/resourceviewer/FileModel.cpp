#include "FileModel.h"
#include <QtGui>
#include <QDebug>
#include <QFile>

using namespace Rangers;

FileModel::FileModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new FileNode;
    rootItem->parent = 0;
    rootItem->name = "";
}

int FileModel::columnCount(const QModelIndex & /* parent */) const
{
    return 1;
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    FileNode *item = getItem(index);

    return item->name;
}

Qt::ItemFlags FileModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

FileNode *FileModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        FileNode *item = static_cast<FileNode*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}

QVariant FileModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    //if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    //    return rootItem->data(section);

    return QVariant();
}

QModelIndex FileModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    FileNode *parentItem = getItem(parent);

    if (row >= parentItem->childs.count())
        return QModelIndex();

    FileNode *childItem = parentItem->childs.at(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex FileModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    FileNode *childItem = getItem(index);
    FileNode *parentItem = childItem->parent;

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childs.count(), 0, parentItem);
}

int FileModel::rowCount(const QModelIndex &parent) const
{
    FileNode *parentItem = getItem(parent);

    return parentItem->childs.count();
}

void convertNode(PKGItem *node, FileNode *fileNode, FileNode *parent, QString fullName)
{
    fileNode->name = node->name;
    fileNode->fullName = fullName + fileNode->name;
    fileNode->parent = parent;
    fileNode->userData = node;
    fileNode->type = NODE_PKG;

    if (node->childCount && fileNode->name != "")
        fullName += fileNode->name + "/";

    for (int i = 0; i < node->childCount; i++)
    {
        FileNode *newNode = new FileNode;
        fileNode->childs.append(newNode);
        convertNode(&node->childs[i], newNode, fileNode, fullName);
    }
}

FileNode* FileModel::addPKG(const QFileInfo& file)
{
    std::ifstream f(file.filePath().toLocal8Bit().data(), std::ios::binary);
    PKGItem *root = Rangers::loadPKG(f);
    f.close();

    this->

    beginResetModel();
    FileNode *node = new FileNode;
    convertNode(root, node, rootItem, "");
    node->name = file.fileName();
    node->fullName = file.filePath();
    rootItem->childs.append(node);
    archives[node] = file;
    endResetModel();
    return node;
}

FileNode* FileModel::addFile(const QFileInfo& file)
{
    beginResetModel();
    FileNode *node = new FileNode;
    node->name = file.fileName();
    node->fullName = file.filePath();
    node->parent = rootItem;
    node->type = NODE_FILE;
    rootItem->childs.append(node);
    archives[node] = file;
    endResetModel();
    return node;
}

FileNode* FileModel::getSiblingNode(FileNode* node, const QString& name)
{
    switch (node->type)
    {
    case NODE_PKG:
    {
        FileNode *parent = node->parent;
        if (!parent)
            return 0;

        for (QList<FileNode*>::const_iterator i = parent->childs.constBegin(); i != parent->childs.constEnd(); i++)
        {
            if ((*i)->name == name)
                return *i;
        }
        break;
    }
    case NODE_FILE:
    {
        QFileInfo info = archives[node];
        if (!info.absoluteDir().entryList().contains(name))
            return 0;
        for (QList<FileNode*>::const_iterator i = rootItem->childs.constBegin(); i != rootItem->childs.constEnd(); i++)
        {
            if ((*i)->name == name)
                return *i;
        }
        return addFile(QFileInfo(info.absoluteDir().canonicalPath() + "/" + name));
    }
    }
    return 0;
}

QByteArray FileModel::getData(FileNode *node)
{
    if (node->childs.count())
        return QByteArray();

    QByteArray result;
    switch (node->type)
    {
    case NODE_PKG:
    {
        PKGItem *pkg = static_cast<PKGItem*>(node->userData);
        FileNode *root = node;
        while (static_cast<PKGItem*>(root->userData)->parent)
            root = root->parent;

        std::ifstream f(archives[root].filePath().toLocal8Bit().data(), std::ios::binary);
        unsigned char *data = extractFile(*pkg, f);
        result = QByteArray((const char*)data, pkg->size);
        delete data;
        f.close();
        break;
    }
    case NODE_FILE:
    {
        QFile f(archives[node].filePath());
        f.open(QIODevice::ReadOnly);
        result = f.readAll();
        f.close();
    }
    }
    return result;
}

QByteArray FileModel::getData(const QModelIndex &index)
{
    FileNode *node = static_cast<FileNode*>(index.internalPointer());
    return getData(node);
}
