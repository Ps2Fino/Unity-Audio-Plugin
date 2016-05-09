//audioplugin.cpp
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


// Dependency and System headers
#include <Juce-Audio/JuceHeader.h>

// Bundle headers
#include "Common.h"
#include "audioPlugin.h"
#include "jucePlayer.h"
#include "soundbank.h"
#include "spatializer.h"

using AudioPlayer = std::unique_ptr<uap::JucePlayer>;
using SoundBank = std::shared_ptr<uap::SoundBank>;
using Spatializer = std::shared_ptr<uap::Spatializer<uap::SPATIALIZER_CALLBACK_IMPL> >;

// Here's our system objects
static int RES = 0; // This is the shared result variable to indicate a successful interface call; 1 for successful, anything else for unsuccessful
static bool isInitiated = false;
static AudioPlayer audioPlayer;
static SoundBank soundBank;
static Spatializer spatializer;
static char buffer[200];

/**
 * I should probably rethink this now
 * as it might cause a race condition at some point
 */
void copyStringFromManagedCaller(char *managedString)
{
    strcpy(buffer, managedString);
}

void initEnv()
{
    if (!isInitiated)
    {
        audioPlayer = AudioPlayer(new uap::JucePlayer);
        soundBank = SoundBank(new uap::SoundBank);
        spatializer = Spatializer(new uap::Spatializer<uap::SPATIALIZER_CALLBACK_IMPL>);
        
        // Init these guys if necessary
        spatializer->init();
        audioPlayer->setSpatializer(spatializer);
        audioPlayer->setSoundBank(soundBank);
        audioPlayer->init();
        RES = 1;
        isInitiated = true;
    }
}

void initEnvWithHRIRS(char *hrirFilePath)
{
    if (!isInitiated)
    {
        printf("I have called the function %s\n", __func__);
        copyStringFromManagedCaller(hrirFilePath);
        std::string fileNameStr(&buffer[0]);
        
        audioPlayer = AudioPlayer(new uap::JucePlayer);
        soundBank = SoundBank(new uap::SoundBank);
        spatializer = Spatializer(new uap::Spatializer<uap::SPATIALIZER_CALLBACK_IMPL>(fileNameStr));
        
        // Init these guys if necessary
        spatializer->init();
        audioPlayer->setSpatializer(spatializer);
        audioPlayer->setSoundBank(soundBank);
        audioPlayer->init();
        RES = 1;
        isInitiated = true;
    }
}

int checkStatus()
{
    return RES;
}

void destroyEnv()
{
    if (isInitiated)
    {
//        audioPlayer = NULL;
//        spatializer = NULL;
//        soundBank = NULL;
        printf("I've called function %s\n", __func__);
        audioPlayer.reset();
        spatializer.reset();
        soundBank.reset();
        RES = 1;
        isInitiated = false;
    }
}

void startJuce()
{
    if (isInitiated)
    {
        if (audioPlayer->start())
            RES = 1;
        else
            RES = 0;
    }
}

void stopJuce()
{
    if (isInitiated)
    {
        if (audioPlayer->stop())
            RES = 1;
        else
            RES = 0;
    }
}

/*
 * Methods for adding/removing sounds
 */

void addSoundToRendererBank(char *filename)
{
    if (isInitiated)
    {
        
    //    printf("Calling the addSoundToRendererBank function with %s as a filename\n", filename);
        copyStringFromManagedCaller(filename);
        std::string fileNameStr(&buffer[0]);
        
        int soundLoc = soundBank->getIDForName(fileNameStr);
        if (soundLoc > 0) { // Sound is already in the bank
            // If its just been deactivated, reactivate it
            std::string key(&buffer[0]);
            soundBank->activateSoundSource(key);
        }
        else
        {
            printf("Function %s, line %d: About to add the sound source %s\n",
                   __func__, __LINE__, fileNameStr.c_str());
            int sourceID = spatializer->addSoundSource(fileNameStr);
    //        printf("Added a sound with ID: %d\n", sourceID);
            soundBank->addAndActivate(fileNameStr, sourceID);
        }
        
    }
}

void addSoundToRendererBankAtPosition(char *filename, float xPos, float zPos)
{
    if (isInitiated)
    {
        
        copyStringFromManagedCaller(filename);
        std::string fileNameStr(&buffer[0]);
        
        int soundLoc = soundBank->getIDForName(fileNameStr);
        if (soundLoc > 0) { // Sound is already in the bank
            soundBank->activateSoundSource(fileNameStr);
        }
        else
        {
            int sourceID = spatializer->addSoundSource(fileNameStr);
            spatializer->moveSoundSource(sourceID, xPos, zPos, 0.0f);
            soundBank->addAndActivate(fileNameStr, sourceID); // Add the reference to the sound bank
        }
        
        soundBank->updatePosition(xPos, zPos); // Update the position inside the sound bank
    
    }
}

bool activateSoundWithName(char *filename)
{
    if (isInitiated)
    {
        
        copyStringFromManagedCaller(filename);
        std::string fileNameStr(&buffer[0]);
        
        printf("Calling function %s in file %s at line %d\n", __func__, __FILE__, __LINE__);
        soundBank->activateSoundSource(fileNameStr);
        return true;
        
    }
    else
        return false;
}

bool resetSoundWithName(char *filename)
{
    if (isInitiated)
    {
        
        copyStringFromManagedCaller(filename);
        std::string fileNameStr(&buffer[0]);
        
        printf("Calling function %s in file %s at line %d\n", __func__, __FILE__, __LINE__);
        soundBank->resetSoundInBank(fileNameStr);
        return true;
        
    }
    else
        return false;
}

bool deactivateSoundInRendererBank(char *filename)
{
    if (isInitiated)
    {
        copyStringFromManagedCaller(filename);
        std::string fileNameStr(&buffer[0]);
        
        printf("Calling function %s in file %s at line %d\n", __func__, __FILE__, __LINE__);
        soundBank->deactivateSoundInBank(fileNameStr);
        return true;
    }
    else
        return false;
}

/*
 * Methods for manipulating the sounds in the renderer bank
 */
bool setSoundWithNameToPosition(char *filename, float xPos, float zPos)
{
    if (isInitiated)
    {
        copyStringFromManagedCaller(filename);
        std::string fileNameStr(&buffer[0]);
        
        int sourceID = soundBank->getIDForName(fileNameStr);
        spatializer->moveSoundSource(sourceID, xPos, zPos, 0.0f);
        return true;
    }
    else
        return false;
}

/*
 * Methods for manipulating the listener position
 */
bool moveListenerToPosition(float xPos, float zPos)
{
    if (isInitiated)
    {
        spatializer->setListenerPosition(xPos, zPos, 0.0f);
        soundBank->updatePosition(xPos, zPos);
        return true; // TODO : Return a sensible value here
    }
    else
        return false;
}

bool orientListenerInDirection(float angle)
{
    if (isInitiated)
    {
        spatializer->setListenerOrientation(angle);
        return true;
    }
    else
        return false;
}













