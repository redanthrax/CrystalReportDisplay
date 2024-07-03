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
            ReportDocument^ reportDocument = gcnew ReportDocument();
            System::Diagnostics::Debug::WriteLine("Setting report source with dataset: " + reportPath);
            try {
                reportDocument->Load(reportPath);
                reportDocument->SetDataSource(dataSet);
                this->crystalReportViewer->ReportSource = reportDocument;
                System::Diagnostics::Debug::WriteLine("Report source set successfully with dataset: " + reportPath);
            }
            catch (Exception^ ex) {
                System::Diagnostics::Debug::WriteLine("Error setting report source with dataset: " + ex->Message);
                MessageBox::Show(ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }

            System::Diagnostics::Debug::WriteLine("Setting up db login");
            try {
                auto connectionInfo = gcnew CrystalDecisions::Shared::ConnectionInfo();
                connectionInfo->ServerName = ".";
                connectionInfo->DatabaseName = "AdventureWorks";
                connectionInfo->UserID = "AdventureWorks";
                connectionInfo->Password = "AdventureWorks";
                CrystalDecisions::CrystalReports::Engine::Tables^ tables = reportDocument->Database->Tables;
                for each (CrystalDecisions::CrystalReports::Engine::Table ^ table in tables) {
                    auto logonInfo = table->LogOnInfo;
                    logonInfo->ConnectionInfo = connectionInfo;
                    table->ApplyLogOnInfo(logonInfo);
                }
            }
            catch (Exception^ ex) {
                System::Diagnostics::Debug::WriteLine(ex->Message);
                MessageBox::Show(ex->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void SetDatabaseLogon(String^ server, String^ database, String^ userId, String^ password) {
            System::Diagnostics::Debug::WriteLine("Setting database logon info.");
            try {
                auto reportDocument = dynamic_cast<CrystalDecisions::CrystalReports::Engine::ReportDocument^>(crystalReportViewer->ReportSource);
                if (reportDocument != nullptr) {
                    auto connectionInfo = gcnew CrystalDecisions::Shared::ConnectionInfo();
                    connectionInfo->ServerName = server;
                    connectionInfo->DatabaseName = database;
                    connectionInfo->UserID = userId;
                    connectionInfo->Password = password;
                    CrystalDecisions::CrystalReports::Engine::Tables^ tables = reportDocument->Database->Tables;
                    for each (CrystalDecisions::CrystalReports::Engine::Table^ table in tables) {
                        auto logonInfo = table->LogOnInfo;
                        logonInfo->ConnectionInfo = connectionInfo;
                        table->ApplyLogOnInfo(logonInfo);
                    }
                }
            }
            catch (Exception^ ex) {
                System::Diagnostics::Debug::WriteLine("Error setting database logon info: " + ex->Message);
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
