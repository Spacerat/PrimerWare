/********************* (C) COPYRIGHT 2007 RAISONANCE S.A.S. *******************/
/**
*
* @file     circle_api.h
* @brief    General header for the STM32-circle projects.
* @author   FL
* @date     07/2007
* @version  1.2
* @date     10/2007
* @version  1.5 types of OutX_F64 and OutX_F256 changed to u32 (same for Y and Z)
* @date     10/2007
* @version  1.6 Add the IRQ handler replacement
* @version  1.7 With vectorized scheduler handler
* @version  3.0 Add Primer2 management and ST lib v2.0.3
* @date     10/2008 
* @version  3.8 CMSIS compliance and ST lib v3.1.0
* @date     08/2009 
*
* It contains the list of the utilities functions organized by sections
* (MEMS, LCD, POINTER, ...)
*
**/
/*******************************************************************************
*
* Use this header with version 1.5 or later of the OS.
*
* For a complete documentation on the CircleOS, please go to:
*  http://www.stm32circle.com
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CIRCLE_API_H
#define __CIRCLE_API_H

#include "stdint.h"

/*-------------------------------- General -------------------------------------*/

#ifndef __STM32F10x_H

typedef int32_t s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t  sc8;   /*!< Read Only */

typedef volatile int32_t  vs32;
typedef volatile int16_t  vs16;
typedef volatile int8_t   vs8;

typedef volatile const int32_t vsc32;  /*!< Read Only */
typedef volatile const int16_t vsc16;  /*!< Read Only */
typedef volatile const int8_t  vsc8;   /*!< Read Only */

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t  uc8;   /*!< Read Only */

typedef volatile uint32_t vu32;
typedef volatile uint16_t vu16;
typedef volatile uint8_t  vu8;

typedef volatile const uint32_t vuc32;  /*!< Read Only */
typedef volatile const uint16_t vuc16;  /*!< Read Only */
typedef volatile const uint8_t  vuc8;   /*!< Read Only */

#endif

/**
* @enum  eSpeed
* @brief Clock speeds.
*
* Available clock speeds.
**/
extern enum eSpeed
   {
   SPEED_VERY_LOW    = 1,
   SPEED_LOW         = 2,
   SPEED_MEDIUM      = 3,
   SPEED_HIGH        = 4,
   SPEED_VERY_HIGH   = 5
   } CurrentSpeed;

enum eSchHandler
   {
   LED_SCHHDL_ID      = 0,
   BUTTON_SCHHDL_ID   = 1,
   BUZZER_SCHHDL_ID   = 2,
   MENU_SCHHDL_ID     = 3,
   POINTER_SCHHDL_ID  = 4,
   LCD_SCHHDL_ID      = 5,
   DRAW_SCHHDL_ID     = 6,
   RTC_SCHHDL_ID      = 7,
   
#ifdef PRIMER1
   UNUSED0_SCHHDL_ID  = 8,
   UNUSED1_SCHHDL_ID  = 9,
   UNUSED2_SCHHDL_ID  = 10,
   UNUSED3_SCHHDL_ID  = 11,  
#else
   AUDIO_SCHHDL_ID    = 8,
   TOUCHSCR_SCHHDL_ID = 9,
   TOOLBAR_SCHHDL_ID  = 10,
   POWER_SCHHDL_ID    = 11,
#endif
   
   UNUSED4_SCHHDL_ID = 12,
   UNUSED5_SCHHDL_ID = 13,
   UNUSED6_SCHHDL_ID = 14,
   UNUSED7_SCHHDL_ID = 15   
   }; 


/// @cond Internal

/* Typedefs ------------------------------------------------------------------*/
typedef u32 (*tCircleFunc0 ) (void);
typedef u32 (*tCircleFunc1 ) (u32 param1);
typedef u32 (*tCircleFunc2 ) (u32 param1, u32 param2);
typedef u32 (*tCircleFunc3 ) (u32 param1, u32 param2, u32 param3);
typedef u32 (*tCircleFunc4 ) (u32 param1, u32 param2, u32 param3, u32 param4);
typedef u32 (*tCircleFunc5 ) (u32 param1, u32 param2, u32 param3, u32 param4, u32 param5);
typedef u32 (*tCircleFunc6 ) (u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6);
typedef u32 (*tCircleFunc7 ) (u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7);

extern tCircleFunc0 (*ptrCircle_API) [];

/* Defines -------------------------------------------------------------------*/
#define Circle_API   (*ptrCircle_API)

#define POINTER_ID   0x00
#define DRAW_ID      0x20
#define LCD_ID       0x40
#define LED_ID       0x60
#define TOUCHSCR_ID  0x61
#define TOOLBAR_ID   0x6A
#define MEMS_ID      0x70
#define BUTTON_ID    0x80
#define JOYSTICK_ID  0x84
#define BUZZER_ID    0x90
#define AUDIO_ID     0x93
#define MENU_ID      0xA0
#define UTIL_ID      0xB0
#define RTC_ID       0xC0
#define AUDIO2_ID    0xC3
#define UTIL2_ID     0xC8
#define SHUTDOWN_ID  0xD0
#define FS_ID        0xD8

/*/ UTIL functions definition.*/
#define UTIL_SET_PLL_ID                 (UTIL_ID + 0)    /*/ Set clock frequency.*/
#define UTIL_GET_PLL_ID                 (UTIL_ID + 1)    /* Get clock frequency.*/
#define UTIL_UINT2STR_ID                (UTIL_ID + 2)    /* Convert an unsigned integer into a string.*/
#define UTIL_INT2STR_ID                 (UTIL_ID + 3)    /* Convert a signed integer into a string.*/
#define UTIL_GET_VERSION_ID             (UTIL_ID + 4)    /* Get CircleOS version.*/
#define UTIL_READ_BACKUPREGISTER_ID     (UTIL_ID + 5)    /* Reads data from the specified Data Backup Register.*/
#define UTIL_WRITE_BACKUPREGISTER_ID    (UTIL_ID + 6)    /* Writes data to the specified Data Backup Register.*/
#define UTIL_GET_BAT_ID                 (UTIL_ID + 7)    /* Return the batterie tension in mV.*/
#define UTIL_GET_USB_ID                 (UTIL_ID + 8)    /* Return the USB connexion state.*/
#define UTIL_SET_IRQ_HANDLER_ID         (UTIL_ID + 9)    /* Replace an irq handler*/
#define UTIL_GET_IRQ_HANDLER_ID         (UTIL_ID + 10)   /* Get the current irq handler*/
#define UTIL_SET_SCH_HANDLER_ID         (UTIL_ID + 11)   /* Replace an irq handler*/
#define UTIL_GET_SCH_HANDLER_ID         (UTIL_ID + 12)   /* Get the current irq handler*/
#define UTIL_GET_TEMP_ID                (UTIL_ID + 13)   /* Return the temperature (1/100 C)*/
#define UTIL_SET_TEMPMODE_ID            (UTIL_ID + 14)   /* Set the temperature mode (0: mCelcius, 1: mFahrenheit*/
#define UTIL_GET_PRIMERTYPE_ID          (UTIL_ID + 15)   /* Get the type of PRIMER (1: Primer1, 2: Primer2)*/
#define UTIL_GET_APP_ADDRESS_ID         (UTIL2_ID + 0)   /* Get an application address.*/

typedef void (*tHandler) (void);

/* Prototypes.*/
#define UTIL_SetPll(a)                          ((tCircleFunc1)(Circle_API [UTIL_SET_PLL_ID])) ((u32)(a))                                    /* void UTIL_SetPll( enum eSpeed speed );*/
#define UTIL_GetPll()                  (u32)    (((tCircleFunc0)(Circle_API [UTIL_GET_PLL_ID])) ())                                          /* enum eSpeed UTIL_GetPll( void );*/
#define UTIL_uint2str(a,b,c,d)                  ((tCircleFunc4)(Circle_API [UTIL_UINT2STR_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d))        /* void uint2str( u8* ptr , u32 X, u16 digit, s32 fillwithzero );*/
#define UTIL_int2str(a,b,c,d)                   ((tCircleFunc4)(Circle_API [UTIL_INT2STR_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d))         /* void  int2str( u8* ptr , s32 X, u16 digit, s32 fillwithzero );*/
#define UTIL_GetVersion()       (const u8*)   (((tCircleFunc0)(Circle_API [UTIL_GET_VERSION_ID])) ())                                      /* u8* UTIL_GetVersion( void );*/
#define UTIL_ReadBackupRegister(a)     (u32)    (((tCircleFunc1)(Circle_API [UTIL_READ_BACKUPREGISTER_ID])) ((u32)(a)))                      /* u16 UTIL_ReadBackupRegister( u16 BKP_DR );*/
#define UTIL_WriteBackupRegister(a,b)           ((tCircleFunc2)(Circle_API [UTIL_WRITE_BACKUPREGISTER_ID])) ((u32)(a),(u32)(b))              /* void UTIL_WriteBackupRegister( u16 BKP_DR, u16 Data );*/
#define UTIL_GetBat()                  (u32)    (((tCircleFunc0)(Circle_API [UTIL_GET_BAT_ID])) ())                                          /* u16 UTIL_GetBat( void );*/
#define UTIL_GetUsb()                  (u32)    (((tCircleFunc0)(Circle_API [UTIL_GET_USB_ID])) ())                                          /* u8 UTIL_GetUsb( void );*/
#define UTIL_SetIrqHandler(a,b)                 (((tCircleFunc2)(Circle_API [UTIL_SET_IRQ_HANDLER_ID])) ((s32)a,(tHandler)b))                /* void UTIL_SetIrqHandler ( s32 , tHandler );*/
#define UTIL_GetIrqHandler(a)          (u32)    (((tCircleFunc1)(Circle_API [UTIL_GET_IRQ_HANDLER_ID])) ((s32)a))                            /* tHandler* UTIL_GetIrqHandler ( s32 );*/
#define UTIL_SetSchHandler(a,b)                 (((tCircleFunc2)(Circle_API [UTIL_SET_SCH_HANDLER_ID])) ((s32)a,(tHandler)b))                /* void UTIL_SetSchHandler ( s32 , tHandler );*/
#define UTIL_GetSchHandler(a)          (u32)    (((tCircleFunc1)(Circle_API [UTIL_GET_SCH_HANDLER_ID])) ((s32)a))                            /* tHandler* UTIL_GetSchHandler ( s32 );*/
#define UTIL_GetTemp()                 (u32)    (((tCircleFunc0)(Circle_API [UTIL_GET_TEMP_ID])) ())                                         /* u16 UTIL_GetTemp( void );*/
#define UTIL_SetTempMode(a)                     (((tCircleFunc1)(Circle_API [UTIL_SET_TEMPMODE_ID])) ((s32)a))                               /* void UTIL_SetTempMode( s32 mode );*/
#define UTIL_GetPrimerType()           (u32)    (((tCircleFunc0)(Circle_API [UTIL_GET_PRIMERTYPE_ID])) ())                                   /* u16 UTIL_GetPrimerType();*/
#define UTIL_GetAppAddress(a)          (u32)    (((tCircleFunc1)(Circle_API [UTIL_GET_APP_ADDRESS_ID])) ((u32)(a)))                          /* long unsigned UTIL_GetAppAddress(u8* AppName);*/

