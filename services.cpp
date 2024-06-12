#include <iostream>
#include <string>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "services.h"
#include "transaction.h"

using namespace std;

void services::withdraw(sql::Connection* con, long long accountno) {
    sql::PreparedStatement* pstmt;
    sql::PreparedStatement* pstmt_c;
    sql::ResultSet* res;

    con->setSchema("bankingManagement");

    long long amount;
    cout << "Enter the amount you want to withdraw: ";
    cin >> amount;

    pstmt = con->prepareStatement("SELECT balance FROM customerDetails WHERE accountno = ?");
    pstmt->setInt64(1, accountno);

    res = pstmt->executeQuery();

    if (res->next()) { // Check if there are any rows in the result set
        double amt = res->getDouble("balance");

        if (amt >= amount) {
            pstmt = con->prepareStatement("UPDATE customerDetails SET balance = balance - ? WHERE accountno = ?");
            pstmt->setDouble(1, amount);
            pstmt->setInt64(2, accountno);
            pstmt->execute();
            delete pstmt;

            // Fetch the updated balance
            pstmt = con->prepareStatement("SELECT balance FROM customerDetails WHERE accountno = ?");
            pstmt->setInt64(1, accountno);
            res = pstmt->executeQuery();
            double currentBal = 0;

            if (res->next()) {
                currentBal = res->getDouble("balance");
            }

            transaction trans;
            trans.getTransaction(con, accountno, "withdraw", amount, amt, currentBal);

            cout << "Amount withdrawal completed successfully" << endl;
        }
        else {
            cout << "Your account doesn't have enough money to withdraw" << endl;
        }
    }
    else {
        cout << "Account not found or no balance available" << endl;
    }

    delete res;
    delete pstmt;
}


void services::deposit(sql::Connection* con, long long accountno) {
    sql::PreparedStatement* pstmt;
    sql::PreparedStatement* pstmt_c;
    sql::ResultSet* res;

    con->setSchema("bankingManagement");

    long long amount;
    cout << "Enter the amount you want to deposit: ";
    cin >> amount;

    pstmt = con->prepareStatement("SELECT balance FROM customerDetails WHERE accountno = ?");
    pstmt->setInt64(1, accountno);

    res = pstmt->executeQuery();

    if (res->next()) { // Check if there are any rows in the result set
        double amt = res->getDouble("balance");

        if (amt >= amount) {
            pstmt = con->prepareStatement("UPDATE customerDetails SET balance = balance + ? WHERE accountno = ?");
            pstmt->setDouble(1, amount);
            pstmt->setInt64(2, accountno);
            pstmt->execute();

            // Fetch the updated balance
            pstmt = con->prepareStatement("SELECT balance FROM customerDetails WHERE accountno = ?");
            pstmt->setInt64(1, accountno);
            res = pstmt->executeQuery();
            double currentBal = 0;

            if (res->next()) {
                currentBal = res->getDouble("balance");
            }

            transaction trans;
            trans.getTransaction(con, accountno, "deposit", amount, amt, currentBal);

            cout << "Amount deposited completed successfully" << endl;
        }
    }
    else {
        cout << "Account not found" << endl;
    }

    delete res;
    delete pstmt;
}

void services::transfer(sql::Connection* con, long long accountno) {
    sql::PreparedStatement* pstmt;
    sql::PreparedStatement* pstmt_a;
    sql::ResultSet* res;
    sql::ResultSet* res_a;

    con->setSchema("bankingManagement");

    // Amount to transfer
    long long amount;
    cout << "Enter the amount you want to transfer: ";
    cin >> amount;

    // Account to transfer
    long long accno;
    cout << "Enter the account no. to transfer: ";
    cin >> accno;

    // Account from which amount will be transferred
    pstmt = con->prepareStatement("SELECT balance FROM customerDetails WHERE accountno = ?");
    pstmt->setInt64(1, accountno);

    // Account to which amount is transferred
    pstmt_a = con->prepareStatement("SELECT balance FROM customerDetails WHERE accountno = ?");
    pstmt_a->setInt64(1, accno);

    res = pstmt->executeQuery();
    res_a = pstmt_a->executeQuery();

    if (res->next()) {
        double amt = res->getDouble("balance");

        if (amt >= amount) {
            // Withdraw from source account
            pstmt = con->prepareStatement("UPDATE customerDetails SET balance = balance - ? WHERE accountno = ?");
            pstmt->setDouble(1, amount);
            pstmt->setInt64(2, accountno);
            pstmt->execute();

            // Log withdrawal transaction
            double newBal = amt - amount;
            transaction trans;
            trans.getTransaction(con, accountno, "withdraw", amount, amt, newBal);

            if (res_a->next()) {
                double amt_a = res_a->getDouble("balance");

                // Deposit to destination account
                pstmt_a = con->prepareStatement("UPDATE customerDetails SET balance = balance + ? WHERE accountno = ?");
                pstmt_a->setDouble(1, amount);
                pstmt_a->setInt64(2, accno);
                pstmt_a->execute();

                // Log deposit transaction
                double newBal_a = amt_a + amount;
                trans.getTransaction(con, accno, "deposit", amount, amt_a, newBal_a);

                cout << "Amount transferred successfully" << endl;
            }
            else {
                // Rollback if destination account is not found
                pstmt = con->prepareStatement("UPDATE customerDetails SET balance = balance + ? WHERE accountno = ?");
                pstmt->setDouble(1, amount);
                pstmt->setInt64(2, accountno);
                pstmt->execute();

                cout << "Destination account not found. Transfer rolled back." << endl;
            }
        }
        else {
            cout << "Your account doesn't have enough money to transfer" << endl;
        }
    }
    else {
        cout << "Account not found or no balance available" << endl;
    }

    delete res;
    delete pstmt;
    delete res_a;
    delete pstmt_a;
}

void services::checkBalance(sql::Connection* con, long long accountno) {
    sql::PreparedStatement* pstmt;
    sql::ResultSet* res;

    con->setSchema("bankingManagement");

    pstmt = con->prepareStatement("SELECT balance FROM customerDetails WHERE accountno = ?");
    pstmt->setInt64(1, accountno);

    res = pstmt->executeQuery();

    if (res->next()) {
        double amt = res->getDouble("balance");
        cout<<"Current balance: " << amt << endl;
    }
    else {
        cout << "Accout no. doesn't exist" << endl;
    }
}