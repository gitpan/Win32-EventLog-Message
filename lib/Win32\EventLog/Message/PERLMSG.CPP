//  PerlMsg.cpp
//  Perl stub for the Win32::Daemon Perl extension.
//  (c) Dave Roth
//  Courtesy of Roth Consulting
//  http://www.roth.net/
//
//  2000.06.12

#define WIN32_LEAN_AND_MEAN

#ifdef __BORLANDC__
typedef wchar_t wctype_t; /* in tchar.h, but unavailable unless _UNICODE */
#endif
 
#include <windows.h>
#include <tchar.h>
#include <wtypes.h>
#include <stdio.h>		//	Gurusamy's right, Borland is brain damaged!
#include <math.h>		//	Gurusamy's right, MS is brain damaged!
#include <time.h>

//	If we are building with the core distribution headers we can not define
//	the function names using C++ because of name mangling
#if defined(__cplusplus) && !defined(PERL_OBJECT)
extern "C" {
#endif
    #include "EXTERN.h"
    #include "perl.h"
    #include "XSub.h"

#if defined(__cplusplus) && !defined(PERL_OBJECT)
}
#endif

#include "PerlMsg.h"
#include "version.h"
#include "Win32Perl.h"


//////////////////////////////////////////////////////////////////
XS( boot_Win32__EventLog__Message )
{
	dXSARGS;
    EXTENSION_VARS;
	LPTSTR file = __FILE__;
	int	retcode = 1;

    XS_VERSION_BOOTCHECK ;

	newXS( EXTENSION "::Constant",			XS_WIN32__EventLog_Message_Constant, file );

    XSRETURN_YES;

}			



BOOL APIENTRY DllMain( HANDLE hModule, DWORD dwMessage, LPVOID lpReserved )
{
	BOOL	bResult = TRUE;

    switch( dwMessage ) {
		case DLL_PROCESS_ATTACH: 
	    case DLL_THREAD_ATTACH:
	    case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;

	}
	return bResult;
}

