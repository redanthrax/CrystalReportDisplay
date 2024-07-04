#include "pch.h"
#define EXPORT_DLL
#include "ExportedFunctions.h"
#include "ManagedWrapper.h"
#include <vcclr.h>
#include <thread>
#include <exception>
#include <Windows.h>
#include <fstream>

using namespace System::Data;

class ReportViewerWrapperImpl {
public:
    gcroot<ManagedWrapper^> reportViewer;

    ReportViewerWrapperImpl() {
        reportViewer = gcnew ManagedWrapper();
    }

    ~ReportViewerWrapperImpl() {
        delete reportViewer;
    }
};

void LoadReportThread(ReportViewerWrapperImpl* wrapper, std::wstring reportPath) {
    if (!IsDebuggerPresent()) {
        DebugBreak();
    }

    OutputDebugStringW(L"LoadReportThread started\n");
    ::MessageBox(NULL, L"LoadReportThread started", L"Debug", MB_OK);

    System::String^ managedReportPath = gcnew System::String(reportPath.c_str());
    wrapper->reportViewer->LoadReport(managedReportPath);
    OutputDebugStringW(L"LoadReportThread finished\n");
    ::MessageBox(NULL, L"LoadReportThread finished", L"Debug", MB_OK);
}

extern "C" {
    EXPORT_API void* __stdcall CreateReportViewer() {
        return new ReportViewerWrapperImpl();
    }

    EXPORT_API void __stdcall DestroyReportViewer(void* instance) {
        delete static_cast<ReportViewerWrapperImpl*>(instance);
    }

    EXPORT_API void __stdcall InitializeReportViewer(void* instance, HWND hwndParent) {
        ReportViewerWrapperImpl* wrapper = static_cast<ReportViewerWrapperImpl*>(instance);
        wrapper->reportViewer->Initialize(System::IntPtr(hwndParent));
    }

    EXPORT_API void __stdcall LoadReport(void* instance, const wchar_t* reportPath) {
        OutputDebugStringW(L"LoadReport called\n");
        ::MessageBox(NULL, L"LoadReport called", L"Debug", MB_OK);
        ReportViewerWrapperImpl* wrapper = static_cast<ReportViewerWrapperImpl*>(instance);
        std::wstring reportPathStr(reportPath);
        std::thread loadReportThread(LoadReportThread, wrapper, reportPathStr);
        loadReportThread.detach();
    }

    EXPORT_API void __stdcall SetCredentials(void* instance, const wchar_t* server) {
        ReportViewerWrapperImpl* wrapper = static_cast<ReportViewerWrapperImpl*>(instance);
        wrapper->reportViewer->SetCredentials(gcnew String(server));
    }

    EXPORT_API void __stdcall CreateDataSet(void* instance) {
        ReportViewerWrapperImpl* wrapper = static_cast<ReportViewerWrapperImpl*>(instance);
        wrapper->reportViewer->CreateDataSet();
    }

    EXPORT_API void __stdcall AddTable(void* instance, const wchar_t* tableName) {
        ReportViewerWrapperImpl* wrapper = static_cast<ReportViewerWrapperImpl*>(instance);
        wrapper->reportViewer->AddTable(gcnew String(tableName));
    }

    EXPORT_API void __stdcall AddColumn(void* instance, const wchar_t* tableName, const wchar_t* columnName, int type) {
        ReportViewerWrapperImpl* wrapper = static_cast<ReportViewerWrapperImpl*>(instance);
        Type^ columnType = nullptr;
        switch (type) {
        case 0:
            columnType = String::typeid;
            break;
        case 1:
            columnType = Int32::typeid;
            break;
            // Add more cases if needed
        default:
            columnType = String::typeid;
        }
        wrapper->reportViewer->AddColumn(gcnew String(tableName), gcnew String(columnName), columnType);
    }

    EXPORT_API void __stdcall AddRow(void* instance, const wchar_t* tableName, const wchar_t* productName) {
        ReportViewerWrapperImpl* wrapper = static_cast<ReportViewerWrapperImpl*>(instance);
        array<Object^>^ values = gcnew array<Object^>(1) { gcnew String(productName) };
        wrapper->reportViewer->AddRow(gcnew String(tableName), values);
    }
}
