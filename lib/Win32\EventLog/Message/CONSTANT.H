//  Constant.h
//  Constant support for Win32 Perl extensions
//  (c) Dave Roth
//  Courtesy of Roth Consulting
//  http://www.roth.net/
//
//  Available under the GNU license.
//  2000.03.19

#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include "PerlMsg.h"
#include "version.h"

//  #include <crtdbg.h>

enum eConstantType { NotDefined = 0, NotPresent, String, Numeric };

eConstantType Constant( LPTSTR pszConstant, LPVOID *ppBuffer );
LPTSTR GetConstantName( DWORD dwIndex );
DWORD GetTotalConstants();
void CountConstants();

typedef struct tagConstStruct
{
    LPTSTR m_Name;
    LPVOID m_pBuffer;
    eConstantType m_eType;
} ConstantStruct;

#define EVENT_SOURCE    MESSAGE_FILTER_EVENTLOG_NAME

#endif _CONSTANT_H_