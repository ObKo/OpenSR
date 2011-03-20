#ifndef PKGMODEL_H
#define PKGMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <libRanger.h>

namespace Rangers
{
class PKGModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    PKGModel(PKGItem *root, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    PKGItem *rootItem;
    PKGItem *getItem(const QModelIndex &index) const;
};
};

#endif // PKGMODEL_H
