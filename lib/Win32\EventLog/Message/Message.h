//  Message.h
//  Header for the Win32::EventLog::Message Perl extension.
//  (c) Dave Roth
//  Courtesy of Roth Consulting
//  http://www.roth.net/
//
//  Available under the GNU license.
//  2000.03.19

#include "Win32Perl.h"
 
#ifndef _MESSAGE_H
#   define _MESSAGE_H 

#   ifndef WIN32
#       ifdef _WIN32
#           define WIN32   
#       endif // _WIN32  
#   endif // WIN32  
  
#   include "PerlMsg.h"
///////////////////////////////////////////////////////////////////////////////////////////
//	Begin resource compiler macro block

    //  Include the version information...
    #include "version.h"

    #define	EXTENSION_NAME			"Message"

    #define	EXTENSION_PARENT_NAMESPACE		"Win32::EventLog"
    #define EXTENSION				EXTENSION_PARENT_NAMESPACE "::" EXTENSION_NAME
    
    #define	EXTENSION_FILE_NAME		EXTENSION_NAME

    #define	EXTENSION_VERSION		VERSION
    #define	EXTENSION_AUTHOR		"Dave Roth <rothd@roth.net>"

    #define	COPYRIGHT_YEAR			"2000"
    #define	COPYRIGHT_NOTICE		"Copyright (c) " COPYRIGHT_YEAR

    #define COMPANY_NAME			"Roth Consulting\r\nhttp://www.roth.net/consult"

    #define	VERSION_TYPE			"Beta"
//	End resource compiler macro block
///////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//  Define a method to report exceptions
#ifdef DEBUG
    #define REPORT_EXCEPTION    _tprintf( TEXT( "Error! An Exception has been caught.\n" ) )
#else   //  DEBUG
    #define REPORT_EXCEPTION    
#endif  //  DEBUG

#define EVENT_LOG_PATH          TEXT( "System\\CurrentControlSet\\Services\\EventLog" )

HMODULE ghModule;
BOOL SourceExists( LPTSTR pszEventLogName, LPTSTR pszSourceName );
BOOL DeleteKey( HKEY hPath, LPCTSTR pszKeyName );
BOOL RegisterMessageTable( LPCTSTR pszEventLogName, LPCTSTR pszSourceName, LPCTSTR pszPath );
BOOL UnRegisterMessageTable( LPCTSTR pszEventLogName, LPCTSTR pszSourceName );


#endif // _MESSAGE_H

