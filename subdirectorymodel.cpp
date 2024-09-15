#include "subdirectorymodel.h"
#include <unordered_map>

SubDirectoryModel::SubDirectoryModel(QObject *parent)
{
    _subDirectores = nullptr;
}

//void SubDirectoryModel::addSubDirectory(const SubDirectory& subDirectory)
//{
//    beginInsertRows(QModelIndex(), rowCount(), rowCount());
//    _subDirectores.push_back(subDirectory);
//    endInsertRows();
//}

int SubDirectoryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !_subDirectores)
        return 0;

    return _subDirectores->items().size();
}

QVariant SubDirectoryModel::data(const QModelIndex &index, int role) const
{ 
    if (!index.isValid() || !_subDirectores)
        return QVariant();

    //===
    //const int myrow = index.row();
    //qDebug()<<"c++ row ===============================+++++===> "<<myrow;
    // AB.getMyRow(myrow);


    //auto item = (_subDirectores->items().begin());
   // std::advance(item, index.row());
    SubDirectoryItem& item = _subDirectores->items()[index.row()];

    if (role == NumRole)
        return QVariant::fromValue(item._Num);
    else if (role == CountryRole)
        return QString::fromStdString(item._Country);
    else if (role == NameRole)
        return QString::fromStdString(item._Name);
    else if (role == PhoneRole)
        return QString::fromStdString(item._Phone);
    else if (role == isInsertRole)
        return QVariant::fromValue(item._isInsert);
    return QVariant();
}

bool SubDirectoryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!_subDirectores || index.row() < 0)
        return false;

    SubDirectoryItem& item = _subDirectores->items()[index.row()];

    bool change = false;

    switch (role)
    {
    case CountryRole:
        item._Country = value.toString().toStdString();
        change = true;
        break;
    case NameRole:
        item._Name = value.toString().toStdString();
        change = true;
        break;
    case PhoneRole:
        item._Phone = value.toString().toStdString();
        change = true;
        break;
    case isInsertRole:
        item._isInsert = value.toBool();
        change = true;
        break;
    }

    if (change)
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

SubDirectory *SubDirectoryModel::list() const
{
    return _subDirectores;
}

void SubDirectoryModel::setList(SubDirectory *list)
{
    beginResetModel();

    if (_subDirectores)
        _subDirectores->disconnect(this);

    _subDirectores = list;

    if (_subDirectores)
    {
        connect(_subDirectores, &SubDirectory::editNum, this, [=](int i)
        {
            emit dataChanged(index(i), index(i), QVector<int>() << NumRole);
        });
    }

    endResetModel();
}

void SubDirectoryModel::updateDB(int index)
{
   // _subDirectores->updateDB(index);
}

void SubDirectoryModel::deleteItem(int index)
{
    if(!_subDirectores)
        return;

    int r = _subDirectores->deleteItem(index);

    beginRemoveRows(QModelIndex(), r, r);
    _subDirectores->items().erase(_subDirectores->items().begin() + r);
    endRemoveRows();
}

void SubDirectoryModel::addSubDirectory()
{
    if(!_subDirectores)
        return;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _subDirectores->Add(SubDirectoryItem{0, "", "", "", false});
    endInsertRows();

}


QHash<int, QByteArray> SubDirectoryModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NumRole] = "numb";
    roles[CountryRole] = "country";
    roles[NameRole] = "name";
    roles[PhoneRole] = "phone";
    roles[isInsertRole] = "isInsert";
    return roles;
}
