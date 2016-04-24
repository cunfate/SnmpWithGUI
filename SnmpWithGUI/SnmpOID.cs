using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SnmpWithGUI
{
    class SnmpOID
    {
        public string[] oidSegStr;
        public Int16[]  oidSegInt;
        public string   oidComplete;
        public Int32    oidLen;

        public SnmpOID(string oid_in)
        {
            string[] temp = oid_in.Split(new char[] {'.'});
            this.oidLen = temp.Length;
            oidSegInt = new Int16[temp.Length];
            oidSegStr = new string[temp.Length];
            Int32 i = 0;
            for(i = 0;i<temp.Length;i++)
            {
                this.oidSegInt[i] = Convert.ToInt16(temp[i]);
                this.oidSegStr[i] = temp[i];
            }
            oidComplete = oid_in;
        }

        public void Change(string oid_in)
        {
            string[] temp = oid_in.Split(new char[] { '.' });
            this.oidLen = temp.Length;
            oidSegInt = new Int16[temp.Length];
            oidSegStr = new string[temp.Length];
            Int32 i = 0;
            for (i = 0; i < temp.Length; i++)
            {
                this.oidSegInt[i] = Convert.ToInt16(temp[i]);
                this.oidSegStr[i] = temp[i];
            }
            oidComplete = oid_in;
        }
 
    }
}
