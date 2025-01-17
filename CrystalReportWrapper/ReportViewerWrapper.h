#pragma once

#include <vcclr.h> // For gcroot
#include "ReportViewerControl.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace CrystalReportWrapper;
using namespace System::Data;

public ref class ReportViewerWrapper {
public:
    ReportViewerWrapper() {
        userControl = gcnew ReportViewerControl();
    }

    IntPtr GetControlHandle() {
        return userControl->Handle;
    }

    void LoadReport(String^ reportPath, DataSet^ dataset, String^ server) {
        if (userControl->InvokeRequired) {
            userControl->Invoke(gcnew Action<String^, DataSet^, String^>(this, &ReportViewerWrapper::LoadReportImpl), reportPath, dataset, server);
        }
        else {
            LoadReportImpl(reportPath, dataset, server);
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
    void LoadReportImpl(String^ reportPath, DataSet^ dataset, String^ server) {
        System::Diagnostics::Debug::WriteLine("Attempting to load report: " + reportPath);
        if (String::IsNullOrEmpty(reportPath) || !System::IO::File::Exists(reportPath)) {
            System::Diagnostics::Debug::WriteLine("Invalid report path: " + reportPath);
            MessageBox::Show("Invalid report path: " + reportPath, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }
        try {
            userControl->LoadReport(reportPath, dataset, server);
            System::Diagnostics::Debug::WriteLine("Report loaded successfully: " + reportPath);
        }
        catch (Exception^ ex) {
            System::Diagnostics::Debug::WriteLine("Error loading report: " + ex->Message);
            MessageBox::Show(ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    void InitializeImpl(IntPtr hwndParent) {
        HWND hwndControl = static_cast<HWND>(userControl->Handle.ToPointer());
        ::SetParent(hwndControl, static_cast<HWND>(hwndParent.ToPointer()));
        ::SetWindowPos(hwndControl, nullptr, 0, 0, 800, 600, SWP_NOZORDER | SWP_NOACTIVATE);
    }

    ReportViewerControl^ userControl;
};
