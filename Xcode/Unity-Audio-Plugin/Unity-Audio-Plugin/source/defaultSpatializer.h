//defaultSpatializer.h
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

#ifndef __Unity_Audio_Plugin__defaultSpatializer__
#define __Unity_Audio_Plugin__defaultSpatializer__

#include <string>
#include "spatializer.h"

namespace uap
{
    
    /*
     * This is the default implementation for the spatializer callback
     */
    class DefSpatializer : public Spatializer<DefSpatializer>
    {
        
    public:
        
        void soundAddedCallback(std::string soundName);
        void soundMovedCallback(int soundSourceID, float x, float y, float z);
        int getNumSources();
    };
    
}

#endif /* defined(__Unity_Audio_Plugin__defaultSpatializer__) */
