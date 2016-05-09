//defaultSpatializer.cpp
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

#include "defaultSpatializer.h"

namespace uap
{
    void DefSpatializer::soundAddedCallback(std::string soundName)
    {
        fprintf(stderr, "The soundAddedCallback was called\n");
    }
    
    void DefSpatializer::soundMovedCallback(int soundSourceID, float x, float y, float z)
    {
        fprintf(stderr, "The soundMoved callback was called\n");
    }
    
}