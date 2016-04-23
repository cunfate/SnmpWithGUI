#ifndef _DLLSNMPSET_H
#define _DLLSNMPSET_H

#include "snmp_pp/snmp_pp.h"
#include <iostream>

using namespace std;
using namespace Snmp_pp;

extern "C"
{
	extern bool determine_vb(SmiUINT32 val, Vb &vb,const char * content);
	extern _declspec(dllexport) const char *snmpSet(const char *ipAddr, const char *oid_in, const char * content);
}
#endif