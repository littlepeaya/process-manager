#ifndef LIBRARIES_DATABASE_HPP
#define LIBRARIES_DATABASE_HPP

#include <sqlite3.h>

#include <string>

class Database {
public:
    Database(const Database &other) = delete;

    void operator=(const Database &other) = delete;

    static Database *GetInstance();

    int Open(std::string pathname);
    void Close();

    int Execute(const std::string& statement, 
                int (*callback)(void *user_data, 
                                int num, 
                                char **values, 
                                char **column), 
                void *user_data);

private:
    static Database *kSelf;
    sqlite3 *database_;

    Database();

};


#endif //LIBRARIES_DATABASE_HPP