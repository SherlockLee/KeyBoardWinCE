#ifndef _PORT_H
#define _PORT_H

#include <windows.h>
#include <tchar.h>
#include <assert.h>

#if 0
#define	INLINE
#define PR_BEGIN_EXTERN_C			extern "C" {
#define	PR_END_EXTERN_C				}
#endif

#define PR_BEGIN_EXTERN_C			
#define	PR_END_EXTERN_C				


#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

/* ----------------------- Defines ------------------------------------------*/

#define ENTER_CRITICAL_SECTION( )
#define EXIT_CRITICAL_SECTION( )
#define MB_PORT_HAS_CLOSE	1
#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif

/* ----------------------- Type definitions ---------------------------------*/

typedef enum
{
    MB_LOG_DEBUG,
    MB_LOG_INFO,
    MB_LOG_WARN,
    MB_LOG_ERROR
} eMBPortLogLevel;

/* ----------------------- Function prototypes ------------------------------*/

void            vMBPortLog( eMBPortLogLevel eLevel, const TCHAR * szModule, 
                            const TCHAR * szFmt, ... );

TCHAR          *WsaError2String( DWORD dwError );


#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
