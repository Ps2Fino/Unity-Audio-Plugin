//jucePlayer.cpp
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
#include "jucePlayer.h"

namespace uap
{

    JucePlayer::JucePlayer() : readyToPlay(false)
    {
        // Init the device manager
        manager = std::make_unique<AudioDeviceManager>();
        
        /*
         * This will grab the default audio context
         * with 2 outputs
         */
        const String errorMsg(manager->initialise(0, 2, 0, true)); // 0 input, 2 output, No XML settings, default device that fits these parameters
        
        std::cout << "Message from juce manager initialise : " << errorMsg << std::endl;
        
        soundBank = nullptr; // This has to be set in the init function
        spatializer = nullptr;
    }

    JucePlayer::~JucePlayer()
    {
        manager->removeAudioCallback(this);
        manager.reset();
//        spatializer = nullptr; // Its not my job to deallocate this...
//        soundBank = nullptr; // ....Or this
    }

    bool JucePlayer::init()
    {
        printf("I called the function %s\n", __func__);
        
        if (soundBank == nullptr) {
            printf("In function %s, the soundbank is null\n", __func__);
        }
        
        if (spatializer == nullptr) {
            printf("In function %s, the spatializer is null\n", __func__);
        }
        
        if (soundBank == nullptr || spatializer == nullptr)
        {
            readyToPlay = false;
        }
        else
        {
            readyToPlay = true;
        }
        
        return readyToPlay;
    }
    
    bool JucePlayer::start()
    {
        if (readyToPlay)
        {
#ifdef DEBUG
            fprintf(stderr, "I'm about to add the audio callback\n");
#endif
            manager->addAudioCallback(this);
            return true;
        }
        else
        {
#ifdef DEBUG
            fprintf(stderr, "Ready to play was false, so I didn't do anything\n");
#endif
        }
        
        return false;
    }

    bool JucePlayer::stop()
    {
        manager->removeAudioCallback(this);
        manager->closeAudioDevice(); // Very important
        return true;
    }
    
    void JucePlayer::generateSilence(float **outputData, int numOuts, int numSamples)
    {
        // TODO : Vectorize this maybe as its called in the Juce
        // callback (overhead of more thread creation and synching with
        // doube loop)
        for (int i=0; i < numOuts; ++i)
            for (int j=0; j < numSamples; ++j)
                *(outputData[i] + j) = 0;
    }

    // The JUCE callback
    void JucePlayer::audioDeviceIOCallback (const float** inData, int numIns,
                                                 float** outData, int numOuts,
                                                 int numSamples)
    {
//        std::cout << "Called the juce callback at line : " << __LINE__ << std::endl;
        if (soundBank->getActiveSoundCount() <= 0) {
#ifdef DEBUG_CALLBACK
            fprintf(stderr, "I'm writing silence\n");
#endif
            
            // Silence
            generateSilence(outData, numOuts, numSamples);
//            std::cout << "Called the juce callback at line : " << __LINE__ << std::endl;
            
            return;
        }
        
        if (numOuts != 2) {
#ifdef DEBUG_CALLBACK
            printf("The numOuts is not 2. We want binaural here...\n");
#endif
            // Silence
            generateSilence(outData, numOuts, numSamples);
//            std::cout << "Called the juce callback at line : " << __LINE__ << std::endl;
            
            return;
            
        }
    
        // It boils down to a one-liner
        bool result = spatializer->process(soundBank.get(), outData, numOuts);
        
        // Route directly to the audio DAC
        if (result)
        {
#ifdef DEBUG_CALLBACK
            fprintf(stderr, "I'm writing data!\n");
#endif
//            std::cout << "Called the juce callback at line : " << __LINE__ << std::endl;
        }
        else
        {
#ifdef DEBUG_CALLBACK
            fprintf(stderr, "I'm writing silence\n");
#endif
            // Silence
            generateSilence(outData, numOuts, numSamples);
            
//            std::cout << "Called the juce callback at line : " << __LINE__ << std::endl;
        }
        
    }

}