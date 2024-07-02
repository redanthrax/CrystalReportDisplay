#pragma once

#include <windows.h>

#ifdef EXPORT_DLL
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#endif

extern "C" {
    EXPORT_API void* CreateReportViewer();
    EXPORT_API void DestroyReportViewer(void* instance);
    EXPORT_API void InitializeReportViewer(void* instance, HWND hwndParent);
    EXPORT_API void LoadReport(void* instance, const wchar_t* reportPath);
}