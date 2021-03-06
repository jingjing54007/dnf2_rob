/* soapflowcontrolProxy.h
   Generated by gSOAP 2.7.15 from IFlowControl.h
   Copyright(C) 2000-2009, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#ifndef soapflowcontrolProxy_H
#define soapflowcontrolProxy_H
#include "soapH.h"

class SOAP_CMAC flowcontrolProxy : public soap
{ public:
	/// Endpoint URL of service 'flowcontrolProxy' (change as needed)
	const char *soap_endpoint;
	/// Constructor
	flowcontrolProxy();
	/// Constructor with copy of another engine state
	flowcontrolProxy(const struct soap&);
	/// Constructor with engine input+output mode control
	flowcontrolProxy(soap_mode iomode);
	/// Constructor with engine input and output mode control
	flowcontrolProxy(soap_mode imode, soap_mode omode);
	/// Destructor frees deserialized data
	virtual	~flowcontrolProxy();
	/// Initializer used by constructors
	virtual	void flowcontrolProxy_init(soap_mode imode, soap_mode omode);
	/// Delete all deserialized data (uses soap_destroy and soap_end)
	virtual	void destroy();
	/// Disables and removes SOAP Header from message
	virtual	void soap_noheader();
	/// Get SOAP Fault structure (NULL when absent)
	virtual	const SOAP_ENV__Fault *soap_fault();
	/// Get SOAP Fault string (NULL when absent)
	virtual	const char *soap_fault_string();
	/// Get SOAP Fault detail as string (NULL when absent)
	virtual	const char *soap_fault_detail();
	/// Force close connection (normally automatic, except for send_X ops)
	virtual	int soap_close_socket();
	/// Print fault
	virtual	void soap_print_fault(FILE*);
#ifndef WITH_LEAN
	/// Print fault to stream
	virtual	void soap_stream_fault(std::ostream&);
	/// Put fault into buffer
	virtual	char *soap_sprint_fault(char *buf, size_t len);
#endif

	/// Web service operation 'CmAuthByUser' (returns error code or SOAP_OK)
	virtual	int CmAuthByUser(int inGroupID, int inServiceID, char *inAuthString, enum fc__AuthResult *outResult);

	/// Web service operation 'CmAuthByQQ' (returns error code or SOAP_OK)
	virtual	int CmAuthByQQ(ULONG64 inQQ, int inGroupID, char *inAuthString, enum fc__AuthResult *outResult);

	/// Web service operation 'CheckMoneySend' (returns error code or SOAP_OK)
	virtual	int CheckMoneySend(char *inUser, int inGroupID, int inWorldID, int inSendValue, enum fc__CheckResult *outResult);

	/// Web service operation 'CheckItemSend' (returns error code or SOAP_OK)
	virtual	int CheckItemSend(char *inUser, int inGroupID, int inWorldID, char *inItemID, int inSendValue, enum fc__CheckResult *outResult);

	/// Web service operation 'CheckBatchMoneySend' (returns error code or SOAP_OK)
	virtual	int CheckBatchMoneySend(char *inUser, int inGroupID, int inWorldID, int inSendValue, char *inTaskID, char *inApprovalCode, enum fc__CheckResult *outResult);

	/// Web service operation 'CheckBatchItemSend' (returns error code or SOAP_OK)
	virtual	int CheckBatchItemSend(char *inUser, int inGroupID, int inWorldID, char *inItemID, int inSendValue, char *inTaskID, char *inApprovalCode, enum fc__CheckResult *outResult);

	/// Web service operation 'GetOAName' (returns error code or SOAP_OK)
	virtual	int GetOAName(char *inAuthString, char **opname);
};
#endif
