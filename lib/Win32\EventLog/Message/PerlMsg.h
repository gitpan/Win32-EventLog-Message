//  PerlMsg.h
//  The Win32::EventLog::Message header file.
//  (c) Dave Roth
//  Courtesy of Roth Consulting
//  http://www.roth.net/
//
//  2000.06.12

//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: EVENT_ID
//
// MessageText:
//
//  %1!s!
//
#define EVENT_ID                         0x00000000L

//
// MessageId: EVENT_SUCCESS_1
//
// MessageText:
//
//  %1!s!
//
#define EVENT_SUCCESS_1                  0x00000001L

//
// MessageId: EVENT_SUCCESS_2
//
// MessageText:
//
//  %1!s!
//  %2!s!
//
#define EVENT_SUCCESS_2                  0x00000002L

//
// MessageId: EVENT_SUCCESS_3
//
// MessageText:
//
//  %1!s!
//  %2!s!
//  %3!s!
//
#define EVENT_SUCCESS_3                  0x00000003L

//
// MessageId: EVENT_INFO_1
//
// MessageText:
//
//  %1!s!
//
#define EVENT_INFO_1                     0x40000001L

//
// MessageId: EVENT_INFO_2
//
// MessageText:
//
//  %1!s!
//  %2!s!
//
#define EVENT_INFO_2                     0x40000002L

//
// MessageId: EVENT_INFO_3
//
// MessageText:
//
//  %1!s!
//  %2!s!
//  %3!s!
//
#define EVENT_INFO_3                     0x40000003L

//
// MessageId: EVENT_WARNING_1
//
// MessageText:
//
//  %1!s!
//
#define EVENT_WARNING_1                  0x80000001L

//
// MessageId: EVENT_WARNING_2
//
// MessageText:
//
//  %1!s!
//  %2!s!
//
#define EVENT_WARNING_2                  0x80000002L

//
// MessageId: EVENT_WARNING_3
//
// MessageText:
//
//  %1!s!
//  %2!s!
//  %3!s!
//
#define EVENT_WARNING_3                  0x80000003L

//
// MessageId: EVENT_ERROR_1
//
// MessageText:
//
//  %1!s!
//
#define EVENT_ERROR_1                    0xC0000001L

//
// MessageId: EVENT_ERROR_2
//
// MessageText:
//
//  %1!s!
//  %2!s!
//
#define EVENT_ERROR_2                    0xC0000002L

//
// MessageId: EVENT_ERROR_3
//
// MessageText:
//
//  %1!s!
//  %2!s!
//  %3!s!
//
#define EVENT_ERROR_3                    0xC0000003L