/// @endcond

/*---------------------------------   MEMS  ------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
* @enum  Rotate_H12_V_Match_TypeDef
* @brief The 4 possible rotations.
*
* The 4 possible MEM rotations.
**/
typedef enum
   {
   V12 = 0,                                        /*!< No rotation.          */
   V3  = 1,                                        /*!< Rotation to the right.*/
   V6  = 2,                                        /*!< Rotation to the left. */
   V9  = 3                                         /*!< Half a rotation.      */
   } Rotate_H12_V_Match_TypeDef;

/**
* @struct   tMEMS_Info
* @brief    MEMS state description.
**/
typedef struct
   {
   s16 OutX;                     /*!< MEMS X position.                        */
   s16 OutX_F4;                  /*!< MEMS X position filtered on 4 values.   */
   s16 OutX_F16;                 /*!< MEMS X position filtered on 16 values.  */
   s32 OutX_F64;                 /*!< MEMS X position filtered on 64 values.  */
   s32 OutX_F256;                /*!< MEMS X position filtered on 256 values. */
   s16 OutY;                     /*!< MEMS Y position.                        */
   s16 OutY_F4;                  /*!< MEMS Y position filtered on 4 values.   */
   s16 OutY_F16;                 /*!< MEMS Y position filtered on 16 values.  */
   s32 OutY_F64;                 /*!< MEMS Y position filtered on 64 values.  */
   s32 OutY_F256;                /*!< MEMS Y position filtered on 256 values. */
   s16 OutZ;                     /*!< MEMS Z position.                        */
   s16 OutZ_F4;                  /*!< MEMS Z position filtered on 4 values.   */
   s16 OutZ_F16;                 /*!< MEMS Z position filtered on 16 values.  */
   s32 OutZ_F64;                 /*!< MEMS Z position filtered on 64 values.  */
   s32 OutZ_F256;                /*!< MEMS Z position filtered on 256 values. */
   s16 Shocked;                  /*!< MEMS shock counter (incremented...)     */
   s16 RELATIVE_X;               /*!< MEMS relative X position.               */
   s16 RELATIVE_Y;               /*!< MEMS relative Y position.               */
   s16 DoubleClick;              /*!< MEMS DoubleClick counter(incremented...)*/
   } tMEMS_Info;

/// @cond Internal

/* Exported defines ----------------------------------------------------------*/

/* MEMS functions definition*/
#define MEMS_GET_POSITION_ID   (MEMS_ID + 0)       /* Return the current (relative) Mems information*/
#define MEMS_GET_ROTATION_ID   (MEMS_ID + 1)       /* Return the current screen orientation of the circle*/
#define MEMS_SET_NEUTRAL_ID    (MEMS_ID + 2)       /* Set the current position as "neutral position"*/
#define MEMS_GET_INFO_ID       (MEMS_ID + 3)       /* Return Mems informations*/

/* Prototypes*/
#define MEMS_GetPosition(a,b)             ((tCircleFunc2)(Circle_API [MEMS_GET_POSITION_ID])) ((u32)(a),(u32)(b))      /*  void MEMS_GetPosition(s16 * pX, s16* pY);*/
#define MEMS_GetRotation(a)               ((tCircleFunc1)(Circle_API [MEMS_GET_ROTATION_ID])) ((u32)(a))      /*  void MEMS_GetRotation(Rotate_H12_V_Match_TypeDef * H12);*/
#define MEMS_SetNeutral()                 ((tCircleFunc0)(Circle_API [MEMS_SET_ROTATION_ID])) ()                       /*  void MEMS_SetNeutral( void );*/
#define MEMS_GetInfo()    ( (tMEMS_Info*) (((tCircleFunc0)(Circle_API [MEMS_GET_INFO_ID])) ()))                        /*  tMEMS_Info* MEMS_GetInfo (void)*/

/// @endcond

/*--------------------------------   POINTER  ----------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
* @enum  POINTER_mode
* @brief Available pointer modes.
*
* Description of all the available pointer modes in CircleOS.
**/
enum POINTER_mode
   {
   POINTER_UNDEF        = -1,    /*!< Pointer's mode is unknown!              */
   POINTER_OFF          =  0,    /*!< Pointer isn't managed and displayed.    */
   POINTER_ON           =  1,    /*!< Pointer mode used in main screen.       */
   POINTER_MENU         =  2,    /*!< Pointer management is used to select item menu (but pointer isn't displayed).  */
   POINTER_APPLICATION  =  3,    /*!< The managment of pointer depend of extern application.                         */
   POINTER_RESTORE_LESS =  4     /*!< The background isn't restored (to go faster).                                  */
   };

/**
* @enum  POINTER_state
* @brief The different pointer modes.
*
* Despite beeing in a undefined state, the pointer can be disabled or enable.
**/
enum POINTER_state
   {
   POINTER_S_UNDEF      = -1,                /*!< Pointer state is unknown!   */
   POINTER_S_DISABLED   =  0,                /*!< Pointer is disabled.        */
   POINTER_S_ENABLED    =  1                 /*!< Pointer is enabled.         */
   };

/**
* @struct   tPointer_Info
* @brief    Pointer position description.
**/
typedef struct
   {
   s16 xPos;                              /*!< X position of pointer.         */
   s16 yPos;                              /*!< Y position of pointer.         */
   s16 shift_PosX;                        /*!< Pointer speed on X axis.       */
   s16 shift_PosY;                        /*!< Pointer speed on Y axis        */
   s16 X_PosMin;                          /*!< Minimum position on X axis.    */
   s16 Y_PosMin;                          /*!< Minimum position on Y axis.    */
   s16 X_PosMax;                          /*!< Maximum position on X axis.    */
   s16 Y_PosMax;                          /*!< Maximum position on Y axis.    */
   } tPointer_Info;

/// @cond Internal

/* Exported defines ---------------------------------------------------------*/
#define POINTER_WIDTH 7

/* POINTER functions definition*/
#define POINTER_SET_RECT_ID                   (POINTER_ID + 0)          /* Set new limits for the move of the pointer*/
#define POINTER_SETRECTSCREEN_ID              (POINTER_ID + 1)          /* Remove any space restriction for the pointer moves.*/
#define POINTER_GETCURRENTANGLESTART_ID       (POINTER_ID + 2)          /* Return the current minimum angle to move pointer*/
#define POINTER_SETCURRENTANGLESTART_ID       (POINTER_ID + 3)          /* Set the current minimum angle to move pointer*/
#define POINTER_GETCURRENTSPEEDONANGLE_ID     (POINTER_ID + 4)          /* Return the ratio speed / angle*/
#define POINTER_SETCURRENTSPEEDONANGLE_ID     (POINTER_ID + 5)          /* Set the ratio speed / angle*/
#define POINTER_SETMODE_ID                    (POINTER_ID + 6)          /* Change the current mode of the pointer management*/
#define POINTER_GETMODE_ID                    (POINTER_ID + 7)          /* Return the current mode of the pointer management*/
#define POINTER_SETCURRENTPOINTER_ID          (POINTER_ID + 8)          /* Set the dimention and bitmap of pointer*/
#define POINTER_GETSTATE_ID                   (POINTER_ID + 9)          /* Return the current state*/
#define POINTER_DRAW_ID                       (POINTER_ID + 10)         /* Draw a pointer*/
#define POINTER_SAVE_ID                       (POINTER_ID + 11)         /* Save the background of the pointer*/
#define POINTER_RESTORE_ID                    (POINTER_ID + 12)         /* Restore the background of the pointer*/
#define POINTER_GETPOSITION_ID                (POINTER_ID + 13)         /* Return the poistion of the cursor (x=lower byte, y = upperbyte)*/
#define POINTER_SETPOSITION_ID                (POINTER_ID + 14)         /* Force the position of the pointer in the screen*/
#define POINTER_SETAPPLICATION_POINTER_MGR_ID (POINTER_ID + 15)         /* Set the application pointer manager*/
#define POINTER_SETCOLOR_ID                   (POINTER_ID + 16)         /* Set pointer color*/
#define POINTER_GETCOLOR_ID                   (POINTER_ID + 17)         /* Return pointer color*/
#define POINTER_GETINFO_ID                    (POINTER_ID + 18)         /* Return pointer informations*/
#define POINTER_SET_CURRENT_AREASTORE_ID      (POINTER_ID + 19)         /* Change the current storage area*/

/* Prototypes*/
#define POINTER_SetRect(a,b,c,d)                     ((tCircleFunc4)(Circle_API [POINTER_SET_RECT_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d))          /*void POINTER_SetRect ( s16 x, s16 y, s16 width, s16 height );  //Restrict the move of the pointer to a rectangle*/
#define POINTER_SetRectScreen()                      ((tCircleFunc0)(Circle_API [POINTER_SETRECTSCREEN_ID])) ()                                        /*void POINTER_SetRectScreen ( void );*/
#define POINTER_GetCurrentAngleStart()    (u16)      (((tCircleFunc0)(Circle_API [POINTER_GETCURRENTANGLESTART_ID])) ())                               /*u16  POINTER_GetCurrentAngleStart ( void );*/
#define POINTER_SetCurrentAngleStart(a)              ((tCircleFunc1)(Circle_API [POINTER_SETCURRENTANGLESTART_ID])) ((u32)(a))                         /*void POINTER_SetCurrentAngleStart ( u16 );*/
#define POINTER_GetCurrentSpeedOnAngle()  (u16)      (((tCircleFunc0)(Circle_API [POINTER_GETCURRENTSPEEDONANGLE_ID])) ())                             /*u16  POINTER_GetCurrentSpeedOnAngle ( void );*/
#define POINTER_SetCurrentSpeedOnAngle(a)            ((tCircleFunc1)(Circle_API [POINTER_SETCURRENTSPEEDONANGLE_ID])) ((u32)(a))                       /*void POINTER_SetCurrentSpeedOnAngle ( u16  newspeed );*/
#define POINTER_SetMode(a)                           ((tCircleFunc1)(Circle_API [POINTER_SETMODE_ID])) ((u32)(a))                                      /*void POINTER_SetMode( enum POINTER_mode mode);*/
#define POINTER_GetMode()       (enum POINTER_mode)  (((tCircleFunc0)(Circle_API [POINTER_GETMODE_ID])) ())                                            /*enum POINTER_mode POINTER_GetMode( void );*/
#define POINTER_SetCurrentPointer(a,b,c)             ((tCircleFunc3)(Circle_API [POINTER_SETCURRENTPOINTER_ID])) ((u32)(a),(u32)(b),(u32)(c))          /*void POINTER_SetCurrentPointer( u8 width, u8 height, u8 *bmp);*/
#define POINTER_GetState()      (enum POINTER_state) (((tCircleFunc0)(Circle_API [POINTER_GETSTATE_ID])) ())                                           /*enum POINTER_state POINTER_GetState(void);*/
#define POINTER_Draw(a,b,c,d,e)                      ((tCircleFunc5)(Circle_API [POINTER_DRAW_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d),(u32)(e))     /*void POINTER_Draw (u8 Line, u8 Column, u8 Width, u8 Height, u8 *Bmp);*/
#define POINTER_Save(a,b,c,d)                        ((tCircleFunc4)(Circle_API [POINTER_SAVE_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d))              /*void POINTER_Save (u8 Line, u8 Column, u8 Width, u8 Height);*/
#define POINTER_Restore(a,b,c,d)                     ((tCircleFunc4)(Circle_API [POINTER_RESTORE_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d))           /*void POINTER_Restore (u8 Line, u8 Column, u8 Width, u8 Height);*/
#define POINTER_GetPos()                  (u16)      (((tCircleFunc0)(Circle_API [POINTER_GETPOSITION_ID])) ())                                        /*u16  POINTER_GetPos(void);*/
#define POINTER_SetPos(a,b)                          ((tCircleFunc2)(Circle_API [POINTER_SETPOSITION_ID])) ((u32)(a),(u32)(b))                         /*void POINTER_SetPos ( u16 x, u16 y );*/
#define POINTER_SetApplication_Pointer_Mgr(a)        ((tCircleFunc1)(Circle_API [POINTER_SETAPPLICATION_POINTER_MGR_ID])) ((u32)(a))                   /*void POINTER_SetApplication_Pointer_Mgr(  tAppPtrMgr mgr );*/
#define POINTER_SetColor(a)                          ((tCircleFunc1)(Circle_API [POINTER_SETCOLOR_ID])) ((u32)(a))                                     /*void POINTER_SetColor ( u16 color )*/
#define POINTER_GetColor()                (u16)      (((tCircleFunc0)(Circle_API [POINTER_GETCOLOR_ID])) ())                                           /*u16  POINTER_GetColor ( void )*/
#define POINTER_GetInfo()      (tPointer_Info*)      (((tCircleFunc0)(Circle_API [POINTER_GETINFO_ID])) ())                                            /*tPointer_Info* POINTER_GetInfo ( void )*/
#define POINTER_SetCurrentAreaStore(a)               ((tCircleFunc1)(Circle_API [POINTER_SET_CURRENT_AREASTORE_ID])) ((u32)(a))                        /*void POINTER_SetCurrentAreaStore ( u8 *ptr )*/

