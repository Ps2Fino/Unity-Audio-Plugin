//spatializer.cpp
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
#include "spatializer.h"

#ifndef SSR_INCLUDES
#define SSR_INCLUDES
#include <apf/pointer_policy.h> // Set the policy for the renderer; the pointer policy gives access to the actual audio data
#include <apf/default_thread_policy.h> // Default threaded policy is single threaded on Windows, pthreads on other platforms
//#include <apf/dummy_thread_policy.h>
#include <ssr/binauralrenderer.h> // For the SSR bin renderer
#endif

/*
 * Most of this class
 * is templated as standard.
 * Just the callbacks are left
 * to the base class
 */
namespace uap
{
 
    /*
     * Specializations for the spatializer
     * Without these, the spatializer
     * can't be instantiated in the audioPlugin
     * interface source
     */
    template <>
    Spatializer<SPATIALIZER_CALLBACK_IMPL>::Spatializer()
    {
        this->hrirs.clear();
    }
    
    template <>
    Spatializer<SPATIALIZER_CALLBACK_IMPL>::Spatializer(std::string &hrirs_location)
    {
        this->hrirs = hrirs_location;
    }

    template <>
    Spatializer<SPATIALIZER_CALLBACK_IMPL>::~Spatializer()
    {
        // TODO : Implement this
        renderer.reset();
    }

    template <>
    void Spatializer<SPATIALIZER_CALLBACK_IMPL>::init()
    {
        num_sources_in_renderer = 0;
        apf::parameter_map map = getDefaultParams();
        renderer = std::make_unique<ssr::BinauralRenderer>(map);
//        renderer = std::unique_ptr<ssr::BinauralRenderer>(new ssr::BinauralRenderer(map));
        renderer->load_reproduction_setup();
        
        // Set an orientation offset to make NORTH the 0 azimuth
        renderer->state.reference_offset_orientation = Orientation(90);
    }

    template <class T>
    apf::parameter_map Spatializer<T>::getDefaultParams()
    {
        apf::parameter_map map;
        
        // Compulsory settings
        map.set("sample_rate", apf::str::A2S(SAMPLE_RATE));
        map.set("block_size", apf::str::A2S(BLOCK_SIZE));
        
        // Binaural specific settings
        if (hrirs.empty())
        {
            std::cout << "The hrir file name is " << HRIR_FILE_NAME << std::endl;
            std::string hrir_str(HRIR_FILE_NAME);
            map.set("hrir_file", hrir_str);
            std::cerr << "Using the default hrir : " << hrir_str << std::endl;
//            printf("Using the default hrir : %s\n", hrir_str.c_str());
        }
        else
        {
            map.set("hrir_file", hrirs);
            std::cerr << "Using a custom hrir : " << hrirs << std::endl;
//            printf("Using a custom hrir : %s\n", hrirs.c_str());
        }

        map.set("hrir_size", BLOCK_SIZE);
        map.set("renderer_type", "binaural");
        
        return map;
    }
    
    template <>
    void Spatializer<SPATIALIZER_CALLBACK_IMPL>::setListenerPosition(float x, float y, float z)
    {
        renderer->state.reference_position = Position(x, y);
    }
    
    template <>
    void Spatializer<SPATIALIZER_CALLBACK_IMPL>::setListenerOrientation(float angle)
    {
        renderer->state.reference_orientation = Orientation(angle);
    }
    
    template <>
    bool Spatializer<SPATIALIZER_CALLBACK_IMPL>::update(float **input, float **output, int numInputChannels, int numOutputChannels)
    {
        if (numOutputChannels != 2) {
            fprintf(stderr, "Number of output channels != 2. This is supposed to be single listener binaural....\n");
            for (int i=0; i < numOutputChannels; ++i) { memset(output[i], 0, BLOCK_SIZE * sizeof(float)); } // Apply silence so...
            return false;
        }
        
        // Otherwise, I guess we're good to go ahead and process the current blocks
        renderer->audio_callback(BLOCK_SIZE, input, output);
        return true;
    }
    
    template <>
    bool Spatializer<SPATIALIZER_CALLBACK_IMPL>::process(SoundBank *bank, float **outData, int numOuts)
    {
        // Update all sounds in the bank and
        // gather the input pointers from the soundbank
        bank->updateSounds();
        float ** inData = bank->getActiveSounds();
        
        return update(inData, outData, bank->getActiveSoundCount(), numOuts);
//        return update(&bank->currentBankInputs[0], outData, bank->getActiveSoundCount(), numOuts);
    }
    
    template <>
    int Spatializer<SPATIALIZER_CALLBACK_IMPL>::addSoundSource(std::string &soundName)
    {
        printf("Function %s, file %s, line %d: About to add the sound source %s\n",
               __func__, __FILE__, __LINE__, soundName.c_str());
        
        
        std::string fnameStr(soundName);
        apf::parameter_map params;
        params.set("audio_file_name", fnameStr);
        
        
        printf("Function %s, file %s, line %d: About to add the sound source %s\n",
               __func__, __FILE__, __LINE__, soundName.c_str());
        if (renderer.get() == nullptr)
            std::cout << "Renderer is null!" << std::endl;
        
        int sourceID = renderer->add_source(params);
        
        
        printf("Function %s, file %s, line %d: Added the sound source %s\n",
               __func__, __FILE__, __LINE__, soundName.c_str());
        
        
        static_cast<SPATIALIZER_CALLBACK_IMPL*>(this)->soundAddedCallback(fnameStr);
        num_sources_in_renderer++;
        
        
        printf("Function %s, line %d: Added successfully the sound source %s\n",
               __func__, __LINE__, soundName.c_str());
        return sourceID;
    }
    
    template <>
    void Spatializer<SPATIALIZER_CALLBACK_IMPL>::moveSoundSource(int soundSourceID, float x, float y, float z)
    {
        renderer->get_source(soundSourceID)->position = Position(x, y); // SSR doesn't recognise elevation yet ((X,Y) is on horizontal plane so really its (X, Z))
        static_cast<SPATIALIZER_CALLBACK_IMPL*>(this)->soundMovedCallback(soundSourceID, x, y, z);
    }
    
    template <>
    int Spatializer<SPATIALIZER_CALLBACK_IMPL>::getNumSources()
    {
        return num_sources_in_renderer;
    }
    
    template <>
    void Spatializer<SPATIALIZER_CALLBACK_IMPL>::removeSoundSource(int soundSourceID)
    {
        renderer->rem_source(soundSourceID);
    }

}