//main.cpp
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

#include "apiHeader.hpp"
#include <string>
#include <vector>

int main(int argc, char * argv[]) {
    
    // Lets find the bundle first
    // All this jazz simply deals with loading the bundle
    LOAD_API_BUNDLE;
    RETAIN_API_BUNDLE_VAR;
    
    initEnvFunc initEnv = set_initEnvFunc(API_BUNDLE_VAR);
    destroyEnvFunc destroyEnv = set_destroyEnvFunc(API_BUNDLE_VAR);
    startJuceFunc startJuce = set_startJuceFunc(API_BUNDLE_VAR);
    stopJuceFunc stopJuce = set_stopJuceFunc(API_BUNDLE_VAR);
    addSoundToRendererBankFunc addSound = set_addSoundToRendererBankFunc(API_BUNDLE_VAR);
    deactivateSoundInRendererBankFunc deactivateSoundInRendererBank = set_deactivateSoundInRendererBankFunc(API_BUNDLE_VAR);
    
    std::string asset_path(TEST_ASSET_PATH);
    std::string asset_one(asset_path + std::string("/ad-demo.wav"));
    std::string asset_two(asset_path + std::string("/cat.wav"));
    
    std::vector<char> asset_one_str(asset_one.begin(), asset_one.end());
    asset_one_str.push_back('\0');
    std::vector<char> asset_two_str(asset_two.begin(), asset_two.end());
    asset_two_str.push_back('\0');
    
// ---------------------------------------------------------------------- //
// ---------------------------------------------------------------------- //
// ---------------------------------------------------------------------- //

#define TESTING_JUCE_PLAYBACK
#ifdef TESTING_JUCE_PLAYBACK
    
    initEnv();
    addSound(&asset_one_str[0]);
    addSound(&asset_two_str[0]);
    startJuce();
    
    sleep(6);
    
    // Go away cat!
    deactivateSoundInRendererBank(&asset_two_str[0]);
    
    sleep(6);
    addSound(&asset_two_str[0]);
    sleep(3);
    
    stopJuce();
    deactivateSoundInRendererBank(&asset_one_str[0]);
    deactivateSoundInRendererBank(&asset_two_str[0]);
    destroyEnv();
    
#else
    
    initEnv();
    addSound(&asset_one_str[0]);
    
    sleep(2);
    
    removeSoundFromRendererBank(&asset_one_str[0]);
    destroyEnv();
    
#endif

// ---------------------------------------------------------------------- //
// ---------------------------------------------------------------------- //
// ---------------------------------------------------------------------- //
    
    printf("Releasing the bundle\n");
    RELEASE_API_BUNDLE_VAR;
    
    printf("Calling it a day\n");
    return 0;
}



