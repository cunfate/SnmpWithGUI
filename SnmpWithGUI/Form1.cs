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
using SnmpWithGUI;

namespace SnmpWithGUI
{

    public partial class FormForFirst : Form
    {

   #region Auto resize all the parts in this Form when its size is changed 

        private float X;
        private float Y;
        private bool firttime = true;
        private void setTag(Control cons)
        {
            foreach (Control con in cons.Controls)
            {
                con.Tag = con.Width + ":" + con.Height + ":" + con.Left + ":" + con.Top + ":" + con.Font.Size;
                if (con.Controls.Count > 0)
                    setTag(con);
            }
        }

        private void setControls(float newX, float newY, Control cons)
        {
            foreach (Control con in cons.Controls)
            {
                string[] mytag = con.Tag.ToString().Split(new char[] {':'});
                float a = Convert.ToSingle(mytag[0]) * newX;
                con.Width = (int)a;
                a = Convert.ToSingle(mytag[1]) * newY;
                con.Height = (int)a;
                a = Convert.ToSingle(mytag[2]) * newX;
                con.Left = (int)a;
                a = Convert.ToSingle(mytag[3]) * newY;
                con.Top = (int)a;
                Single currentSize = Convert.ToSingle(mytag[4]) * Math.Min(newX,newY);
                con.Font = new Font(con.Font.Name,currentSize,con.Font.Style,con.Font.Unit);
                if (con.Controls.Count > 0)
                {
                    setControls(newX,newY,con);
                }
            }
        }

        void Form1_Resize(object sender, EventArgs e)
        {
            if (!firttime)
            {
                float newX = (this.Width) / X;
                float newY = (this.Height) / Y;
                setControls(newX, newY, this);
            }
        }
   #endregion

        public FormForFirst()
        {
            InitializeComponent();
            firttime = false;
            //SnmpTrapListen.sendPtrToCallback(textBoxForTrap.Text);
            X = (float)this.Width;
            Y = (float)this.Height;
            timerForUpdate.Enabled = true;
        }

        #region execute the get/set/walk operation in background thread
        private string[] buff;
        private SnmpSession[] snmpsession;

        private void buttonToStart_Click(object sender, EventArgs e)
        {
            buff = new string[]{ textBoxForIP.Text, textBoxForOID.Text, textBoxForInput.Text, comboBoxForSnmpOperation.Text };
            this.bgwForRefreshResult.RunWorkerAsync();
        }
        private void GetSnmpSession()
        {
            SnmpValue snmpInquiry = new SnmpValue(buff[0],buff[1],buff[2],buff[3]);
            snmpsession = snmpInquiry.snmpInquriy_new();
        }
       private void RefreshResultTree()
       {
                foreach (SnmpSession s in snmpsession)
                {
                    if (treeViewForResult != null)
                    {
                        TreeNode t = treeViewForResult.Nodes.Add(s.address);
                        t.Nodes.Add("version:" + s.version);
                        t.Nodes.Add("ip address :" + s.address);
                        t.Nodes.Add("oid :" + s.oid);
                        t.Nodes.Add("value :" + s.value);
                    }
                }
        }

       private void bgwForRefreshResult_DoWork(object sender, DoWorkEventArgs e)
       {
           this.GetSnmpSession();
       }

       private void bgwForRefreshResult_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
       {
           this.RefreshResultTree();
       }
        #endregion

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
                   // textBoxResult.Text = snmpInquiry.snmpInquriy();
                    snmpsession = snmpInquiry.snmpInquriy_new();
                    foreach (SnmpSession s in snmpsession)
                    {
                        if (treeViewForResult != null)
                        {
                            TreeNode t = treeViewForResult.Nodes.Add(s.address);
                            t.Nodes.Add("version:" + s.version);
                            t.Nodes.Add("ip address :" + s.address);
                            t.Nodes.Add("oid :" + s.oid);
                            t.Nodes.Add("value :" + s.value);
                        }
                    }
                }
            }

         }

        private void FormForFirst_Load(object sender, EventArgs e)
        {
            this.Resize += new EventHandler(Form1_Resize);
            X = this.Width;
            Y = this.Height;
            setTag(this);
            Form1_Resize(new object(),new EventArgs());
        }

        //public long trapCnt = 0;
        //public long trapCntPre = 0;
        private void timerForUpdate_Tick(object sender, EventArgs e)
        {
            long trapCnt = 0;
            trapCnt = SnmpTrapListen.getTrapCounter();
            if (trapCnt > 0)
            {
                StringBuilder UpdateInformation = new StringBuilder();
                IntPtr ptr;
                ptr = SnmpTrapListen.getTrapInformation();
                string temp = Marshal.PtrToStringAnsi(ptr);
                //textBoxForTrap.Text += temp;
                //trapCntPre = trapCnt;
                string[] part = temp.Split(new char[] {'\n'});
                //if (part.Length != trapCnt)
                //{
                //    return;
                //}
                long i = 0;
                for (i = 0; i < part.Length - 1; i++)
                {
                    SnmpTrapSession newsession = new SnmpTrapSession(part[i]);
                    TreeNode t = treeViewForTrap.Nodes.Add(newsession.address + "  " + System.DateTime.Now.ToString());
                    t.Nodes.Add("version:"+newsession.version);
                    t.Nodes.Add("ip adress:"+newsession.address);
                    t.Nodes.Add("port:"+newsession.port);
                    t.Nodes.Add("trap oid:" + newsession.oid);
                    SnmpUseInform s = newsession.head;
                    while (s != null)
                    {
                        t.Nodes.Add("Context oid:" + s.oid);
                        t.Nodes.Add("Context value:" + s.value);
                        s = s.next;
                    }
 
                }
            }
        }

        private void contextMenuStrip1_Opening(object sender, CancelEventArgs e)
        {

        }


    }
}


