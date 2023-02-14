#include "Libraries/Database/Database.hpp"

#include "Libraries/Log/LogPlus.hpp"

Database *Database::kSelf = nullptr;

Database::Database() :
                        database_(nullptr) {

}

Database *
Database::GetInstance() {
    if (kSelf == nullptr)
        kSelf = new Database();
        
    return kSelf;
}

int 
Database::Open(std::string pathname) {
    int ret;

    ret = sqlite3_open(pathname.c_str(), &database_);
    if (ret != 0) {
        LOG_ERRO("Failed to open '%s' database, err: %s", pathname.c_str(), sqlite3_errmsg(database_));
        return -1;
    }

    return 0; 
}

void 
Database::Close() {
    sqlite3_close(database_);
}

int 
Database::Execute(const std::string& statement, 
                    int (*callback)(void *user_data, 
                                    int num, 
                                    char **values, 
                                    char **column),
                    void *user_data) {
    int ret;
    char *error;

    ret = sqlite3_exec(database_, statement.c_str(), callback, user_data, &error);
    if (ret != SQLITE_OK) {
        LOG_ERRO("Failed to exec '%s', err: %s", statement.c_str(), error);
        return -1;
    }

    return 0;
}



