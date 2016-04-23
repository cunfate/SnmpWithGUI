// mySnmpApp.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#define STDCXX_98_HEADERS
#define D_SCL_SECURE_NO_WARNINGS 
#include <iostream>
#include <string>
#include "snmp_pp/snmp_pp.h"
#include "snmpGet.h"
#include "snmpSet.h"
#include "snmpWalk.h"
using namespace std;
using namespace Snmp_pp;

int main(int argc, char* argv[])
{
	string ip, oid;
	int re = 0;
	cout << "Choose the operate:" << endl
		<< "1.get" << endl
		<< "2.set" << endl
		<< "3.walk" << endl;
	int select;
	cin >> select;
	cout << "Input ip :";
	cin >> ip;
	cout << "Input oid :";
	cin >> oid;
	switch (select)
	{
		case 1:
			re = snmpget((char *)ip.data(), (char *)oid.data());
			break;
		case 2:
			re = snmpset((char *)ip.data(), (char *)oid.data());
			break;
		case 3:
			re = snmpwalk((char *)ip.data(), (char *)oid.data());
			break;
		default:
			break;
	}
	system("pause");
	return re;
}

