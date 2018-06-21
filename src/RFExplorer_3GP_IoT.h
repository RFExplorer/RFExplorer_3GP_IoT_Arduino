//============================================================================
//RF Explorer 3G+ IoT for Arduino - A Spectrum Analyzer for everyone!
//Copyright � 2010-18 Ariel Rocholl, www.rf-explorer.com
//
//Contributed by:
//       Manuel Ballesteros
//
//This application is free software; you can redistribute it and/or
//modify it under the terms of the GNU Lesser General Public
//License as published by the Free Software Foundation; either
//version 3.0 of the License, or (at your option) any later version.
//
//This software is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//General Public License for more details.
//
//You should have received a copy of the GNU General Public
//License along with this library; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//=============================================================================

#include "Arduino.h"


#ifndef RFEXPLORER_3GP_IOT_H_
#define RFEXPLORER_3GP_IOT_H_

//------------------------- Definitions & Declarations-------------------------

//Definition of input stage values
enum eInputStage
{
	Direct = 0,
	Attenuator_30dB,
	LNA_25dB
};

#define MONITOR_SERIAL

#define _HEADER_LIB_3GP  "---RFExplorer 3G+ IoT Arduino---"
#define _VERSION_LIB_3GP "-------Library v1.0.1806.1------"

//IOT RF Explorer is only supported by Arduino DUE  
//and Seeeduino UNO (currently version 4.2) platforms, 
//These platforms operate at 3.3V. 
//Any other plate is incompatible because voltage levels that handles 5V.


#if defined(_SAM3XA_)  && defined(__AVR_ATmega328P__)
    #error REVIEW COMPILATION DEFINED BOARD
#endif

#if !defined(_SAM3XA_)  && !defined(__AVR_ATmega328P__)
    #error REVIEW COMPILATION DEFINED BOARD
#endif


#ifdef MONITOR_SERIAL
    #if defined (_SAM3XA_)
        #define m_objMonitorSerial Serial
    #else
        #define m_objMonitorSerial m_objDebugSP
    #endif
#endif

#endif //End of RFEXPLORER_3GP_IOT_H_

//------------------------- Includes-------------------------------------------
#include "CommonValues.h"
#include "RFEUtilities.h"
#include "RFEConfiguration.h"
#include "RFESweepData.h"

#if !defined (_SAM3XA_)
#include <SoftwareSerial.h>
#endif

#include "ByteBuffer_RFE.h"
#include "RFExplorer_3GP_IoT_class.h"
