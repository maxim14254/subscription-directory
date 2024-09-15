#ifndef SUBDIRECTORY_H
#define SUBDIRECTORY_H

#include <string>
#include <QObject>
#include <map>


struct SubDirectoryItem
{
    size_t _Num;
    std::string _Country;
    std::string _Name;
    std::string _Phone;
    bool _isInsert;
};


class SubDirectory : public QObject
{
    Q_OBJECT
public:

    Q_PROPERTY(QString country WRITE setCountry)
    Q_PROPERTY(QString name WRITE setName)
    Q_PROPERTY(QString phone WRITE setPhone)


    SubDirectory(const QString& path, QObject *parent = nullptr);
    ~SubDirectory();
    //SubDirectory(const int Num, const std::string& Country, const std::string& Name, const std::string& Phone, bool isInsert);

    std::vector<SubDirectoryItem> &items();

    void Add(SubDirectoryItem&& item);

    Q_INVOKABLE void updateDB(int index);
    Q_INVOKABLE void insertToDB();

    int deleteItem(int index);

    void setCountry(const QString& country);
    void setName(const QString& name);
    void setPhone(const QString& phone);


signals:
    void showError(const QString&);
    void editNum(int);

private:
    std::vector<SubDirectoryItem> _Items;

    QString _Country;
    QString _Name;
    QString _Phone;

};

#endif // SUBDIRECTORY_H
