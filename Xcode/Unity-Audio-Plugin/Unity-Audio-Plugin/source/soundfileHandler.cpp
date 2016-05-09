//soundfileHandler.cpp
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

#include "soundfileHandler.h"
#include "Common.h"

/** 
  * TODO :
  * There is a known memory leak with this class. The
  * reference count in the fileHandle is increased with
  * every call to the copy constructor.
  * This is actually causing runtime errors
  * when sounds are deleted from the bank
  */

namespace uap
{
    
    // Standard constructors
    SoundfileHandler::SoundfileHandler()
    {
#ifdef DEBUG
        printf("SoundfileHandler default-ctor called\n");
#endif
        filePath = std::string("");
        block_size = 0;
//        currentDataBlock = nullptr;
        currentDataBlock = new float[BLOCK_SIZE];
        fileIsOpen = false;
        isActive = false;
    }
    
    SoundfileHandler::~SoundfileHandler()
    {
#ifdef DEBUG
        printf("SoundfileHandler destructor called\n");
#endif
        if (fileIsOpen)
            closeFile();
    }
    
    SoundfileHandler::SoundfileHandler(std::string &file)
    {
#ifdef DEBUG
        printf("SoundfileHandler ctor with string called\n");
#endif
        filePath = std::string(file);
        block_size = 512;
        currentDataBlock = new float[BLOCK_SIZE];
        fileIsOpen = false;
    }
    
    void SoundfileHandler::swap (SoundfileHandler &handler) throw ()
    {        
        std::swap(handler.filePath, this->filePath);
        std::swap(handler.currentDataBlock, this->currentDataBlock);
        std::swap(handler.fileHandle, this->fileHandle);
        block_size = handler.block_size;
        fileIsOpen = handler.fileIsOpen;
    }
    
    // Copy constructor and assignment operator
    SoundfileHandler::SoundfileHandler(const SoundfileHandler &orig)
    {
#ifdef DEBUG
        printf("SoundfileHandler copy-ctor called\n");
#endif
        filePath = std::string(orig.filePath);
        block_size = orig.block_size;
        isActive = orig.isActive;
        
        // Swap the data block over
//        if (orig.currentDataBlock == nullptr)
//        {
//            currentDataBlock = nullptr;
//        }
//        else
//        {
//            currentDataBlock = new float[orig.block_size];
//            memcpy(currentDataBlock, orig.currentDataBlock, orig.block_size * sizeof(float));
//        }
        
        currentDataBlock = new float[orig.block_size];
        memcpy(currentDataBlock, orig.currentDataBlock, orig.block_size * sizeof(float));
        fileIsOpen = orig.fileIsOpen;
        fileHandle = orig.fileHandle; // libsndfile handles itself with RAII
    }
    
    SoundfileHandler& SoundfileHandler::operator=(const SoundfileHandler &origHandler)
    {
#ifdef DEBUG
        printf("SoundfileHandler =operator called\n");
#endif
//        SoundfileHandler temp(origHandler);
//        temp.swap(*this);
//        return *this;
        
        filePath = std::string(origHandler.filePath);
        block_size = origHandler.block_size;
        isActive = origHandler.isActive;
        float *local_temp_block = new float[block_size];
        memcpy(local_temp_block, origHandler.currentDataBlock, block_size * sizeof(float));
        delete currentDataBlock;
        currentDataBlock = local_temp_block;
        fileIsOpen = origHandler.fileIsOpen;
        fileHandle = origHandler.fileHandle;
        return *this;
    }

    std::string SoundfileHandler::getFilePath() const
    {
        return filePath;
    }
    
    void SoundfileHandler::openFile()
    {
        // Allocate the file handle
        fileHandle = SndfileHandle(filePath, SFM_READ);
        
        // Allocate and clear the block of data
        memset(currentDataBlock, 0, BLOCK_SIZE * sizeof(float));
        
        // Set variables
        block_size = BLOCK_SIZE;
        fileIsOpen = true;
        isActive = true;
    }
    
    void SoundfileHandler::closeFile()
    {
        // libsndfile handles itself with RAII
        isActive = false;
        fileIsOpen = false;
        delete [] currentDataBlock;
    }
    
//    void SoundfileHandler::loadData()
//    {
//        // TODO : Implement this
//    }
    
    float * SoundfileHandler::getNextDataBlock()
    {
        return currentDataBlock;
    }

    bool SoundfileHandler::isOpen()
    {
        return fileIsOpen;
    }
    
    void SoundfileHandler::activate()
    {
        isActive = true;
    }
    
    void SoundfileHandler::deactivate()
    {
        printf("Deactivate called in file %s at line %d\n", __FILE__, __LINE__);
        isActive = false;
    }
    
    void SoundfileHandler::resetDataBlock() // Quite literally resets the sound to 0
    {
        if (fileIsOpen)
        {
            fileHandle.seek(0, SEEK_SET);
        }
    }
    
    void SoundfileHandler::update()
    {
        if (fileIsOpen)
        {
            if (isActive) {
                sf_count_t result = fileHandle.readf(currentDataBlock, BLOCK_SIZE);
                if (result != BLOCK_SIZE) {
                    fileHandle.seek(0, SEEK_SET);
                    fileHandle.readf(currentDataBlock + result, BLOCK_SIZE - result);
                }
            }
            else
            {
//                printf("Writing silence in %s at line %d\n", __func__, __LINE__);
                memset(currentDataBlock, 0, BLOCK_SIZE * sizeof(float)); // Update for silence
            }
        }
        else
        {
            memset(currentDataBlock, 0, BLOCK_SIZE * sizeof(float)); // Update for silence
        }

//#ifdef DEBUG
//        int i=0;
//        printf("The current data block:\n[");
//        for (; i < BLOCK_SIZE - 1; ++i) {
//            printf("%.2f, ", *(currentDataBlock + i));
//        }
//        printf("%.2f]", *(currentDataBlock + i));
//#endif
    }

}