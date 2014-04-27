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

    if (role != Qt::DisplayRole && role != Qt::DecorationRole)
        return QVariant();

    FileNode *item = getItem(index);

    switch (role)
    {
    case Qt::DisplayRole:
        return item->name;
    case Qt::DecorationRole:
        if (item->childs.count())
            return iconProvider.icon(QFileIconProvider::Folder);
        else
            return iconProvider.icon(QFileInfo(item->name));
    }
    return QVariant();
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

void convertPKGNode(PKGItem *node, FileNode *fileNode, FileNode *parent, QString fullName)
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
        convertPKGNode(&node->childs[i], newNode, fileNode, fullName);
    }
}

void convertRPKGEntry(const RPKGEntry *entry, FileNode *root, FileNode *fileNode)
{
    QString name = QString::fromUtf8(entry->name.c_str());
    QFileInfo fileInfo(name);
    fileNode->name = fileInfo.fileName();
    fileNode->fullName = name;

    QString dir = fileInfo.dir().path();
    QStringList dirs = dir.split("/");

    FileNode *currentNode = root;
    QString fullDirName;
    foreach(QString dir, dirs)
    {
        fullDirName += dir + "/";
        bool hasChild = false;
        foreach(FileNode * child, currentNode->childs)
        {
            if (child->name == dir)
            {
                hasChild = true;
                currentNode = child;
                break;
            }
        }
        if (!hasChild)
        {
            FileNode *newNode = new FileNode();
            currentNode->childs.append(newNode);
            newNode->parent = currentNode;
            newNode->fullName = fullDirName;
            newNode->fullName.chop(1);
            newNode->name = dir;
            newNode->type = NODE_RPKG;
            newNode->userData = 0;
            currentNode = newNode;
        }
    }

    fileNode->parent = currentNode;
    currentNode->childs.append(fileNode);
    fileNode->userData = new RPKGEntry(*entry);
    fileNode->type = NODE_RPKG;
}

FileNode* FileModel::addPKG(const QFileInfo& file)
{
#ifdef Q_OS_WIN32
    std::ifstream f((wchar_t *)file.filePath().utf16(), std::ios::binary);
#else
    std::ifstream f(QFile::encodeName(file.filePath()).data(), std::ios::binary);
#endif
    PKGItem *root = Rangers::loadPKG(f);
    f.close();

    beginResetModel();
    FileNode *node = new FileNode;
    convertPKGNode(root, node, rootItem, "");
    node->name = file.fileName();
    node->fullName = file.filePath();
    rootItem->childs.append(node);
    archives[node] = file;
    endResetModel();
    return node;
}

FileNode* FileModel::addRPKG(const QFileInfo& file)
{
#ifdef Q_OS_WIN32
    std::ifstream f((wchar_t *)file.filePath().utf16(), std::ios::binary);
#else
    std::ifstream f(QFile::encodeName(file.filePath()).data(), std::ios::binary);
#endif
    std::list<RPKGEntry> entries = Rangers::loadRPKG(f);
    f.close();

    beginResetModel();
    FileNode *node = new FileNode;
    std::list<RPKGEntry>::const_iterator end = entries.end();
    for (std::list<RPKGEntry>::const_iterator i = entries.begin(); i != end; ++i)
    {
        FileNode *n = new FileNode();
        convertRPKGEntry(&(*i), node, n);
    }
    node->parent = rootItem;
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
    case NODE_RPKG:
    {
        FileNode *parent = node->parent;
        if (!parent)
            return 0;

        for (QList<FileNode*>::const_iterator i = parent->childs.constBegin(); i != parent->childs.constEnd(); ++i)
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
        for (QList<FileNode*>::const_iterator i = rootItem->childs.constBegin(); i != rootItem->childs.constEnd(); ++i)
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

#ifdef Q_OS_WIN32
        std::ifstream f((wchar_t *)archives[root].filePath().utf16(), std::ios::binary);
#else
        std::ifstream f(QFile::encodeName(archives[root].filePath()).data(), std::ios::binary);
#endif
        unsigned char *data = extractFile(*pkg, f);
        result = QByteArray((const char*)data, pkg->size);
        delete data;
        f.close();
        break;
    }
    case NODE_RPKG:
    {
        RPKGEntry *pkg = static_cast<RPKGEntry*>(node->userData);
        FileNode *root = node;
        //FIXME: Possible errors
        while (root && root->parent && root->parent->parent)
            root = root->parent;

#ifdef Q_OS_WIN32
        std::ifstream f((wchar_t *)archives[root].filePath().utf16(), std::ios::binary);
#else
        std::ifstream f(QFile::encodeName(archives[root].filePath()).data(), std::ios::binary);
#endif
        size_t size = 0;
        char *data = extractFile(*pkg, f, size);
        result = QByteArray(data, size);
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
