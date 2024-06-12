#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <sstream>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "transaction.h"

using namespace std;

void transaction::getTransaction(sql::Connection* con, long long accountno, const string& transactionType, double amount, double prevBalance, double currBalance) {
    sql::PreparedStatement* pstmt;

    // Get current date/time in a suitable format for MySQL
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm localTime;
    localtime_s(&localTime, &currentTime);

    stringstream ss;
    ss << put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    string transactionDate = ss.str();

    pstmt = con->prepareStatement("INSERT INTO transactionHistory (AccountNo, TransactionDate, TransactionType, TransactionAmount, PreviousBalance, CurrentBalance) VALUES (?, ?, ?, ?, ?, ?)");
    pstmt->setInt64(1, accountno);
    pstmt->setString(2, transactionDate);
    pstmt->setString(3, transactionType);
    pstmt->setDouble(4, amount);
    pstmt->setDouble(5, prevBalance);
    pstmt->setDouble(6, currBalance);
    pstmt->execute();
    delete pstmt;
}
