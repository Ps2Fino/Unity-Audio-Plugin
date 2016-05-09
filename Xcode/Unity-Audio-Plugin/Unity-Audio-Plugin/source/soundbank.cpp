//soundbank.cpp
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

#include "Common.h"
#include "soundbank.h"
#include <utility>
#include <boost/foreach.hpp>

#define foreach_ BOOST_FOREACH

#include <iostream>
using namespace std;

namespace uap
{

    SoundBank::SoundBank()
    {
//        sourceIDMap = new SoundSourceMap();
        soundSources.reserve(MAX_NUM_SOUND_SOURCES); // Lets go with 50 and see how we get on...
        listenerYPos = listenerXPos = 0.0f;
        cachedActiveCount = 0;
        soundCount = 0;
    }
        
    SoundBank::~SoundBank()
    {
//        sourceIDMap->clear();
//        delete sourceIDMap;
        
        currentBankInputs.clear();
        sourceIDMap.clear();
        soundSources.clear();
//        currentBankInputs.clear();
    }
        
    bool SoundBank::addSoundToBank(std::string filePath, int sourceID)
    {
//        printf("Function %s, line %d: filePath = %s, sourceID = %d\n",
//               __func__, __LINE__, filePath.c_str(), sourceID);
        
        // TODO : Do some actual memory checking here rather than assuming we have space for 50 wav audio files!!
        if (soundCount >= MAX_NUM_SOUND_SOURCES)
            return false; // We've reached our quota
        
        // Update the map
        std::string fpStr(filePath);
        sourceIDMap.insert(std::make_pair(fpStr, soundCount));
        
        // Actually add the sound to the vector and increment
        // our total counter
        soundSources.push_back(SoundSource(fpStr, sourceID));
        currentBankInputs.push_back(soundSources.back().getData());
        soundCount++;
        
//        printf("Function %s, line %d: Sound %s added successfully!\n",
//               __func__, __LINE__, filePath.c_str());
    
        return true;
    }
    
    bool SoundBank::addAndActivate(std::string filePath, int sourceID)
    {
        bool result = addSoundToBank(filePath, sourceID);
        if (result) {
            std::string key(filePath);
            activateSoundSource(key);
            return true;
        }
        else
            return false;
    }
    
    /* This method returns -1 if no sound source exists with the filePath as a key */
    int SoundBank::getIDForName(std::string filePath)
    {
//        printf("Function %s, line %d: filepath = %s\n", __func__, __LINE__, filePath.c_str());
        int location;
        SoundSourceMapIterator it = sourceIDMap.find(filePath);
        if (it == sourceIDMap.end())
            location = -1;
        else
            location = it->second;
        
//        printf("The location returned from the sourceID map: %d\n", location);
        if (location < 0) {
            return -1;
        }
        
        SoundSource *source = &soundSources[location];
        return source->getID();
    }
    
    const char * SoundBank::getNameForID(int ID)
    {
        SoundSourceMapIterator it;
        for (it = sourceIDMap.begin(); it != sourceIDMap.end(); ++it)
        {
            SoundSource *source = &soundSources[it->second];
            if (source->getID() == ID)
                break;
        }
        
        // Return the empty string for an invalid ID
        if (it != sourceIDMap.end())
            return (it->first).c_str();
        else
            return nullptr;
    }
    
    SoundSource * const SoundBank::getSoundSourceForName(std::string filePath)
    {
        int location;
        
        SoundSourceMapIterator it = sourceIDMap.find(filePath);
        if (it == sourceIDMap.end())
            location = -1;
        else
            location = it->second;
        
        if (location < 0)
            return nullptr;
        else
            return &soundSources[location];
    }
    
    bool SoundBank::setSoundPositionForName(std::string filePath, float xPos, float yPos)
    {
        int location;
        
        SoundSourceMapIterator it = sourceIDMap.find(filePath);
        if (it == sourceIDMap.end())
            location = -1;
        else
            location = it->second;
        
        if (location < 0) {
            return false;
        }
        
        SoundSource *source = &soundSources[location];
        source->setXPos(xPos);
        source->setYPos(yPos);
        return true;
    }
    
    void SoundBank::activateSoundSource(std::string key)
    {
        
//        printf("Function %s, line %d: Activating %s\n",
//               __func__, __LINE__, key.c_str());
        
        int location;
        
        SoundSourceMapIterator it = sourceIDMap.find(key);
        if (it == sourceIDMap.end())
            location = -1;
        else
            location = it->second;
        
        if (location < 0)
            return;
        
        SoundSource *source = &soundSources[location];
        source->activate();
        
//        printf("Function %s, line %d: Successfully activating %s!\n",
//               __func__, __LINE__, key.c_str());
    }
    
