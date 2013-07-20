// ServiceHandler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

#include <stdio.h>
#include "APIServer_h.h"

int _tmain()
{
	RPC_STATUS status;

	// Uses the protocol combined with the endpoint for receiving
	// remote procedure calls.
	status = RpcServerUseProtseqEp(
				(RPC_WSTR)_T("ncalrpc"),// Use TCP/IP protocol   
				RPC_C_PROTSEQ_MAX_REQS_DEFAULT,    // Backlog q length for TCP/IP.
				(RPC_WSTR)_T("XtOS_APIServer"),    // TCP/IP port to use.
				NULL);       // No security.

	if(status)
		return status;

	// Registers the DoRPC interface.
	status = RpcServerRegisterIf(
				APIServer_v1_0_s_ifspec, // Interface to register.
				NULL,   // Use the MIDL generated entry-point vector.
				NULL);   // Use the MIDL generated entry-point vector.

	if (status)
	exit(status);

	// Start to listen for remote procedure calls for all registered interfaces.
	// This call will not return until RpcMgmtStopServerListening is called.
	status = RpcServerListen(
				1,       // Recommended minimum number of threads.
				RPC_C_LISTEN_MAX_CALLS_DEFAULT,  // Recommended maximum number of threads.
				FALSE);              // Start listening now.

	if(status)
		return status;

	return 0;
}

// Memory allocation function for RPC.
// The runtime uses these two functions for allocating/deallocating
// enough memory to pass the string to the server.
void* __RPC_USER midl_user_allocate(size_t size)
{
    return malloc(size);
}

// Memory deallocation function for RPC.
void __RPC_USER midl_user_free(void* p)
{
    free(p);
}

// Now we implement our server function.
int CallFunction( 
    /* [in] */ int funcId,
    /* [out][in] */ byte *bData,
    /* [in] */ int bLen,
    /* [out][in] */ int *retLen)
{
	switch(funcId)
	{
	case 0:
		return 0;

	case 1: // Send message to System Debug Log (server console)
		{
			char *t=(char*)malloc(bLen+1);
			memcpy(t,bData,bLen);
			t[bLen]=0;
			printf("%s\n",t);
			free(t);
		}
		return 0;

	default:
		return -1;
	}
}
