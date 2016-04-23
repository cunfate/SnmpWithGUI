
//char snmpnext_cpp_version[] = "@(#) SNMP++ $Id: snmpNext.cpp 2647 2014-05-11 11:22:42Z fock $";
#include <libsnmp.h>
#include <string>
#include "dllSnmpGetNext.h"
#include "dllSnmpConfig.h"

#ifdef WIN32
#define strcasecmp _stricmp
#endif

#include "snmp_pp/snmp_pp.h"

#define SNMP_PP_NAMESPACE
using namespace Snmp_pp;
using namespace std;

extern "C"
{
	//char nextNowVal[100];
	string nextOID;
	_declspec(dllexport)
	char * snmpGetNextOid(char * ip, char *oid_in)
	{

		Snmp::socket_startup();  // Initialize socket subsystem
		string temp;
		//---------[ make a GenAddress and Oid object to retrieve ]---------------
		UdpAddress address(ip);      // make a SNMP++ Generic address
		if (!address.valid()) {           // check validity of address
			nextOID = "Invalid IP Address or DNS name";
			//strcpy_s(nextNowVal, temp.c_str());
			return (char *)(nextOID.c_str());
		}
		Oid oid("1.3.6.1.2.1.1.1");      // default is sysDescr
		oid = oid_in;
		if (!oid.valid()) {            // check validity of user oid
			//	std::cout << "Invalid Oid " << argv[2] << "\n";
			nextOID = "Invalid OID";
			//strcpy_s(nextNowVal, temp.c_str());
			return (char *)(nextOID.c_str());
		}

		snmp_version version = version1;                  // default is v1
		int retries = 1;                                  // default retries is 1
		int timeout = 100;                                // default is 1 second
		u_short port = 161;                               // default snmp port is 161
		OctetStr community("public");                   // community name


		char *ptr;



		//----------[ create a SNMP++ session ]-----------------------------------
		int status;
		// bind to any port and use IPv6 if needed
		Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

		if (status != SNMP_CLASS_SUCCESS) {
#ifdef _DEBUG_COUT
			cout << "SNMP++ Session Create Fail, " << snmp.error_msg(status) << "\n";
#endif
			nextOID = "SNMP++ Session Create Fail";
			//strcpy_s(nextNowVal, temp.c_str());
			
			return (char *)(nextOID.c_str());
		}

		//--------[ build up SNMP++ object needed ]-------------------------------
		Pdu pdu;                               // construct a Pdu object
		Vb vb;                                 // construct a Vb object
		vb.set_oid(oid);                      // set the Oid portion of the Vb
		pdu += vb;                             // add the vb to the Pdu

		address.set_port(port);
		CTarget ctarget(address);             // make a target using the address
		ctarget.set_version(version);         // set the SNMP version SNMPV1 or V2
		ctarget.set_retry(retries);           // set the number of auto retries
		ctarget.set_timeout(timeout);         // set timeout
		ctarget.set_readcommunity(community); // set the read community name

		//-------[ issue the request, blocked mode ]-----------------------------
		//	cout << "SNMP++ GetNext to " << argv[1] << " SNMPV"
		//		<< (version + 1)
		//		<< " Retries=" << retries
		//		<< " Timeout=" << timeout * 10 << "ms";
		//	cout << " Community=" << community.get_printable() << endl << flush;

		SnmpTarget *target;
		target = &ctarget;

		status = snmp.get_next(pdu, *target);

		if (status == SNMP_CLASS_SUCCESS)
		{
			pdu.get_vb(vb, 0);
#ifdef _DEBUG_COUT
			cout << "Oid = " << vb.get_printable_oid() << endl
				<< "Value = " << vb.get_printable_value() << endl;
#endif
			nextOID = vb.get_printable_oid();
			//strcpy_s(nextNowVal, temp.c_str());
		}
		else
		{
#ifdef _DEBUG_COUT
			cout << "SNMP++ GetNext Error, " << snmp.error_msg(status)
				<< " (" << status << ")" << endl;
#endif
			nextOID = "SNMP++ GetNext Error";
			//strcpy_s(nextNowVal, temp.c_str());
		}

		Snmp::socket_cleanup();  // Shut down socket subsystem
		return (char *)(nextOID.c_str());
	}


	_declspec(dllexport)
	char * snmpGetNext(char * ip,char * oid_in)
	{
		char oidNext[100];
		strcpy_s(oidNext,snmpGetNextOid(ip, oid_in));
		return (char *)snmpGet(ip, oidNext);
	}
}