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

#include "Message.h"
#include "constant.h"

/*----------------------- M I S C   F U N C T I O N S -------------------*/
//////////////////////////////////////////////////////////////////
BOOL DeleteKey( HKEY hPath, LPCTSTR pszKeyName )
{
    HKEY hKey;
    BOOL fResult = FALSE;

    if( ERROR_SUCCESS == RegOpenKeyEx( hPath, pszKeyName, 0, KEY_ALL_ACCESS, &hKey ) )
    {
        TCHAR szKeyName[ MAX_PATH ];
        DWORD dwKeySize = sizeof( szKeyName ) / sizeof( TCHAR );
        DWORD dwIndex =0;

        while( ERROR_NO_MORE_ITEMS != RegEnumKeyEx( hKey, dwIndex, szKeyName, &dwKeySize, 0, NULL, NULL, NULL ) )
        {
            fResult ^= DeleteKey( hKey, szKeyName );
        }
        RegCloseKey( hKey );
    }
    return( fResult );
}

//////////////////////////////////////////////////////////////////
BOOL SourceExists( LPCTSTR pszMachine, LPTSTR pszEventLogName, LPTSTR pszSourceName )
{
    BOOL fResult = FALSE;
    TCHAR szBuffer[ MAX_PATH ];
    DWORD dwTemp;
    HKEY  hSourcesKey = NULL;
    HKEY  hRoot = HKEY_LOCAL_MACHINE;

    if( ( NULL == pszEventLogName ) || ( NULL == pszSourceName ) )
    {
        return( FALSE );
    }

    if( ( 0 == ( _tcscmp( TEXT( "" ), pszEventLogName ) ) )
        || ( 0 == ( _tcscmp( TEXT( "" ), pszSourceName ) ) ) )
    {
        return( FALSE );
    }

    if( NULL != pszMachine )
    {
        if( ERROR_SUCCESS != RegConnectRegistry( pszMachine, hRoot, &hRoot ) )
        {
            return( FALSE );
        }
    }

    _stprintf( szBuffer, EVENT_LOG_PATH TEXT( "\\%s" ), pszEventLogName );

	if( ERROR_SUCCESS == RegOpenKeyEx(
				hRoot,
				szBuffer,
				0,
				KEY_ALL_ACCESS,
				&hSourcesKey ) )
    {
        DWORD dwType = 0;
        DWORD dwRequiredBufferSize = 0;

        if( ERROR_SUCCESS == RegQueryValueEx( hSourcesKey, TEXT( "Sources" ), NULL, &dwType, (LPBYTE) NULL, &dwRequiredBufferSize ) )
        {
            LPTSTR pszBuffer;
            DWORD dwBufferSize = dwRequiredBufferSize + _tcslen( pszSourceName ) + 1;
        
            pszBuffer = new TCHAR [ dwBufferSize ];
            if( NULL != pszBuffer )
            {
                ZeroMemory( pszBuffer, dwBufferSize );
                
                dwRequiredBufferSize = dwBufferSize;
                if( ( ERROR_SUCCESS == RegQueryValueEx( hSourcesKey, TEXT( "Sources" ), NULL, &dwType, (LPBYTE) pszBuffer, &dwRequiredBufferSize ) )
                    && ( REG_MULTI_SZ == dwType ) )
                {
                    LPTSTR pszTemp = pszBuffer;
                    DWORD dwLengthRemain = dwRequiredBufferSize;

                    while( '\0' != *pszTemp )
                    {
                        DWORD dwLength = _tcslen( pszTemp );
                        LPTSTR pszNextBlock = &pszTemp[ dwLength + 1 ];

                        dwLengthRemain -= ( dwLength + 1 ) * sizeof( TCHAR );
                        if( 0 == _tcsicmp( pszSourceName, pszTemp ) )
                        {
                            fResult = TRUE;
                            break;
                        }
                        pszTemp = pszNextBlock;
                    }
                }
                delete [] pszBuffer;
                pszBuffer = NULL;
            }
        }
        RegCloseKey( hSourcesKey );
	}

    if( NULL != pszMachine )
    {
        RegCloseKey( hRoot );
    }

    return( fResult );
}

