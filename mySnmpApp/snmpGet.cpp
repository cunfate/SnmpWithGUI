#include "snmp_pp/snmp_pp.h"
#include "snmpGet.h"
#include <iostream>

#ifdef WIN32
#define strcasecmp _stricmp
#endif

using namespace std;
using namespace Snmp_pp;
int snmpget(char *ipaddr, char *oid_in)
{
	Snmp::socket_startup();  // Initialize socket subsystem

	UdpAddress address(ipaddr);// make a SNMP++ Generic address
	if (!address.valid())
	{           // check validity of address
		cout << "Invalid Address or DNS Name, " << ipaddr << "\n";
		//	  usage();
	}
	snmp_version version = version1;                  // default is v1
	int retries = 1;                                  // default retries is 1
	int timeout = 100;                                // default is 1 second
	u_short port = 161;                               // default snmp port is 161
	OctetStr community("public");                   // community name

	int status;

	Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

	if (status != SNMP_CLASS_SUCCESS) {
		cout << "SNMP++ Session Create Fail, " << snmp.error_msg(status) << "\n";
		return 1;
	}
	Oid oid("1.3.6.1.2.1.1.1.0");      // default is sysDescr
	Pdu pdu;
	Vb  vb;
	oid = oid_in;
	if (!oid.valid()){
		cout << "Oid " << oid_in << " is not valid" << endl;
		return 1;
	}
	vb.set_oid(oid);                       // set the Oid portion of the Vb
	pdu += vb;                             // add the vb to the Pdu

	address.set_port(port);
	CTarget ctarget(address);             // make a target using the address
	ctarget.set_version(version);         // set the SNMP version SNMPV1 or V2
	ctarget.set_retry(retries);           // set the number of auto retries
	ctarget.set_timeout(timeout);         // set timeout
	ctarget.set_readcommunity(community); // set the read community name



	SnmpTarget *target;
	target = &ctarget;
	status = snmp.get(pdu, *target);

	if (status == SNMP_CLASS_SUCCESS)
	{
		for (int i = 0; i < pdu.get_vb_count(); i++)
		{
			pdu.get_vb(vb, i);
			cout << "**************************" << endl;
			cout << "VB nr: " << i << endl;
			cout << "Oid = " << vb.get_printable_oid() << endl
				<< "Value = " << vb.get_printable_value() << endl;
			cout << "Syntax = " << vb.get_syntax() << endl;

			if ((vb.get_syntax() == sNMP_SYNTAX_ENDOFMIBVIEW) ||
				(vb.get_syntax() == sNMP_SYNTAX_NOSUCHINSTANCE) ||
				(vb.get_syntax() == sNMP_SYNTAX_NOSUCHOBJECT))
				cout << "Exception: " << vb.get_syntax() << " occured." << endl;
		}
	}
	else
	{
		cout << "SNMP++ Get Error, " << snmp.error_msg(status)
			<< " (" << status << ")" << endl;
	}
	Snmp::socket_cleanup();  // Shut down socket subsystem
	return 0;
}