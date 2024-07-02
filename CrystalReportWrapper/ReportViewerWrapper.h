#pragma once

#include <vcclr.h> // For gcroot
#include "ReportViewerControl.h"

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
        try {
            System::Diagnostics::Debug::WriteLine("Loading report: " + reportPath);
            userControl->LoadReport(reportPath);
            System::Diagnostics::Debug::WriteLine("Report loaded successfully: " + reportPath);
        }
        catch (Exception^ ex) {
            System::Diagnostics::Debug::WriteLine("Error loading report: " + ex->Message);
            MessageBox::Show(ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    void Initialize(IntPtr hwndParent) {
        if (userControl->InvokeRequired) {
            userControl->Invoke(gcnew Action<IntPtr>(this, &ReportViewerWrapper::InitializeImpl), hwndParent);
        }
        else {
            InitializeImpl(hwndParent);
        }
    }

private:
    void InitializeImpl(IntPtr hwndParent) {
        HWND hwndControl = static_cast<HWND>(userControl->Handle.ToPointer());
        ::SetParent(hwndControl, static_cast<HWND>(hwndParent.ToPointer()));
        ::SetWindowPos(hwndControl, nullptr, 0, 0, 800, 600, SWP_NOZORDER | SWP_NOACTIVATE);
    }

    ReportViewerControl^ userControl;
};
