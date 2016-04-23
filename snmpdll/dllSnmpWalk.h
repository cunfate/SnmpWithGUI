#ifndef _SNMPWALK_H_
#define _SNMPWALK_H_

#include "snmp_pp/snmp_pp.h"
#include <iostream>
extern "C"
{
	extern 	_declspec(dllexport) const char * snmpwalk(char * ip, char * oid_in);
}
#endif