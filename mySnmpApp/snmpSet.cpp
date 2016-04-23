#include "snmpSet.h"

using namespace std;
using namespace Snmp_pp;

bool determine_vb(SmiUINT32 val, Vb &vb)
{
	char buffer[255];

	if (val == sNMP_SYNTAX_NOSUCHINSTANCE)
	{
		cout << "Instance does not exists but can be created.\n";
		cout << "Please enter one of the following types:\n\n";
		cout << "Integer:  " << sNMP_SYNTAX_INT << "\n";
		cout << "Bits:     " << sNMP_SYNTAX_BITS << "\n";
		cout << "STRING:   " << sNMP_SYNTAX_OCTETS << "\n";
		cout << "Oid:      " << sNMP_SYNTAX_OID << "\n";
		cout << "IpAddress:" << sNMP_SYNTAX_IPADDR << "\n\n";
		cout << "Please choose value type: ";
		cin >> val;
		vb.set_syntax(val);
	}
	cout << "Value Type is ";

	switch (val) {

		// octet string
	case sNMP_SYNTAX_OCTETS:
	{
		cout << "Octet String\n";
		cout << "Please enter new value: ";
		cin >> buffer;
		OctetStr octetstr(buffer);
		if (octetstr.valid()) {
			vb.set_value(octetstr);
			return true;
		}
		else {
			cout << "Invalid OctetStr\n";
			return false;
		}
	}

		// IP Address
	case sNMP_SYNTAX_IPADDR:
	{
		cout << "IP Address\n";
		cout << "Please enter new value: ";
		cin >> buffer;
		IpAddress ipaddress(buffer);
		if (ipaddress.valid()) {
			vb.set_value(ipaddress);
			return true;
		}
		else {
			cout << "Invalid IP Address\n";
			return false;
		}
	}

		// Oid
	case sNMP_SYNTAX_OID:
	{
		cout << "Oid\n";
		cout << "Please enter new value: ";
		cin >> buffer;
		Oid oid(buffer);
		if (oid.valid()) {
			vb.set_value(oid);
			return true;
		}
		else {
			cout << "Invalid Oid\n";
			return false;
		}
	}

		// TimeTicks
	case sNMP_SYNTAX_TIMETICKS:
	{
		cout << "TimeTicks\n";
		cout << "Please enter new value: ";
		cin >> buffer;
		unsigned long i;
		i = atol(buffer);
		TimeTicks timeticks(i);
		if (timeticks.valid()) {
			vb.set_value(timeticks);
			return true;
		}
		else {
			cout << "Invalid TimeTicks\n";
			return false;
		}
	}

		// Gauge32
	case sNMP_SYNTAX_GAUGE32:
	{
		cout << "Gauge32\n";
		cout << "Please enter new value: ";
		cin >> buffer;
		unsigned long i;
		i = atol(buffer);
		Gauge32 gauge32(i);
		if (gauge32.valid()) {
			vb.set_value(gauge32);
			return true;
		}
		else {
			cout << "Invalid Gauge32\n";
			return false;
		}
	}

	case sNMP_SYNTAX_CNTR32:
	{
		cout << "Counter32\n";
		cout << "Please enter new value: ";
		cin >> buffer;
		unsigned long i;
		i = atol(buffer);
		Counter32 counter32(i);
		if (counter32.valid()) {
			vb.set_value(counter32);
			return true;
		}
		else {
			cout << "Invalid Counter32\n";
			return false;
		}
	}

	case sNMP_SYNTAX_CNTR64:
	{
		cout << "Counter64\n";
		cout << "Please enter value (low 32 bit): ";
		cin >> buffer;
		unsigned long i;
		i = atol(buffer);
		Counter64 counter64;
		counter64.set_low(i);

		cout << "Please enter value (high 32 bit): ";
		cin >> buffer;
		i = atol(buffer);
		counter64.set_high(i);

		if (counter64.valid()) {
			vb.set_value(counter64);
			return true;
		}
		else {
			cout << "Invalid Counter64\n";
			return false;
		}
	}

	case sNMP_SYNTAX_INT:
	{
		cout << "Integer\n";
		cout << "Please enter new value: ";
		cin >> buffer;
		unsigned long i;
		i = atol(buffer);
		long l;
		l = (long)i;
		vb.set_value(l);
		return true;
	}

	case sNMP_SYNTAX_NOSUCHOBJECT:
	{
		cout << "NO SUCH OBJECT\n";
		cout << "Object cannot be created.\n";
		return false;
	}
	default:
		cout << "Unknown Data Type " << val << "\n";
		return false;
	}
}




int snmpset(char *ipAddr, char *oid_in)
{
	Snmp::socket_startup();  // Initialize socket subsystem
	//---------[ make a GenAddress and Oid object to retrieve ]---------------
	UdpAddress address(ipAddr);      // make a SNMP++ Generic address
	if (!address.valid()) {           // check validity of address
		cout << "Invalid Address or DNS Name, " << ipAddr << "\n";
	}
	Oid oid("1.3.6.1.2.1.1.4.0");      // default is sysName
	oid = oid_in;
	if (!oid.valid()) {            // check validity of user oid
		cout << "Invalid Oid, " << oid_in << "\n";
	}

	//---------[ determine options to use ]-----------------------------------
	snmp_version version = version1;                  // default is v1
	int retries = 1;                                  // default retries is 1
	int timeout = 100;                                // default is 1 second
	u_short port = 161;                               // default snmp port is 161
	OctetStr community("private");                   // community name
	OctetStr get_community;


	char *ptr;
	if (get_community.len() == 0)
		get_community = community;

	//----------[ create a SNMP++ session ]-----------------------------------
	int status;
	// bind to any port and use IPv6 if needed
	Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

	if (status != SNMP_CLASS_SUCCESS) {
		cout << "SNMP++ Session Create Fail, " << snmp.error_msg(status) << "\n";
		return 1;
	}


	//--------[ build up SNMP++ object needed ]-------------------------------
	Pdu pdu;                               // construct a Pdu object
	Vb vb;                                 // construct a Vb object
	vb.set_oid(oid);                      // set the Oid portion of the Vb
	pdu += vb;                             // add the vb to the Pdu

	address.set_port(port);
	CTarget ctarget(address);             // make a target using the address
	ctarget.set_timeout(timeout);         // set timeout
	ctarget.set_readcommunity(get_community); // set the read community name
	ctarget.set_writecommunity(community);// set the write community name

	SnmpTarget *target;
	target = &ctarget;

	// first get the variabel to determine its type
	if ((status = snmp.get(pdu, *target)) == SNMP_CLASS_SUCCESS) {
		pdu.get_vb(vb, 0);
		cout << "Oid = " << vb.get_printable_oid() << endl
			<< "Current Value = " << vb.get_printable_value() << endl;
		if (determine_vb(vb.get_syntax(), vb)) {
			// do the Set
			Pdu setpdu;


			vb.set_oid(oid);           // use the same oid as the inquire
			setpdu += vb;
			status = snmp.set(setpdu, *target);
			cout << "Set Status = " << snmp.error_msg(status) << "\n";
		}
	}
	else
		cout << "SNMP++ Set Error, " << snmp.error_msg(status) << "\n";

	Snmp::socket_cleanup();  // Shut down socket subsystem
	return 0;
}