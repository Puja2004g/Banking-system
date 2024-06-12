#pragma once
#ifndef	SERVICES_H
#define SERVICES_H

#include <cppconn/connection.h>

class services {
public:
	void withdraw(sql::Connection* con, long long accountno);
	void deposit(sql::Connection* con, long long accountno);
	void transfer(sql::Connection* con, long long accountno);
	void checkBalance(sql::Connection* con, long long accountno);
};

#endif