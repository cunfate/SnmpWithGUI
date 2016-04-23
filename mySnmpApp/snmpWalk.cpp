#include "snmpWalk.h"
#include <libsnmp.h>

using namespace std;
using namespace Snmp_pp;

#ifdef WIN32
#define strcasecmp _stricmp
#endif

#define BULK_MAX (10)

int snmpwalk(char * ip, char * oid_in)
{
	int requests = 0;        // keep track of # of requests
	int objects = 0;
	Snmp::socket_startup();  // Initialize socket subsystem

	//---------[ make a GenAddress and Oid object to retrieve ]---------------
	UdpAddress address(ip);      // make a SNMP++ Generic address
	if (!address.valid()) {           // check validity of address
		std::cout << "Invalid Address or DNS Name, " << ip << endl;
	}
	Oid oid("1");                      // default is beginning of MIB 
			if (!oid.valid()) {            // check validity of user oid
				std::cout << "Invalid Oid, " << oid_in << endl;
		}
	

	//---------[ determine options to use ]-----------------------------------
	snmp_version version = version1;                  // default is v1
	int retries = 1;                                  // default retries is 1
	int timeout = 100;                                // default is 1 second
	u_short port = 161;                               // default snmp port is 161
	OctetStr community("public");                   // community name
	bool subtree = false;



	char *ptr;


	

	//----------[ create a SNMP++ session ]-----------------------------------
	int status;
	// bind to any port and use IPv6 if needed
	Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

	if(status != SNMP_CLASS_SUCCESS) {
		cout << "SNMP++ Session Create Fail, " << snmp.error_msg(status) << endl;
		return 1;
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
	cout << "SNMP++ snmpWalk to " << ip << " SNMPV"

		<< (version + 1)
		<< " Retries=" << retries
		<< " Timeout=" << timeout * 10 << "ms";
		cout << " Community=" << community.get_printable() << endl << flush;

	SnmpTarget *target;
		target = &ctarget;

	while((status = snmp.get_bulk(pdu, *target, 0, BULK_MAX)) == SNMP_CLASS_SUCCESS) {
		requests++;
		for (int z = 0; z<pdu.get_vb_count(); z++) {
			pdu.get_vb(vb, z);

			Oid tmp;
			vb.get_oid(tmp);
			if (subtree && (oid.nCompare(oid.len(), tmp) != 0))
			{
				cout << "End of SUBTREE Reached\n";
				cout << "Total # of Requests = " << requests << endl;
				cout << "Total # of Objects  = " << objects << endl;
				return -4;
			}
			objects++;
			// look for var bind exception, applies to v2 only   
			if (vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW) {
				cout << vb.get_printable_oid() << " = ";
				cout << vb.get_printable_value() << endl;
			}
			else {
				cout << "End of MIB Reached\n";
				cout << "Total # of Requests = " << requests << endl;
				cout << "Total # of Objects  = " << objects << endl;
				return -4;
			}
		}
		// last vb becomes seed of next rquest
		pdu.set_vblist(&vb, 1);
	}
	if (status != SNMP_ERROR_NO_SUCH_NAME)
		cout << "SNMP++ snmpWalk Error, " << snmp.error_msg(status) << endl;
	cout << "Total # of Requests = " << requests << endl;
	cout << "Total # of Objects  = " << objects << endl;

	Snmp::socket_cleanup();  // Shut down socket subsystem
}