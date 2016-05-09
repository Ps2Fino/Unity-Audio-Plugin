//soundfileHandler.h
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

#ifndef Unity_Audio_Plugin_soundfileHandler_h
#define Unity_Audio_Plugin_soundfileHandler_h

#include <sndfile.hh>
#include <boost/scoped_ptr.hpp>
#include <string>
#include <vector>

namespace uap
{
    
    /*
     * This is the class for handling low level file IO and the like
     * @author Daniel J. Finnegan
     */

    class SoundfileHandler
    {
        
    protected:
        
        // For the temp-and-swap idiom
        void swap (SoundfileHandler &handler) throw ();
        
        bool fileIsOpen;
//        SndfileHandle *fileHandle;
        bool isActive;
        SndfileHandle fileHandle;
        std::string filePath;
        int block_size;
        float *currentDataBlock;

    public:
        
        // Standard constructors
        SoundfileHandler();
        ~SoundfileHandler();
        SoundfileHandler(std::string &file);
        
        // Copy constructor and operator
        SoundfileHandler(const SoundfileHandler &orig);
        SoundfileHandler& operator=(const SoundfileHandler &origHandler);
        
        std::string getFilePath() const; // Marked as const as it doesn't modify the calling instance of the SoundfileHandler class
        void openFile();
        void closeFile();
//        void loadData();
        float * getNextDataBlock();
        void resetDataBlock(); // This function resets the sound data to the beginning of the file
        void activate();
        void deactivate();
        
        bool isOpen();
        void update();
        
    };
    
}

/* ------------------------------------------------
   ------------------------------------------------ */

#endif