//////////////////////////////////////////////////////////////////
BOOL RegisterMessageTable( LPCTSTR pszMachine, LPCTSTR pszEventLogName, LPCTSTR pszSourceName, LPCTSTR pszPath )
{
    BOOL fResult = FALSE;
    TCHAR szBuffer[ 128 ];
    DWORD dwTemp;
    HKEY hKey;
    HKEY hRoot = HKEY_LOCAL_MACHINE;


    if( ( NULL == pszEventLogName ) 
        || ( NULL == pszSourceName ) 
        || ( NULL == pszPath ) )
    {
        return( FALSE );
    }

    if( ( 0 == ( _tcscmp( TEXT( "" ), pszEventLogName ) ) )
        || ( 0 == ( _tcscmp( TEXT( "" ), pszSourceName ) ) )
        || ( 0 == ( _tcscmp( TEXT( "" ), pszPath ) ) ) )
    {
        return( FALSE );
    }

    if( NULL != pszMachine )
    {
        if( ERROR_SUCCESS != RegConnectRegistry( pszMachine, hRoot, &hRoot ) )
        {
            return( FALSE );
        }
    }


    _stprintf( szBuffer, EVENT_LOG_PATH TEXT( "\\%s\\%s" ), pszEventLogName, pszSourceName );

	//	Add our name to the event log registry key
	if( ERROR_SUCCESS == RegCreateKeyEx( 
				hRoot,
				szBuffer,
				NULL,
				NULL,
				REG_OPTION_NON_VOLATILE,
				KEY_ALL_ACCESS,
				NULL,
				&hKey,
				&dwTemp ) )
    {
        DWORD dwResult;
        //
        //  Add (or update) the EventMessageFile key...
        //
		dwResult = RegSetValueEx( hKey, TEXT( "EventMessageFile" ), 0, REG_EXPAND_SZ, (const BYTE*) pszPath, _tcslen( pszPath ) + 1 );

        //
        //  Add (or update) the TypesSupported key...
        //
        dwTemp = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE; 
		dwResult = RegSetValueEx( hKey, TEXT( "TypesSupported" ), 0, REG_DWORD, (const BYTE*) &dwTemp, sizeof( dwTemp ) );

        //
        //  Now let's do what they don't tell you about in the SDK...
        //  ...We must register with the event log by adding our pszSourceName to the following key:
        //      hkey_local_machine\system\CurrentControlSet\Services\EventLog\pszEventLogName
        //  The actual value updated is "Sources" which is of type REG_MULTI_SZ. We must search it to
        //  make sure we are in there if not add us.
        //

        HKEY  hSourcesKey = NULL;
        _stprintf( szBuffer, EVENT_LOG_PATH TEXT( "\\%s" ), pszEventLogName );

		if( ERROR_SUCCESS == RegCreateKeyEx( 
					hRoot,
					szBuffer,
					NULL,
					NULL,
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS,
					NULL,
					&hSourcesKey,
					&dwTemp ) )
        {
            DWORD dwType = 0;
            DWORD dwRequiredBufferSize = 0;

            if( ERROR_SUCCESS == RegQueryValueEx( hSourcesKey, TEXT( "Sources" ), NULL, &dwType, (LPBYTE) NULL, &dwRequiredBufferSize ) )
            {
                LPTSTR pszBuffer;
                DWORD dwBufferSize = dwRequiredBufferSize + _tcslen( pszSourceName ) + 1;
            
                pszBuffer = new TCHAR [ dwBufferSize ];
                if( NULL != pszBuffer )
                {
                    ZeroMemory( pszBuffer, dwBufferSize );
                    
                    dwRequiredBufferSize = dwBufferSize;
                    if( ( ERROR_SUCCESS == RegQueryValueEx( hSourcesKey, TEXT( "Sources" ), NULL, &dwType, (LPBYTE) pszBuffer, &dwRequiredBufferSize ) )
                        && ( REG_MULTI_SZ == dwType ) )
                    {
                        LPTSTR pszTemp = pszBuffer;
                        BOOL fFound = FALSE;

                        while( '\0' != *pszTemp )
                        {
                            if( 0 == _tcsicmp( pszSourceName, pszTemp ) )
                            {
                                fResult = TRUE;
                                break;
                            }
                            pszTemp = &pszTemp[ _tcslen( pszTemp ) + 1 ];
                        }

                        if( FALSE == fResult )
                        {
                           _tcscpy( pszTemp, pszSourceName );
                           fResult = (BOOL) ( ERROR_SUCCESS == RegSetValueEx( hSourcesKey, TEXT( "Sources" ), 0, REG_MULTI_SZ, (const BYTE*) pszBuffer, dwBufferSize ) );
                        }
                    }
                    delete [] pszBuffer;
                    pszBuffer = NULL;
                }
            }
            RegCloseKey( hSourcesKey );
        }
		RegCloseKey( hKey );
	}

    if( NULL != pszMachine )
    {
        RegCloseKey( hRoot );
    }

    return( fResult );
}

