#pragma once
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <vector>
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>
#include <vcclr.h>
#include <System/Data.h>

class DatabaseHelper {
public:
    DatabaseHelper(const std::wstring& connectionString);
    ~DatabaseHelper();
    bool Connect();
    void Disconnect();
    System::Data::DataTable^ GetPersonData();

private:
    std::wstring m_connectionString;
    SQLHANDLE m_sqlEnvHandle;
    SQLHANDLE m_sqlConnectionHandle;
    bool m_connected;
};
