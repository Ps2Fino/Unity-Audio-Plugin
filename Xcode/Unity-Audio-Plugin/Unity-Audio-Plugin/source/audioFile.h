//
//  audioFile.h
//  Unity-Audio-Plugin
//
//  Created by Daniel J. Finnegan on 17/02/2015.
//  Copyright (c) 2015 Somethin Else Sound Directions Ltd. All rights reserved.
//

#ifndef __Unity_Audio_Plugin__audioFile__
#define __Unity_Audio_Plugin__audioFile__

#include <stdio.h>
#include <vector>
#include "sndfile.hh"  // C++ bindings for libsndfile

#define BLOCK_SIZE 512

namespace uap
{

/**
  * Small handle over the SndfilHandle so I can
  * use my own API rather than remembering libsndfiles
  *
  * @author Daniel J. Finnegan
  */
class AudioFileHandle {
public:
    
    AudioFileHandle();
    ~AudioFileHandle();
    
    bool loadFileHandle(const char *fileName, float sample_rate, int channels);
    bool releaseFileHandle();
    bool readNextBlock();
    
    /*
     * This is the public function for reading the data currently
     * stored in the block buffer
     */
    float * getCurrentDataBlock() { return audioDataBlock.data(); }
private:

    std::unique_ptr<SndfileHandle> audioFile;
    std::vector<float> audioDataBlock;
};
    
}

#endif /* defined(__JUCE_Unity_Plugin__audioFile__) */
