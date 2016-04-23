#ifndef _DLLSNMPGET_H
#define _DLLSNMPGET_H
#include <iostream>
#include <string>
#define _EXTERN_C_  extern "C"  _declspec(dllexport)
using namespace std;
extern "C"
{
	extern _declspec(dllexport) const char * snmpGet(const char * ipaddr, const char * oid_in);
}
#endif