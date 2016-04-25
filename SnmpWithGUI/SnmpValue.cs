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
using SnmpWithGUI;


/// <summary>
/// Snmp的查询操作，每个进程最多只能有一个这种类。
/// 它的一些方法不是线程安全的。
/// </summary>
public class SnmpValue
{
    public string inquiryIP { get; set; }
    public string inquiryOID { get; set; }
    public string inquiryResult{ get; set; }
    public string operation{ get; set; }
    public string content{ get; set; }

    private IntPtr intPtr;


    //
    public string snmpInquriy()
    {
        StringBuilder tempIP = new StringBuilder();
        StringBuilder tempOID = new StringBuilder();
        StringBuilder tempContent = new StringBuilder();
        tempIP.Append(inquiryIP);
        tempOID.Append(inquiryOID);
        tempContent.Append(content);
        switch (operation)
        {
            case "get":
                intPtr = snmpGet(tempIP,tempOID);
                inquiryResult = Marshal.PtrToStringAnsi(intPtr);
                break;
            case "set":
                intPtr = snmpSet(tempIP, tempOID, tempContent);
                inquiryResult = Marshal.PtrToStringAnsi(intPtr);
                break;
            case "walk":
                intPtr = snmpWalkByNext(tempIP, tempOID);
                inquiryResult = Marshal.PtrToStringAnsi(intPtr);
                break;
            case "walk2":
                break;
            default:
                break;
        }
        inquiryResult =
            "IP:" + inquiryIP + "\r\n" +
            "OID:" + inquiryOID + "\r\n" +
             inquiryResult + "\r\n";
        return inquiryResult;
    }

    public SnmpSession[] snmpInquriy_new()
    {
        StringBuilder tempIP = new StringBuilder();
        StringBuilder tempOID = new StringBuilder();
        StringBuilder tempContent = new StringBuilder();
        tempIP.Append(inquiryIP);
        tempOID.Append(inquiryOID);
        tempContent.Append(content);
        switch (operation)
        {
            case "get":
                string t = Marshal.PtrToStringAnsi(snmpGet(tempIP, tempOID));
                SnmpSession[] s = new SnmpSession[1];
                s[0] = new SnmpSession("2c:"+inquiryIP+":"+inquiryOID+":"+t);
                return s;
               // break;
            case "set":
                string t1 = Marshal.PtrToStringAnsi(snmpSet(tempIP, tempOID,tempContent));
                SnmpSession[] s1 = new SnmpSession[1];
                s1[0] = new SnmpSession("2c:"+inquiryIP+":"+inquiryOID+":"+t1);
                return s1;
             //   break;
            case "walk":
                return snmpWalk_i();
            default:
                return null;
        }

    }

    public SnmpSession[] snmpWalk_i()
    {
        string[] tempresult;
        tempresult = snmpWalk(inquiryIP, inquiryOID);
        SnmpSession[] result = new SnmpSession[tempresult.Length - 1];
        uint i = 0;
        for (i = 0; i < tempresult.Length - 1; i++)
        {
            result[i] = new SnmpSession(tempresult[i]);
        }
        return result;
    }

    //构造函数
    //三值入口顺序为：IP,OID,OPERATION（即要进行的操作）
    //四值构造函数入口顺序为：IP，OID,CONTENT（即填充值）,OPERATION
    public SnmpValue() { }

    public SnmpValue(string ip, string oid, string con, string op)
    {
        this.inquiryIP = ip;
        this.inquiryOID = oid;
        this.content = con;
        this.operation = op;
    }

    public SnmpValue(string ip, string oid, string op)
    {
        this.inquiryIP = ip;
        this.inquiryOID = oid;
        this.operation = op;
    }

    public string[] snmpWalk(string ip, string oid_in)
    {
        StringBuilder tempIP = new StringBuilder();
        StringBuilder tempOID = new StringBuilder();
        SnmpOID oidInput = new SnmpOID(oid_in);
        SnmpOID oidTemp = new SnmpOID(oid_in);
        string result = "";
        tempIP.Append(ip);
        tempOID.Append(oid_in);
        while ((oidTemp.oidSegInt[oidInput.oidLen - 1]) == (oidInput.oidSegInt[oidInput.oidLen - 1]))
        {
           // IntPtr temp;
            string tempstr;
            tempstr = Marshal.PtrToStringAnsi(snmpGetNextOid(tempIP, tempOID));
            oidTemp.Change(tempstr);

            if ((oidTemp.oidSegInt[oidInput.oidLen - 1]) != (oidInput.oidSegInt[oidInput.oidLen - 1]))
            {
                break;
            }
            tempOID.Clear();
            tempOID.Append(tempstr);

            tempstr = "2c:" + ip + ":" + tempstr + ":" + Marshal.PtrToStringAnsi(snmpGet(tempIP, tempOID));
            result += tempstr + ";";
        }
        string[] result2 = result.Split(new char[] {';'});
        return result2;
        

    }



    //snmpDll.DLL中的函数调用
    //其中，C++中的char *类型对应C#的StringBuilder类
    //      C++中的char*作为返回值时则需要用IntPtr类来接收
    //目前实现的函数有：
    //const char * snmpget(const char * ip,const char * oid);
    //const char * snmpset(const char * ip,const char * oid,const char * content);
    //const char * snmpBulk(const char * ip,cnst char * oid);
    [DllImport("snmpDll.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr snmpGet(StringBuilder ip, StringBuilder oid_in);

    [DllImport("snmpDll.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr snmpSet(StringBuilder ip, StringBuilder oid_in, StringBuilder content);

    //[DllImport("snmpDll.dll", CallingConvention = CallingConvention.Cdecl)]
    //public static extern IntPtr snmpWalk(StringBuilder ip, StringBuilder oid_in);
    [DllImport("snmpDll.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr snmpGetNext(StringBuilder ip, StringBuilder oid_in);
    [DllImport("snmpDll.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr snmpWalkByNext(StringBuilder ip, StringBuilder oid_in);
    [DllImport("snmpDll.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr snmpGetNextOid(StringBuilder ip, StringBuilder oid_in);




}
