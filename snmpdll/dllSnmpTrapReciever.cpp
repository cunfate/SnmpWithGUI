// snmpTrapReceiver.cpp : 定义控制台应用程序的入口点。
//


/*_############################################################################
_##
_##  receive_trap.cpp
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

char receivetrap_cpp_version[] = "@(#) SNMP++ $Id: receive_trap.cpp 2359 2013-05-09 20:07:01Z fock $";
#include <libsnmp.h>
#include <iostream>

#include "snmp_pp/snmp_pp.h"
#include "snmp_pp/collect.h"
#include "snmp_pp/notifyqueue.h"
#include <string>
#include <cstdlib>

//#ifdef SNMP_PP_NAMESPACE
using namespace Snmp_pp;
//#endif
using namespace std;
extern "C"
{
	string trapoid;
	string re;
	int trapCounter = 0;

	_declspec(dllexport) 
	int getTrapCounter(void)
	{
		int t = trapCounter;
//		trapCounter = 0;
		return t;
	}
	//char Result[100];
	_declspec(dllexport)
	void sendPtrToCallback(char * in)
	{
	//	Result = in;
	}
	_declspec(dllexport)
	const char * getTrapInformation()
	{
		re = trapoid;
		trapoid = "";
		trapCounter = 0;
		return re.c_str();
		//return (trapoid.c_str());
	}
	//_declspec(dllexport)

	void snmpTrapCallback(int reason, Snmp *snmp, Pdu &pdu, SnmpTarget &target, void *cd)
	{
		Vb nextVb;
		int pdu_error;
		GenAddress addr;

		target.get_address(addr);
		UdpAddress from(addr);
		
		trapCounter = 1;
		cout << "reason: " << reason << endl
			<< "msg: " << snmp->error_msg(reason) << endl
			<< "from: " << from.get_printable() << endl;

		pdu_error = pdu.get_error_status();
		if (pdu_error){
			cout << "Response contains error: "
				<< snmp->error_msg(pdu_error) << endl;
		}
		Oid id;
		pdu.get_notify_id(id);
		cout << "ID:  " << id.get_printable() << endl;
		cout << "Type:" << pdu.get_type() << endl;
		//trapoid = (string)"\r\nFrom:" + from.get_printable();
		//trapoid += (string)"\r\nOid : " + (id.get_printable());
		trapoid += (string)"2c:" + from.get_printable() + (string)":";
		trapoid += id.get_printable() + (string)";";

		for (int i = 0; i < pdu.get_vb_count(); i++)
		{
			pdu.get_vb(nextVb, i);

			cout << "Oid: " << nextVb.get_printable_oid() << endl
				<< "Val: " << nextVb.get_printable_value() << endl;
		//	trapoid += nextVb.get_printable_oid();
			trapoid += nextVb.get_printable_oid() + (string)":" ;
			trapoid += nextVb.get_printable_value() + (string)":";
		}
		trapoid += (string)"\n";
		if (pdu.get_type() == sNMP_PDU_INFORM) {
			cout << "pdu type: " << pdu.get_type() << endl;
			cout << "sending response to inform: " << endl;
			nextVb.set_value("This is the response.");
			pdu.set_vb(nextVb, 0);
			snmp->response(pdu, target);
		}
		cout << endl;
	}

	_declspec(dllexport)
	int snmpTrapRecieveStart(int trap_port)
	{
		//		trap_port = 10162; // no need to be root
		//	else
		//		trap_port = atoi(argv[1]);

		//----------[ create a SNMP++ session ]-----------------------------------
		//int status;
		int status;
		Snmp snmptrap(status);                // check construction status
		Snmp::socket_startup();  // Initialize socket subsystem

		if (status != SNMP_CLASS_SUCCESS)
		{
			cout << "SNMP++ Session Create Fail, " << snmptrap.error_msg(status) << "\n";
			return 1;
		}
		//cout << trap_port << endl;

		//---------[ init SnmpV3 ]--------------------------------------------
#ifdef _SNMPv3
		const unsigned char engineId[] =
		{ 0x80, 0x00, 0x13, 0x70, 0xC0, 0xA8, 0x01, 0x0D };
		OctetStr engineID(engineId, 8);
		const char *filename = "snmpv3_boot_counter";
		unsigned int snmpEngineBoots = 0;
		v3MP *v3_MP;

		status = getBootCounter(filename, engineID, snmpEngineBoots);
		if ((status != SNMPv3_OK) && (status < SNMPv3_FILEOPEN_ERROR))
		{
			cout << "Error loading snmpEngineBoots counter: " << status << endl;
			return 1;
		}
		snmpEngineBoots++;
		status = saveBootCounter(filename, engineID, snmpEngineBoots);
		if (status != SNMPv3_OK)
		{
			cout << "Error saving snmpEngineBoots counter: " << status << endl;
			return 1;
		}

		int construct_status;
		v3_MP = new v3MP(engineID, snmpEngineBoots, construct_status);
		if (construct_status != SNMPv3_MP_OK)
		{
			cout << "Error initializing v3MP: " << construct_status << endl;
			return 1;
		}

		USM *usm = v3_MP->get_usm();

		// users at UCD
		usm->add_usm_user("SHADESUser",
			SNMP_AUTHPROTOCOL_HMACSHA, SNMP_PRIVPROTOCOL_DES,
			"The UCD Demo Password", "The UCD Demo Password");

		usm->add_usm_user("SHAMD5User",
			SNMP_AUTHPROTOCOL_HMACMD5, SNMP_PRIVPROTOCOL_DES,
			"The UCD Demo Password", "The UCD Demo Password");

		usm->add_usm_user("noAuthUser",
			SNMP_AUTHPROTOCOL_HMACMD5, SNMP_PRIVPROTOCOL_DES,
			"Password", "Password");

		// testusers
		usm->add_usm_user("unsecureUser",
			SNMP_AUTHPROTOCOL_NONE, SNMP_PRIVPROTOCOL_NONE,
			"", "");

		usm->add_usm_user("MD5",
			SNMP_AUTHPROTOCOL_HMACMD5, SNMP_PRIVPROTOCOL_NONE,
			"MD5UserAuthPassword", "");

		usm->add_usm_user("SHA",
			SNMP_AUTHPROTOCOL_HMACSHA, SNMP_PRIVPROTOCOL_NONE,
			"SHAUserAuthPassword", "");

		usm->add_usm_user("MD5DES",
			SNMP_AUTHPROTOCOL_HMACMD5, SNMP_PRIVPROTOCOL_DES,
			"MD5DESUserAuthPassword", "MD5DESUserPrivPassword");

		usm->add_usm_user("SHADES",
			SNMP_AUTHPROTOCOL_HMACSHA, SNMP_PRIVPROTOCOL_DES,
			"SHADESUserAuthPassword", "SHADESUserPrivPassword");

		usm->add_usm_user("MD5IDEA",
			SNMP_AUTHPROTOCOL_HMACMD5, SNMP_PRIVPROTOCOL_IDEA,
			"MD5IDEAUserAuthPassword", "MD5IDEAUserPrivPassword");

		usm->add_usm_user("SHAIDEA",
			SNMP_AUTHPROTOCOL_HMACSHA, SNMP_PRIVPROTOCOL_IDEA,
			"SHAIDEAUserAuthPassword", "SHAIDEAUserPrivPassword");

#endif
		OidCollection oidc;
		TargetCollection targetc;
		
		cout << "Trying to register for traps on port " << trap_port << "." << endl;
		snmptrap.notify_set_listen_port(trap_port);
		status = snmptrap.notify_register(oidc, targetc, snmpTrapCallback, NULL);
		if (status != SNMP_CLASS_SUCCESS)
		{
			cout << "Error register for notify (" << status << "): "
				<< snmptrap.error_msg(status) << endl;
			exit(1);
		}
		else
			cout << "Waiting for traps/informs..." << endl;

		if (snmptrap.start_poll_thread(1000))

			cout << "strat thread successful!" << endl;
		else
			cout << "start thread failed!" << endl;

		
//		strcpy(Result, "NONONONO!");
		cout << "press return to stop\n";
		//getc(stdin);

		while (1);
		snmptrap.stop_poll_thread();
		Snmp::socket_cleanup();  // Shut down socket subsystem
	}

	//_declspec(dllexport)
	//	void snmpStartPollThread()
	//{
	//	if (snmptrap.start_poll_thread(1000))

	//		cout << "strat thread successful!" << endl;
	//	else
	//		cout << "start thread failed!" << endl;
	//}
	//_declspec(dllexport)
	//	void snmpStopPollThread()
	//{
	//	snmptrap.stop_poll_thread();
	//}
}


