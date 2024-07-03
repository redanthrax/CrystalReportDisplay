#include "DatabaseHelper.h"
#include <iostream>

DatabaseHelper::DatabaseHelper(const std::wstring& connectionString)
    : m_connectionString(connectionString), m_sqlEnvHandle(NULL), m_sqlConnectionHandle(NULL), m_connected(false) {}

DatabaseHelper::~DatabaseHelper() {
    Disconnect();
}

bool DatabaseHelper::Connect() {
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_sqlEnvHandle))
        return false;

    if (SQL_SUCCESS != SQLSetEnvAttr(m_sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0))
        return false;

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, m_sqlEnvHandle, &m_sqlConnectionHandle))
        return false;

    SQLWCHAR retConString[1024];
    switch (SQLDriverConnect(m_sqlConnectionHandle,
        NULL,
        (SQLWCHAR*)m_connectionString.c_str(),
        SQL_NTS,
        retConString,
        1024,
        NULL,
        SQL_DRIVER_NOPROMPT)) {
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        m_connected = true;
        return true;
    case SQL_INVALID_HANDLE:
    case SQL_ERROR:
    default:
        return false;
    }
}

void DatabaseHelper::Disconnect() {
    if (m_connected) {
        SQLDisconnect(m_sqlConnectionHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, m_sqlConnectionHandle);
        SQLFreeHandle(SQL_HANDLE_ENV, m_sqlEnvHandle);
        m_connected = false;
    }
}

System::Data::DataTable^ DatabaseHelper::GetPersonData() {
    System::Data::DataTable^ datatable = gcnew System::Data::DataTable("Person");

    if (!m_connected)
        return datatable;

    SQLHANDLE sqlStatementHandle;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, m_sqlConnectionHandle, &sqlStatementHandle))
        return datatable;

    if (SQL_SUCCESS != SQLExecDirect(sqlStatementHandle, (SQLWCHAR*)L"SELECT FirstName, LastName FROM Person", SQL_NTS)) {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStatementHandle);
        return datatable;
    }

    SQLWCHAR firstName[256], lastName[256];
    SQLINTEGER age;
    SQLLEN indicator;

    datatable->Columns->Add("FirstName", System::Type::GetType("System.String"));
    datatable->Columns->Add("LastName", System::Type::GetType("System.String"));

    while (SQLFetch(sqlStatementHandle) == SQL_SUCCESS) {
        SQLGetData(sqlStatementHandle, 1, SQL_C_WCHAR, firstName, sizeof(firstName), &indicator);
        SQLGetData(sqlStatementHandle, 2, SQL_C_WCHAR, lastName, sizeof(lastName), &indicator);
        SQLGetData(sqlStatementHandle, 3, SQL_C_LONG, &age, 0, &indicator);

        System::Data::DataRow^ row = datatable->NewRow();
        row["FirstName"] = gcnew System::String(firstName);
        row["LastName"] = gcnew System::String(lastName);
        datatable->Rows->Add(row);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, sqlStatementHandle);
    return datatable;
}
