//****************************************************************************//
// File:          compiler.h                                                  //
// Description:   Compiler independent data types for embedded solutions      //
// Author:        Uwe Koppe                                                   //
// e-mail:        koppe@microcontrol.net                                      //
//                                                                            //
// Copyright (C) MicroControl GmbH & Co. KG                                   //
// 53842 Troisdorf - Germany                                                  //
// www.microcontrol.net                                                       //
//                                                                            //
//----------------------------------------------------------------------------//
// Redistribution and use in source and binary forms, with or without         //
// modification, are permitted provided that the following conditions         //
// are met:                                                                   //
// 1. Redistributions of source code must retain the above copyright          //
//    notice, this list of conditions, the following disclaimer and           //
//    the referenced file 'COPYING'.                                          //
// 2. Redistributions in binary form must reproduce the above copyright       //
//    notice, this list of conditions and the following disclaimer in the     //
//    documentation and/or other materials provided with the distribution.    //
// 3. Neither the name of MicroControl nor the names of its contributors      //
//    may be used to endorse or promote products derived from this software   //
//    without specific prior written permission.                              //
//                                                                            //
// Provided that this notice is retained in full, this software may be        //
// distributed under the terms of the GNU Lesser General Public License       //
// ("LGPL") version 3 as distributed in the 'COPYING' file.                   //
//                                                                            //
//----------------------------------------------------------------------------//
//                                                                            //
// Date        History                                                        //
// ----------  -------------------------------------------------------------- //
// 04.12.1998  Initial version                                                //
// 29.04.1999  New data type definitions, Machine dependent definitions       //
// 07.09.2000  New symbol for const/code                                      //
// 18.12.2008  Use 'typedef' instead of '#define', introduce MC_COMPILER      //
//                                                                            //
//****************************************************************************//


#ifndef  _COMPILER_H_
#define  _COMPILER_H_


//------------------------------------------------------------------------------
// SVN  $Date: 2009-02-28 18:45:47 +0100 (Sa, 28. Feb 2009) $
// SVN  $Rev: 150 $ --- $Author: microcontrol $
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/*!
** \file    compiler.h
** \brief   Definition of compiler independent data types
**
** Due to different implementations of data types in the world of C compilers,
** the following data types are used for CANpie API:
** \li _BIT :   Boolean value, True or False
** \li _U08 :   1 Byte value, value range 0 .. 28 - 1 (0 .. 255)
** \li _S08 :   1 Byte value, value range -27 .. 27 - 1 (-128 .. 127)
** \li _U16 :   2 Byte value, value range 0 .. 216 - 1 (0 .. 65535)
** \li _S16 :   2 Byte value, value range -215 .. 215 - 1
** \li _U32 :   4 Byte value, value range 0 .. 232 - 1
** \li _S32 :   4 Byte value, value range -231 .. 231 - 1
**
** The compiler type/version is evaluated via its internal predefined
** symbols. If your compiler is not (yet) supported, you will get the
** following error message upon compilation:
** <p>
** <b>Data types are not defined! Please check compiler definition.</b>
** <p>
*/


//#ifndef  FALSE
//#define  FALSE  0
//#endif

//#ifndef  TRUE
//#define  TRUE   1
//#endif



//----------------------------------------------------------------------------//
// Target Machine / Compiler dependant definitions                            //
//                                                                            //
//----------------------------------------------------------------------------//



/*---------------------------------------------------------
** Microsoft Visual C/C++
**
*/
#ifdef _MSC_VER

#define  MC_COMPILER          1
#define  _CON                 const

typedef  unsigned char        _BIT;
typedef  unsigned char        _U08;
typedef  signed char          _S08;
typedef  unsigned short       _U16;
typedef  short                _S16;
typedef  unsigned long        _U32;
typedef  long                 _S32;

#endif
/* End of definition: _MSC_VER
**-------------------------------------------------------*/

/*---------------------------------------------------------
** CVI, National Instruments
**
*/
#ifdef _CVI_

typedef  unsigned char        _BIT;
typedef  unsigned char        _U08;
typedef  signed char          _S08;
typedef  unsigned short       _U16;
typedef  short                _S16;
typedef  unsigned long        _U32;
typedef  long                 _S32;

#endif
/* End of definition: _CVI_
**-------------------------------------------------------*/

/*---------------------------------------------------------
** LINUX, gcc
**
*/
#ifdef __linux__

#ifdef __KERNEL__
#include <linux/types.h>   // data types uint8_t ... uint32_t, kernel space
#else
#include <stdint.h>        // data types uint8_t ... uint32_t, user space
#endif

#define  MC_COMPILER          1
#define  _CON                 const

typedef  uint8_t              _BIT;
typedef  uint8_t              _U08;
typedef  int8_t               _S08;
typedef  uint16_t             _U16;
typedef  int16_t              _S16;
typedef  uint32_t             _U32;
typedef  int32_t              _S32;
typedef  uint64_t             _U64;
typedef  int64_t              _S64;
#endif
/* End of definition: __linux__
**-------------------------------------------------------*/


/*---------------------------------------------------------
** Keil C for 8051 cores
**
*/
#ifdef __C51__

#define  MC_COMPILER          1
#define  _CON

typedef  unsigned char        _BIT;
typedef  unsigned char        _U08;
typedef  signed char          _S08;
typedef  unsigned short       _U16;
typedef  short                _S16;
typedef  unsigned long        _U32;
typedef  long                 _S32;

#endif
/* End of definition: __C51__
**-------------------------------------------------------*/


/*---------------------------------------------------------
** Keil C for C16x cores
**
*/
#ifdef __C166__

#define  MC_COMPILER          1
#define  _CON

