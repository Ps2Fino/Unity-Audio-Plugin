//
//  audioFile.cpp
//  Unity-Audio-Plugin
//
//  Created by Daniel J. Finnegan on 17/02/2015.
//  Copyright (c) 2015 Somethin Else Sound Directions Ltd. All rights reserved.
//

#include "audioFile.h"

namespace uap
{

AudioFileHandle::AudioFileHandle() : audioFile(), audioDataBlock(BLOCK_SIZE)
{}

AudioFileHandle::~AudioFileHandle()
{}

/*
 * This function is just copied from the APF library
 * to avoid the dependency on the APF.
 * Exception throwing has been removed
 * for the sake of trying to get this to work
 */
bool AudioFileHandle::loadFileHandle(const char *name, float sample_rate, int channels)
{
    std::string myName(name);
    audioFile = std::make_unique<SndfileHandle>(myName, SFM_READ);
    
    // Check for an error
    if (audioFile->error() != SF_ERR_NO_ERROR) {
        return false;
    }
    
#if 0
    // rawHandle() is available since libsndfile version 1.0.24
    if (!audioFile->rawHandle())
#else
        if (!audioFile->channels())
#endif
        {
            //                throw std::logic_error(
            //                                       "apf::load_sndfile(): \"" + name + "\" couldn't be loaded!");
        }
    
    if (sample_rate)
    {
        const size_t true_sample_rate = audioFile->samplerate();
        if (sample_rate != true_sample_rate)
        {
            //                throw std::logic_error("apf::load_sndfile(): \"" + name
            //                                       + "\" has sample rate " + str::A2S(true_sample_rate) + " instead of "
            //                                       + str::A2S(sample_rate) + "!");
        }
    }
    
    if (channels)
    {
        const size_t true_channels = audioFile->channels();
        
        if (channels != true_channels)
        {
            //                throw std::logic_error("apf::load_sndfile(): \"" + name + "\" has "
            //                                       + str::A2S(true_channels) + " channels instead of "
            //                                       + str::A2S(channels) + "!");
        }
    }
    
    return true;
}

bool AudioFileHandle::releaseFileHandle()
{
    return true;
}

/*
 * realistically, here you would buffer the whole
 * file as we have access to it on disk and we have
 * the memory. We don't really want to be doing
 * an IO read every cycle.
 */
bool AudioFileHandle::readNextBlock()
{
    sf_count_t result = audioFile->readf(audioDataBlock.data(), BLOCK_SIZE);
    if (result != BLOCK_SIZE)
    {
//        fprintf(stderr, "The result was less than the block size\n");
        audioFile->seek(0, SEEK_SET);
        audioFile->readf(audioDataBlock.data() + result, BLOCK_SIZE - result);
    }
    else
    {
//        fprintf(stderr, "The result was equal to the block size\n");
    }
    
    return true;
}

}