//////////////////////////////////////////////////////////////////
BOOL UnRegisterMessageTable( LPCTSTR pszMachine, LPCTSTR pszEventLogName, LPCTSTR pszSourceName )
{
    BOOL fResult = FALSE;
    TCHAR szBuffer[ MAX_PATH ];
    DWORD dwTemp;
    HKEY  hSourcesKey = NULL;
    HKEY  hRoot = HKEY_LOCAL_MACHINE;

    if( ( NULL == pszEventLogName ) || ( NULL == pszSourceName ) )
    {
        return( FALSE );
    }

    if( ( 0 == ( _tcscmp( TEXT( "" ), pszEventLogName ) ) )
        || ( 0 == ( _tcscmp( TEXT( "" ), pszSourceName ) ) ) )
    {
        return( FALSE );
    }

    if( NULL != pszMachine )
    {
        if( ERROR_SUCCESS != RegConnectRegistry( pszMachine, hRoot, &hRoot ) )
        {
            return( FALSE );
        }
    }

    _stprintf( szBuffer, EVENT_LOG_PATH TEXT( "\\%s\\%s" ), pszEventLogName, pszSourceName );

    //  Delete the event log source registry key
    DeleteKey( hRoot, szBuffer );

    //
    //  Now let's do what they don't tell you about in the SDK...
    //  ...We must register with the event log by adding our pszSourceName to the following key:
    //      hkey_local_machine\system\CurrentControlSet\Services\EventLog\pszEventLogName
    //  The actual value updated is "Sources" which is of type REG_MULTI_SZ. We must search it to
    //  make sure we are in there if not add us.
    //

    _stprintf( szBuffer, EVENT_LOG_PATH TEXT( "\\%s" ), pszEventLogName );

	if( ERROR_SUCCESS == RegOpenKeyEx(
				hRoot,
				szBuffer,
				0,
				KEY_ALL_ACCESS,
				&hSourcesKey ) )
    {
        DWORD dwType = 0;
        DWORD dwRequiredBufferSize = 0;

        if( ERROR_SUCCESS == RegQueryValueEx( hSourcesKey, TEXT( "Sources" ), NULL, &dwType, (LPBYTE) NULL, &dwRequiredBufferSize ) )
        {
            LPTSTR pszBuffer;
            DWORD dwBufferSize = dwRequiredBufferSize + _tcslen( pszSourceName ) + 1;
        
            pszBuffer = new TCHAR [ dwBufferSize ];
            if( NULL != pszBuffer )
            {
                ZeroMemory( pszBuffer, dwBufferSize );
                
                dwRequiredBufferSize = dwBufferSize;
                if( ( ERROR_SUCCESS == RegQueryValueEx( hSourcesKey, TEXT( "Sources" ), NULL, &dwType, (LPBYTE) pszBuffer, &dwRequiredBufferSize ) )
                    && ( REG_MULTI_SZ == dwType ) )
                {
                    LPTSTR pszTemp = pszBuffer;
                    DWORD dwLengthRemain = dwRequiredBufferSize;

                    while( '\0' != *pszTemp )
                    {
                        DWORD dwLength = _tcslen( pszTemp );
                        LPTSTR pszNextBlock = &pszTemp[ dwLength + 1 ];

                        dwLengthRemain -= ( dwLength + 1 ) * sizeof( TCHAR );
                        if( 0 == _tcsicmp( pszSourceName, pszTemp ) )
                        {
                            MoveMemory( pszTemp, pszNextBlock, dwLengthRemain );
                            dwBufferSize -= ( dwLength + 1 );
                            fResult = TRUE;
                        }
                        pszTemp = pszNextBlock;
                    }

                    if( TRUE == fResult )
                    {
                       fResult = (BOOL) ( ERROR_SUCCESS == RegSetValueEx( hSourcesKey, TEXT( "Sources" ), 0, REG_MULTI_SZ, (const BYTE*) pszBuffer, dwBufferSize ) );
                    }
                }
                delete [] pszBuffer;
                pszBuffer = NULL;
            }
        }
        RegCloseKey( hSourcesKey );
	}

    if( NULL != pszMachine )
    {
        RegCloseKey( hRoot );
    }

    return( fResult );
}

