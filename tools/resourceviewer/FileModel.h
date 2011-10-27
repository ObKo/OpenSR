#ifndef PKGMODEL_H
#define PKGMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <libRanger.h>
#include <QList>
#include <QFileInfo>
#include <QMap>
#include <QFileIconProvider>

namespace Rangers
{
enum NodeType {NODE_FILE, NODE_PKG, NODE_RPKG};
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
};

#endif // PKGMODEL_H
