//  Constant.cpp
//  Constant support for Win32 Perl extensions
//  (c) Dave Roth
//  Courtesy of Roth Consulting
//  http://www.roth.net/
//
//  2000.06.23

#define WIN32_LEAN_AND_MEAN

#ifdef __BORLANDC__
    typedef wchar_t wctype_t; /* in tchar.h, but unavailable unless _UNICODE */
#endif
 
#include <windows.h>
#include <tchar.h>
#include <wtypes.h>
#include <stdio.h>		//	Gurusamy's right, Borland is brain damaged!
#include <math.h>		//	Gurusamy's right, MS is brain damaged!

#include "constant.h" 

static DWORD gdwConstTotal = 0;
static ConstantStruct gsConst[] =
{
    { TEXT( "EVENT_ID" ),		            (LPVOID) (EVENT_ID),    			Numeric },

    { TEXT( "EVENT_SUCCESS_1" ),		    (LPVOID) (EVENT_SUCCESS_1),			Numeric },
    { TEXT( "EVENT_SUCCESS_2" ),		    (LPVOID) (EVENT_SUCCESS_2),			Numeric },
    { TEXT( "EVENT_SUCCESS_3" ),		    (LPVOID) (EVENT_SUCCESS_3),			Numeric },
    
    { TEXT( "EVENT_INFO_1" ),		        (LPVOID) (EVENT_INFO_1),    		Numeric },
    { TEXT( "EVENT_INFO_2" ),		        (LPVOID) (EVENT_INFO_2),	    	Numeric },
    { TEXT( "EVENT_INFO_3" ),	        	(LPVOID) (EVENT_INFO_3),		    Numeric },

    { TEXT( "EVENT_WARNING_1" ),		    (LPVOID) (EVENT_WARNING_1), 		Numeric },
    { TEXT( "EVENT_WARNING_1" ),		    (LPVOID) (EVENT_WARNING_2),	    	Numeric },
    { TEXT( "EVENT_WARNING_1" ),	        (LPVOID) (EVENT_WARNING_3),		    Numeric },

    { TEXT( "EVENT_ERROR_1" ),	    	    (LPVOID) (EVENT_ERROR_1),			Numeric },
    { TEXT( "EVENT_ERROR_2" ),  		    (LPVOID) (EVENT_ERROR_2),			Numeric },
    { TEXT( "EVENT_ERROR_3" ),		        (LPVOID) (EVENT_ERROR_3),   		Numeric },

    { TEXT( "EVENT_SOURCE" ),               (LPVOID) (EVENT_SOURCE),            String  },
    
    // Terminating structure. Leave this here!
    { NULL,                             (LPVOID) NULL,                          Numeric } 
};

eConstantType Constant( LPTSTR pszConstant, LPVOID *ppBuffer )
{
    eConstantType eResult = NotDefined;
    DWORD dwIndex = 0;

    while( NULL != gsConst[ dwIndex ].m_Name )
    {
        if( NULL == gsConst[ dwIndex ].m_Name )
        {
            break;
        }

        if( *pszConstant == *gsConst[ dwIndex ].m_Name )
        {
            int iResult = _tcsicmp( gsConst[ dwIndex ].m_Name, pszConstant );
            if( 0 == iResult )
            {
                *ppBuffer = gsConst[ dwIndex ].m_pBuffer;
                eResult = gsConst[ dwIndex ].m_eType;
                break;
            }
            /*
                //  This code segment is commented out so that we don't run into 
                //  the problem of a constant being out of alpha order hence not
                //  resolving.
            else if( 0 < iResult )
            {
                    //  We have passed the spot where this constant
                    //  *should* have been if it were in alpha order
                break;
            }
            */
        }
        dwIndex++;
    }
    return( eResult );
}

LPTSTR GetConstantName( DWORD dwIndex )
{
    LPTSTR pszBuffer = NULL;
    if( gdwConstTotal > dwIndex )
    {
        pszBuffer = gsConst[ dwIndex ].m_Name;
    }
    return( pszBuffer );
}

DWORD GetTotalConstants()
{
    return( gdwConstTotal );
}

void CountConstants()
{
   gdwConstTotal = 0;
   while( NULL != gsConst[ gdwConstTotal++ ].m_Name ){};  
}

