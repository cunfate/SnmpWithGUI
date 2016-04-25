using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace SnmpWithGUI
{
    /// <summary>
    /// 定义了Snmp的一个Trap的必要的成员。
    /// </summary>
    class SnmpTrapSession
    {
        public string version {get;set;}
        public string address{get;set;}
        public Int32 port{get;set;}
        public string oid { get; set; }
        public Int32 timeTick { get; set; }
        public string AllofThem { get; set; }

        public SnmpUseInform head;

        #region Create Methods
        public SnmpTrapSession(string all)
        {
            //string[] temp = all.Split(new char[] {':'});
            //this.version = temp[0];
            //this.address = temp[1];
            //this.port = Convert.ToInt16(temp[2]);
            //this.oid = temp[3];
            //if(temp.Length >= 5)
            //    this.timeTick = Convert.ToInt32(temp[4]);
            //this.AllofThem = all;

            string[] part = all.Split(new char[] {';'});
            if(part.Length != 2)
                return;
            string[] part1 = part[0].Split(new char[] {':'});
            if(part1.Length < 3)
                return;
            this.version = part1[0];
            //this.address = part1[1];
            //this.port = Convert.ToInt16(part1[2]);
            string[] part2 = part1[1].Split(new char[] {'/'});
            this.address = part2[0];
            this.port = Convert.ToInt32(part2[1]);
            this.oid = part1[2];
            this.head = new SnmpUseInform(part[1]);
            AllofThem = all;

        }
        public SnmpTrapSession(string ver, string add, string po, string oi, string time)
        {
            this.version = ver;
            this.address = add;
            this.port = Convert.ToInt32(po);
            this.oid = oi;
            this.timeTick = Convert.ToInt32(time);
            this.AllofThem = ver + ":" + add + ":" + po + ":" + oi + "time";
        }
        #endregion
    }
}
