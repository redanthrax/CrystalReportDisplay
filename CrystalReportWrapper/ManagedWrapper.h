#pragma once

#include <vcclr.h>
#include "ReportViewerWrapper.h"

using namespace System;
using namespace System::Data;

public ref class ManagedWrapper {
public:
    ManagedWrapper() {
        reportViewer = gcnew ReportViewerWrapper();
        dataSet = gcnew DataSet();
    }

    void Initialize(IntPtr hwndParent) {
        reportViewer->Initialize(hwndParent);
    }

    void LoadReport(String^ reportPath) {
        reportViewer->LoadReport(reportPath, dataSet);
    }

    void CreateDataSet() {
        dataSet = gcnew DataSet();
    }

    void AddTable(String^ tableName) {
        DataTable^ table = gcnew DataTable(tableName);
        dataSet->Tables->Add(table);
    }

    void AddColumn(String^ tableName, String^ columnName, Type^ type) {
        DataTable^ table = dataSet->Tables[tableName];
        table->Columns->Add(gcnew DataColumn(columnName, type));
    }

    void AddRow(String^ tableName, array<Object^>^ values) {
        DataTable^ table = dataSet->Tables[tableName];
        DataRow^ row = table->NewRow();
        row->ItemArray = values;
        table->Rows->Add(row);
    }

    void SetDatabaseLogon(String^ server, String^ database, String^ userId, String^ password) {
        reportViewer->SetDatabaseLogon(server, database, userId, password);
    }

    DataSet^ GetDataSet() {
        return dataSet;
    }

    void SetDataSet(DataSet^ ds) {
        dataSet = ds;
    }

private:
    ReportViewerWrapper^ reportViewer;
    DataSet^ dataSet;
};
