#ifndef _SNMPSET_H
#define _SNMPSET_H

#include "snmp_pp/snmp_pp.h"
#include <iostream>

using namespace std;
using namespace Snmp_pp;

extern bool determine_vb(SmiUINT32 val, Vb &vb);
extern int snmpset(char *ipAddr, char *oid_in);
#endif