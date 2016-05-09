//
//  mySoundfileHandler.h
//  Unity-Audio-Plugin
//
//  Created by Daniel J. Finnegan on 25/02/2015.
//  Copyright (c) 2015 Somethin Else Sound Directions Ltd. All rights reserved.
//

#ifndef __Unity_Audio_Plugin__mySoundfileHandler__
#define __Unity_Audio_Plugin__mySoundfileHandler__

#include <soundfileHandler.h>

namespace uap
{
    
    /*
     * The default implementation is to use libsndfile
     *
     * @author Daniel J. Finnegan
     */
    
    class MySoundfileHandler : public SoundfileHandler<MySoundfileHandler>
    {
    public:
        
        // No need for constructors or destructors as the parent handler will be called
        
        void openFile();
        void closeFile();
        
        bool isOpen();
        void update();
        
    private:
        
        std::string filePath;
    };
    
}

#endif /* defined(__Unity_Audio_Plugin__mySoundfileHandler__) */