    void SoundBank::updatePosition(float x, float y)
    {
//        // This method updates the sounds.
//        // It activates sounds who are now in the
//        // vicinity of the listener and deactivates
//        // those that are no longer nearby
//        listenerXPos = x;
//        listenerYPos = y;
//        
//        // Just iterate through all the sound file, activating those within range
//        // and deactivating those that are not
//        SoundSourceMapIterator it = sourceIDMap.begin();
//        for (it = sourceIDMap.begin(); it != sourceIDMap.end(); ++it)
//        {
//            // I have to take a reference here due to the semantics of the class
//            // As I have declared it moveable and left out the copy constructor,
//            // I can't make calls like SoundSource source = soundSources[it->second]
//            // as this would actually perform a copy of the source object that would
//            // have function level scope.
//            // Taking a reference is ok though ;-)
//            SoundSource *source = &soundSources[it->second];
//            
//            float sourceXPos = source->getXPos();
//            float sourceYPos = source->getYPos();
//            float dist = sqrt(powf(listenerXPos - sourceXPos, 2)
//                              + powf(listenerYPos - sourceYPos, 2));
//            if (dist < MINIMUM_AUDIBLE_DISTANCE)
//                source->activate();
//            else
//                source->deactivate();
//        }
    }
    
    /*
     * This just updates all the sounds currently in the bank
     * This should be like, I don't know,.... vectorized maybe Dan....!?
     */
    void SoundBank::updateSounds()
    {
        // TODO: Vectorize this
//        SoundSourceMapIterator it = sourceIDMap.begin();
//        for (it = sourceIDMap.begin(); it != sourceIDMap.end(); ++it)
//        {
//            SoundSource *source = &soundSources[it->second];
//            source->update();
//        }
        std::vector<SoundSource>::iterator it;
        for (it = soundSources.begin(); it != soundSources.end(); ++it)
        {
            it->update();
        }
    }
    
    /*
     * This function loops through the sound bank
     * and builds up a structure that is fed into
     * the spatializer's input
     *
     * The structure is essentially a matrix of
     * audio blocks for all active sounds in the system
     */
    float ** SoundBank::getActiveSounds()
    {
//        std::vector<float*>::iterator inputIt;
//        std::vector<SoundSource>::iterator soundSourceIt;
//        soundSourceIt = soundSources.begin();
//        inputIt = currentBankInputs.begin();
//        
//        for (; soundSourceIt != soundSources.end(); ++inputIt, ++soundSourceIt)
//            *inputIt = soundSourceIt->getData();
        
        /*
         * Toggling the activation state of
         * a sound source will cause it to emit silence.
         * I'm just going to return all sounds in this case
         */
        return &currentBankInputs[0];
    }
    
    int SoundBank::getActiveSoundCount()
    {
        int i=0;
        std::vector<SoundSource>::iterator it;
        for (it = soundSources.begin(); it != soundSources.end(); ++it) {
            if (it->checkIsActive()) {
                ++i;
            }
        }
        return i;
    }
    
    /* On success, returns the ID of the deactivated sound source. On failure, returns -1 */
    int SoundBank::deactivateSoundInBank(std::string filePath)
    {
        int location;
        
        SoundSourceMapIterator it = sourceIDMap.find(filePath);
        if (it == sourceIDMap.end())
            location = -1;
        else
            location = it->second;
        
        if (location < 0) {
            printf("Couldn't find sound with name %s so exiting\n", filePath.c_str());
            return -1;
        }
        
        printf("Deactivating sound %s in function %s at line %d\n", filePath.c_str(), __func__, __LINE__);
        
        // Get a pointer in order to deactivate the source
        SoundSource *source = &soundSources[location];
        int ID = source->getID();
        source->deactivate();
        
        /*
         * NB : After having a read of the pointer policy source, it says the in and out
         * ** passed to the audio_callback function can only grow bigger.
         * This implies that I should not be decreasing the size of my vector, I should merely
         * be setting a source to the deactivated state. This will ensure that the float ** passed to the
         * SSR will only ever grow bigger
         */
        
        // Remove the sound source from the vector. Remember, this vector should really be reorganized after removal
//        std::vector<SoundSource>::iterator ss_it;
//        int index=0;
//        for (auto &i : soundSources)
//        {
//            if (i.getID() == ID) {
//                soundSources.erase(soundSources.begin() + index);
//            }
//            ++index;
//        }
//        sourceIDMap.erase(key); // Don't forget to remove it from the table too
        return ID;
    }
    
    int SoundBank::resetSoundInBank(std::string filePath)
    {
        int location;
        
        SoundSourceMapIterator it = sourceIDMap.find(filePath);
        if (it == sourceIDMap.end())
            location = -1;
        else
            location = it->second;
        
        if (location < 0) {
            printf("Couldn't find sound with name %s so exiting\n", filePath.c_str());
            return -1;
        }
        
        printf("Resetting sound %s in function %s at line %d\n", filePath.c_str(), __func__, __LINE__);
        
        // Get a pointer in order to deactivate the source
        SoundSource *source = &soundSources[location];
        int ID = source->getID();
        source->resetData();
        return ID;
    }
    
}
