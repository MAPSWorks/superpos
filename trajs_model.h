#ifndef TRAJS_MODEL_H
#define TRAJS_MODEL_H

#include <QList>
#include <QVariant>

#include <QAbstractItemModel>
#include <QModelIndex>

#include "trajectory.h"

class TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();

    bool setData(int column, const QVariant &value);

private:
    QList<TreeItem*> m_childItems;
    QList<QVariant> m_itemData;
    TreeItem *m_parentItem;
};


class TrajsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TrajsModel(const QString &data, QObject *parent = 0);
    ~TrajsModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

private:
    void setupModelData(const QStringList &lines, TreeItem *parent);
    TreeItem *getItem(const QModelIndex &index) const;

    TreeItem *rootItem;

    Trajectories trajs;
};

#endif // TRAJS_MODEL_H
