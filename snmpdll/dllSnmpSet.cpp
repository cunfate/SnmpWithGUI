#include "dllSnmpSet.h"
#include "dllSnmpConfig.h"

using namespace std;
using namespace Snmp_pp;
extern "C"
{
	bool determine_vb(SmiUINT32 val, Vb &vb,const char * content)
	{
		char buffer[255];

		if (val == sNMP_SYNTAX_NOSUCHINSTANCE)
		{
#ifdef _DEBUG_COUT
				cout << "Instance does not exists but can be created.\n";
				cout << "Please enter one of the following types:\n\n";
				cout << "Integer:  " << sNMP_SYNTAX_INT << "\n";
				cout << "Bits:     " << sNMP_SYNTAX_BITS << "\n";
				cout << "STRING:   " << sNMP_SYNTAX_OCTETS << "\n";
				cout << "Oid:      " << sNMP_SYNTAX_OID << "\n";
				cout << "IpAddress:" << sNMP_SYNTAX_IPADDR << "\n\n";
				cout << "Please choose value type: ";
#endif
			cin >> val;
			vb.set_syntax(val);
		}
		cout << "Value Type is ";

		switch (val) {

			// octet string
		case sNMP_SYNTAX_OCTETS:
		{
#ifdef _DEBUG_COUT
				cout << "Octet String\n";
				cout << "Please enter new value: ";
#endif
			//strcpy_s(buffer, content);
			strcpy_s(buffer,content);
			OctetStr octetstr(buffer);
			if (octetstr.valid()) {
				vb.set_value(octetstr);
				return true;
			}
			else {
#ifdef _DEBUG_COUT
					cout << "Invalid OctetStr\n";
#endif
				return false;
			}
		}

			// IP Address
		case sNMP_SYNTAX_IPADDR:
		{
			cout << "IP Address\n";
			cout << "Please enter new value: ";
			//strcpy_s(buffer, content);
			strcpy_s(buffer, content);
			IpAddress ipaddress(buffer);
			if (ipaddress.valid()) {
				vb.set_value(ipaddress);
				return true;
			}
			else {
#ifdef _DEBUG_COUT
				cout << "Invalid IP Address\n";
#endif
				return false;
			}
		}

			// Oid
		case sNMP_SYNTAX_OID:
		{
			cout << "Oid\n";
			cout << "Please enter new value: ";
			//strcpy_s(buffer, content);
			strcpy_s(buffer, content);
			Oid oid(buffer);
			if (oid.valid()) {
				vb.set_value(oid);
				return true;
			}
			else {
#ifdef _DEBUG_COUT
				cout << "Invalid Oid\n";
#endif
				return false;
			}
		}

			// TimeTicks
		case sNMP_SYNTAX_TIMETICKS:
		{
#ifdef _DEBUG_COUT
			cout << "TimeTicks\n";
			cout << "Please enter new value: ";
#endif
			strcpy_s(buffer, content);
			unsigned long i;
			i = atol(buffer);
			TimeTicks timeticks(i);
			if (timeticks.valid()) {
				vb.set_value(timeticks);
				return true;
			}
			else {
#ifdef _DEBUG_COUT
				cout << "Invalid TimeTicks\n";
#endif
				return false;
			}
		}

			// Gauge32
		case sNMP_SYNTAX_GAUGE32:
		{
#ifdef _DEBUG_COUT
			cout << "Gauge32\n";
			cout << "Please enter new value: ";
#endif			
			strcpy_s(buffer, content);
			unsigned long i;
			i = atol(buffer);
			Gauge32 gauge32(i);
			if (gauge32.valid()) {
				vb.set_value(gauge32);
				return true;
			}
			else {
#ifdef _DEBUG_COUT
				cout << "Invalid Gauge32\n";
#endif
				return false;
			}
		}

		case sNMP_SYNTAX_CNTR32:
		{
#ifdef _DEBUG_COUT
			cout << "Counter32\n";
			cout << "Please enter new value: ";
#endif
			strcpy_s(buffer, content);
			unsigned long i;
			i = atol(buffer);
			Counter32 counter32(i);
			if (counter32.valid()) {
				vb.set_value(counter32);
				return true;
			}
			else {
#ifdef _DEBUG_COUT
				cout << "Invalid Counter32\n";
#endif
				return false;
			}
		}

		case sNMP_SYNTAX_CNTR64:
		{
#ifdef _DEBUG_COUT
			cout << "Counter64\n";
			cout << "Please enter value (low 32 bit): ";
#endif
			strcpy_s(buffer, content);
			unsigned long i;
			i = atol(buffer);
			Counter64 counter64;
			counter64.set_low(i);
#ifdef _DEBUG_COUT
			cout << "Please enter value (high 32 bit): ";
#endif
			strcpy_s(buffer, content);
			i = atol(buffer);
			counter64.set_high(i);

			if (counter64.valid()) {
				vb.set_value(counter64);
				return true;
			}
			else {
#ifdef _DEBUG_COUT
				cout << "Invalid Counter64\n";
#endif
				return false;
			}
		}

		case sNMP_SYNTAX_INT:
		{
#ifdef _DEBUG_COUT
			cout << "Integer\n";
			cout << "Please enter new value: ";
#endif
			strcpy_s(buffer, content);
			unsigned long i;
			i = atol(buffer);
			long l;
			l = (long)i;
			vb.set_value(l);
			return true;
		}

		case sNMP_SYNTAX_NOSUCHOBJECT:
		{
#ifdef _DEBUG_COUT
			cout << "NO SUCH OBJECT\n";
			cout << "Object cannot be created.\n";
#endif
			return false;
		}
		default:
#ifdef _DEBUG_COUT
			cout << "Unknown Data Type " << val << "\n";
#endif	
			return false;
		}
	}



	//char nowVal[1000];
	string setResult;
	_declspec(dllexport) 
	const char * 
	snmpSet(const char *ipAddr, const char *oid_in, const char * content)
	{
		string temp;
		Snmp::socket_startup();  // Initialize socket subsystem
		//---------[ make a GenAddress and Oid object to retrieve ]---------------
		UdpAddress address(ipAddr);      // make a SNMP++ Generic address
		if (!address.valid()) {           // check validity of address
#ifdef _DEBUG_COUT
			cout << "Invalid Address or DNS Name, " << ipAddr << "\n";
#endif
		}
		Oid oid("1.3.6.1.2.1.1.4.0");      // default is sysName
		oid = oid_in;
		if (!oid.valid()) {            // check validity of user oid
#ifdef _DEBUG_COUT
			cout << "Invalid Oid, " << oid_in << "\n";
#endif
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
#ifdef _DEBUG_COUT
			cout << "SNMP++ Session Create Fail, " << snmp.error_msg(status) << "\n";
#endif
			setResult = "SNMP++ Session Create Fail!";
			return setResult.c_str();
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
#ifdef _DEBUG_COUT
			cout << "Oid = " << vb.get_printable_oid() << endl
				<< "Current Value = " << vb.get_printable_value() << endl;
#endif
			if (determine_vb(vb.get_syntax(), vb, content)) {
				// do the Set
				Pdu setpdu;


				vb.set_oid(oid);           // use the same oid as the inquire
				setpdu += vb;
				status = snmp.set(setpdu, *target);
#ifdef _DEBUG_COUT
				cout << "Set Status = " << snmp.error_msg(status) << "\n";
#endif
			}
		}
		else
#ifdef _DEBUG_COUT
			cout << "SNMP++ Set Error, " << snmp.error_msg(status) << "\n";
#endif

		setResult = vb.get_printable_value();
		//setResult = "Now value: " + setResult;
		//strcpy_s(nowVal, temp.c_str());
		Snmp::socket_cleanup();  // Shut down socket subsystem
		//return 0;
		return setResult.c_str();
	}
}