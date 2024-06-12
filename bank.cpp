#include <iostream>
#include <string>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "bank.h"
#include "services.h"

using namespace std;

void bank::createAccount(sql::Connection* con) {
    sql::PreparedStatement* pstmt;
    int AccountNo;
    string name;
    string email;
    string pass;
    double balance;

    con->setSchema("bankingManagement");

    pstmt = con->prepareStatement("INSERT INTO customerDetails(AccountNo, name, email, password, balance) VALUES(?,?,?,?,?)");

    cout << "Enter account number: ";
    cin >> AccountNo;
    cin.ignore(); // Clear input buffer
    cout << "Enter account name: ";
    getline(cin, name);
    cout << "Enter your email: ";
    getline(cin, email);
    cout << "Create password (minimum 8 characters, maximum 10 characters): ";
    getline(cin, pass);
    while (pass.length() < 8 || pass.length() > 10) {
        cout << "Password must be between 8 and 10 characters. Try again: ";
        getline(cin, pass);
    }
    cout << "Enter your current balance: ";
    cin >> balance;

    pstmt->setInt64(1, AccountNo);
    pstmt->setString(2, name);
    pstmt->setString(3, email);
    pstmt->setString(4, pass);
    pstmt->setDouble(5, balance);
    pstmt->executeUpdate();

    cout << "Account created successfully" << endl;

    delete pstmt;
}

void bank::login(sql::Connection* con) {
    sql::PreparedStatement* pstmt;
    sql::ResultSet* res;

    con->setSchema("bankingManagement");

    string name;
    string email;
    string pass;
    string password;

    long long accountNo;
    cout << "Enter account number: ";
    cin >> accountNo;
    cin.ignore(); // Clear input buffer
    cout << "Enter your email: ";
    getline(cin, email);

    pstmt = con->prepareStatement("SELECT password FROM customerDetails WHERE AccountNo = ? AND email=?");
    pstmt->setInt64(1, accountNo);
    pstmt->setString(2, email);

    res = pstmt->executeQuery();
    if (res->next()) {
        password = res->getString("password");
    }
    else {
        cout << "Account number not found." << endl;
        delete res;
        delete pstmt;
        return;
    }

    delete res;

    int attempts = 0;
    while (attempts < 3) {
        cout << "Enter password: ";
        getline(cin, pass);
        if (pass != password) {
            cout << "Wrong password" << endl;
            attempts++;
        }
        else {
            cout << "Password matched!!!" << endl;
            while (true) {
                cout << "What do you want to do: " << endl;
                cout << "1. Check balance" << endl <<
                    "2. Deposit money" << endl <<
                    "3. Transfer money" << endl <<
                    "4. Withdraw money" << endl <<
                    "5. exit" << endl;
                services service;
                int ch = 0;
                cin >> ch;
                switch (ch) {
                case 1:
                    service.checkBalance(con, accountNo);
                    break;

                case 2:
                    service.deposit(con, accountNo);
                    break;

                case 3:
                    service.transfer(con, accountNo);
                    break;

                case 4:
                    service.withdraw(con, accountNo);
                    break;

                default:
                    return;
                }
            }

            break;
        }
    }

    if (attempts == 3) {
        cout << "Too many incorrect attempts. Exiting." << endl;
    }

    delete pstmt;
}