/// @endcond

/*--------------------------------   BUTTON -----------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
* @enum  BUTTON_mode
* @brief Available button modes.
*
* List of all the available button mode in the CircleOS.
**/
enum BUTTON_mode
   {
   BUTTON_DISABLED      = -1,       /*!< No action on the button is detected. */
   BUTTON_ONOFF         =  0,       /*!< Detect ON/OFF pression type.         */
   BUTTON_ONOFF_FORMAIN =  1,       /*!< Special mode for main screen.        */
   BUTTON_WITHCLICK     =  2        /*!< Currently unused.                    */
   };

/**
* @enum  BUTTON_state
* @brief CircleOS button states.
*
* Description of the button states provided by CircleOS.
**/
enum BUTTON_state
   {
   BUTTON_UNDEF            = -1,    /*!< Undefined state.                     */
   BUTTON_RELEASED         =  0,    /*!< Button is released.                  */
   BUTTON_PUSHED           =  1,    /*!< Button was just pushed.              */
   BUTTON_PUSHED_FORMAIN   =  2,    /*!< Same as BUTTON_PUSHED when button mode is BUTTON_ONOFF_FORMAIN. */
   BUTTON_CLICK            =  3,    /*!< Currently unused.                    */
   BUTTON_DBLCLICK         =  4     /*!< Currently unused.                    */
   };

/// @cond Internal

/* Exported defines ----------------------------------------------------------*/


/* BUTTON functions definition */
#define BUTTON_GETSTATE_ID                   (BUTTON_ID + 0)         /* Return state of button*/
#define BUTTON_SETMODE_ID                    (BUTTON_ID + 1)         /* Set button mode*/
#define BUTTON_GETMODE_ID                    (BUTTON_ID + 2)         /* Return button mode*/
#define BUTTON_WAITFORRELEASE_ID             (BUTTON_ID + 3)         /* Disable temporarily any new button event*/

/* Prototypes*/
#define BUTTON_GetState()       (enum BUTTON_state)   (((tCircleFunc0)(Circle_API [BUTTON_GETSTATE_ID])) ())        /* enum BUTTON_state BUTTON_GetState(void);*/
#define BUTTON_SetMode(a);                            ((tCircleFunc1)(Circle_API [BUTTON_SETMODE_ID])) ((u32)(a))   /* void BUTTON_SetMode( enum BUTTON_mode mode);*/
#define BUTTON_GetMode();       (enum BUTTON_mode)    (((tCircleFunc0)(Circle_API [BUTTON_GETMODE_ID])) ())         /* enum BUTTON_mode BUTTON_GetMode ( void ) ;*/
#define BUTTON_WaitForRelease()                       ((tCircleFunc0)(Circle_API [BUTTON_WAITFORRELEASE_ID])) ()    /* void BUTTON_WaitForRelease(void);*/

/// @endcond

/*------------------------------   JOYSTICK -----------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
* @enum  JOYSTICK_state
* @brief CircleOS joystick states.
*
* Description of the joystick states provided by CircleOS.
**/
enum JOYSTICK_state
    {
    JOYSTICK_UNDEF     = -1,        /*!< Undefined state.                */
    JOYSTICK_RELEASED  =  0,        /*!< Joystick is released.           */
    JOYSTICK_LEFT      =  1,        /*!< Joystick was just pushed left.  */
    JOYSTICK_UP        =  2,        /*!< Joystick was just pushed up.    */
    JOYSTICK_RIGHT     =  3,        /*!< Joystick was just pushed right. */
    JOYSTICK_DOWN      =  4,         /*!< Joystick was just pushed down.  */
    JOYSTICK_RIGHT_UP   =  11,  /*JOYSTICK_RIGHT | JOYSTICK_UP*/
	JOYSTICK_LEFT_UP    =  5,  /*JOYSTICK_LEFT  | JOYSTICK_UP*/
	JOYSTICK_RIGHT_DOWN =  13, /*JOYSTICK_RIGHT | JOYSTICK_DOWN*/
	JOYSTICK_LEFT_DOWN  =  7,  /*JOYSTICK_LEFT  | JOYSTICK_DOWN */
    };

/// @cond Internal

/* Exported defines ----------------------------------------------------------*/


/* JOYSTICK functions definition*/
#define JOYSTICK_GETSTATE_ID                   (JOYSTICK_ID + 0)     /* Return state of joystick*/
#define JOYSTICK_WAITFORRELEASE_ID             (JOYSTICK_ID + 1)     /* Disable temporarily any new joystick event*/

/* Prototypes*/
#define JOYSTICK_GetState()       (enum JOYSTICK_state) (((tCircleFunc0)(Circle_API [JOYSTICK_GETSTATE_ID])) ())        // enum JOYSTICK_state JOYSTICK_GetState(void);
#define JOYSTICK_WaitForRelease()                        ((tCircleFunc0)(Circle_API [JOYSTICK_WAITFORRELEASE_ID])) ()   // void JOYSTICK_WaitForRelease(void);

/// @endcond

/*----------------------------------   LCD   -----------------------------------*/

/* Exported defines ----------------------------------------------------------*/

/* RGB is 16-bit coded as    G2G1G0B4 B3B2B1B0 R4R3R2R1 R0G5G4G3*/
#define RGB_MAKE(xR,xG,xB)    ( ( (xG&0x07)<<13 ) + ( (xG)>>5 )  +      \
                                ( ((xB)>>3) << 8 )          +      \
                                ( ((xR)>>3) << 3 ) )                    /*!< Macro to make a LCD compatible color format from RGB.  */

#define RGB_RED         0x00F8                  /*!<  Predefined color.       */
#define RGB_BLACK       0x0000                  /*!<  Predefined color.       */
#define RGB_WHITE       0xffff                  /*!<  Predefined color.       */
#define RGB_BLUE        0x1F00                  /*!<  Predefined color.       */
#define RGB_GREEN       0xE007                  /*!<  Predefined color.       */
#define RGB_YELLOW      (RGB_GREEN|RGB_RED)     /*!<  Predefined color.       */
#define RGB_MAGENTA     (RGB_BLUE|RGB_RED)      /*!<  Predefined color.       */
#define RGB_LIGHTBLUE   (RGB_BLUE|RGB_GREEN)    /*!<  Predefined color.       */
#define RGB_ORANGE      (RGB_RED | 0xE001)      /*!<  Predefined color ( Green/2 + red ).  */
#define RGB_PINK        (RGB_MAGENTA | 0xE001)  /*!<  Predefined color ( Green/2 + magenta ). */

/* PWM rates.*/
#define BACKLIGHTMIN                0x1000   /*!< Minimal PWM rate.           */
#define DEFAULT_CCR_BACKLIGHTSTART  0x8000   /*!< Default PWM rate.           */

/* SCREEN Infos*/
#define SCREEN_WIDTH          128         /*!< Width of visible screen in pixels.   */
#define SCREEN_HEIGHT         128         /*!< Height of visible screen in pixels.  */
#define CHIP_SCREEN_WIDTH     132         /*!< Width of screen driven by LCD controller in pixels.  */
#define CHIP_SCREEN_HEIGHT    132         /*!< Height of screen driven by LCD controller in pixels.  */

/* Characters Infos*/
#define CHAR_WIDTH            7           /*!< Width of a character.   */
#define CHAR_HEIGHT           14          /*!< Height of a character.   */

/// @cond Internal

/* LCD functions definition*/
#define LCD_SETRECTFORCMD_ID                   (LCD_ID + 0)          /* Define the rectangle (for the next command to be applied)*/
#define LCD_GETPIXEL_ID                        (LCD_ID + 1)          /* Read the value of one pixel*/
#define LCD_DRAWPIXEL_ID                       (LCD_ID + 2)          /* Draw a Graphic image on slave LCD.*/
#define LCD_SENDLCDCMD_ID                      (LCD_ID + 3)          /* Send one byte command to LCD LCD.*/
#define LCD_SENDLCDDATA_ID                     (LCD_ID + 4)          /* Display one byte data to LCD LCD.*/
#define LCD_READLCDDATA_ID                     (LCD_ID + 5)          /* Read LCD byte data displayed on LCD LCD.*/
#define LCD_FILLRECT_ID                        (LCD_ID + 6)          /* Fill a rectangle with one color*/
#define LCD_DRAWRECT_ID                        (LCD_ID + 7)          /* Draw a rectangle with one color*/
#define LCD_DISPLAYCHAR_ID                     (LCD_ID + 8)          /* Display one character*/
#define LCD_RECTREAD_ID                        (LCD_ID + 9)          /* Save a rectangle of the monitor RAM*/
#define LCD_SETBACKLIGHT_ID                    (LCD_ID + 10)         /* Modify the PWM rate*/
#define LCD_GETBACKLIGHT_ID                    (LCD_ID + 11)         /* Return the PWM rate*/
#define LCD_SETROTATESCREEN_ID                 (LCD_ID + 12)         /* Enable/Disable screen rotation*/
#define LCD_GETROTATESCREEN_ID                 (LCD_ID + 13)         /* Return screen rotation mode*/
#define LCD_SETSCREENORIENTATION_ID            (LCD_ID + 14)         /* Set screen orientation*/
#define LCD_GETSCREENORIENTATION_ID            (LCD_ID + 15)         /* Return screen orientation*/
#define LCD_SETBACKLIGHT_OFF_ID                (LCD_ID + 16)         /* Switch the LCD back light off.*/
#define LCD_SETBACKLIGHT_ON_ID                 (LCD_ID + 17)         /* Switch the LCD back light on.*/
#define LCD_SETFONT_ID                         (LCD_ID + 18)         /* Switch to a new font.*/
#define LCD_SETDEFAULT_ID                      (LCD_ID + 19)         /* Switch to the default system font.*/

