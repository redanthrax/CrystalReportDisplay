// ReportViewerControl.h
#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace CrystalDecisions::Windows::Forms;
using namespace CrystalDecisions::CrystalReports::Engine;
using namespace CrystalDecisions::Shared;
using namespace System::Data;

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

        void LoadReport(String^ reportPath, DataSet^ dataSet) {
            System::Diagnostics::Debug::WriteLine("Setting report source with dataset: " + reportPath);
            try {
                ReportDocument^ reportDocument = gcnew ReportDocument();
                reportDocument->Load(reportPath);
                reportDocument->SetDataSource(dataSet);
                this->crystalReportViewer->ReportSource = reportDocument;
                System::Diagnostics::Debug::WriteLine("Report source set successfully with dataset: " + reportPath);
            }
            catch (Exception^ ex) {
                System::Diagnostics::Debug::WriteLine("Error setting report source with dataset: " + ex->Message);
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