typedef  unsigned char        _BIT;
typedef  unsigned char        _U08;
typedef  signed char          _S08;
typedef  unsigned short       _U16;
typedef  short                _S16;
typedef  unsigned long        _U32;
typedef  long                 _S32;

#endif
/* End of definition: __C51__
**-------------------------------------------------------*/


/*---------------------------------------------------------
** Fujitsu C Compiler for 16LX series
**
*/
#ifdef __COMPILER_FCC907__

#define  MC_COMPILER          1
#define  _CON                 const

typedef  unsigned char        _BIT;
typedef  unsigned char        _U08;
typedef  signed char          _S08;
typedef  unsigned short       _U16;
typedef  short                _S16;
typedef  unsigned long        _U32;
typedef  long                 _S32;

#endif
/* End of definition: __COMPILER_FCC907S__
**-------------------------------------------------------*/


/*---------------------------------------------------------
** Imagecraft C Compiler for Atmel AVR series
**
*/
#ifdef _AVR

#define  MC_COMPILER          1
#define  _CON

typedef  unsigned char        _BIT;
typedef  unsigned char        _U08;
typedef  signed char          _S08;
typedef  unsigned short       _U16;
typedef  short                _S16;
typedef  unsigned long        _U32;
typedef  long                 _S32;

#endif
/* End of definition: _AVR
**-------------------------------------------------------*/


/*---------------------------------------------------------
** GNU C Compiler for Atmel AVR series
**
*/
#ifdef AVR

#define  MC_COMPILER          1
#define  _CON

typedef  unsigned char        _BIT;
typedef  unsigned char        _U08;
typedef  signed char          _S08;
typedef  unsigned short       _U16;
typedef  short                _S16;
typedef  unsigned long        _U32;
typedef  long                 _S32;

#endif
/* End of definition: AVR
**-------------------------------------------------------*/


/*---------------------------------------------------------
** IAR ARM compiler
**
*/
#ifdef __ICCARM__

#define  MC_COMPILER          1
#define  _CON                 const

typedef  unsigned char        _BIT;
typedef  unsigned char        _U08;
typedef  signed char          _S08;
typedef  unsigned short       _U16;
typedef  short                _S16;
typedef  unsigned long        _U32;
typedef  long                 _S32;

#endif
/* End of definition: __ICCARM__
**-------------------------------------------------------*/


/*---------------------------------------------------------
** Hi-Tech PICC18
**
*/
#ifdef HI_TECH_C

#define  MC_COMPILER          1
#define  _CON                 const

typedef  unsigned char        _BIT;
typedef  unsigned char        _U08;
typedef  signed char          _S08;
typedef  unsigned short       _U16;
typedef  short                _S16;
typedef  unsigned long        _U32;
typedef  long                 _S32;

#endif
/* End of definition: _HI_TECH_C
**-------------------------------------------------------*/



/*---------------------------------------------------------
** Microchip PIC18 C Compiler
**
*/
#ifdef __18CXX

#define  MC_COMPILER          1
#define  _CON                 const

typedef  unsigned char        _BIT;
typedef  unsigned char        _U08;
typedef  signed char          _S08;
typedef  unsigned short       _U16;
typedef  short                _S16;
typedef  unsigned long        _U32;
typedef  long                 _S32;

#endif
/* End of definition: _HI_TECH_C
**-------------------------------------------------------*/


/*---------------------------------------------------------
** Metrowerks C Compiler for XGATE
**
*/
#ifdef __MWERKS__

#define  MC_COMPILER          1
#define  _CON                 const

typedef  unsigned char        _BIT;
typedef  unsigned char        _U08;
typedef  signed char          _S08;
typedef  unsigned short       _U16;
typedef  short                _S16;
typedef  unsigned long        _U32;
typedef  long                 _S32;

#endif
/* End of definition: __MWERKS__
**-------------------------------------------------------*/


/*---------------------------------------------------------
** GNU C for ARM controller
**
*/
#ifdef __GNUC__
#if __arm__ > 0

#include <stdint.h>           // data types uint8_t ... uint64_t

#define  MC_COMPILER          1
#define  _CON                 const

typedef  uint8_t              _BIT;
typedef  uint8_t              _U08;
typedef  int8_t               _S08;
typedef  uint16_t             _U16;
typedef  int16_t              _S16;
typedef  uint32_t             _U32;
typedef  int32_t              _S32;
typedef  uint64_t             _U64;
typedef  int64_t              _S64;

#endif
#endif
/* End of definition: __arm__
**-------------------------------------------------------*/


/*---------------------------------------------------------
** GNU C for ColdFire controller
**
*/
#ifdef __GNUC__
#if __mc68000__ > 0

#include <stdint.h>           // data types uint8_t ... uint64_t

#define  MC_COMPILER          1
#define  _CON                 const

typedef  uint8_t              _BIT;
typedef  uint8_t              _U08;
typedef  int8_t               _S08;
typedef  uint16_t             _U16;
typedef  int16_t              _S16;
typedef  uint32_t             _U32;
typedef  int32_t              _S32;
typedef  uint64_t             _U64;
typedef  int64_t              _S64;

#endif
#endif
/* End of definition: __mc68000__
**-------------------------------------------------------*/


/*---------------------------------------------------------
** TI Code Composer
**
*/
#ifdef __TMS320C2000__

#define  MC_COMPILER          1
#define  _CON                 const

typedef  unsigned char        _BIT;
typedef  unsigned char        _U08;
typedef  signed char          _S08;
typedef  unsigned short       _U16;
typedef  short                _S16;
typedef  unsigned long        _U32;
typedef  long                 _S32;

#endif
/* End of definition:  __TMS320C2000__
**-------------------------------------------------------*/


#ifndef  MC_COMPILER
#error   Data types are not defined! Please check compiler definition.
#endif

#endif   /* _COMPILER_H_ */
