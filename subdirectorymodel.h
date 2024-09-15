#ifndef SUBDIRECTORYMODEL_H
#define SUBDIRECTORYMODEL_H

#include <QAbstractListModel>
#include "subdirectory.h"



class SubDirectoryModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(SubDirectory *list READ list WRITE setList)

public:
    enum SubDirectoryRoles {
        NumRole = Qt::UserRole + 1,
        CountryRole = 258,
        NameRole = 259,
        PhoneRole = 260,
        isInsertRole = 261
    };

    SubDirectoryModel(QObject *parent = nullptr);

    //void addSubDirectory(const SubDirectory& subDirectory);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    //bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    SubDirectory *list() const;

    void setList(SubDirectory *list);

signals:
//    void preItemRemoved(int index);
//    void postItemRemoved();

public slots:
    void addSubDirectory();

    void updateDB(int index);

    void deleteItem(int index);


protected:
    QHash<int, QByteArray> roleNames() const override;
private:
    SubDirectory* _subDirectores;
};

#endif // SUBDIRECTORYMODEL_H
