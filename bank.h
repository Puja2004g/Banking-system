#ifndef BANK_H
#define BANK_H

#include <cppconn/connection.h>

class bank {
public:
    void createAccount(sql::Connection* con);
    void login(sql::Connection* con);
};

#endif