/* Prototypes*/
#define LCD_SetRect_For_Cmd(a,b,c,d)                 ((tCircleFunc4)(Circle_API [LCD_SETRECTFORCMD_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d))                  /*void  LCD_SetRect_For_Cmd ( s16 x, s16 y, s16 width, s16 height)*/
#define LCD_GetPixel(a,b)                 (u16)      (((tCircleFunc2)(Circle_API [LCD_GETPIXEL_ID])) ((u32)(a),(u32)(b)))                                       /*u16   LCD_GetPixel (u8 x, u8 y)*/
#define LCD_DrawPixel(a,b,c)                         ((tCircleFunc3)(Circle_API [LCD_DRAWPIXEL_ID])) ((u32)(a),(u32)(b),(u32)(c))                               /*void  LCD_SetPixel (u8 x, u8 y, u16 Pixel) ;*/
#define LCD_SendLCDCmd(a)                            ((tCircleFunc1)(Circle_API [LCD_SENDLCDCMD_ID])) ((u32)(a))                                                /*void  LCD_SendLCDCmd(u8 Cmd);*/
#define LCD_SendLCDData(a)                           ((tCircleFunc1)(Circle_API [LCD_SENDLCDDATA_ID])) ((u32)(a))                                               /*void  LCD_SendLCDData(u8 Data);*/
#define LCD_ReadLCDData()                 (u32)      (((tCircleFunc0)(Circle_API [LCD_READLCDDATA_ID])) ())                                                     /*u32   LCD_ReadLCDData(void);*/
#define LCD_FillRect(a,b,c,d,e)                      ((tCircleFunc5)(Circle_API [LCD_FILLRECT_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d),(u32)(e))              /*void  LCD_FillRect ( u16 x, u16 y, u16 width, u16 height, u16 color );*/
#define LCD_DrawRect(a,b,c,d,e)                      ((tCircleFunc5)(Circle_API [LCD_DRAWRECT_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d),(u32)(e))              /*void  LCD_DrawRect ( u16 x, u16 y, u16 width, u16 height, u16 color );*/
#define LCD_DisplayChar(a,b,c,d,e,f)                 ((tCircleFunc6)(Circle_API [LCD_DISPLAYCHAR_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d),(u32)(e),(u32)(f))  /*void  LCD_DisplayChar(u8 x, u8 y, u8 Ascii, u16 TextColor, u16 BGndColor, u16 CharMagniCoeff);*/
#define LCD_RectRead(a,b,c,d,e)                      ((tCircleFunc5)(Circle_API [LCD_RECTREAD_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d),(u32)(e))              /*void  LCD_RectRead ( u16 x, u16 y, u16 width, u16 height, u8* bmp );*/
#define LCD_SetBackLight(a)                          ((tCircleFunc1)(Circle_API [LCD_SETBACKLIGHT_ID])) ((u32)(a))                                              /**oid  LCD_SetBackLight(u32 newBaclightStart);*/*/
#define LCD_GetBackLight()                (u32)      (((tCircleFunc0)(Circle_API [LCD_GETBACKLIGHT_ID])) ())                                                    /*u32   LCD_GetBackLight(void);*/
#define LCD_SetRotateScreen(a)                       ((tCircleFunc1)(Circle_API [LCD_SETROTATESCREEN_ID])) ((u32)(a))                                           /*void  LCD_SetRotateScreen ( u8 RotateScreen)*/
#define LCD_GetRotateScreen()             (u32)      (((tCircleFunc0)(Circle_API [LCD_GETROTATESCREEN_ID])) ())                                                 /*u8    LCD_GetRotateScreen (void)*/
#define LCD_SetScreenOrientation(a)                  ((tCircleFunc1)(Circle_API [LCD_SETSCREENORIENTATION_ID])) ((u32)(a))                                      /*void LCD_SetScreenOrientation (Rotate_H12_V_Match_TypeDef ScreenOrientation)*/
#define LCD_GetScreenOrientation()        (u32)      (((tCircleFunc0)(Circle_API [LCD_GETSCREENORIENTATION_ID])) ())                                            /*Rotate_H12_V_Match_TypeDef LCD_GetScreenOrientation (void)*/
#define LCD_SetBackLightOff()                        ((tCircleFunc0)(Circle_API [LCD_SETBACKLIGHT_OFF_ID])) ()
#define LCD_SetBackLightOn()                         ((tCircleFunc0)(Circle_API [LCD_SETBACKLIGHT_ON_ID])) ()
#define LCD_SetFont(a)                               ((tCircleFunc1)(Circle_API [LCD_SETFONT_ID])) ((u32)(a))                                                   //void LCD_SetFont(u8* NewFont);
#define LCD_SetDefaultFont()                         ((tCircleFunc0)(Circle_API [LCD_SETDEFAULT_ID])) ()                                                        //void LCD_SetDefaultFont(void);
/// @endcond

/*----------------------------------   DRAW   ----------------------------------*/

/// @cond Internal

/* Exported defines ----------------------------------------------------------*/

/* DRAW functions definition*/
#define DRAW_SETDEFAULTCOLOR_ID                   (DRAW_ID + 0)         /* Reset colors (bgnd + text)*/
#define DRAW_CLEAR_ID                             (DRAW_ID + 1)         /* Clear the LCD display*/
#define DRAW_SETIMAGE_ID                          (DRAW_ID + 2)         /* Draw a colored image*/
#define DRAW_SETIMAGEBW_ID                        (DRAW_ID + 3)         /* Draw a black and white image*/
#define DRAW_SETLOGOBW_ID                         (DRAW_ID + 4)         /* Draw logo*/
#define DRAW_DISPLAYVBAT_ID                       (DRAW_ID + 5)         /* Display the voltage of battery in ascii*/
#define DRAW_DISPLAYTIME_ID                       (DRAW_ID + 6)         /* Display time in ascii*/
#define DRAW_DISPLAYSTRING_ID                     (DRAW_ID + 7)         /* Display a 17char max string of characters*/
#define DRAW_DISPLAYSTRINGINVERTED_ID             (DRAW_ID + 8)         /* Display a 17char max string of characters with inverted colors*/
#define DRAW_GETCHARMAGNICOEFF_ID                 (DRAW_ID + 9)         /* Return the magnifying value for the characters*/
#define DRAW_SETCHARMAGNICOEFF_ID                 (DRAW_ID + 10)        /* Set the magnifying value for the characters*/
#define DRAW_GETTEXTCOLOR_ID                      (DRAW_ID + 11)        /* Return the current text color*/
#define DRAW_SETTEXTCOLOR_ID                      (DRAW_ID + 12)        /* Set the current text color*/
#define DRAW_GETBGNDCOLOR_ID                      (DRAW_ID + 13)        /* Return the current background color*/
#define DRAW_SETBGNDCOLOR_ID                      (DRAW_ID + 14)        /* Set the current background color*/
#define DRAW_LINE_ID                              (DRAW_ID + 15)        /* Draw a Line between (using Bresenham algorithm) */
#define DRAW_SETIMAGESEL_ID                       (DRAW_ID + 16)        /* Draw a colored image with selected background color*/
#define DRAW_PUTC_ID                              (DRAW_ID + 17)        /* Set the current position of the cursor*/
#define DRAW_PUTS_ID                              (DRAW_ID + 18)        /* Draw a colored image with selected background color*/
#define DRAW_SETCURSOR_POS_ID                     (DRAW_ID + 19)        /* Set the current position of the cursor*/
#define DRAW_GETCURSOR_POS_ID                     (DRAW_ID + 20)        /* Get the current position of the cursor*/
#define DRAW_SETCURSORMARGIN_ID                   (DRAW_ID + 21)        /* Set the current margins of the cursor*/
#define DRAW_GETCURSORMARGIN_ID                   (DRAW_ID + 22)        /* Get the current margins of the cursor*/

