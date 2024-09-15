#include "subdirectory.h"
#include <QGuiApplication>
#include <QFile>
#include <sqlite3.h>
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;


sqlite3* DB = nullptr;
sqlite3_stmt* stmt = nullptr;

SubDirectory::SubDirectory(const QString& path, QObject *parent)
{
    std::string dbDir = fs::path(path.toStdString()).remove_filename().string() + "db";
    std::string dbPath = fs::path(path.toStdString()).remove_filename().string() + "db\\database.db";

    QFile Fout(dbDir.c_str());

    if(!Fout.exists())
    {
        std::string dbDir2 = "mkdir "+ fs::path(path.toStdString()).remove_filename().string() + "db";
        system(dbDir2.c_str());
    }

    if(sqlite3_open(dbPath.data(), &DB) == SQLITE_OK)
    {
        sqlite3_prepare_v2(DB, "CREATE TABLE IF NOT EXISTS people (id INTEGER PRIMARY KEY, country TEXT CHECK( country IN ('США','Россия','Китай','Германия') ) , name TEXT, phone TEXT)",  -1, &stmt, NULL);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        int r = sqlite3_prepare_v2(DB, "SELECT id, country, name, phone FROM people",  -1, &stmt, NULL);

        if(r == SQLITE_OK)
        {
            while ((r = sqlite3_step(stmt)) == SQLITE_ROW)
            {
                _Items.push_back(SubDirectoryItem{(size_t)sqlite3_column_int(stmt, 0), (char*)sqlite3_column_text(stmt, 1), (char*)sqlite3_column_text(stmt, 2), (char*)sqlite3_column_text(stmt, 3), true});
            }

            sqlite3_finalize(stmt);
        }
    }
    else
        DB = nullptr;
}

SubDirectory::~SubDirectory()
{
    if(DB)
        sqlite3_close(DB);
}

std::vector<SubDirectoryItem>& SubDirectory::items()
{
    return  _Items;
}

void SubDirectory::Add(SubDirectoryItem &&item)
{
    _Items.push_back(std::move(item));
}

void SubDirectory::updateDB(int index)
{
    if(DB)
    {
        int r = sqlite3_prepare_v2(DB, "UPDATE people SET country = ?, name = ?, phone = ? WHERE id = ?",  -1, &stmt, NULL);

        sqlite3_bind_text(stmt, 1, _Country.toStdString().c_str(), _Country.toStdString().size(), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, _Name.toStdString().c_str(), _Name.toStdString().size(), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, _Phone.toStdString().c_str(), _Phone.toStdString().size(), SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, index);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if(r != SQLITE_OK)
            emit showError("Данные не удалось обновить");
    }
    else
    {
        emit showError("Отсутствует подключение к БД");
    }

}

void SubDirectory::insertToDB()
{
    if(DB)
    {
        int r = sqlite3_prepare_v2(DB, "INSERT INTO people(country, name, phone) VALUES (?,?,?)",  -1, &stmt, NULL);

        sqlite3_bind_text(stmt, 1, _Country.toStdString().c_str(), _Country.toStdString().size(), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, _Name.toStdString().c_str(), _Name.toStdString().size(), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, _Phone.toStdString().c_str(), _Phone.toStdString().size(), SQLITE_TRANSIENT);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if(r != SQLITE_OK)
        {
            emit showError("Данные не удалось добавить");
            return;
        }

        r = sqlite3_prepare_v2(DB, "SELECT id FROM people WHERE id=(SELECT max(id) FROM people);",  -1, &stmt, NULL);

        if(r == SQLITE_OK)
        {
            while ((r = sqlite3_step(stmt)) == SQLITE_ROW)
            {
                _Items.back()._Num = (size_t)sqlite3_column_int(stmt, 0);
                //emit editNum(_Items.size() - 1);
                //_Items.push_back(SubDirectoryItem{(size_t)sqlite3_column_int(stmt, 0), (char*)sqlite3_column_text(stmt, 1), (char*)sqlite3_column_text(stmt, 2), (char*)sqlite3_column_text(stmt, 3), true});
            }

            sqlite3_finalize(stmt);
        }
    }
    else
    {
        emit showError("Отсутствует подключение к БД");
    }

}

int SubDirectory::deleteItem(int index)
{
    if(DB)
    {
        int r = sqlite3_prepare_v2(DB, "DELETE FROM people WHERE id = ?",  -1, &stmt, NULL);
        sqlite3_bind_int(stmt, 1, index);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        if(r != SQLITE_OK)
        {
            emit showError("Данные не удалось удалить");
            return -1;
        }

        int it = 0;
        for(; it < _Items.size(); ++it)
        {
            if(_Items[it]._Num == index)
            {
                break;
            }
        }

        return it;
    }
    else
    {
        emit showError("Отсутствует подключение к БД");
        return -1;
    }
}

void SubDirectory::setCountry(const QString& country)
{
    _Country = country;
}

void SubDirectory::setName(const QString& name)
{
    _Name = name;
}

void SubDirectory::setPhone(const QString& phone)
{
    _Phone = phone;
}

//void SubDirectory::setisInsert(bool isInsert, int i)
//{
//    _Items[i]._isInsert = isInsert;
//}

