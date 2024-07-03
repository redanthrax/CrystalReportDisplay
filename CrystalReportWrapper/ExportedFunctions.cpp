#include "pch.h"  // Include precompiled header
#define EXPORT_DLL
#include "ExportedFunctions.h"
#include <vcclr.h>
#include "ReportViewerWrapper.h"
#include <thread>
#include <exception>  // Include the exception header
#include <Windows.h>  // For OutputDebugString and MessageBox
#include <fstream>

class ReportViewerWrapperImpl {
public:
    gcroot<ReportViewerWrapper^> reportViewer;

    ReportViewerWrapperImpl() {
        reportViewer = gcnew ReportViewerWrapper();
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

    // Convert std::wstring to System::String^
    System::String^ managedReportPath = gcnew System::String(reportPath.c_str());
    System::Diagnostics::Debug::WriteLine("LoadReportThread: " + managedReportPath);
    wrapper->reportViewer->LoadReport(managedReportPath);
    OutputDebugStringW(L"LoadReportThread finished\n");
    ::MessageBox(NULL, L"LoadReportThread finished", L"Debug", MB_OK);
}

extern "C" {
    EXPORT_API void* CreateReportViewer() {
        return new ReportViewerWrapperImpl();
    }

    EXPORT_API void DestroyReportViewer(void* instance) {
        delete static_cast<ReportViewerWrapperImpl*>(instance);
    }

    EXPORT_API void InitializeReportViewer(void* instance, HWND hwndParent) {
        ReportViewerWrapperImpl* wrapper = static_cast<ReportViewerWrapperImpl*>(instance);
        wrapper->reportViewer->Initialize(System::IntPtr(hwndParent));
    }

    EXPORT_API void LoadReport(void* instance, const wchar_t* reportPath) {
        OutputDebugStringW(L"LoadReport called\n");
        ::MessageBox(NULL, L"LoadReport called", L"Debug", MB_OK);
        ReportViewerWrapperImpl* wrapper = static_cast<ReportViewerWrapperImpl*>(instance);

        // Convert wchar_t* to std::wstring
        std::wstring reportPathStr(reportPath);
        std::thread loadReportThread(LoadReportThread, wrapper, reportPathStr);
        loadReportThread.detach();
    }
}