/*Prototypes*/
#define DRAW_SetDefaultColor()                      ((tCircleFunc0)(Circle_API [DRAW_SETDEFAULTCOLOR_ID])) ()                                                 /*void  DRAW_SetDefaultColor (void);*/
#define DRAW_Clear()                                ((tCircleFunc0)(Circle_API [DRAW_CLEAR_ID])) ()                                                           /*void  DRAW_Clear(void);*/
#define DRAW_SetImage(a,b,c,d,e)                    ((tCircleFunc5)(Circle_API [DRAW_SETIMAGE_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d),(u32)(e))            /*void  DRAW_SetImage(const u16 *imageptr, u8 x, u8 y, u8 width, u8 height);*/
#define DRAW_SetImageBW(a,b,c,d,e)                  ((tCircleFunc5)(Circle_API [DRAW_SETIMAGEBW_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d),(u32)(e))          /*void  DRAW_SetImageBW(const u8 *imageptr, u8 x, u8 y, u8 width, u8 height);*/
#define DRAW_SetLogoBW()                            ((tCircleFunc0)(Circle_API [DRAW_SETLOGOBW_ID])) ()                                                       /*void  DRAW_SetLogoBW(void);*/
#define DRAW_DisplayVbat(a,b)                       ((tCircleFunc2)(Circle_API [DRAW_DISPLAYVBAT_ID])) ((u32)(a),(u32)(b))                                    /*void  DRAW_DisplayVbat(u8 x, u8 y);*/
#define DRAW_DisplayTime(a,b)                       ((tCircleFunc2)(Circle_API [DRAW_DISPLAYTIME_ID])) ((u32)(a),(u32)(b))                                    /*void  DRAW_DisplayTime(u8 x, u8 y);*/
#define DRAW_DisplayString(a,b,c,d)                 ((tCircleFunc4)(Circle_API [DRAW_DISPLAYSTRING_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d))                /*void  DRAW_DisplayString( u8 x, u8 y, u8 *ptr, u8 len );*/
#define DRAW_DisplayStringInverted(a,b,c,d)         ((tCircleFunc4)(Circle_API [DRAW_DISPLAYSTRINGINVERTED_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d))        /*void  DRAW_DisplayStringInverted( u8 x, u8 y, u8 *ptr, u8 len );*/
#define DRAW_GetCharMagniCoeff()              (u16) (((tCircleFunc0)(Circle_API [DRAW_GETCHARMAGNICOEFF_ID])) ())                                             /*u16   DRAW_GetCharMagniCoeff(void);*/
#define DRAW_SetCharMagniCoeff(a)                   ((tCircleFunc1)(Circle_API [DRAW_SETCHARMAGNICOEFF_ID])) ((u32)(a))                                       /*void  DRAW_SetCharMagniCoeff(u16 Coeff);*/
#define DRAW_GetTextColor()                   (u16) (((tCircleFunc0)(Circle_API [DRAW_GETTEXTCOLOR_ID])) ())                                                  /*u16   DRAW_GetTextColor(void);*/
#define DRAW_SetTextColor(a)                        ((tCircleFunc1)(Circle_API [DRAW_SETTEXTCOLOR_ID])) ((u32)(a))                                            /*void  DRAW_SetTextColor(u16 Color);*/
#define DRAW_GetBGndColor()                   (u16) (((tCircleFunc0)(Circle_API [DRAW_GETBGNDCOLOR_ID])) ())                                                  /*u16   DRAW_GetBGndColor(void);*/
#define DRAW_SetBGndColor(a)                        ((tCircleFunc1)(Circle_API [DRAW_SETBGNDCOLOR_ID])) ((u32)(a))                                            /*void  DRAW_SetBGndColor(u16 Color);*/
#define DRAW_Line(a,b,c,d,e)                        ((tCircleFunc5)(Circle_API [DRAW_LINE_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d),(u32)(e))                /*void  DRAW_Line(s16 x1, s16 y1, s16 x2, s16 y2, u16 color );*/
#define DRAW_SetImageSel(a,b,c,d,e,f,g)             ((tCircleFunc7)(Circle_API [DRAW_SETIMAGESEL_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d),(u32)(e),(u32)(f),(u32)(g))  /*void DRAW_SetImageSel(const u16* imageptr, u8 x, u8 y, u8 width, u8 height, u16 oldBgndColor, u16 newBgndColor);*/
#define DRAW_Putc(a)                                ((tCircleFunc1)(Circle_API [DRAW_PUTC_ID])) ((u32)(a))                                                    /*void DRAW_Putc( u8 Ascii );*/
#define DRAW_Puts(a)                                ((tCircleFunc1)(Circle_API [DRAW_PUTS_ID])) ((u32)(a))                                                    /*void DRAW_Puts( const u8* ptr );*/
#define DRAW_SetCursorPos(a,b)                      ((tCircleFunc2)(Circle_API [DRAW_SETCURSOR_POS_ID])) ((u32)(a),(u32)(b))                                  /*void DRAW_SetCursorPos( s32 x, s32 y );*/
#define DRAW_GetCursorPos()                   (u16) (((tCircleFunc0)(Circle_API [DRAW_GETCURSOR_POS_ID])) ())                                                 /*s32 DRAW_GetCursorPos();*/
#define DRAW_SetCursorMargin(a,b,c,d)               ((tCircleFunc4)(Circle_API [DRAW_SETCURSORMARGIN_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d))              /*void DRAW_SetCursorMargin( s32 lx, s32 rx, s32 hy, s32 ly );*/
#define DRAW_GetCursorMargin(a,b,c,d)               ((tCircleFunc4)(Circle_API [DRAW_GETCURSORMARGIN_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d))              /*void DRAW_GetCursorMargin( s32* lx, s32* rx, s32* hy, s32* ly );*/

/// @endcond


/*--------------------------------   BUZZER  -----------------------------------*/

/* Exported type def ---------------------------------------------------------*/

/**
* @enum  BUZZER_mode
* @brief CircleOS buzzer modes.
*
* Without the undefined mode, the CircleOS provides 5 modes for its buzzer.
**/
enum BUZZER_mode
   {
   BUZZER_UNDEF      = -1,         /*!<  undefined mode for buzzer            */
   BUZZER_OFF        =  0,         /*!<  The buzzer is put off.               */
   BUZZER_ON         =  1,         /*!<  The buzzer is put on.                */
   BUZZER_SHORTBEEP  =  2,         /*!<  Make buzzer to bip for a short time  */
   BUZZER_LONGBEEP   =  3,         /*!<  Make buzzer to bip for a long time   */
   BUZZER_PLAYMUSIC  =  4          /*!<  Make buzzer to play a music          */
   };

/// @cond Internal

/* Exported defines ----------------------------------------------------------*/
#define BUZZER_BEEP  BUZZER_SHORTBEEP

/* BUZZER functions definition*/
#define BUZZER_SETMODE_ID                       (BUZZER_ID + 0)   /* Set new buzzer mode*/
#define BUZZER_GETMODE_ID                       (BUZZER_ID + 1)   /* Get the current buzzer mode.*/
#define BUZZER_PLAY_MUSIC_ID                    (BUZZER_ID + 2)   /* Plays the provided melody that follows the RTTTL Format.*/

/* Prototypes*/
#define BUZZER_SetMode(a)                            ((tCircleFunc1)(Circle_API [BUZZER_SETMODE_ID])) ((u32)(a))               /*void  BUZZER_SetMode( enum BUZZER_mode mode);*/
#define BUZZER_GetMode()      (enum  BUZZER_mode)    (((tCircleFunc0)(Circle_API [BUZZER_GETMODE_ID])) ())                     /*enum  BUZZER_mode BUZZER_GetMode( void );*/
#define BUZZER_PlayMusic(a)                          ((tCircleFunc1)(Circle_API [BUZZER_PLAY_MUSIC_ID])) ((u32)(a))            /*void BUZZER_PlayMusic (const u8 *melody );*/


/// @endcond


/*---------------------------------   MENU   -----------------------------------*/

/* Exported defines ----------------------------------------------------------*/
#define REMOVE_MENU     0x01  /*!< Menu flag: remove menu when item selected. */
#define APP_MENU        0x02  /*!< Menu flag: item is an application.         */
#define MENU_MAXITEM    8     /*!< Maximum number of item in a menu.          */

/* Exported type def ---------------------------------------------------------*/

/**
* @struct   tMenuItem
* @brief    Menu item description.
**/
typedef struct
   {
   const u8* Text;                            /*!<  Name of Item displayed in menu   */
   enum MENU_code (*Fct_Init)  ( void );        /*!<  First function launched if item is selected. */
   enum MENU_code (*Fct_Manage)( void );        /*!<  Second function launched after a "return MENU_CONTINU_COMMAND" in the first function   */
   s32 fRemoveMenu;                             /*!<  Flag to know if remove menu at end  */
   } tMenuItem;

/**
* @struct   tMenu
* @brief    Menu description.
**/
typedef struct
   {
   unsigned fdispTitle: 1;                /*!< Display title is set.          */
   const u8* Title;                     /*!< Menu title.                    */
   s32 NbItems;                           /*!< Number of items in the menu ( must be <= MENU_MAXITEM )  */
   s32 LgMax;                             /*!< Unused.                        */
   s32 XPos;                              /*!< X position of menu bottom-left corner. */
   s32 YPos;                              /*!< Y position of menu bottom-left corner. */
   s32 XSize;                             /*!< Unused.                        */
   s32 YSize;                             /*!< Unused.                        */
   u32 SelectedItem;             /*!< ID of selected item (0 for first item, 1 for second item, ...) */
   tMenuItem Items[MENU_MAXITEM];         /*!< Items of menu.  */
   } tMenu;

/**
* @enum  MENU_code
* @brief Application return values.
*
* List of all the codes available for CircleOS application return values.
**/
enum MENU_code
   {
   MENU_LEAVE              = 0,                    /*!< Leave application.    */
   MENU_CONTINUE           = 1,                    /*!< Continue application. */
   MENU_REFRESH            = 2,                    /*!< Refresh current menu. */
   MENU_CHANGE             = 3,                    /*!< Change current menu.  */
   MENU_CONTINUE_COMMAND   = 4,                    /*!< Sent by Ini functions.*/
   MENU_LEAVE_AS_IT        = 5,                    /*!< Abort current menu or command.*/
   MENU_RESTORE_COMMAND    = 6                     /*!< Restore the previous command.*/
   };

/// @cond Internal

/* Exported defines ----------------------------------------------------------*/

/* MENU functions definition*/
#define MENU_SET_ID                             (MENU_ID + 0)        /* Display a menu*/
#define MENU_REMOVE_ID                          (MENU_ID + 1)        /* Remove the current menu, DRAW_Clear and set pointer mode to "POINTER_ON".*/
#define MENU_QUESTION_ID                        (MENU_ID + 2)        /* Dedicated menu for ask question and yes/no responses*/
#define MENU_PRINT_ID                           (MENU_ID + 3)        /* Display a popup menu with a string.*/
#define MENU_CLEAR_CURRENT_COMMAND_ID           (MENU_ID + 4)        /* Set CurrentCommand to 0*/
#define MENU_SET_LEVELTITLE_ID                  (MENU_ID + 5)        /* Set the title of level menu managed by MENU_SetLevel_Mgr.*/
#define MENU_SET_TEXTCOLOR_ID                   (MENU_ID + 6)        /* Set the color used for text menu.*/
#define MENU_GET_TEXTCOLOR_ID                   (MENU_ID + 7)        /* Return the color used for text menu.*/
#define MENU_SET_BGNDCOLOR_ID                   (MENU_ID + 8)        /* Set the background color used for menu.*/
#define MENU_GET_BGNDCOLOR_ID                   (MENU_ID + 9)        /* Return the background color used for menu.*/
#define MENU_QUIT_ID                            (MENU_ID + 10)       /* Leave the current menu (stand for "cancel" and do a DRAW_Clear)*/
#define MENU_SET_LEVELINI_ID                    (MENU_ID + 11)       /* Initialise a generic function to set a avalue in the range of [0,4]*/
#define MENU_CLEAR_CURRENT_MENU_ID              (MENU_ID + 12)       /* Set CurrentMenu to 0*/
#define MENU_SET_LEVEL_MGR_ID                   (MENU_ID + 13)       /* Generic function to set a avalue in the range of [0,4] (handling of the control)*/
#define MENU_SET_APP_DIVIDER_ID                 (MENU_ID + 14)       /* Set divider for applications calls*/
#define MENU_RESTORE_APP_DIVIDER_ID             (MENU_ID + 15)       /* Restore the factory divider for applications calls*/

