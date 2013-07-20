//API Data Types
//Explicit-sized types
typedef __int8  INT8;	//Signed Integers
typedef __int16 INT16;
typedef __int32 INT32;
typedef __int64 INT64;
typedef unsigned __int8  UINT8;	//Unsigned Integers
typedef unsigned __int16 UINT16;
typedef unsigned __int32 UINT32;
typedef unsigned __int64 UINT64;

//Special types
typedef int INT;		//biggest native integer type (signed)
typedef unsigned UINT;	//biggest native integer type (unsigned)
typedef void* PANY;		//a container for an address of unspecified type
typedef UINT RESULT;

#define IN(x)	x	//Input parameters are passed directly
#define OUT(x)	x &	//Output parameters are passed by reference

//Memory Functions
RESULT MemAlloc  (IN(UINT) size, OUT(PANY) ptr);	//Allocate <size> bytes of memory
RESULT MemMap    (IN(PANY) addr, OUT(UINT) size);	//Map <size> bytes of memory to the specified address
RESULT MemRealloc(IN(PANY) ptr, IN(UINT) size, OUT(PANY) nptr); //Resizes an allocation or creates a new one and copies the contents
RESULT MemDup    (IN(PANY) src, IN(UINT) size, OUT(PANY) nptr); //Duplicates a memory buffer
RESULT MemFree   (IN(PANY) addr);

RESULT MemCopy   (IN(PANY) dest, IN(PANY) src, IN(UINT) size);
RESULT MemSet    (IN(PANY) dest, IN(INT8) value, IN(UINT) size);
RESULT MemZero   (IN(PANY) dest, IN(UINT) size);

//File Functions
RESULT FileOpen  (
RESULT FileClose

RESULT FileRead
RESULT FileWrite

RESULT FileGetPointer
RESULT FileSetPointer

RESULT FileGetAttr
RESULT FileSetAttr

Directory Functions
RESULT DirOpen
RESULT DirClose

RESULT DirGetEntry
RESULT DirAddEntry
RESULT DirDelEntry

RESULT DirGetAttr (=FileGetAttr)
RESULT DirSetAttr (=FileGetAttr)

Service Functions
RESULT ServiceAdd
RESULT ServiceDel
RESULT ServiceSetCallback
