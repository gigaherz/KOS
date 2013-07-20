// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SYSTEMLIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SYSTEMLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SYSTEMLIB_EXPORTS
#define SYSTEMLIB_API __declspec(dllexport)
#else
#define SYSTEMLIB_API __declspec(dllimport)
#endif

// This class is exported from the SystemLib.dll
class SYSTEMLIB_API CSystemLib {
public:
	CSystemLib(void);
	// TODO: add your methods here.
};

extern SYSTEMLIB_API int nSystemLib;

SYSTEMLIB_API int fnSystemLib(void);
