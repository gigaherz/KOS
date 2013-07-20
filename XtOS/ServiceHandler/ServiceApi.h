#pragma once

typedef enum:unsigned int
{
	ResultOk			= 0x00000000,
	ResultFail			= 0x80000000,

	//Reasons
	ReasonUnknown		= 0x00000000,
	ReasonPermission	= 0x00010000,
	ReasonPathError		= 0x00020000,
	//...

	ReasonUserDefined	= 0x7fff0000,

	//Permission Errors
	ErrorNoPermission	= 0x00000001,
	ErrorLocked			= 0x00000002,
   
	//File/Directory Errors
	ErrorPathNotFound	= 0x00000001,

	//other misc error messages here


	ErrorGeneric		= 0x0000ffff
} ApiResult;
// Example: 0xFFFFFFFF = Failed for an user-defined generic error.
//			0x80010001 = ERROR: Not enough permissions to do the requested action.
typedef unsigned char Byte;
typedef unsigned int  UInt;
typedef unsigned int  ApiObject;

typedef struct {
	unsigned int length;
	wchar_t* data;
} ApiString;

typedef struct Guid {
	UInt a;
	UInt b;
	UInt c;
	UInt d;
} Guid;

typedef struct AccessControlList {
	UInt dummy;
} AccessControlList;

// Object Api
ApiResult ObjectOpenFromGuid(ApiObject& retVal, const Guid& guid);
ApiResult ObjectClose(ApiObject& object);

// Security Api
ApiResult ObjectGetAccessControlList(ApiObject& object, const AccessControlList& acl);
ApiResult ObjectSetAccessControlList(ApiObject& object, const AccessControlList& acl);

// Directory Api
ApiResult DirectoryOpen(ApiObject& retVal, const ApiString& directoryPath);

ApiResult DirectoryGetFirstEntry(ApiString& retVal, ApiObject& dir);
ApiResult DirectoryGetNextEntry (ApiString& retVal, ApiObject& dir);

// File Api

// Debug Api
int SystemLogPrint(const char* text);

// System
int APIMain();