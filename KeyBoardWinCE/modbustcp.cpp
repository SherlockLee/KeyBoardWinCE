/*
 * FreeModbus Libary: Win32 Demo Application
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: demo.cpp,v 1.2 2006/06/26 19:24:07 wolti Exp $
 */
#pragma once
#include "stdafx.h"

#include "modbustcp.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "mbutils.h"


/* ----------------------- Defines ------------------------------------------*/
#define PROG            _T("freemodbus")

//////////////////////////////////////////////////////////////////////////
//		Modbus Slave
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

/* ----------------------- Static variables ---------------------------------*/
extern USHORT   usRegInputStart;
extern USHORT   usRegInputBuf[REG_INPUT_NREGS];

extern USHORT   usRegHoldingStart;
extern USHORT   usRegHoldingBuf[REG_HOLDING_NREGS];

extern USHORT   usInputStatusStart;
extern UCHAR	ucInputStatusBuf[INPUT_STATUS_NREGS];

extern USHORT   usInputCoilStart;
extern UCHAR	ucInputCoilBuf[INPUT_COIL_NREGS];


static HANDLE   hPollThread;
static CRITICAL_SECTION hPollLock;
static enum ThreadState
{
    STOPPED,
    RUNNING,
    SHUTDOWN
} ePollThreadState;

/* ----------------------- Static functions ---------------------------------*/
static BOOL     bCreatePollingThread( void );
static enum ThreadState eGetPollingThreadState( void );
static void     eSetPollingThreadState( enum ThreadState eNewState );
static DWORD WINAPI dwPollingThread( LPVOID lpParameter );
/* ----------------------- Start implementation -----------------------------*/
int StartModbusTcp()
{
    int             iExitCode;
    if( eMBTCPInit( MB_TCP_PORT_USE_DEFAULT ) != MB_ENOERR )
    {
        _ftprintf( stderr, _T( "%s: can't initialize modbus stack!\r\n" ), PROG );
        iExitCode = EXIT_FAILURE;
    }
    else
    {
        /* Create synchronization primitives and set the current state
         * of the thread to STOPPED.
         */
        InitializeCriticalSection( &hPollLock );
        eSetPollingThreadState( STOPPED );

        /* Start the modbus protocol. */
		if( bCreatePollingThread(  ) != TRUE )
		{
			_tprintf( _T( "Can't start protocol stack! Already running?\r\n" ) );
		}
        iExitCode = EXIT_SUCCESS;
    }
    return iExitCode;
}

enum ModbusState GetModbusState()
{
	enum ModbusState ePollModbusState;

	switch(eGetPollingThreadState())
	{
	case STOPPED:
		ePollModbusState = MB_STOPPED;
		break;
	case RUNNING:
		ePollModbusState = MB_RUNNING;
		break;
	case SHUTDOWN:
		ePollModbusState = MB_SHUTDOWN;
		break;
	}
	return ePollModbusState;
}

void StopModbusTcp()
{
	eSetPollingThreadState( SHUTDOWN );
	/* Release hardware resources. */
	( void )eMBClose(  );
}

BOOL
bCreatePollingThread( void )
{
    BOOL            bResult;

    if( eGetPollingThreadState(  ) == STOPPED )
    {
        if( ( hPollThread = CreateThread( NULL, 0, dwPollingThread, NULL, 0, NULL ) ) == NULL )
        {
            /* Can't create the polling thread. */
            bResult = FALSE;
        }
        else
        {
            bResult = TRUE;
        }
    }
    else
    {
        bResult = FALSE;
    }

    return bResult;
}

DWORD           WINAPI
dwPollingThread( LPVOID lpParameter )
{
    eSetPollingThreadState( RUNNING );

    if( eMBEnable(  ) == MB_ENOERR )
    {
        do
        {
            if( eMBPoll(  ) != MB_ENOERR )
                break;
        }
        while( eGetPollingThreadState(  ) != SHUTDOWN );
    }

    ( void )eMBDisable(  );

    eSetPollingThreadState( STOPPED );

    return 0;
}

enum ThreadState
eGetPollingThreadState(  )
{
    enum ThreadState eCurState;

    EnterCriticalSection( &hPollLock );
    eCurState = ePollThreadState;
    LeaveCriticalSection( &hPollLock );

    return eCurState;
}

void
eSetPollingThreadState( enum ThreadState eNewState )
{
    EnterCriticalSection( &hPollLock );
    ePollThreadState = eNewState;
    LeaveCriticalSection( &hPollLock );
}

eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLDING_START ) &&
        ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegHoldingStart );
        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
                *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( UCHAR ) ( usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

            /* Update current register values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
                usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;
	SHORT			iNCoils;
	iNCoils = (SHORT) usNCoils;

	if( ( usAddress >= INPUT_COIL_START ) &&
		( usAddress + iNCoils <= INPUT_COIL_START + INPUT_COIL_NREGS ) )
	{
		iRegIndex = ( int )( usAddress - usInputCoilStart );
		switch ( eMode )
		{
			/* Pass current register values to the protocol stack. */
		case MB_REG_READ:
			while( iNCoils > 0 )
			{
				*pucRegBuffer++ = xMBUtilGetBits( ucInputCoilBuf, iRegIndex, ( iNCoils > 8 ? 8 : iNCoils ));
				iRegIndex += 8;
				iNCoils -= 8;
			}
			break;

			/* Update current register values with new values from the
			 * protocol stack. */
		case MB_REG_WRITE:
			while( iNCoils > 0 )
			{
				xMBUtilSetBits( ucInputCoilBuf, iRegIndex, ( iNCoils > 8 ? 8 : iNCoils ), *pucRegBuffer++);
				iRegIndex += 8;
				iNCoils -= 8;
			}
		}
	}
	else
	{
		eStatus = MB_ENOREG;
    }

	return eStatus;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	USHORT          iRegIndex;
	int				iNDiscrete;

	iNDiscrete = (int)usNDiscrete;

	if( ( usAddress >= INPUT_STATUS_START )
		&& ( usAddress + iNDiscrete <= INPUT_STATUS_START + INPUT_STATUS_NREGS ) )
	{
		iRegIndex = ( USHORT )( usAddress - usInputStatusStart );
		while( iNDiscrete > 0 )
		{
			*pucRegBuffer++ = xMBUtilGetBits( ucInputStatusBuf, iRegIndex, ( iNDiscrete > 8 ? 8 : iNDiscrete ));
			iRegIndex += 8;
			iNDiscrete -= 8;
		}
	}
	else
	{
		eStatus = MB_ENOREG;
	}

	return eStatus;
}