/* Prototypes*/
#define MENU_Set(a)                                  ((tCircleFunc1)(Circle_API [MENU_SET_ID])) ((u32)(a))                    /*void  MENU_Set ( tMenu *mptr );*/
#define MENU_Remove()                                ((tCircleFunc0)(Circle_API [MENU_REMOVE_ID])) ()                         /*void  MENU_Remove ( void ) ;*/
#define MENU_Question(a,b)                           ((tCircleFunc2)(Circle_API [MENU_QUESTION_ID])) ((u32)(a),(u32)(b))      /*void  MENU_Question ( u8 *str, s32 *answer );*/
#define MENU_Print(a)                                ((tCircleFunc1)(Circle_API [MENU_PRINT_ID])) ((u32)(a))                  /*void  MENU_Print ( u8 *str );*/
#define MENU_ClearCurrentCommand()                   ((tCircleFunc0)(Circle_API [MENU_CLEAR_CURRENT_COMMAND_ID])) ()          /*void MENU_ClearCurrentCommand(void)*/
#define MENU_SetLevelTitle(a)                        ((tCircleFunc1)(Circle_API [MENU_SET_LEVELTITLE_ID])) ((u32)(a))         /*void MENU_SetLevelTitle(u8* title)*/
#define MENU_SetTextColor(a)                         ((tCircleFunc1)(Circle_API [MENU_SET_TEXTCOLOR_ID])) ((u32)(a))          /*void MENU_SetTextColor ( s32 TextColor )*/
#define MENU_GetTextColor()                 (u32)    (((tCircleFunc0)(Circle_API [MENU_GET_TEXTCOLOR_ID])) ())                /*s32 MENU_GetTextColor ( void )*/
#define MENU_SetBGndColor(a)                         ((tCircleFunc1)(Circle_API [MENU_SET_BGNDCOLOR_ID])) ((u32)(a))          /*void MENU_SetBGndColor ( s32 BGndColor )*/
#define MENU_GetBGndColor()                 (u32)    (((tCircleFunc0)(Circle_API [MENU_GET_BGNDCOLOR_ID])) ())                /*s32 MENU_GetBGndColor ( void )*/
#define MENU_Quit()              (enum MENU_code)    (((tCircleFunc0)(Circle_API [MENU_QUIT_ID])) ())                         /*enum MENU_code MENU_Quit ( void )*/
#define MENU_SetLevel_Ini()      (enum MENU_code)    (((tCircleFunc0)(Circle_API [MENU_SET_LEVELINI_ID])) ())                 /*enum MENU_code MENU_SetLevel_Ini ( void )*/
#define MENU_ClearCurrentMenu()                       ((tCircleFunc0)(Circle_API [MENU_CLEAR_CURRENT_MENU_ID])) ()            /*void MENU_ClearCurrentMenu(void)*/
#define MENU_SetLevel_Mgr(a,b)   (enum MENU_code)    ((tCircleFunc2)(Circle_API [MENU_SET_LEVEL_MGR_ID])) ((u32)(a),(u32)(b)) /*enum MENU_code MENU_SetLevel_Mgr ( u32 *value, u32 value_range [] )*/
#define MENU_SetAppliDivider(a)                       ((tCircleFunc1)(Circle_API [MENU_SET_APP_DIVIDER_ID])) ((u32)(a))       /*void MENU_SetAppliDivider( s32 divider )*/
#define MENU_RestoreAppliDivider()                    ((tCircleFunc0)(Circle_API [MENU_RESTORE_APP_DIVIDER_ID])) ()           /*void MENU_RestoreAppliDivider()*/

/// @endcond

/*----------------------------------   LED -------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/**
* @enum  LED_mode
* @brief LED modes.
*
* LEDs may be on, off or blinking slowly or fastly!
**/
enum LED_mode
   {
   LED_UNDEF       = -1,                     /*!< Undefined led mode.         */
   LED_OFF         = 0,                      /*!< Put off the led.            */
   LED_ON          = 1,                      /*!< Put on the led.             */
   LED_BLINKING_LF = 2,                      /*!< Slow blinking led mode.     */
   LED_BLINKING_HF = 3                       /*!< Fast blinking led mode.     */
   };

/**
* @enum LED_id
* @brief Available LEDs.
*
* List of all the available LEDs.
**/
enum LED_id
   {
   LED_GREEN = 0,                                        /*!< Green led id.   */
   LED_RED = 1                                           /*!< Red led id.     */
   };

/// @cond Internal

/* Exported defines ----------------------------------------------------------*/

/* LED functions definition*/
#define LED_SET_ID            (LED_ID + 0)      /* Set a specified LED in a specified mode.*/

/* Prototypes*/
#define LED_Set(a,b)          ((tCircleFunc2)(Circle_API [LED_SET_ID])) ((u32)(a),(u32)(b))  /*void LED_Set ( enum LED_id id, enum LED_mode mode )               //void LED_Set ( enum LED_id id, enum LED_mode mode );*/

/// @endcond

/*--------------------------------   RTC  --------------------------------------*/

/* Exported defines ----------------------------------------------------------*/

/* Backup registers*/
#define BKP_SYS1  1        /*!<  Backup register reserved for OS  */
#define BKP_SYS2  2        /*!<  Backup register reserved for OS  */
#define BKP_SYS3  3        /*!<  Backup register reserved for OS  */
#define BKP_SYS4  4        /*!<  Backup register reserved for OS  */
#define BKP_SYS5  5        /*!<  Backup register reserved for OS  */
#define BKP_SYS6  6        /*!<  Backup register reserved for OS  */
#define BKP_SYS7  11       /*!<  Backup register reserved for OS  */

#define BKP_USER1    7     /*!<  Backup available for users application */
#define BKP_USER2    8     /*!<  Backup available for users application */
#define BKP_USER3    9     /*!<  Backup available for users application */
#define BKP_USER4    10    /*!<  Backup available for users application */

/// @cond Internal

/*RTC functions definition*/
#define RTC_SET_TIME_ID       (RTC_ID + 0)      /* Set current time.*/
#define RTC_GET_TIME_ID       (RTC_ID + 1)      /* Return current time.*/
#define RTC_DISPLAY_TIME_ID   (RTC_ID + 2)      /* Display current time on the 6th line at column 0.*/

/* Prototypes*/
#define RTC_SetTime(a,b,c)    ((tCircleFunc3)(Circle_API [RTC_SET_TIME_ID])) ((u32)(a),(u32)(b),(u32)(c))     /*void  RTC_SetTime (u32 THH, u32 TMM, u32 TSS);*/
#define RTC_GetTime(a,b,c)    ((tCircleFunc3)(Circle_API [RTC_GET_TIME_ID])) ((u32)(a),(u32)(b),(u32)(c))     /*void  RTC_GetTime (u32 * THH, u32 * TMM, u32 * TSS);*/
#define RTC_DisplayTime()     ((tCircleFunc0)(Circle_API [RTC_DISPLAY_TIME_ID])) ()                           /*void  RTC_DisplayTime ( void );*/

/// @endcond

/*------------------------------   TOOLBAR -----------------------------------*/

/* Exported defines ----------------------------------------------------------*/
#define TOOLBAR_MAXITEMS 4
#define BUTTON_HEIGHT    32
#define BUTTON_WIDTH     32

/* Exported types ------------------------------------------------------------*/
/**
* @struct   tToolbarItem
* @brief    Toolbar item description.
**/
typedef struct
   {
   const u8  *icon;
   enum MENU_code (*Fct_Manage)( void );
   } tToolbarItem;

/**
* @struct   tToolbar
* @brief    Toolbar description.
**/
typedef struct
   {
   s32 NbItems;
   u32 FirstDispItem;            /* index of the first displayed item (if > 4)*/
   tToolbarItem Items[TOOLBAR_MAXITEMS];
   } tToolbar;


/// @cond Internal

/* TOOLBAR functions definition*/
#define TOOLBAR_SET_ID                   (TOOLBAR_ID + 0)     /* Set new toolbar*/
#define TOOLBAR_SET_DEFAULT_TOOLBAR_ID   (TOOLBAR_ID + 1)     /* Restore default factory toolbar*/
#define TOOLBAR_CHANGE_BUTTON_ID         (TOOLBAR_ID + 2)     /* Change icone and function of a button*/

/* Prototypes*/
#define TOOLBAR_Set(a)                   (((tCircleFunc1)(Circle_API [TOOLBAR_SET_ID])) ((u32)(a)))                                /* void TOOLBAR_Set(tToolbar* NewToolbar);*/
#define TOOLBAR_SetDefaultToolbar()       ((tCircleFunc0)(Circle_API [TOOLBAR_SET_DEFAULT_TOOLBAR_ID])) ()                         /* void TOOLBAR_SetDefaultToolbar(void);*/
#define TOOLBAR_ChangeButton(a,b,c)      (((tCircleFunc3)(Circle_API [TOOLBAR_CHANGE_BUTTON_ID])) ((u32)(a),(u32)(b),(u32)(c) ))   /* void TOOLBAR_ChangeButton(s32 button, const u8  *newicon, enum MENU_code (*newfct)( void ));*/
/// @endcond

/*------------------------------ TOUCHSCREEN ----------------------------------*/

/* Exported defines ----------------------------------------------------------*/
#define LIST_MAXITEM 64

/* Exported types ------------------------------------------------------------*/
typedef enum {TS_NORMAL, TS_DRAWING, TS_CALIBRATION} TOUCHSCR_Mode_enum;

/**
* @struct   tListItem
* @brief    List item description.
**/
typedef struct
   {
   const u8 *Text;
   } tListItem;

/**
* @struct   tList
* @brief    List description.
**/
typedef struct
   {
   unsigned     fdispTitle : 1;
   const u8   *Title;
   s32          NbItems;
   s32          LgMax;
   s32          XPos, YPos;
   s32          XSize, YSize;
   s32          NbDisp;             /* Number of lines to be displayed*/
   u32 SelectedItem;       /* Current selected item*/
   u32 FirstDisplayItem;   /* Index of the first displayed item*/
   tListItem    Items[LIST_MAXITEM];
   } tList;

/// @cond Internal

/* TOUCHSCREEN functions definition*/
#define TOUCHSCR_GET_POS_ID               (TOUCHSCR_ID + 0)      /* Get the position of the hit point according the LCD position*/
#define TOUCHSCR_GET_ABS_POS_ID           (TOUCHSCR_ID + 1)      /* Get the absolute position of the hit point*/
#define TOUCHSCR_IS_PRESSED_ID            (TOUCHSCR_ID + 2)      /* See if toucscreen touched or not*/
#define TOUCHSCR_GET_MODE_ID              (TOUCHSCR_ID + 3)      /* Get the running mode of the touchscreen (normal, calibration...)*/
#define TOUCHSCR_SET_SENS_ID              (TOUCHSCR_ID + 4)      /* Set the sensitivity if the touch (0 to 4095)*/
#define LIST_MANAGER_ID                   (TOUCHSCR_ID + 5)      /* Manage the current list*/
#define LIST_SET_ID                       (TOUCHSCR_ID + 6)      /* Set the new list to manage with the touchscreen*/

/* Prototypes*/
#define TOUCHSCR_GetPos()           (u16)                        (((tCircleFunc0)(Circle_API [TOUCHSCR_GET_POS_ID])) () )           /* u16 TOUCHSCR_GetPos( void );*/
#define TOUCHSCR_GetAbsPos()        (u16)                        (((tCircleFunc0)(Circle_API [TOUCHSCR_GET_ABS_POS_ID])) () )       /* u16 TOUCHSCR_GetAbsPos( void );*/
#define TOUCHSCR_IsPressed()        (bool)                       (((tCircleFunc0)(Circle_API [TOUCHSCR_IS_PRESSED_ID])) () )        /* bool TOUCHSCR_IsPressed( void )*/
#define TOUCHSCR_GetMode()          (TOUCHSCR_Mode_enum)         (((tCircleFunc0)(Circle_API [TOUCHSCR_GET_MODE_ID])) ()  )         /* TOUCHSCR_Mode_enum TOUCHSCR_GetMode( void );*/
#define TOUCHSCR_SetSensibility(a)                               (((tCircleFunc1)(Circle_API [TOUCHSCR_SET_SENS_ID])) ((u32)(a)) )  /* void TOUCHSCR_SetSensibility( u16 sens );*/
#define LIST_Manager()              (s32)                        (((tCircleFunc0)(Circle_API [LIST_MANAGER_ID])) ()  )              /* s32 LIST_Manager( void );*/
#define LIST_Set(a,b,c,d)                                        (((tCircleFunc4)(Circle_API [LIST_SET_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d))  ) /* void LIST_Set( tList *lptr, u16 posX, u16 posY, bool center );*/

