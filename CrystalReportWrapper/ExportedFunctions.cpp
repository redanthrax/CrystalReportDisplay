#include "pch.h"  // Include precompiled header
#define EXPORT_DLL
#include "ExportedFunctions.h"
#include <vcclr.h>
#include "ReportViewerWrapper.h"
#include <thread>
#include <exception>  // Include the exception header
#include <comdef.h>   // For _com_error

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

void LoadReportThread(ReportViewerWrapperImpl* wrapper, const wchar_t* reportPath) {
    try {
        CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED); // Ensure COM is initialized
        wrapper->reportViewer->LoadReport(gcnew String(reportPath));
        CoUninitialize(); // Clean up COM
    }
    catch (const std::exception& e) {
        OutputDebugStringA(e.what());
        OutputDebugStringA("\n");
    }
    catch (_com_error& e) {
        _bstr_t bstrSource(e.Source());
        _bstr_t bstrDescription(e.Description());
        OutputDebugString(L"COM Error occurred:\n");
        OutputDebugString(bstrSource);
        OutputDebugString(L"\n");
        OutputDebugString(bstrDescription);
        OutputDebugString(L"\n");
    }
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
        wrapper->reportViewer->Initialize(IntPtr(hwndParent));
    }

    EXPORT_API void LoadReport(void* instance, const wchar_t* reportPath) {
        ReportViewerWrapperImpl* wrapper = static_cast<ReportViewerWrapperImpl*>(instance);
        std::thread loadReportThread(LoadReportThread, wrapper, reportPath);
        loadReportThread.detach();
    }
}
