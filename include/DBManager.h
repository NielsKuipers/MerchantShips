//
// Created by Niels on 11/12/21.
//

#ifndef MERCHANTSHIPS_DBMANAGER_H
#define MERCHANTSHIPS_DBMANAGER_H

#include <sqlite/sqlite3.h>
#include <iostream>
#include <vector>
#include <string>

namespace DB
{
    static sqlite3 *db;

    static void connect()
    {
        int conn{sqlite3_open("../database/handelsschepen.db", &db)};

        if (conn)
            std::cout << "can't open db" << sqlite3_errmsg(db);
        else
            std::cout << "database connection success" << std::endl;
    }

    static std::vector<std::vector<std::string>> selectData(const std::string &query)
    {
        sqlite3_stmt *stmt{nullptr};
        sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        sqlite3_step(stmt);
        const int columns{sqlite3_data_count(stmt)};
        std::vector<std::vector<std::string>> result;

        while (sqlite3_column_text(stmt, 0))
        {
            std::vector<std::string> data{};

            for (int i = 0; i < columns; ++i)
                data.emplace_back((char *) sqlite3_column_text(stmt, i));

            sqlite3_step(stmt);
            result.emplace_back(data);
        }

        return result;
    }
}

#endif //MERCHANTSHIPS_DBMANAGER_H
