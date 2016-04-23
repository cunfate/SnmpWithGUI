/*_############################################################################
_##
_##  snmpWalk.cpp
_##
_##  SNMP++ v3.3
_##  -----------------------------------------------
_##  Copyright (c) 2001-2013 Jochen Katz, Frank Fock
_##
_##  This software is based on SNMP++2.6 from Hewlett Packard:
_##
_##    Copyright (c) 1996
_##    Hewlett-Packard Company
_##
_##  ATTENTION: USE OF THIS SOFTWARE IS SUBJECT TO THE FOLLOWING TERMS.
_##  Permission to use, copy, modify, distribute and/or sell this software
_##  and/or its documentation is hereby granted without fee. User agrees
_##  to display the above copyright notice and this license notice in all
_##  copies of the software and any documentation of the software. User
_##  agrees to assume all liability for the use of the software;
_##  Hewlett-Packard and Jochen Katz make no representations about the
_##  suitability of this software for any purpose. It is provided
_##  "AS-IS" without warranty of any kind, either express or implied. User
_##  hereby grants a royalty-free license to any and all derivatives based
_##  upon this software code base.
_##
_##########################################################################*/
/*
snmpWalk.cpp

Copyright (c) 1996
Hewlett-Packard Company

ATTENTION: USE OF THIS SOFTWARE IS SUBJECT TO THE FOLLOWING TERMS.
Permission to use, copy, modify, distribute and/or sell this software
and/or its documentation is hereby granted without fee. User agrees
to display the above copyright notice and this license notice in all
copies of the software and any documentation of the software. User
agrees to assume all liability for the use of the software; Hewlett-Packard
makes no representations about the suitability of this software for any
purpose. It is provided "AS-IS" without warranty of any kind,either express
or implied. User hereby grants a royalty-free license to any and all
derivatives based upon this software code base.

Peter E. Mellquist
*/
char snmpwalk_cpp_version[] = "@(#) SNMP++ $Id: snmpWalk.cpp 2471 2013-11-14 19:49:48Z fock $";
#include <libsnmp.h>
#include <string>

#include "snmp_pp/snmp_pp.h"

#ifdef WIN32
#define strcasecmp _stricmp
#endif


using namespace Snmp_pp;
using namespace std;

#define BULK_MAX 10


char snmpWalkResult[1000] = "";
char * snmpWalk2(char * ip, char * oid_in)
{
	int requests = 0;        // keep track of # of requests
	int objects = 0;
	string temp1 = "";
	string temp2 = "";

	Snmp::socket_startup();  // Initialize socket subsystem

	//---------[ make a GenAddress and Oid object to retrieve ]---------------
	UdpAddress address(ip);      // make a SNMP++ Generic address
	if (!address.valid()) {           // check validity of address
    //		cout << "Invalid Address or DNS Name, " << argv[1] << "\n";
		temp1 = "Invalid Address!";
		strcpy_s(snmpWalkResult,temp1.c_str());
	}
	Oid oid("1");                      // default is beginning of MIB 
	oid = oid_in;
	if (!oid.valid()) {            // check validity of user oid
		temp1 = "Invalid OID!";
		strcpy_s(snmpWalkResult, temp1.c_str());
	}
		
	

	//---------[ determine options to use ]-----------------------------------
	snmp_version version = version2c;                  // default is v1
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

	if (status != SNMP_CLASS_SUCCESS) {
		cout << "SNMP++ Session Create Fail, " << snmp.error_msg(status) << "\n";
	//	return 1;
		temp1 = "SNMP++ Session Create Fail";
		strcpy_s(snmpWalkResult, temp1.c_str());
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
	cout << "SNMP++ snmpWalk to " << argv[1] << " SNMPV"

		<< (version + 1)
		<< " Retries=" << retries
		<< " Timeout=" << timeout * 10 << "ms";
	cout << " Community=" << community.get_printable() << endl << flush;

	SnmpTarget *target;
	target = &ctarget;

	while ((status = snmp.get_bulk(pdu, *target, 0, BULK_MAX)) == SNMP_CLASS_SUCCESS) {
		requests++;
		for (int z = 0; z<pdu.get_vb_count(); z++) {
			pdu.get_vb(vb, z);
			Oid tmp;
			vb.get_oid(tmp);
			if (subtree && (oid.nCompare(oid.len(), tmp) != 0))
			{
				cout << "End of SUBTREE Reached\n";
				cout << "Total # of Requests = " << requests << "\n";
				cout << "Total # of Objects  = " << objects << "\n";
				return -4;
			}
			objects++;
			// look for var bind exception, applies to v2 only   
			if (vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW) {
				cout << vb.get_printable_oid() << " = ";
				cout << vb.get_printable_value() << "\n";
			}
			else {
				cout << "End of MIB Reached\n";
				cout << "Total # of Requests = " << requests << "\n";
				cout << "Total # of Objects  = " << objects << "\n";
				return -4;
			}
		}
		// last vb becomes seed of next rquest
		pdu.set_vblist(&vb, 1);
	}
	if (status != SNMP_ERROR_NO_SUCH_NAME)
		cout << "SNMP++ snmpWalk Error, " << snmp.error_msg(status) << "\n";
	cout << "Total # of Requests = " << requests << "\n";
	cout << "Total # of Objects  = " << objects << "\n";

	Snmp::socket_cleanup();  // Shut down socket subsystem
}
