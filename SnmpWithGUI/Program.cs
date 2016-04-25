using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;
using SnmpWithGUI;

namespace SnmpWithGUI
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            #region create a thread to listen for trap

            SnmpTrapListen trapListener = new SnmpTrapListen(10162);
            trapListener.sendPortToCallback(10162);
            Thread snmpTrapListener1 = new Thread(new ParameterizedThreadStart(trapListener.sendPortToCallback));
            snmpTrapListener1.IsBackground = true;
            snmpTrapListener1.Start(trapListener.portToListen);

            #endregion
            Application.Run(new FormForFirst());
        }
    }
}
