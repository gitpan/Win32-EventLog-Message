//  Version.h
//  The Win32::EventLog::Message version file.
//  (c) Dave Roth
//  Courtesy of Roth Consulting
//  http://www.roth.net/
//
//  2000.06.12

#ifndef _MESSAGE_VERSION_H
#define _MESSAGE__VERSION_H

#pragma message( "Reading the Message Resource version header.\n" )

#ifndef SIZEOF
#define SIZEOF(x)       ( sizeof( x ) / sizeof( x[0] ) )
#endif // SIZEOF(x)

#define MESSAGE_FILE_VERSION_ANSI                   "1.0"

#define MESSAGE_FILE_VERSION_MAJOR_HI               0x00
#define MESSAGE_FILE_VERSION_MAJOR_LO               0x01    
#define MESSAGE_FILE_VERSION_MINOR_HI               0x00
#define MESSAGE_FILE_VERSION_MINOR_LO               0x00

#define MESSAGE_PRODUCT_VERSION_ANSI                "1.0"

#define MESSAGE_PRODUCT_VESION_MAJOR_HI             0x00
#define MESSAGE_PRODUCT_VERSION_MAJOR_LO            0x01
#define MESSAGE_PRODUCT_VERSION_MINOR_HI            0x00
#define MESSAGE_PRODUCT_VERSION_MINOR_LO            0x00

//
//  Define the CHAR based strings...
#define MESSAGE_COMPANY_NAME_ANSI           "Roth Consulting" 
#define MESSAGE_COMPANY_URL_ANSI            "http://www.roth.net/"
#define MESSAGE_COMPANY_CONTACT_ANSI        "consult@roth.net"
#define MESSAGE_PRODUCT_NAME_ANSI           "Win32 Perl"
#define MESSAGE_PRODUCT_NAME_FULL_ANSI      "Roth Consulting's Win32 Perl EventLog Message File"
#define MESSAGE_PRODUCT_NICKNAME_ANSI       "Win32 Perl EventLog Messages"
#define MESSAGE_PRODUCT_URL_ANSI            "http://www.roth.net/"
#define MESSAGE_PRODUCT_COPYRIGHT_YEAR_ANSI "2000"
#define MESSAGE_PRODUCT_COMMENT_ANSI        "Message database for use with Win32 Perl's Win32::EventLog extension.\r\n\r\n" MESSAGE_COMPANY_NAME_ANSI "\r\n" MESSAGE_COMPANY_URL_ANSI

#define MESSAGE_FILTER_NAME_ANSI            MESSAGE_PRODUCT_NAME_FULL_ANSI
#define MESSAGE_FILTER_NICKNAME_ANSI        MESSAGE_PRODUCT_NICKNAME_ANSI
#define MESSAGE_FILTER_VERSION_ANSI         "v" MESSAGE_PRODUCT_VERSION_ANSI
#define MESSAGE_FILTER_EVENTLOG_NAME_ANSI   MESSAGE_PRODUCT_NAME
#define MESSAGE_FILTER_FILE_NAME_ANSI       "PerlMsg.dll"

//
//  Define the TCHAR versions...
//
#define MESSAGE_COMPANY_NAME                TEXT( MESSAGE_COMPANY_NAME_ANSI )
#define MESSAGE_COMPANY_URL                 TEXT( MESSAGE_COMPANY_URL_ANSI )
#define MESSAGE_COMPANY_CONTACT             TEXT( MESSAGE_COMPANY_CONTACT_ANSI )
#define MESSAGE_PRODUCT_NAME                TEXT( MESSAGE_PRODUCT_NAME_ANSI )
#define MESSAGE_PRODUCT_NAME_FULL           TEXT( MESSAGE_PRODUCT_NAME_FULL_ANSI )
#define MESSAGE_PRODUCT_NICKNAME            TEXT( MESSAGE_PRODUCT_NICKNAME_ANSI )
#define MESSAGE_PRODUCT_VERSION             TEXT( MESSAGE_PRODUCT_VERSION_ANSI )
#define MESSAGE_PRODUCT_URL                 TEXT( MESSAGE_PRODUCT_URL_ANSI )
#define MESSAGE_PRODUCT_COPYRIGHT_YEAR      TEXT( MESSAGE_PRODUCT_COPYRIGHT_YEAR_ANSI );
#define MESSAGE_PRODUCT_COMMENT             TEXT( MESSAGE_COMPANY_COMMENT_ANSI )

#define MESSAGE_FILTER_NAME                 TEXT( MESSAGE_FILTER_NAME_ANSI )
#define MESSAGE_FILTER_NICKNAME             TEXT( MESSAGE_FILTER_NICKNAME_ANSI )
#define MESSAGE_FILTER_VERSION              TEXT( MESSAGE_FILTER_VERSION_ANSI )
#define MESSAGE_FILTER_EVENTLOG_NAME        TEXT( MESSAGE_FILTER_EVENTLOG_NAME_ANSI )
#define	MESSAGE_FILTER_FILE_NAME            TEXT( MESSAGE_FILTER_FILE_NAME_ANSI )			







#endif // _MESSAGE_VERSION_H

