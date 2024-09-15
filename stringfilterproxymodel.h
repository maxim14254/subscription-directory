#ifndef STRINGFILTERPROXYMODEL_H
#define STRINGFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>


class StringFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(QString filterString READ filterString WRITE setFilterString)
    Q_PROPERTY(QObject *source READ source WRITE setSource)
    Q_PROPERTY(int count READ count)


public:

    StringFilterProxyModel(QObject *parent = nullptr);

    QString filterString() const;

    //QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QObject *source() const;

    void setSource(QObject *source);

signals:
    void showError(const QString&);

public slots:
    void setFilterString(const QString &filter);

    int count() const;

    void exportJSON(const QString & path);

protected:
    //int roleKey(const QByteArray &role) const;
    QHash<int, QByteArray> roleNames() const override;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
private:
    QString _filterString;
};

#endif // STRINGFILTERPROXYMODEL_H
