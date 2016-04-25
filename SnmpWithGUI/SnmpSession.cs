using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SnmpWithGUI;

namespace SnmpWithGUI
{
    /// <summary>
    /// 定义了一个get/set/walk操作的必要的成员
    /// </summary>
    public class SnmpSession
    {
        /// <summary>
        /// SNMP返回的版本
        /// </summary>
        public string version{get;set;}
        /// <summary>
        /// 所访问的代理的IP地址
        /// </summary>
        public string address { get; set; }
        /// <summary>
        /// 所访问的值的OID
        /// </summary>
        public string oid { get; set; }
        /// <summary>
        /// 查询到的值，在set的情形下，为最新值
        /// </summary>
        public string value { get; set; }
        /// <summary>
        /// 此请求所有必要信息的集合，用"："隔开，格式为：
        /// version:ipadress:oid:value
        /// </summary>
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
