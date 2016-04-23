using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Threading;

namespace SnmpWithGUI
{
    class SnmpTrapListen
    {
       // public Thread Snmplistener;
        public int portToListen{get;set;}

        public SnmpTrapListen(int port)
        {
 //           Snmplistener = new Thread(new ParameterizedThreadStart(SnmpTrapListen.sendPortToCallback));
            this.portToListen = port;
  //          Snmplistener.IsBackground = true;
  //          Snmplistener.Start(port);
        }

        public void sendPortToCallback(object trap_port)
        {
            snmpTrapRecieveStart((int)trap_port);
        }

        [DllImport("snmpDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int snmpTrapRecieveStart(int trap_port);

        [DllImport("snmpDll.dll", CallingConvention = CallingConvention.Cdecl)]
        // public static extern void snmpTrapCallback();
        public static extern void sendPtrToCallback([MarshalAs(UnmanagedType.LPStr)]string input);
        //[DllImport("snmpDll.dll", CallingConvention = CallingConvention.Cdecl)]
        //public static extern void snmpStartPollThread();
        //[DllImport("snmpDll.dll", CallingConvention = CallingConvention.Cdecl)]
        //public static extern void snmpStopPollThread();
        [DllImport("snmpDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr getTrapInformation();
    }
}
