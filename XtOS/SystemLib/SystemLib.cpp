// SystemLib.cpp : Defines the entry point for the DLL application.
//
#include "SystemLib.h"
#include "../ServiceHandler/ServiceApi.h"

#include <stdio.h>
#include "../ServiceHandler/APIServer_h.h"

int main()
{
	RPC_STATUS status;
	RPC_WSTR szStringBinding = NULL;

	// Creates a string binding handle.
	// This function formats the passed values in a
	// predefined format for use by RPC. Just like printf
	// Connection is not done here.
	status = RpcStringBindingCompose(
				NULL, // UUID to bind to.
				(RPC_WSTR)L"ncalrpc",
				(RPC_WSTR)L"",
				(RPC_WSTR)L"XtOS_APIServer",
				NULL,     // Protocol dependent network options to use.
				&szStringBinding);   // String binding output.

	if (status)
		return status;

	// Validates the format of the string binding handle and converts
	// it to a binding handle.
	// Connection is not done here either.
	status = RpcBindingFromStringBinding(
				szStringBinding, // The string binding to validate.
				&hAPIServerBinding); // Put the result in the implicit binding

	// handle defined in the IDL file.

	if(status)
		return status;

	RpcTryExcept
		{
			int ret = APIMain();
		}
	RpcExcept(1)
		{
			printf("Runtime exception occured: %d\n",RpcExceptionCode());
		}
	RpcEndExcept

	// Free the memory allocated by a string.
	status = RpcStringFree(&szStringBinding);
	// String to be freed.

	if (status)
		return status;

	// Releases binding handle resources and disconnects from the server.
	status = RpcBindingFree(
		&hAPIServerBinding); // Frees the implicit binding handle defined in the IDL file.

	if (status)
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
