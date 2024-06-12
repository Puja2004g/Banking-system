#include <stdlib.h>
#include <iostream>
#include <string>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "bank.h"

using namespace std;

// for demonstration only. never save your password in the code!
const string server = "tcp://127.0.0.1:3308";
const string username = "root";
const string password = "";

void createDatabase() {
    sql::Driver* driver;
    sql::Connection* con;
    sql::Statement* stmt;

    try
    {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);

        // Create database if not exists
        stmt = con->createStatement();
        stmt->execute("CREATE DATABASE IF NOT EXISTS bankingManagement");
        delete stmt;

        con->setSchema("bankingManagement");

        // Create tables
        stmt = con->createStatement();
        stmt->execute("DROP TABLE IF EXISTS customerDetails");
        cout << "Finished dropping table (if existed)" << endl;
        stmt->execute("CREATE TABLE customerDetails("
            "SNo SERIAL PRIMARY KEY, "
            "AccountNo BIGINT, "
            "name VARCHAR(50), "
            "email VARCHAR(50), "
            "password VARCHAR(20), "
            "balance DECIMAL(20,2));");

        stmt->execute("CREATE TABLE transactionHistory ("
            "SNo SERIAL PRIMARY KEY, "
            "AccountNo BIGINT, "
            "TransactionDate DATE, "
            "TransactionType VARCHAR(20), "
            "TransactionAmount DECIMAL(20,2), "
            "PreviousBalance DECIMAL(20,2), "
            "CurrentBalance DECIMAL(20,2));");

        cout << "Finished creating both tables" << endl;
        delete stmt;
        delete con;
    }
    catch (sql::SQLException& e)
    {
        cout << "SQL Exception: " << e.what() << endl;
        cout << "MySQL error code: " << e.getErrorCode() << endl;
        cout << "SQLState: " << e.getSQLState() << endl;
        system("pause");

    }
    catch (exception& e)
    {
        cout << "Standard exception: " << e.what() << endl;
        system("pause");
    }

    system("pause");
}

int main()
{
    sql::Driver* driver;
    sql::Connection* con;

    try
    {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);

        //createDatabase();

        bank obj;

        while (true) {
            int ch=0;
            cout << "1. Create account" << endl <<
                "2. Log In into your account" << endl<<
                "3. Exit"<<endl;

            cin >> ch;

            switch (ch) {
            case 1:
                obj.createAccount(con);
                break;

            case 2:
                obj.login(con);
                break;

            default:
                cout << "Exiting..." << endl;
                return 0;
            }
        }

        delete con;
    }
    catch (sql::SQLException& e)
    {
        cout << "SQL Exception: " << e.what() << endl;
        cout << "MySQL error code: " << e.getErrorCode() << endl;
        cout << "SQLState: " << e.getSQLState() << endl;
    }

    return 0;
}
