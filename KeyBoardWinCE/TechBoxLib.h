/*    techBoxLib1.2   */


#ifndef  TECHBOXLIB
    #define  TECHBOXLIB


#if defined DLL_EXPORT  
#define DECLDIR __declspec(dllexport)  
#else  
#define DECLDIR __declspec(dllimport)  
#endif 

typedef enum TECH_BOX_ERR_CODE
{
    TECHBOX_NOERR,              /*!< no error. */
    TECHBOX_INIT_ERROR_1,       /*!< TECHBOX init error 1: HANDLE error */
    TECHBOX_INIT_ERROR_2,       /*!< TECHBOX init error  2: SetState error*/
    TECHBOX_INIT_ERROR_3,       /*!<  TECHBOX init error  3: SetMask error*/
    TECHBOX_INIT_ERROR_4,       /*!<  TECHBOX init error 4:  SetTimeouts error*/
    TECHBOX_INIT_ERROR_5,       /*!<  TECHBOX init error 5: CreateThread  error*/
    TECHBOX_INIT_ERROR_6,       /*!<  TECHBOX init error 6:  Done Init*/
    TECHBOX_INIT_ERROR_7,       /*!<  TECHBOX init error 7:  */
	TECHBOX_INIT_ERROR_8
} ;


#ifdef __cplusplus
  extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#define WM_TECHBOX_KEY_CHANGE (WM_USER+100)
#define WM_TECHBOX_MPG (WM_USER+101)

  #define TEACHBOX_BUZZER	(0x80)

#define TEACHBOX_LED1		(0x1	)
#define TEACHBOX_LED2		(0x1<<1	)
#define TEACHBOX_LED3		(0x1<<2	)
#define TEACHBOX_LED4		(0x1<<3	)
#define TEACHBOX_LED5		(0x1<<4	)
#define TEACHBOX_LED6		(0x1<<5	)
#define TEACHBOX_LED7		(0x1<<6	)
#define TEACHBOX_LED8		(0x1<<7	)
#define TEACHBOX_LED9		(0x1<<8	)
#define TEACHBOX_LED10		(0x1<<9	)
#define TEACHBOX_LED11		(0x1<<10	)
#define TEACHBOX_LED12		(0x1<<11	)
#define TEACHBOX_LED13		(0x1<<12	)
#define TEACHBOX_LED14		(0x1<<13	)
#define TEACHBOX_LED15		(0x1<<14	)
#define TEACHBOX_LED16		(0x1<<15	)
#define TEACHBOX_LED17		(0x1<<16	)
#define TEACHBOX_LED18		(0x1<<17	)
#define TEACHBOX_LED19		(0x1<<18	)
#define TEACHBOX_LED20		(0x1<<19	)
#define TEACHBOX_LED21		(0x1<<20	)
#define TEACHBOX_LED22		(0x1<<21	)
#define TEACHBOX_LED23		(0x1<<22	)
#define TEACHBOX_LED24		(0x1<<23	)
#define TEACHBOX_LED25		(0x1<<24	)
#define TEACHBOX_LED26		(0x1<<25	)
#define TEACHBOX_LED27		(0x1<<26	)
#define TEACHBOX_LED28		(0x1<<27	)
#define TEACHBOX_LED29		(0x1<<28	)
#define TEACHBOX_LED30		(0x1<<29	)
#define TEACHBOX_LED31		(0x1<<30	)

#define TEACHBOX_KEY1		(0x1	)
#define TEACHBOX_KEY2		(0x1<<1	)
#define TEACHBOX_KEY3		(0x1<<2	)
#define TEACHBOX_KEY4		(0x1<<3	)
#define TEACHBOX_KEY5		(0x1<<4	)
#define TEACHBOX_KEY6		(0x1<<5	)
#define TEACHBOX_KEY7		(0x1<<6	)
#define TEACHBOX_KEY8		(0x1<<7	)
#define TEACHBOX_KEY9		(0x1<<8	)
#define TEACHBOX_KEY10		(0x1<<9	)
#define TEACHBOX_KEY11		(0x1<<10	)
#define TEACHBOX_KEY12		(0x1<<11	)
#define TEACHBOX_KEY13		(0x1<<12	)
#define TEACHBOX_KEY14		(0x1<<13	)
#define TEACHBOX_KEY15		(0x1<<14	)
#define TEACHBOX_KEY16		(0x1<<15	)
#define TEACHBOX_KEY17		(0x1<<16	)
#define TEACHBOX_KEY18		(0x1<<17	)
#define TEACHBOX_KEY19		(0x1<<18	)
#define TEACHBOX_KEY20		(0x1<<19	)
#define TEACHBOX_KEY21		(0x1<<20	)
#define TEACHBOX_KEY22		(0x1<<21	)
#define TEACHBOX_KEY23		(0x1<<22	)
#define TEACHBOX_KEY24		(0x1<<23	)
#define TEACHBOX_KEY25		(0x1<<24	)
#define TEACHBOX_KEY26		(0x1<<25	)
#define TEACHBOX_KEY27		(0x1<<26	)
#define TEACHBOX_KEY28		(0x1<<27	)
#define TEACHBOX_KEY29		(0x1<<28	)
#define TEACHBOX_KEY30		(0x1<<29	)
#define TEACHBOX_KEY31		(0x1<<30	)



  DECLDIR TECH_BOX_ERR_CODE  TechBoxLibInit(HWND wnd,DWORD keyMsgValue,DWORD mpgMsgValue);
  DECLDIR void TechBoxLibClose(void);
  DECLDIR unsigned int   GetIdData1(void) ;
  DECLDIR unsigned int   GetIdData2(void);
  DECLDIR unsigned int   GetIdData3(void);
  DECLDIR void    SetOutputData1(unsigned int value) ;
  DECLDIR void    SetOutputData2(unsigned int value) ;

#endif