/// @endcond

/*-------------------------------- AUDIO -------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum { OFF, ON} ON_OFF_enum ;
typedef enum { AUDIO_MODE, VOICE_MODE, AUDIO_CIRCULAR_MODE, VOICE_CIRCULAR_MODE}  AUDIO_DeviceMode_enum;
typedef enum { NO_SOUND, IS_PLAYING }                           AUDIO_Playback_status_enum;
typedef enum { NO_RECORD, IS_RECORDING }                        AUDIO_Recording_status_enum ;
typedef enum { LG_8_BITS, LG_16_BITS}  AUDIO_Length_enum;
typedef enum { FRQ_48KHZ ,FRQ_44KHZ ,FRQ_22KHZ ,FRQ_16KHZ , FRQ_8KHZ} AUDIO_Frequency_enum; // KJ - Changed to support more FRQ
typedef enum { MONO, STEREO}                                    AUDIO_Format_enum;
typedef enum { FULL=0,LOW_EMPTY=1,HIGH_EMPTY=2 }                AUDIO_PlaybackBuffer_Status ;
typedef enum { EMPTY=0,LOW_FULL=1,HIGH_FULL=2 }                 AUDIO_RecordBuffer_Status ;
typedef s8 sound_type;

/// @cond Internal

/* AUDIO functions definition*/
#define AUDIO_SET_MODE_ID               (AUDIO_ID + 0)     /* Set new codec mode*/
#define AUDIO_GET_MODE_ID               (AUDIO_ID + 1)     /* Get the current codec mode*/
#define AUDIO_PLAY_ID                   (AUDIO_ID + 2)     /* Issues audio samples (stored in buffer) to the audio codec via I2S*/
#define AUDIO_SPEAKER_ONOFF_ID          (AUDIO_ID + 3)     /* Set the PLS switch of the audio codec ON or OFF, in order to mute or not the loudspeaker.*/
#define AUDIO_MUTE_ONOFF_ID             (AUDIO_ID + 4)     /* Set the MUT switch of the audio codec ON or OFF*/
#define AUDIO_ISMUTE_ID                 (AUDIO_ID + 5)     /* Indicates if the audio is MUTE or not*/
#define AUDIO_INC_VOLUME_ID             (AUDIO_ID + 6)     /* Increment the volume of the loudspeaker and headphones*/
#define AUDIO_DEC_VOLUME_ID             (AUDIO_ID + 7)     /* Decrement the volume of the loudspeaker and headphones*/
#define AUDIO_PLAYBACK_GETSTATUS_ID     (AUDIO_ID + 8)     /* Get the current status of the playback mode*/
#define AUDIO_RECORD_GETSTATUS_ID       (AUDIO_ID + 9)     /* Get the current status of the record mode*/
#define AUDIO_RECORD_ID                 (AUDIO_ID + 10)    /* Store audio samples into buffer from the audio codec via I2S*/
#define AUDIO_READ_REGISTER_ID          (AUDIO_ID + 11)    /* Reads a data byte from one of STw5094A configuration registers*/
#define AUDIO_WRITE_REGISTER_ID         (AUDIO_ID + 12)    /* Send a data byte to one of STw5094A configuration registers*/

#define AUDIO_SET_BUFFER_SIZE_ID        (AUDIO2_ID + 0)    /* Adjust the size of the local buffer used in MONO mode.*/
#define AUDIO_RECORD_BUFF_GETSTATUS_ID  (AUDIO2_ID + 1)    /* Stores audio samples to the buffer from the audio codec via I2S.*/
#define AUDIO_RECORD_STOP_ID            (AUDIO2_ID + 2)    /* Stop the record by stopping the DMA transfer.*/
#define AUDIO_PLAY_BUFF_GETSTATUS_ID    (AUDIO2_ID + 3)    /* Issues audio samples (stored in buffer) to the audio codec via I2S.*/
#define AUDIO_PLAY_STOP_ID              (AUDIO2_ID + 4)    /* Stop the playback by stopping the DMA transfer.*/

/* Prototypes*/
#define AUDIO_SetMode(a,b,c,d)                                     (((tCircleFunc4)(Circle_API [AUDIO_SET_MODE_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d)))
                                                                                                                                                    /* void AUDIO_SetMode( AUDIO_DeviceMode_enum mode,  */
                                                                                                                                                    /*                     AUDIO_Playback_length_enum length,*/
                                                                                                                                                    /*                     AUDIO_Playback_frequence_enum frequence,*/
                                                                                                                                                    /*                     AUDIO_format_enum format);*/
#define AUDIO_GetMode()                   (AUDIO_DeviceMode_enum)  (((tCircleFunc0)(Circle_API [AUDIO_GET_MODE_ID])) () )                           /* AUDIO_DeviceMode_enum AUDIO_GetMode();*/
#define AUDIO_Play(a,b)                                             ((tCircleFunc2)(Circle_API[AUDIO_PLAY_ID])) ((u32) (a), (u32) (b))              /* void AUDIO_Play( sound_type * buffer, s32 size );*/
#define AUDIO_SPEAKER_OnOff(a)                                      ((tCircleFunc1)(Circle_API [AUDIO_SPEAKER_ONOFF_ID])) ((u32) (a))               /* void AUDIO_SPEAKER_OnOff(ON_OFF_enum mode);*/
#define AUDIO_MUTE_OnOff(a)                                         ((tCircleFunc1)(Circle_API [AUDIO_MUTE_ONOFF_ID]))    ((u32) (a))               /* void AUDIO_MUTE_OnOff(ON_OFF_enum mode);*/
#define AUDIO_IsMute()                                     (bool)  (((tCircleFunc0)(Circle_API [AUDIO_ISMUTE_ID])) () )                             /* bool AUDIO_IsMute(void);*/
#define AUDIO_Inc_Volume(a)                                         ((tCircleFunc1)(Circle_API [AUDIO_INC_VOLUME_ID])) ((u32) (a))                  /* void AUDIO_Inc_Volume(u8 dB);*/
#define AUDIO_Dec_Volume(a)                                         ((tCircleFunc1)(Circle_API [AUDIO_DEC_VOLUME_ID])) ((u32) (a))                  /* void AUDIO_Dec_Volume(u8 dB)*/
#define AUDIO_Playback_GetStatus()    (AUDIO_Playback_status_enum) (((tCircleFunc0)(Circle_API [AUDIO_PLAYBACK_GETSTATUS_ID])) () )                 /* AUDIO_Playback_status_enum AUDIO_Playback_GetStatus();*/
#define AUDIO_Recording_GetStatus()  (AUDIO_Recording_status_enum) (((tCircleFunc0)(Circle_API [AUDIO_RECORD_GETSTATUS_ID])) () )                   /* AUDIO_Record_status_enum AUDIO_Playback_GetStatus();*/
#define AUDIO_Record(a,b)                                           ((tCircleFunc2)(Circle_API[AUDIO_RECORD_ID])) ((u32) (a), (u32) (b))            /* void AUDIO_Record( sound_type * buffer, s32 size );*/
#define AUDIO_ReadRegister(a)                                 (u8)  ((tCircleFunc1)(Circle_API[AUDIO_READ_REGISTER_ID])) ((u32) (a))                /* u8 AUDIO_ReadRegister(u8 register_to_read);*/
#define AUDIO_WriteRegister(a,b)                                    ((tCircleFunc2)(Circle_API[AUDIO_WRITE_REGISTER_ID])) ((u32) (a), (u32) (b))    /* void AUDIO_WriteRegister(u8 register_to_write, u8 data_to_write);*/

#define AUDIO_RecordBuffer_GetStatus(a)   (AUDIO_RecordBuffer_Status)   ((tCircleFunc1)(Circle_API[AUDIO_RECORD_BUFF_GETSTATUS_ID])) ((u32) (a))    /* AUDIO_RecordBuffer_Status AUDIO_RecordBuffer_GetStatus(AUDIO_RecordBuffer_Status value);*/
#define AUDIO_Record_Stop()                                             ((tCircleFunc0)(Circle_API [AUDIO_RECORD_STOP_ID])) ()                      /* void AUDIO_Record_Stop( );*/
#define AUDIO_PlaybackBuffer_GetStatus(a) (AUDIO_PlaybackBuffer_Status) ((tCircleFunc1)(Circle_API[AUDIO_PLAY_BUFF_GETSTATUS_ID])) ((u32) (a))      /* AUDIO_PlaybackBuffer_Status AUDIO_PlaybackBuffer_GetStatus(AUDIO_PlaybackBuffer_Status value);*/
#define AUDIO_Playback_Stop()                                           ((tCircleFunc0)(Circle_API [AUDIO_PLAY_STOP_ID])) ()                        /* void AUDIO_Playback_Stop( );*/
#define AUDIO_SetLocalBufferSize(a)                                     ((tCircleFunc1)(Circle_API [AUDIO_SET_BUFFER_SIZE_ID]))    ((u32) (a))      /* void AUDIO_SetLocalBufferSize(s32 size);*/

/// @endcond


/*------------------------------ POWER / SHUTDOWN -----------------------------*/

/* Exported types ------------------------------------------------------------*/


/// @cond Internal

/* Exported defines ----------------------------------------------------------*/

/* SHUTDOWN functions definition*/
#define SHUTDOWN_ACTION_ID           (SHUTDOWN_ID + 0)     /* Switch off the power supply*/

/* Prototypes*/
#define SHUTDOWN_Action()            ((tCircleFunc0)(Circle_API [SHUTDOWN_ACTION_ID])) ()  /* void SHUTDOWN_Action( void );*/

/// @endcond


/*------------------------------ Filesystem -----------------------------------*/

/* Exported defines ----------------------------------------------------------*/
#define MAX_PATH_LENGTH 200

/*===================================================================*/
/* 32-bit error codes*/
#define FS_OK			0			// no error
#define FS_EOF			1			// end of file (not an error)
#define FS_WRITEPROT	2			// volume is write protected
#define FS_NOTFOUND	    3			// path or file not found
#define FS_PATHLEN		4			// path too long
#define FS_ALLOCNEW	    5			// must allocate new directory cluster
#define FS_ERRMISC		0xffffffff	// generic error

/*===================================================================*/
/* File access modes*/
#define FS_READ		    1			// read-only
#define FS_WRITE		2			// write-only

/*===================================================================*/
/* DOS attribute bits*/
#define FS_ATTR_READ_ONLY	        0x01
#define FS_ATTR_HIDDEN		        0x02
#define FS_ATTR_SYSTEM		        0x04
#define FS_ATTR_VOLUME_ID	        0x08
#define FS_ATTR_DIRECTORY	        0x10
#define FS_ATTR_ARCHIVE	        0x20
#define FS_ATTR_LONG_NAME	(FS_ATTR_READ_ONLY | FS_ATTR_HIDDEN | FS_ATTR_SYSTEM | FS_ATTR_VOLUME_ID)