/*----------------------- P E R L   F U N C T I O N S -------------------*/
//////////////////////////////////////////////////////////////////
XS( XS_WIN32__EventLog_Message_Constant )
{ 
	dXSARGS;
    EXTENSION_VARS;
	char *pszName = NULL;
    LPVOID pBuffer = NULL;
    DWORD dwValue;
    eConstantType eResult;

	if( 2 != items )
	{
		croak( "Usage: " EXTENSION "::Constant( $Name, $Arg )\n" );
    }
	
	pszName = (char*) SvPV( ST( 0 ), na );

	eResult = Constant( pszName, &pBuffer );
    switch( eResult )
    {
        case String:
            sv_setpv( ST( 1 ), (char*) pBuffer );
            break;

        case Numeric:
            sv_setiv( ST( 1 ), (IV) pBuffer );
            break;
    }

        //  Return the result type.
    PUSH_IV( eResult );

    EXTENSION_RETURN;
}

//////////////////////////////////////////////////////////////////
XS( XS_WIN32__EventLog_Message_Register )
{ 
	dXSARGS;
    EXTENSION_VARS;
    BOOL fResult = FALSE;
    int iParamIndex = 0;
    LPTSTR pszEventLogName = TEXT( "" );
    LPTSTR pszName = MESSAGE_FILTER_EVENTLOG_NAME;
    LPTSTR pszMachine = NULL;
    TCHAR szPath[ MAX_PATH ];

    if( ( 1 > items ) || ( 4 < items ) )
	{
		croak( "Usage: " EXTENSION "::Register( [$Machine, ] $EventLogName [, $Name [, $Path ] ])\n" );
    }
	
    if( 3 < items )
    {
        pszMachine = SvPV( ST( iParamIndex ), na );
        iParamIndex++;
    }

    ZeroMemory( szPath, sizeof( szPath ) );
    if( GetModuleFileName( ghModule, szPath, sizeof( szPath ) / sizeof( TCHAR ) ) )
    {
        pszName = MESSAGE_FILTER_EVENTLOG_NAME;
        switch( items )
        {
        case 3:
            _tcscpy( szPath, SvPV( ST( iParamIndex + 2 ), na ) );

        case 2:
            pszName = SvPV( ST( iParamIndex + 1 ), na );

        case 1:
            pszEventLogName = SvPV( ST( iParamIndex ), na );
        }

        fResult = RegisterMessageTable( pszMachine, pszEventLogName, pszName, szPath );
            
        //  Return the result.
        PUSH_IV( fResult );
    }
    else
    {
        PUSH_NOREF( &sv_undef );
    }

    EXTENSION_RETURN;
}

