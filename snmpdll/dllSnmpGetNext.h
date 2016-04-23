#ifndef _DLLSNMPGETNEXT_H
#define _DLLSNMPGETNEXT_H
#include <iostream>
#include <string>
#include <libsnmp.h>
#include "dllSnmpGet.h"
extern "C"
{
	extern
	_declspec(dllexport)
	char * snmpGetNextOid(char * ip, char *oid_in);
}
#endif 