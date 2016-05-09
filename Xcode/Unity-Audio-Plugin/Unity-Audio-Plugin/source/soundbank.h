//soundbank.h
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

#ifndef __Unity_Audio_Plugin__soundbank__
#define __Unity_Audio_Plugin__soundbank__

#include "soundfileHandler.h"
//#include <boost/unordered_map.hpp>
//#include <unordered_map>
#include <map>
#include <iostream>
#include "Common.h"

namespace uap
{
    // Forward declaration for the friendship relationship between the bank and the sound source
    class SoundBank;

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    /////// Sound Source class ///////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    
    class SoundSource {
        
    private:
        
        void swap (SoundSource &orig) throw ()
        {
            std::swap(orig.soundFileHandle, this->soundFileHandle);
            xPos = orig.xPos;
            yPos = orig.yPos;
            isSoundSourceActive = orig.isSoundSourceActive;
            ID = orig.ID;
        }
        
        SoundfileHandler soundFileHandle;
        float xPos, yPos;
        int ID;
        bool isSoundSourceActive;
        
    public:
        
        // Default constructor. Needed for the boost::unordered_map
        SoundSource()
        {
#ifdef DEBUG
            printf("SoundSource default-ctor called\n");
#endif
            soundFileHandle = SoundfileHandler();
            ID = 0;
            isSoundSourceActive = false;
        }
        
        // Main constructor
        SoundSource(std::string &filePath, int sourceID)
        {
#ifdef DEBUG
            printf("SoundSource ctor with string called\n");
#endif
            soundFileHandle = SoundfileHandler(filePath);
            xPos = 0.0f;
            yPos = 0.0f;
            ID = sourceID;
            isSoundSourceActive = false;
        }
        
        //////////////////////////////////////////
        //////////////////////////////////////////
        // Copy constructor and =operator
        //////////////////////////////////////////
        //////////////////////////////////////////
        
        SoundSource(const SoundSource &orig)
        {
            // TODO: Copy over the soundFileHandlePtr
#ifdef DEBUG
            printf("SoundSource copy-ctor called\n");
#endif
            soundFileHandle = orig.soundFileHandle;
            xPos = orig.xPos;
            yPos = orig.yPos;
            isSoundSourceActive = orig.isSoundSourceActive;
            ID = orig.ID;
        }
        
        SoundSource& operator=(const SoundSource &origSource)
        {
            // TODO: Copy over the soundFileHandlePtr
#ifdef DEBUG
            printf("SoundSource =operator called\n");
#endif

            SoundSource temp (origSource);
            temp.swap(*this);
            return *this;
        }
        
        // Destructor
        ~SoundSource()
        {
            if (soundFileHandle.isOpen())
                soundFileHandle.closeFile();
        }
        
        void resetData()
        {
            soundFileHandle.resetDataBlock();
        }
        
        // Return a pointer to the underlying data in the current audio block
        float * getData()
        {
            return soundFileHandle.getNextDataBlock();
        }
        
        void activate()
        {
            if (!soundFileHandle.isOpen())
                soundFileHandle.openFile();
            
            soundFileHandle.activate();
            isSoundSourceActive = true;
        }
        
        bool checkIsActive()
        {
            return isSoundSourceActive;
        }
        
        float getXPos()
        {
            return xPos;
        }
        
        void setXPos(float xPos)
        {
            this->xPos = xPos;
        }
    
        float getYPos()
        {
            return yPos;
        }
        
        void setYPos(float yPos)
        {
            this->yPos = yPos;
        }
        
        /*
         * for the moment, all resources are held onto
         * until program termination
         */
        void deactivate()
        {
            isSoundSourceActive = false;
            soundFileHandle.deactivate();
            // TODO: 
            // Not 100% sure that I want to close the handle on deactivation.
            // Maybe keep this to the destructor
//            if (soundFileHandle.isOpen())
//                soundFileHandle.closeFile();
        }
        
        void update()
        {
            soundFileHandle.update(); // Just forward this on as the work is done in the file handle
        }
        
        int getID()
        {
            return ID;
        }
        
    };
    
    // Handy alias' for later
    // I might try having a look at these when I get a chance
    // as they might improve performance
//    using SoundSourceMap = boost::unordered_map<std::string, int>;
//    using SoundSourceMapIterator = boost::unordered_map<std::string, int>::iterator;
//    using SoundSourceMap = std::unordered_map<std::string, int>;
//    using SoundSourceMapIterator = std::unordered_map<std::string, int>::iterator;
//    using SoundSourceMap = std::map<char*, int>;
//    using SoundSourceMapIterator = std::map<char*, int>::iterator;
    
    using SoundSourceMap = std::map<std::string, int>;
    using SoundSourceMapPtr = SoundSourceMap*;
    using SoundSourceMapIterator = std::map<std::string, int>::iterator;
    


    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    /////// Sound Bank class ///////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    
    class SoundBank
    {
        
    public:
        
        SoundBank();
        ~SoundBank();
        
        // TODO : Declare the API for the sound bank
        
        bool addSoundToBank(std::string filename, int sourceID);
        void activateSoundSource(std::string key);
        
        int getIDForName(std::string filename);
        const char * getNameForID(int ID);
        
        bool setSoundPositionForName(std::string filename, float xPos, float yPos);
        SoundSource * const getSoundSourceForName(std::string filePath);
        
        bool addAndActivate(std::string filePath, int sourceID);
        void updatePosition(float x, float y);
        
        float ** getActiveSounds(); // This is the main function of the bank. It grabs a pointer to a set of sources for the renderer for this cycle
        int getActiveSoundCount();
        void updateSounds();
        
        int deactivateSoundInBank(std::string filePath);
        int resetSoundInBank(std::string filePath);
        std::vector<float*> currentBankInputs;
        
    private:
        
        std::vector<SoundSource> soundSources; // A vector that holds the actual sound sources
//        SoundSourceMapPtr sourceIDMap;
        SoundSourceMap sourceIDMap;
        float listenerXPos, listenerYPos;
        
        int cachedActiveCount;
        int soundCount;
//        float **currentBankInputs;
//        std::vector<float*> currentBankInputs;
    };
    
}

#endif /* defined(__JUCE_Unity_Plugin__soundbank__) */
