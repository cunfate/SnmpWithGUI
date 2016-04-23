char receivetrap_cpp_version[]="@(#) SNMP++ $Id: receive_trap.cpp 2359 2013-05-09 20:07:01Z fock $";
#include <libsnmp.h>

#include "snmp_pp/snmp_pp.h"
#include "snmp_pp/collect.h"
#include "snmp_pp/notifyqueue.h"

#define SNMP_PP_NAMESPACE
#ifdef SNMP_PP_NAMESPACE
using namespace Snmp_pp;
#endif

void callback( int reason, Snmp *snmp, Pdu &pdu, SnmpTarget &target, void *cd)
{
  Vb nextVb;
  int pdu_error;
  GenAddress addr;

  target.get_address(addr);
  UdpAddress from(addr);

  cout << "reason: " << reason << endl
       << "msg: " << snmp->error_msg(reason) << endl
       << "from: " << from.get_printable() << endl;

  pdu_error = pdu.get_error_status();
  if (pdu_error){
    cout << "Response contains error: " 
	 << snmp->error_msg(pdu_error)<< endl;
  }
  Oid id;
  pdu.get_notify_id(id);
  cout << "ID:  " << id.get_printable() << endl;
  cout << "Type:" << pdu.get_type() << endl;

  for (int i=0; i<pdu.get_vb_count(); i++)
  {
    pdu.get_vb(nextVb, i);

    cout << "Oid: " << nextVb.get_printable_oid() << endl
	 << "Val: " <<  nextVb.get_printable_value() << endl;
  }
  if (pdu.get_type() == sNMP_PDU_INFORM) {
      cout << "pdu type: " << pdu.get_type() << endl;
      cout << "sending response to inform: " << endl;
      nextVb.set_value("This is the response.");
      pdu.set_vb(nextVb, 0);
      snmp->response(pdu, target);
  }
  cout << endl;
}

int main(int argc, char **argv)
{
  int trap_port;
  if (argc < 2)
    trap_port = 10162; // no need to be root
  else
    trap_port = atoi(argv[1]);

  //----------[ create a SNMP++ session ]-----------------------------------
  int status; 
  Snmp::socket_startup();  // Initialize socket subsystem
  Snmp snmp(status);                // check construction status
  if ( status != SNMP_CLASS_SUCCESS)
  {
    cout << "SNMP++ Session Create Fail, " << snmp.error_msg(status) << "\n";
    return 1;
  }

   //---------[ init SnmpV3 ]--------------------------------------------

   OidCollection oidc;
   TargetCollection targetc;

   cout << "Trying to register for traps on port " << trap_port << "." << endl;
   snmp.notify_set_listen_port(trap_port);
   status = snmp.notify_register(oidc, targetc, callback, NULL);
   if (status != SNMP_CLASS_SUCCESS)
   {
     cout << "Error register for notify (" << status << "): "
	  << snmp.error_msg(status) << endl;
     exit(1);
   }
   else
     cout << "Waiting for traps/informs..." << endl;


  snmp.start_poll_thread(1000);

  cout << "press return to stop\n";
  getc(stdin);

  snmp.stop_poll_thread();

  Snmp::socket_cleanup();  // Shut down socket subsystem
}
  