//////////////////////////////////////////////////////////////////
XS( XS_WIN32__EventLog_Message_UnRegister )
{ 
	dXSARGS;
    EXTENSION_VARS;
    BOOL fResult = FALSE;
    int iParamIndex = 0;
    LPTSTR pszEventLogName = TEXT( "" );
    LPTSTR pszName = MESSAGE_FILTER_EVENTLOG_NAME;
    LPTSTR pszMachine = NULL;

    if( ( 1 > items ) || ( 3 < items ) )
	{
		croak( "Usage: " EXTENSION "::UnRegister( [$Machine, ] $EventLogName [, $Name ] )\n" );
    }
	
    if( 2 < items )
    {
        pszMachine = SvPV( ST( iParamIndex ), na );
        iParamIndex++;
    }

    switch( items )
    {
    case 2:
        pszName = SvPV( ST( iParamIndex + 1 ), na );

    case 1:
        pszEventLogName = SvPV( ST( iParamIndex ), na );
    }

    fResult = UnRegisterMessageTable( pszMachine, pszEventLogName, pszName );
        
    //  Return the result.
    PUSH_IV( fResult );

    EXTENSION_RETURN;
}

//////////////////////////////////////////////////////////////////
XS( XS_WIN32__EventLog_Message_GetSource )
{ 
	dXSARGS;
    EXTENSION_VARS;
    int iParamIndex = 0;
    BOOL fResult = FALSE;
    LPTSTR pszEventLogName = TEXT( "" );
    LPTSTR pszSourceName = TEXT( "" );
    LPTSTR pszMachine = NULL;
    TCHAR szBuffer[ MAX_PATH ];
    HKEY  hKey = NULL;
    HV *pHv = NULL;
    
    if( ( 3 > items ) || ( 4 < items ) )
	{
		croak( "Usage: " EXTENSION "::GetSource( [$Machine, ] $EventLogName, $SourceName, \\%List )\n" );
    }

    if( 3 < items )
    {
        pszMachine = SvPV( ST( iParamIndex ), na );
        iParamIndex++;
    }

    pszEventLogName = SvPV( ST( iParamIndex ), na );
    pszSourceName   = SvPV( ST( iParamIndex + 1 ), na );
    if( NULL != ( pHv = EXTRACT_HV( ST( iParamIndex + 2 ) ) ) )
    {
        HKEY hRoot = HKEY_LOCAL_MACHINE;

        hv_clear( pHv );

        if( NULL != pszMachine )
        {
            if( ERROR_SUCCESS != RegConnectRegistry( pszMachine, hRoot, &hRoot ) )
            {
                EXTENSION_RETURN_BOOL( FALSE );
            }
        }

        if( SourceExists( pszMachine, pszEventLogName, pszSourceName ) )
        {
            _stprintf( szBuffer, EVENT_LOG_PATH TEXT( "\\%s\\%s" ), pszEventLogName, pszSourceName );

	        if( ERROR_SUCCESS == RegOpenKeyEx( 
				        hRoot,
				        szBuffer,
                        0,
                        KEY_ALL_ACCESS,
				        &hKey ) )
            {
                union
                {
                    DWORD   dwValue;
                    TCHAR   szValue[ MAX_PATH ];
                } uValue;
                DWORD dwSize = 0;
                DWORD dwType = 0;
            
                fResult = TRUE;

                //
                //  Query the EventMessageFile key...
                //
                dwSize = sizeof( uValue );
	            if( ERROR_SUCCESS == RegQueryValueEx( hKey, TEXT( "EventMessageFile" ), NULL, &dwType, (LPBYTE) &uValue, &dwSize ) )
                {
                    HASH_STORE_PV( pHv, TEXT( "path" ), uValue.szValue );
                }

                //
                //  Query the TypesSupported key...
                //
                dwSize = sizeof( uValue );
                if( ERROR_SUCCESS == RegQueryValueEx( hKey, TEXT( "TypesSupported" ), NULL, &dwType, (LPBYTE) &uValue, &dwSize ) )
                {
                    HASH_STORE_IV( pHv, TEXT( "types_supported" ), uValue.dwValue );
                }

                RegCloseKey( hKey  );
            }
        }
        if( NULL != pszMachine )
        {
            RegCloseKey( hRoot );
        }
    }
        
    //  Return the result.
    PUSH_IV( FALSE != fResult  );

    EXTENSION_RETURN;
}

