using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;

namespace SnmpWithGUI
{
    public partial class FormForFirst : Form
    {
        public FormForFirst()
        {
            InitializeComponent();
            SnmpTrapListen.sendPtrToCallback(textBoxForTrap.Text);
            timerForUpdate.Enabled = true;
        }

        private void buttonToStart_Click(object sender, EventArgs e)
        {
            SnmpValue snmpInquiry = new SnmpValue(textBoxForIP.Text, textBoxForOID.Text, textBoxForInput.Text, comboBoxForSnmpOperation.Text);
            textBoxResult.Text = snmpInquiry.snmpInquriy();
        }

        private void comboBoxForSnmpOperation_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBoxForSnmpOperation.Text != "set")
                textBoxForInput.Enabled = false;
            else
                textBoxForInput.Enabled = true;
            if (comboBoxForSnmpOperation.Text != "get" && comboBoxForSnmpOperation.Text != "")
            {
                buttonToNext.Enabled = false;
            }
            else
            {
                buttonToNext.Enabled = true;
            }
        }

        private void buttonToNext_Click(object sender, EventArgs e)
        {
            StringBuilder tempIP = new StringBuilder();
            StringBuilder tempOID = new StringBuilder();
            IntPtr intPtr;
            tempIP.Append(textBoxForIP.Text);
            tempOID.Append(textBoxForOID.Text);
            SnmpValue snmpInquiry = new SnmpValue(textBoxForIP.Text, textBoxForOID.Text, textBoxForInput.Text, comboBoxForSnmpOperation.Text);
            intPtr = SnmpValue.snmpGetNextOid(tempIP, tempOID);
            string temp = Marshal.PtrToStringAnsi(intPtr);
            if (!temp.Contains("Invalid"))
            {
                textBoxForOID.Text = temp;
                snmpInquiry.inquiryOID = temp;
                if (snmpInquiry.operation == "get")
                {
                    textBoxResult.Text = snmpInquiry.snmpInquriy();
                }
            }
            else
            {
                textBoxResult.Text = temp;
            }

         }

        private void FormForFirst_Load(object sender, EventArgs e)
        {

        }

        private void timerForUpdate_Tick(object sender, EventArgs e)
        {
            StringBuilder UpdateInformation = new StringBuilder();
            IntPtr ptr;
            ptr = SnmpTrapListen.getTrapInformation();
            string temp = Marshal.PtrToStringAnsi(ptr);
            textBoxForTrap.Text = temp;
        }
    }
}


