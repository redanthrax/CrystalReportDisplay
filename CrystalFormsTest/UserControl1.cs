using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CrystalFormsTest
{
    public partial class UserControl1: UserControl
    {
        public UserControl1()
        {
            InitializeComponent();
            crystalReportViewer1.ReportSource = "C:\\Users\\ggagnon\\source\\CrystalReportDisplay\\Debug\\BlankReport.rpt";
        }
    }
}
