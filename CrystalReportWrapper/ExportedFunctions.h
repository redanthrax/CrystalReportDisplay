#pragma once

#ifdef EXPORT_DLL
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif

#include <Windows.h>

extern "C" {
    EXPORT_API void* __stdcall CreateReportViewer();
    EXPORT_API void __stdcall DestroyReportViewer(void* instance);
    EXPORT_API void __stdcall InitializeReportViewer(void* instance, HWND hwndParent);
    EXPORT_API void __stdcall LoadReport(void* instance, const wchar_t* reportPath);
    EXPORT_API void __stdcall CreateDataSet(void* instance);
    EXPORT_API void __stdcall AddTable(void* instance, const wchar_t* tableName);
    EXPORT_API void __stdcall AddColumn(void* instance, const wchar_t* tableName, const wchar_t* columnName, int type);
    EXPORT_API void __stdcall AddRow(void* instance, const wchar_t* tableName, const wchar_t* productName);
    EXPORT_API void __stdcall SetDatabaseLogon(void* instance, const wchar_t* server, const wchar_t* database, const wchar_t* userId, const wchar_t* password);
}
