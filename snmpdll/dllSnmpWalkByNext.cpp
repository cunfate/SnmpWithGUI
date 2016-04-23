#include "dllSnmpGetNext.h"
#include "dllSnmpGet.h"
#include "dllSnmpConfig.h"
#include "snmp_pp/snmp_pp.h"


using namespace Snmp_pp;
using namespace std;

extern "C"
{
	//char walk2NowVal[10000];
	string walkResult;
	_declspec(dllexport)
	const char * snmpWalkByNext(char * ip, char * oid_in)
	{
		#ifdef DEBUG_DLL
			cout << "This is entrance!" << endl;
		#endif
		unsigned long oidLen,oidLast;
		char oidTemp[100];
		string temp = "Result:\n";
		string temp2 = "";
		Oid oid(oid_in);
		oidLen = oid.len();
		oidLast = oid[oidLen - 1];
		#ifdef DEBUG_DLL
			cout << "Oid length is " << oidLen << endl;
			cout << "Oid Last Value is " << oidLast << endl;
		#endif
		while (oid[oidLen - 1] == oidLast)
		{
			temp2 = oid.get_printable();
			strcpy_s(oidTemp,temp2.c_str());
			oid = snmpGetNextOid(ip,oidTemp);
			if (oid[oidLen - 1] != oidLast)
			{
				break;
			}
			temp2 = oid.get_printable();
			strcpy_s(oidTemp,temp2.c_str());
			//snmpGet(ip, oidTemp);

			walkResult += "\r\noid:";
			walkResult += temp2;
			walkResult += "\r\nValue:";
			walkResult += snmpGet(ip, oidTemp);
		}
		//strcpy_s(walk2NowVal,temp.c_str());
		return (walkResult.c_str());

	}
}