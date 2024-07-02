// ReportViewerControl.h
#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace CrystalDecisions::Windows::Forms;

namespace CrystalReportWrapper {
    public ref class ReportViewerControl : public UserControl {
    public:
        ReportViewerControl() {
            InitializeComponent();
        }

        void LoadReport(String^ reportPath) {
            System::Diagnostics::Debug::WriteLine("Setting report source: " + reportPath);
            try {
                this->crystalReportViewer->ReportSource = reportPath;
                System::Diagnostics::Debug::WriteLine("Report source set successfully: " + reportPath);
            }
            catch (Exception^ ex) {
                System::Diagnostics::Debug::WriteLine("Error setting report source: " + ex->Message);
                MessageBox::Show(ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

    private:
        void InitializeComponent() {
            this->crystalReportViewer = gcnew CrystalReportViewer();
            this->SuspendLayout();
            // 
            // crystalReportViewer
            // 
            this->crystalReportViewer->Dock = DockStyle::Fill;
            this->crystalReportViewer->Location = Point(0, 0);
            this->crystalReportViewer->Name = L"crystalReportViewer";
            this->crystalReportViewer->Size = Drawing::Size(800, 600);
            this->crystalReportViewer->TabIndex = 0;
            // 
            // ReportViewerControl
            // 
            this->Controls->Add(this->crystalReportViewer);
            this->Name = L"ReportViewerControl";
            this->Size = Drawing::Size(800, 600);
            this->ResumeLayout(false);
        }

    private:
        CrystalReportViewer^ crystalReportViewer;
    };
}
