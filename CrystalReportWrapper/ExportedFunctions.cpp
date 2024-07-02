#include "pch.h"  // Include precompiled header
#define EXPORT_DLL
#include "ExportedFunctions.h"
#include <vcclr.h>
#include "ReportViewerWrapper.h"
#include <thread>
#include <exception>  // Include the exception header

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
    wrapper->reportViewer->LoadReport(gcnew System::String(reportPath));
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
        ReportViewerWrapperImpl* wrapper = static_cast<ReportViewerWrapperImpl*>(instance);
        std::thread loadReportThread(LoadReportThread, wrapper, reportPath);
        loadReportThread.detach();
    }
}
