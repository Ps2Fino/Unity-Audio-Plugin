//Common.h
//Copyright (C) 2016  Daniel J. Finnegan
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef Unity_Audio_Plugin_Common_h
#define Unity_Audio_Plugin_Common_h

/* Some test definition here */
#define DEFINED_BUT_NO_VALUE
#define DEFINED_INT 3
#define DEFINED_STR "ABC"

/* definition to expand macro then apply to pragma message */
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

/* --------------------------------------------------------------------------------
 * ------- Everything in here is important. DO NOT DELETE! ------------------------
 * -------------------------------------------------------------------------------- */

#ifndef NECESSARY_DEFINITIONS
#define NECESSARY_DEFINITIONS

// Handy debug macro
//#define DEBUG_PRINT_CMD(x) \
//#ifdef DEBUG \
//    #x \
//#endif

#pragma message "HRIR_FILE_NAME is " HRIR_FILE_NAME

//#define DEBUG_CALLBACK // Use this to toggle the debug messages in the juceplayer callback method
#define SAMPLE_RATE 44100
#define MAX_NUM_SSR_INPUTS 20 // This is the maximum number of inputs that will be allowed as input to the SSR
#define MAX_NUM_SOUND_SOURCES 50 // Max number of permissable sound sources (this should be properly determined via stress testing)
#define BLOCK_SIZE 512

#endif // NECESSARY_DEFINITIONS

/* --------------------------------------------------------------------------------
 * --------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------- */

// Custom implementation can go here
//#ifndef AUDIO_FILE_HANDLER
//#define AUDIO_FILE_HANDLER DefSoundfileHandler
//#include "defaultSoundfileHandler.h"
//#endif
//
//// Implementation for the spatializer callback
//#ifndef SPATIALIZER_CALLBACK_IMPL
//#define SPATIALIZER_CALLBACK_IMPL DefSpatializer
//#include "defaultSpatializer.h"
//#endif

/**
 * This distance defines the threshold
 * for activation in the engine.
 * Any sounds further away than this will be turned off
 */
#ifndef MINIMUM_AUDIBLE_DISTANCE
#define MINIMUM_AUDIBLE_DISTANCE 10
#endif


#endif
