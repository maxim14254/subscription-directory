#include "stringfilterproxymodel.h"
#include "subdirectorymodel.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDir>


StringFilterProxyModel::StringFilterProxyModel(QObject *parent)
{
//    connect(this, &QSortFilterProxyModel::rowsInserted, this, &StringFilterProxyModel::countChanged);
//    connect(this, &QSortFilterProxyModel::rowsRemoved, this, &StringFilterProxyModel::countChanged);
}

QString StringFilterProxyModel::filterString() const
{
    return _filterString;
}

//QVariant StringFilterProxyModel::data(const QModelIndex &index, int role) const
//{
//    if (QAbstractItemModel *source = sourceModel())
//        return source->data(index, role);
//    return QVariant();
//}


//int StringFilterProxyModel::rowCount(const QModelIndex &parent) const
//{
//    if (QAbstractItemModel *source = sourceModel())
//        return source->rowCount(parent);
//    return 0;
//}


QObject *StringFilterProxyModel::source() const
{
    return sourceModel();
}

void StringFilterProxyModel::setSource(QObject *source)
{
    setSourceModel(qobject_cast<QAbstractItemModel*>(source));

//    connect(static_cast<SubDirectoryModel*>(source), &SubDirectoryModel::preItemRemoved, this, [=](int index) {
//        beginRemoveRows(QModelIndex(), index, index);
//    });
//    connect(static_cast<SubDirectoryModel*>(source), &SubDirectoryModel::postItemRemoved, this, [=]() {
//        endRemoveRows();
//    });

}

void StringFilterProxyModel::setFilterString(const QString &filter)
{
    _filterString = filter;
    //setFilterRegExp(QRegExp(filter, filterCaseSensitivity(), QRegExp::PatternSyntax::Wildcard));
    invalidateFilter();
}

int StringFilterProxyModel::count() const
{
    return rowCount();
}

void StringFilterProxyModel::exportJSON(const QString& path)
{
    const QUrl url(path);
    QString folder;

    if (url.isLocalFile())
        folder= QDir::toNativeSeparators(url.toLocalFile());
    else
        folder = path;

    QFile file(folder + "\\export_js.json");

    if(!file.open(QIODevice::ReadWrite))
    {
        emit showError("Не удалось создать файл export_js.json");
        return;
    }

    file.resize(0);

    QJsonArray jsonArray;

    for(int i = 0; i < count(); ++i)
    {
        QModelIndex item = index(i, 0);

        QString country = data(item, 258).toString();
        QString name = data(item, 259).toString();
        QString phone = data(item, 260).toString();

        QJsonObject jsonObject;

        jsonObject.insert("Country", std::move(country));
        jsonObject.insert("Name", std::move(name));
        jsonObject.insert("Phone", std::move(phone));

        jsonArray.push_back(std::move(jsonObject));
    }

    QJsonDocument jsonDoc;
    jsonDoc.setArray(std::move(jsonArray));
    file.write(std::move(jsonDoc.toJson()));
    file.close();
}


QHash<int, QByteArray> StringFilterProxyModel::roleNames() const
{
    if (QAbstractItemModel *source = sourceModel())
        return source->roleNames();
    return QHash<int, QByteArray>();
}

bool StringFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QRegExp rx = filterRegExp();

    if (_filterString.isEmpty())
        return true;

    QAbstractItemModel *model = sourceModel();

    QHash<int, QByteArray> roles = roleNames();
    for (auto it = roles.cbegin(); it != roles.cend(); ++it)
    {
        if(it.key() == 261)
            continue;

        QModelIndex sourceIndex = model->index(sourceRow, 0, sourceParent);
        QString key = model->data(sourceIndex, it.key()).toString();

        if (key.contains(_filterString))
            return true;
    }

    return false;
}
