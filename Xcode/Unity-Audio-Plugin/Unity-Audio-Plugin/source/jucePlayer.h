//jucePlayer.h
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

#ifndef __Unity_Audio_Plugin__jucePlayback__
#define __Unity_Audio_Plugin__jucePlayback__

#include <Juce-Audio/JuceHeader.h>
#include "Common.h"
#include "spatializer.h"
#include "soundbank.h"

namespace uap
{

/* This class implements the JUCE callback */
class JucePlayer : public AudioIODeviceCallback
{
public:
    
    JucePlayer();
    ~JucePlayer();
    
    // Functions
    bool init();
    bool start();
    bool stop();
    
    // JUCE callback function; this is called by the JUCE framework at every audio cycle
    void audioDeviceIOCallback (const float** inputChannelData, int totalNumInputChannels,
                                float** outputChannelData, int totalNumOutputChannels,
                                int numSamples);
    
    void generateSilence(float **outputData, int numOuts, int numSamples);
    
    // I don't bother with these for the moment
    void audioDeviceAboutToStart (AudioIODevice*) {}
    void audioDeviceStopped() {}
    
    // Setters for the drones
    void setSpatializer(std::shared_ptr<Spatializer<SPATIALIZER_CALLBACK_IMPL> > spatializer) { this->spatializer = spatializer; }
    void setSoundBank(std::shared_ptr<SoundBank> bank) { this->soundBank = bank; }
    
protected:
    
    bool readyToPlay; // This is a flag to check for status errors; very rudimentary and probaly not the best way of doing things
    std::unique_ptr<AudioDeviceManager> manager;
    
    // Members that carry out the callbacks
    std::shared_ptr<Spatializer<SPATIALIZER_CALLBACK_IMPL> > spatializer;
    std::shared_ptr<SoundBank> soundBank;
    
};
    
}

#endif /* defined(__JUCE_Unity_Plugin__jucePlayback__) */
