//audioplugin.h
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

/*
 * Note that the SSR implements spatial audio on the 2D horizontal plane
 * This means elevation is not supported, but thats fine, because we're
 * looking at depth for this stuff
 */

#ifndef Unity_Audio_Plugin_audioPlugin_h
#define Unity_Audio_Plugin_audioPlugin_h

extern "C" {
    
    void copyStringFromManagedCaller(char *managedString);
    
    void initEnv();
    void initEnvWithHRIRS(char *hrirFilePath);
    void destroyEnv();
    int checkStatus();
    
    void startJuce();
    void stopJuce();
    
    /*
     * Methods for adding/removing sounds and their manipulation
     */
    void addSoundToRendererBank(char *filename);
    void addSoundToRendererBankAtPosition(char *filename, float xPos, float zPos);
    bool activateSoundWithName(char *filename);
    bool deactivateSoundInRendererBank(char *filename);
    bool resetSoundWithName(char *filename);
    bool setSoundWithNameToPosition(char *filename, float xPos, float zPos);
    
    /*
     * Methods for manipulating the listener position
     */
    bool moveListenerToPosition(float xPos, float zPos);
    bool orientListenerInDirection(float orientAngle);
}

#endif
