#include "trajs_model.h"

#include <QStringList>

TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
    return m_childItems.value(row);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::columnCount() const
{
    return m_itemData.count();
}

QVariant TreeItem::data(int column) const
{
    return m_itemData.value(column);
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

bool TreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= m_itemData.size())
        return false;

    m_itemData[column] = value;
    return true;
}



TrajsModel::TrajsModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "lat" << "lon";
    rootItem = new TreeItem(rootData);
    setupModelData(data.split(QString("\n")), rootItem);
}

TrajsModel::~TrajsModel()
{
    delete rootItem;
}

int TrajsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant TrajsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = getItem(index);

    return item->data(index.column());
}

Qt::ItemFlags TrajsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return  Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

TreeItem *TrajsModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

QVariant TrajsModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex TrajsModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TrajsModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TrajsModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool TrajsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    //BaseTrajectory *t = static_cast<BaseTrajectory>value;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);
    result &= item->setData(index.column() + 1, value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool TrajsModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void TrajsModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    int position = 0;
    int number = 0;

    parents << parent;
    indentations << 0;

    QList<QVariant> columnData, columnData2;

    columnData << "Traj 1" << "";
    columnData2 << 11.2 << 22.3;

    position = 2;

    indentations << position;

    parents.last()->appendChild(new TreeItem(columnData, parents.last()));

    parents << parents.last()->child(parents.last()->childCount()-1);

    parents.last()->appendChild(new TreeItem(columnData2, parents.last()));

    ++number;


    /*
    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new TreeItem(columnData, parents.last()));
        }

        ++number;
    }
    */
}