//////////////////////////////////////////////////////////////////
XS( XS_WIN32__EventLog_Message_GetSourceList )
{ 
	dXSARGS;
    EXTENSION_VARS;
    DWORD dwTotal = 0;
    int iParamIndex = 0;
    LPTSTR pszEventLogName = TEXT( "" );
    LPTSTR pszMachine = NULL;
    TCHAR szBuffer[ MAX_PATH ];
    HKEY  hSourcesKey = NULL;

    AV *pAv = NULL;
    
    if( ( 2 > items ) || ( 3 < items ) )
	{
		croak( "Usage: " EXTENSION "::GetSourceList( [$Machine, ] $EventLogName, \\@List )\n" );
    }
	
    if( 2 < items )
    {
        pszMachine = SvPV( ST( iParamIndex ), na );
        iParamIndex++;
    }

    pszEventLogName = SvPV( ST( iParamIndex ), na );
    if( NULL != ( pAv = EXTRACT_AV( ST( iParamIndex + 1 ) ) ) )
    {
        HKEY hRoot = HKEY_LOCAL_MACHINE;
        
        av_clear( pAv );

        if( NULL != pszMachine )
        {
            RegConnectRegistry( pszMachine, hRoot, &hRoot );
        }
    
        _stprintf( szBuffer, EVENT_LOG_PATH TEXT( "\\%s" ), pszEventLogName );

	    if( ERROR_SUCCESS == RegOpenKeyEx( 
				    hRoot,
				    szBuffer,
                    0,
                    KEY_ALL_ACCESS,
				    &hSourcesKey ) )
        {
            DWORD dwType = 0;
            DWORD dwRequiredBufferSize = 0;

            if( ERROR_SUCCESS == RegQueryValueEx( hSourcesKey, TEXT( "Sources" ), NULL, &dwType, (LPBYTE) NULL, &dwRequiredBufferSize ) )
            {
                LPTSTR pszBuffer;
                DWORD dwBufferSize = dwRequiredBufferSize;

                pszBuffer = new TCHAR [ dwBufferSize ];
                if( NULL != pszBuffer )
                {
                    ZeroMemory( pszBuffer, dwBufferSize );
        
                    dwRequiredBufferSize = dwBufferSize;
                    if( ( ERROR_SUCCESS == RegQueryValueEx( hSourcesKey, TEXT( "Sources" ), NULL, &dwType, (LPBYTE) pszBuffer, &dwRequiredBufferSize ) )
                        && ( REG_MULTI_SZ == dwType ) )
                    {
                        LPTSTR pszTemp = pszBuffer;
                        
                        while( '\0' != *pszTemp )
                        {
                            DWORD dwLength = _tcslen( pszTemp );

                            ARRAY_PUSH_PV( pAv, pszTemp );
                            dwTotal++;

                            pszTemp = &pszTemp[ dwLength + 1 ];
                        }
                    }
                    delete [] pszBuffer;
                    pszBuffer = NULL;
                }
                RegCloseKey( hSourcesKey );
            }
        }
        if( NULL != pszMachine )
        {
            RegCloseKey( hRoot );
        }
    }
        
    //  Return the result.
    PUSH_IV( dwTotal );

    EXTENSION_RETURN;
}

