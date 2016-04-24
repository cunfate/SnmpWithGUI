using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SnmpWithGUI
{
    class SnmpTrapSession
    {
        public string version {get;set;}
        public string address{get;set;}
        public Int16 port{get;set;}
        public string oid { get; set; }
        public Int32 timeTick { get; set; }
        public string AllofThem { get; set; }

        #region Create Methods
        public SnmpTrapSession(string all)
        {
            string[] temp = all.Split(new char[] {':'});
            this.version = temp[0];
            this.address = temp[1];
            this.port = Convert.ToInt16(temp[2]);
            this.oid = temp[3];
            if(temp.Length >= 5)
                this.timeTick = Convert.ToInt32(temp[4]);
            this.AllofThem = all;
        }
        public SnmpTrapSession(string ver, string add, string po, string oi, string time)
        {
            this.version = ver;
            this.address = add;
            this.port = Convert.ToInt16(po);
            this.oid = oi;
            this.timeTick = Convert.ToInt32(time);
            this.AllofThem = ver + ":" + add + ":" + po + ":" + oi + "time";
        }
        #endregion
    }
}
