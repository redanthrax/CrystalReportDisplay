#pragma once

#include <vcclr.h> // For gcroot
#include "ReportViewerControl.h"
#include <thread>
#include <windows.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace CrystalReportWrapper;

public ref class ReportViewerWrapper {
public:
    ReportViewerWrapper() {
        userControl = gcnew ReportViewerControl();
    }

    IntPtr GetControlHandle() {
        return userControl->Handle;
    }

    void LoadReport(String^ reportPath) {
        // Ensure the call is made on the UI thread
        if (userControl->InvokeRequired) {
            userControl->Invoke(gcnew Action<String^>(this, &ReportViewerWrapper::LoadReportImpl), reportPath);
        }
        else {
            LoadReportImpl(reportPath);
        }
    }

    void Initialize(IntPtr hwndParent) {
        // Ensure the call is made on the UI thread
        if (userControl->InvokeRequired) {
            userControl->Invoke(gcnew Action<IntPtr>(this, &ReportViewerWrapper::InitializeImpl), hwndParent);
        }
        else {
            InitializeImpl(hwndParent);
        }
    }

private:
    void LoadReportImpl(String^ reportPath) {
        userControl->LoadReport(reportPath);
    }

    void InitializeImpl(IntPtr hwndParent) {
        HWND hwndControl = static_cast<HWND>(userControl->Handle.ToPointer());
        ::SetParent(hwndControl, static_cast<HWND>(hwndParent.ToPointer()));
        ::SetWindowPos(hwndControl, nullptr, 0, 0, 800, 600, SWP_NOZORDER | SWP_NOACTIVATE);
    }

    ReportViewerControl^ userControl;
};
