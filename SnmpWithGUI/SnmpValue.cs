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
            default:
                break;
        }
        inquiryResult =
            "IP:" + inquiryIP + "\r\n" +
            "OID:" + inquiryOID + "\r\n" +
             inquiryResult + "\r\n";
        return inquiryResult;
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
