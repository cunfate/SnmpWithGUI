using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SnmpWithGUI
{
    class SnmpUseInform
    {
        public string oid { get; set; }
        public string value { get; set; }
        public SnmpUseInform next;

        public SnmpUseInform(string s)
        {
            string[] temp = s.Split(new char[] { ':' });
            string toNext = "";
            Int32 i = 0;
            if (temp.Length < 2)
                return;
            this.oid = temp[0];
            this.value = temp[1];
            if (temp.Length < 4)
                this.next = null;
            else
            {
                for(i = 2;i<temp.Length;i++)
                {
                    toNext += temp[i] + ":";
                }
                this.next = new SnmpUseInform(toNext);
            }
        }
    }
}