//////////////////////////////////////////////////////////////////
XS( XS_WIN32__EventLog_Message_GetEventLogList )
{ 
	dXSARGS;
    EXTENSION_VARS;
    DWORD dwTotal = 0;
    int iParamIndex = 0;
    LPTSTR pszMachine = NULL;
    AV *pAv = NULL;
    
    if( ( 1 > items ) || ( 2 < items ) )
	{
		croak( "Usage: " EXTENSION "::GetEventLogList( [$Machine, ] \\@List )\n" );
    }
	
    if( 1 < items )
    {
        pszMachine = SvPV( ST( iParamIndex ), na );
        iParamIndex++;
    }

    if( NULL != ( pAv = EXTRACT_AV( ST( iParamIndex ) ) ) )
    {
        HKEY hKey = NULL;
        HKEY hRoot = HKEY_LOCAL_MACHINE;

        av_clear( pAv );
    
        if( NULL != pszMachine )
        {
            RegConnectRegistry( pszMachine, hRoot, &hRoot );
        }

	    if( ERROR_SUCCESS == RegOpenKeyEx( 
				    hRoot,
                    EVENT_LOG_PATH,
                    0,
                    KEY_ALL_ACCESS,
				    &hKey ) )
        {
            DWORD dwRequiredBufferSize = 0;
            TCHAR szBuffer[ 256 ];
            DWORD dwIndex = 0;
            DWORD dwBufferSize = sizeof( szBuffer ) / sizeof( TCHAR );
            FILETIME Time;
            while( ERROR_NO_MORE_ITEMS != RegEnumKeyEx( hKey,
                                                        dwIndex++,
                                                        szBuffer,
                                                        &dwBufferSize,
                                                        NULL,
                                                        NULL,
                                                        NULL,
                                                        &Time ) )
            {
                ARRAY_PUSH_PV( pAv, szBuffer );
                dwTotal++;
                dwBufferSize = sizeof( szBuffer ) / sizeof( TCHAR );
            }
            RegCloseKey( hKey );
        }
        if( NULL != pszMachine )
        {
            RegCloseKey( hRoot );
        }
    }
        
    //  Return the result.
    PUSH_IV( dwTotal );

    EXTENSION_RETURN;
}


//////////////////////////////////////////////////////////////////
XS( boot_Win32__EventLog__Message )
{
	dXSARGS;
    EXTENSION_VARS;
	LPTSTR file = __FILE__;
	int	retcode = 1;

    XS_VERSION_BOOTCHECK ;

	newXS( EXTENSION "::Constant",			XS_WIN32__EventLog_Message_Constant, file );
    newXS( EXTENSION "::RegisterSource",	XS_WIN32__EventLog_Message_Register, file );
    newXS( EXTENSION "::UnRegisterSource",	XS_WIN32__EventLog_Message_UnRegister, file );
    newXS( EXTENSION "::GetSourceList",	    XS_WIN32__EventLog_Message_GetSourceList, file );
    newXS( EXTENSION "::GetSource",	        XS_WIN32__EventLog_Message_GetSource, file );
    newXS( EXTENSION "::GetEventLogList",   XS_WIN32__EventLog_Message_GetEventLogList, file );


    XSRETURN_YES;

}			

//////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain( HINSTANCE hModule, DWORD dwMessage, LPVOID lpReserved )
{
	BOOL	fResult = TRUE;

    switch( dwMessage ) 
    {
		case DLL_PROCESS_ATTACH: 
            ghModule = (HMODULE) hModule;

	    case DLL_THREAD_ATTACH:
	    case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;

	}
	return fResult;
}

