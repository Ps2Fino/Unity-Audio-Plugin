//spatializer.h
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

#ifndef __Unity_Audio_Plugin__spatializer__
#define __Unity_Audio_Plugin__spatializer__

#include <string>
#include <memory>

#include "soundbank.h"

// ------------------------------------------------------------------------------ //

/*
 * This forward dec is required as otherwise the templates get
 * confused. Or my C++ is just terrible
 */
namespace ssr {
    class BinauralRenderer;
}

/*
 * Same issue here
 */
namespace apf {
    struct parameter_map;
}

// ------------------------------------------------------------------------------ //

namespace uap
{

    /**
     * This class wraps around the SSR as a Spatializer
     * class which handles the acitivation and deactivation
     * of all the sources based upon the location of the player.
     *
     * It uses the SSR as its convolution renderer and the
     * soundbank class in order to query the next blocks
     * of data required
     *
     * It uses a ' specialized template ' pattern to allow
     * client programmers to write callbacks for all the events 
     * that it will be involved in
     *
     * @author Daniel J. Finnegan
     */
    template <class T>
    class Spatializer
    {
    public:
        
        Spatializer();
        Spatializer(std::string &hrirs_location);
        ~Spatializer();
        
        // Init the spatializer
        void init();
        
        // Position the listener in space
        void setListenerPosition(float x, float y, float z);
        
        // Orient the listener in space
        void setListenerOrientation(float angle);
        
        /*
         * This function takes a set of pointers to point to its output as
         * well as an output count, which should be 2 for binaural
         */
        bool process(SoundBank *bank, float **outData, int numOuts);
    
        /* Use CRTP to implement this in a subclass */
        int addSoundSource(std::string &soundName);
        void removeSoundSource(int ID);
        void moveSoundSource(int soundSourceID, float x, float y, float z);
        int getNumSources();
        
    private:
        
        /*
         * This function will pass the current
         * head position of the renderer to the soundbank
         * in order to choose which sounds should be passed
         * to the renderer. Only sounds in the vicinity
         * of the listener are rendered to save CPU time
         */
        bool update(float **input, float **output, int numInputChannels, int numOutputChannels);
        
        // Get the default HRTF paramaters
        apf::parameter_map getDefaultParams();
        std::string hrirs; // This makes more sense from the Unity perspective
        
        int num_sources_in_renderer;
        
    protected:
        
        // This is the only object it should have
        // The manager will pass around pointers to
        // its own members when things need to do things...
        std::unique_ptr<ssr::BinauralRenderer> renderer;
        
    };
    
}

// Include common here, after the spatializer class has been declared
// otherwise the templates go a little crazy
#include "Common.h"

#ifndef SPATIALIZER_CALLBACK_IMPL
#define SPATIALIZER_CALLBACK_IMPL DefSpatializer
#include "defaultSpatializer.h"
#endif

#endif /* defined(__JUCE_Unity_Plugin__spatializer__) */
