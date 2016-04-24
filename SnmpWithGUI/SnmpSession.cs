using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SnmpWithGUI
{
    class SnmpSession
    {
        public string version{get;set;}
        public string address { get; set; }
        public string oid { get; set; }
        public string value { get; set; }
        public string allOfThem {get;set;}

        #region  Create Methods
        public SnmpSession(string all) 
        {
            this.allOfThem = all;
            string[] temp = all.Split(new char[] {':'});
            this.version = temp[0];
            this.address = temp[1];
            this.oid = temp[2];
            this.value = temp[3];
        }
        public SnmpSession(string ver, string ip, string oid, string value)
        {
            this.version = ver;
            this.address = ip;
            this.oid = oid;
            this.value = value;
            this.allOfThem = ver + ":" + ip + ":" + oid + ":" + value;
        }
        #endregion


    }
}
