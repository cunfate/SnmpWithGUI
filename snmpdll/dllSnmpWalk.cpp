#include <libsnmp.h>
#include "snmp_pp/snmp_pp.h"
#include <string>
#include "dllSnmpConfig.h"

#ifdef WIN32
#define strcasecmp _stricmp
#endif

#define SNMP_PP_NAMESPACE
#ifdef SNMP_PP_NAMESPACE
using namespace Snmp_pp;
#endif

extern "C"
{
	//int main(int argc, char **argv)
	char walkNowVal[10000];
	_declspec(dllexport) 
	char * snmpWalk(char * ip, char * oid_in)
	{
#define _NO_LOGGING
#if !defined(_NO_LOGGING) && !defined(WITH_LOG_PROFILES)
		// Set filter for logging
		DefaultLog::log()->set_filter(ERROR_LOG, 7);
		DefaultLog::log()->set_filter(WARNING_LOG, 7);
		DefaultLog::log()->set_filter(EVENT_LOG, 7);
		DefaultLog::log()->set_filter(INFO_LOG, 7);
		DefaultLog::log()->set_filter(DEBUG_LOG, 7);
#endif
		std::string temp1 = "";
		std::string temp2 = "";
		Snmp::socket_startup();  // Initialize socket subsystem

		//---------[ make a GenAddress and Oid object to retrieve ]---------------
		UdpAddress address(ip);      // make a SNMP++ Generic address
		if (!address.valid()) {           // check validity of address
			std::cout << "Invalid Address or DNS Name, " << ip << "\n";
			//return 1;
		}
		Pdu pdu;                              // construct a Pdu object
		Vb vb;                                // construct a Vb object

		Oid oid(oid_in);
		if (!oid.valid()) {            // check validity of user oid
			std::cout << "Invalid Oid, " << oid_in << "\n";
			//		return -2;
			temp2 = "Invalid OID!";
			strcpy_s(walkNowVal, temp2.c_str());
			return walkNowVal;
		}
		vb.set_oid(oid);
		pdu += vb;                            // add the vb to the Pdu

		//---------[ determine options to use ]-----------------------------------
		snmp_version version = version2c;          // default is v2c
		int retries = 1;                          // default retries is 1
		int timeout = 100;                        // default is 1 second
		u_short port = 161;                       // default snmp port is 161
		OctetStr community("public");           // community name
		int non_reps = 0;                         // non repeaters default is 0
		int max_reps = 10;                        // maximum repetitions default is 10


		char *ptr;


#ifdef WITH_LOG_PROFILES
		if (strstr(argv[x], "-L") != 0) {
			ptr = argv[x]; ptr++; ptr++;
			DefaultLog::log()->set_profile(ptr);
		}
#endif




		//----------[ create a SNMP++ session ]-----------------------------------
		int status;
		// bind to any port and use IPv6 if needed
		Snmp snmp(status, 0, (address.get_ip_version() == Address::version_ipv6));

		if (status != SNMP_CLASS_SUCCESS) {
			std::cout << "SNMP++ Session Create Fail, " << snmp.error_msg(status) << "\n";
		}

		//---------[ init SnmpV3 ]--------------------------------------------


		//--------[ build up SNMP++ object needed ]-------------------------------
		address.set_port(port);
		CTarget ctarget(address);             // make a target using the address

		ctarget.set_version(version);         // set the SNMP version SNMPV1 or V2
		ctarget.set_retry(retries);           // set the number of auto retries
		ctarget.set_timeout(timeout);         // set timeout
		ctarget.set_readcommunity(community); // set the read community name


		//-------[ issue the request, blocked mode ]-----------------------------
		std::cout << "SNMP++ GetBulk to " << ip << " SNMPV"

			<< (version + 1)
			<< " Retries=" << retries
			<< " Timeout=" << timeout * 10 << "ms"
			<< " Non Reptrs=" << non_reps
			<< " Max Reps=" << max_reps << std::endl;
		std::cout << " Community=" << community.get_printable() << std::endl << std::flush;

		SnmpTarget *target;
		target = &ctarget;

		if ((status = snmp.get_bulk(pdu, *target, non_reps, max_reps)) == SNMP_CLASS_SUCCESS) {
			for (int z = 0; z < pdu.get_vb_count(); z++) {
				pdu.get_vb(vb, z);
				std::cout << "Oid = " << vb.get_printable_oid() << "\n";
				temp2 = vb.get_printable_oid();
				temp1 += "Oid = " + temp2 + "\r\n";
				if (vb.get_syntax() != sNMP_SYNTAX_ENDOFMIBVIEW) {
					std::cout << "Value = " << vb.get_printable_value() << "\n\n";
					temp2 = vb.get_printable_value();
					temp1 += "Value = " + temp2 + "\r\n";
				}
				else {
					std::cout << "End of MIB view.\n\n";
				}
			}
		}
		else
			std::cout << "SNMP++ GetBulk Error, " << snmp.error_msg(status) << "\n";
		temp1 = "Walk List :\r\n" + temp1;
		strcpy_s(walkNowVal,temp1.c_str());
		Snmp::socket_cleanup();  // Shut down socket subsystem
		return walkNowVal;
	}
}