/* Exported types ------------------------------------------------------------*/

typedef enum STORAGE_device
{
  MMCSD_SDIO,
  INTERNAL_FLASH // currently not used
}eSTORAGE_device;

/**
* @struct   _tagVOLINFO
* @brief    Volume information structure.
**/
typedef struct _tagVOLINFO 
{
	u8 unit;				// unit on which this volume resides
	u8 filesystem;			// formatted filesystem

/* These two fields aren't very useful, so support for them has been commented out to*/
/* save memory. (Note that the "system" tag is not actually used by DOS to determine*/
/* filesystem type - that decision is made entirely on the basis of how many clusters*/
/* the drive contains. DOSFS works the same way).*/
/* See tag: OEMID in dosfs.c*/
/*	u8 oemid[9];			// OEM ID ASCIIZ*/
/*	u8 system[9];			// system ID ASCIIZ*/
	u8 label[12];			/* volume label ASCIIZ*/
	u32 startsector;		/* starting sector of filesystem*/
	u8 secperclus;			/* sectors per cluster*/
	u16 reservedsecs;		/* reserved sectors*/
	u32 numsecs;			/* number of sectors in volume*/
	u32 secperfat;			/* sectors per FAT*/
	u16 rootentries;		/* number of root dir entries*/

	u32 numclusters;		/* number of clusters on drive*/

	/* The fields below are PHYSICAL SECTOR NUMBERS.*/
	u32 fat1;				/* starting sector# of FAT copy 1*/
	u32 rootdir;			/* starting sector# of root directory (FAT12/FAT16) or cluster (FAT32)*/
	u32 dataarea;			/* starting sector# of data area (cluster #2)*/
} VOLINFO, *PVOLINFO;



/**
* @struct   _tagDIRENT
* @brief    Directory entry structure.
*	
* Note: if name[0] == 0xe5, this is a free dir entry
*	      if name[0] == 0x00, this is a free entry and all subsequent entries are free
*		  if name[0] == 0x05, the first character of the name is 0xe5 [a kanji nicety]
*
*	Date format: bit 0-4  = day of month (1-31)
*	             bit 5-8  = month, 1=Jan..12=Dec
*				 bit 9-15 =	count of years since 1980 (0-127)
*	Time format: bit 0-4  = 2-second count, (0-29)
*	             bit 5-10 = minutes (0-59)
*				 bit 11-15= hours (0-23)
**/

typedef struct _tagDIRENT {
	u8 name[11];			/* filename*/
	u8 attr;				/* attributes (see ATTR_* constant definitions)*/
	u8 reserved;			/* reserved, must be 0*/
	u8 crttimetenth;		/* create time, 10ths of a second (0-199 are valid)*/
	u8 crttime_l;			/* creation time low byte*/
	u8 crttime_h;			/* creation time high byte*/
	u8 crtdate_l;			/* creation date low byte*/
	u8 crtdate_h;			/* creation date high byte*/
	u8 lstaccdate_l;		/* last access date low byte*/
	u8 lstaccdate_h;		/* last access date high byte*/
	u8 startclus_h_l;		/* high word of first cluster, low byte (FAT32)*/
	u8 startclus_h_h;		/* high word of first cluster, high byte (FAT32)*/
	u8 wrttime_l;			/* last write time low byte*/
	u8 wrttime_h;			/* last write time high byte*/
	u8 wrtdate_l;			/* last write date low byte*/
	u8 wrtdate_h;			/* last write date high byte*/
	u8 startclus_l_l;		/* low word of first cluster, low byte*/
	u8 startclus_l_h;		/* low word of first cluster, high byte*/
	u8 filesize_0;			/* file size, low byte*/
	u8 filesize_1;			/**/
	u8 filesize_2;			/**/
	u8 filesize_3;			/* file size, high byte*/
} DIRENT, *PDIRENT;


/**
* @struct   _tagDIRINFO
* @brief    Directory search structure.
**/
typedef struct _tagDIRINFO {
	u32 currentcluster;	    /* current cluster in dir*/
	u8 currentsector;		/* current sector in cluster*/
	u8 currententry;		/* current dir entry in sector*/
	u8 *scratch;			/* ptr to user-supplied scratch buffer (one sector)*/
	u8 flags;				/* internal DOSFS flags*/
} DIRINFO, *PDIRINFO;

/**
* @struct   _tagFILEINFO
* @brief    File handle structure.
**/
typedef struct _tagFILEINFO {
	PVOLINFO volinfo;		/* VOLINFO used to open this file*/
	u32 dirsector;			/* physical sector containing dir entry of this file*/
	u8 diroffset;			/* # of this entry within the dir sector*/
	u8 mode;				/* mode in which this file was opened*/
	u32 firstcluster;		/* first cluster of file*/
	u32 filelen;			/* byte length of file*/

	u32 cluster;			/* current cluster*/
	u32 pointer;			/* current (BYTE) pointer*/
} FILEINFO, *PFILEINFO;


/// @cond Internal

/* FILE SYSTEM functions definition*/

#define FS_MOUNT_ID 			  (FS_ID+0)  /* Mount device: perform necessary initialization*/
#define FS_UNMOUNT_ID 			  (FS_ID+1)  /* Unmount device: perform necessary deinitialization*/
#define FS_OPENFILE_ID 			  (FS_ID+2)  /* Opens file / directory from specified path*/
#define FS_READFILE_ID 			  (FS_ID+3)  /* Reads from specified file*/
#define FS_WRITEFILE_ID 		  (FS_ID+4)  /* writs data to specified file*/
#define FS_CLOSE_ID 			  (FS_ID+5)  /* closes file*/
#define FS_SEEK_ID 				  (FS_ID+6)  /* seek in file*/
#define FS_DELETE_ID 			  (FS_ID+7)  /* delete file or directory*/
#define FS_GET_NEXT_ENTRY_ID 	  (FS_ID+8)  /* Reads next entry from dir*/
#define FS_OPEN_DIRECTORY_ID 	  (FS_ID+9)  /* Open directory*/
#define FS_GET_VOLUME_INFO_ID 	  (FS_ID+10) /* get volume info*/
#define FS_EXPLORER_INI_ID 	      (FS_ID+11) /* Initialize the explorer list*/
#define FS_EXPLORER_ID      	  (FS_ID+12) /* Manage the navigation into the SDCard*/
#define FS_GET_SD_CURRENT_PATH_ID (FS_ID+13) /* Get the current path of the SDCard*/
#define FS_GET_SD_VOLINFO_ID	  (FS_ID+14) /* Get the current path of the SDCard*/
#define FS_GET_PATH_FILTER_ID	  (FS_ID+15) /* Get the current path filter*/
#define FS_SET_PATH_FILTER_ID	  (FS_ID+16) /* Set the current path filer*/

/* Prototypes*/
#define FS_Mount(a)								(u32) ((tCircleFunc1)(Circle_API [FS_MOUNT_ID])) ((u32) (a))									/* u32 FS_Mount(enum STORAGE_device device);*/
#define FS_Unmount(a)							(u32) ((tCircleFunc1)(Circle_API [FS_UNMOUNT_ID])) ((u32) (a))									/* u32 FS_Unmount(enum STORAGE_device device);*/
#define FS_OpenFile(a,b,c,d)					(u32) (((tCircleFunc4)(Circle_API [FS_OPENFILE_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d))  )	/* u32 FS_OpenFile(PVOLINFO volinfo, u8 *path, u8 mode, PFILEINFO fileinfo);*/
#define FS_ReadFile(a,b,c,d)					(u32) (((tCircleFunc4)(Circle_API [FS_READFILE_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d))  )	/* u32 FS_ReadFile(PFILEINFO fileinfo,  u8 *buffer, u32 *successcount, u32 len);*/
#define FS_WriteFile(a,b,c,d)					(u32) (((tCircleFunc4)(Circle_API [FS_WRITEFILE_ID])) ((u32)(a),(u32)(b),(u32)(c),(u32)(d))  )	/* u32 FS_WriteFile(PFILEINFO fileinfo,  u8 *buffer, u32 *successcount, u32 len);*/
#define FS_Close(a)								(u32) ((tCircleFunc1)(Circle_API [FS_CLOSE_ID])) ((u32) (a))									/* u32 FS_Close(PFILEINFO fileinfo);*/
#define FS_Seek(a,b)								  ((tCircleFunc2)(Circle_API[FS_SEEK_ID])) ((u32) (a), (u32) (b))							/* void FS_Seek(PFILEINFO fileinfo, u32 offset);*/
#define FS_Delete(a,b)							(u32) ((tCircleFunc2)(Circle_API[FS_DELETE_ID])) ((u32) (a), (u32) (b))							/* u32 FS_Delete(PVOLINFO volinfo, u8 *path);*/
#define FS_GetNextEntry(a,b,c)					(u32) (((tCircleFunc3)(Circle_API [FS_GET_NEXT_ENTRY_ID])) ((u32)(a),(u32)(b),(u32)(c) ))		/* u32 FS_GetNextEntry(PVOLINFO volinfo, PDIRINFO dirinfo, PDIRENT dirent);*/
#define FS_OpenDirectory(a,b,c)					(u32) (((tCircleFunc3)(Circle_API [FS_OPEN_DIRECTORY_ID])) ((u32)(a),(u32)(b),(u32)(c) ))		/* u32 FS_OpenDirectory(PVOLINFO volinfo,u8 *dirname, PDIRINFO dirinfo);	*/
#define FS_GetVolumeInfo(a,b,c)					(u32) (((tCircleFunc3)(Circle_API [FS_GET_VOLUME_INFO_ID])) ((u32)(a),(u32)(b),(u32)(c) ))		/* u32 FS_GetVolumeInfo(u8 unit,  u32 startsector, PVOLINFO volinfo);	*/
#define FS_Explorer_Ini()                       (u32) (((tCircleFunc0)(Circle_API [FS_EXPLORER_INI_ID])) () )                                   /* enum MENU_code FS_Explorer_Ini ( void );*/
#define FS_Explorer()                           (u32) (((tCircleFunc0)(Circle_API [FS_EXPLORER_ID])) () )                                       /* s32 FS_Explorer (void);*/
#define FS_GetSDCardCurrentPath()               (u32) (((tCircleFunc0)(Circle_API [FS_GET_SD_CURRENT_PATH_ID])) () )                            /* u8* FS_GetSDCardCurrentPath (void);*/
#define FS_GetSDCardVolInfo()                   (u32) (((tCircleFunc0)(Circle_API [FS_GET_SD_VOLINFO_ID])) () )                                 /* VOLINFO* FS_GetSDCardVolInfo (void);*/
#define FS_GetPathFilter()                      (u32) (((tCircleFunc0)(Circle_API [FS_GET_PATH_FILTER_ID])) () )                                /* u8* FS_GetPathFilter ( void );*/
#define FS_SetPathFilter(a)	   				          ((tCircleFunc1)(Circle_API [FS_SET_PATH_FILTER_ID])) ((u32) (a))						    /* void FS_SetPathFilter ( u8* filter );*/

/// @endcond

//--------------------------------- Application -------------------------------
typedef void  (*tAppPtrMgr) ( s32 , s32 );

	
#endif /*__CIRCLE_API_H */
