#pragma once
#ifndef	TRANSACTION_H
#define TRANSACTION_H
#include <cppconn/connection.h>
#include <string>
using namespace std;

class transaction {
public:
	void getTransaction(sql::Connection* con, long long accountno, const string& transactionType, double amount, double prevBalance, double currBalance);
};

#endif