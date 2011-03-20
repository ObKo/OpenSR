#include "PKGModel.h"
#include <QtGui>

using namespace Rangers;

PKGModel::PKGModel(PKGItem *root, QObject *parent)
        : QAbstractItemModel(parent)
{
    rootItem = root;
}

int PKGModel::columnCount(const QModelIndex & /* parent */) const
{
    return 1;
}

QVariant PKGModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    PKGItem *item = getItem(index);

    return QString::fromAscii(item->name);
}

Qt::ItemFlags PKGModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

PKGItem *PKGModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        PKGItem *item = static_cast<PKGItem*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}

QVariant PKGModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    //if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    //    return rootItem->data(section);

    return QVariant();
}

QModelIndex PKGModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    PKGItem *parentItem = getItem(parent);

    PKGItem *childItem = &parentItem->childs[row];
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex PKGModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    PKGItem *childItem = getItem(index);
    PKGItem *parentItem = childItem->parent;

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childCount, 0, parentItem);
}

int PKGModel::rowCount(const QModelIndex &parent) const
{
    PKGItem *parentItem = getItem(parent);

    return parentItem->childCount;
}
