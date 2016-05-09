Unity Plugin for Spatial Audio
@ author Daniel J. Finnegan
@ date March 2015

#######################################################################################################################
####### Project dependencies ##########################################################################################
#######################################################################################################################

This plugin requires an implementation of the low level file handling functionality.
The default functionality is provided by libsndfile.
In order to use your own, you need to include the header file for your implementation (anywhere, it just needs to exist)
and define the AUDIO_FILE_HANDLE to the name of the implementing class, as a preprocessor flag or in the source.
If you don't define AUDIO_FILE_HANDLE, the default implementation will be built
Best way to do this is to actually create a prefix header file and define AUDIO_FILE_HANDLE and include your implementation header there

Same applies for the spatialization. I use the SSR (spatialaudio.net).
Its pretty good, has been developed over many years and its clientelle includes the BBC (they use it for the object based broadcasting)
research.
Later (when we have Papa Mac perhaps....) we should be able to simply replace the SSR delegate for the Papa Engine and this
should just work (but I doubt very much it will be that simple)

Lastly is audio output.
The default implementation is provided by Juce (GPL licensed)
The steps required to use your own are the same as the file handle.

#######################################################################################################################
####### Workspace build settings ######################################################################################
#######################################################################################################################

The build settings for the plugin include some scripts, one of which is a python script named funcDefs.py.
This script is a code generator that will automatically generate the set of public API functions
that the bundle contains, so they can be called from a client application.
However, the script cannot be used by itself and is actually used as input to another script called generate_code_generator.sh
generate_code_generator.sh actually generates the full python script that will create the public API header.

To this end, the maintenance programmers job is simple; all she has to do is modify the user defined build settings in
the Unity-Audio-Plugin target.
Namely:

1) CLANG_INSTALL_DIRECTORY: Set this to the root directory of your clang installation. *NB* MAKE SURE YOU HAVE THE PYTHON BINDINGS IN $CLANG_INSTALL_DIRECTORY/bindings/python !!!
3) LD_LIBRARY_PATH: This should be automatically set, but in case you've any issues, set it to $CLANG_INSTALL_DIRECTORY/lib

Note that Xcode doesn't expand the ~/ user directory so you need absolute system paths for these bad boys

All this is purely for fast development of console applications in Xcode. This was because originally I had the free version of Unity 4
which didn't allow native plugins to be used in the editor; you had to make a build every time you wanted to test the current scripts.
Unity 5 changed this so the use of clang for generating an API header is probably not needed anymore...


*NB*!!
If you're having DLLNotFoundException issues in Unity, double check the bundle is looking for the dylibs in the correct place.
To do this, run otool -L on the executable 'Unity-Audio-Plugin' inside the bundle ({BUNDLE_ROOT}Content/MacOS)
What you should see is paths to libsndfile, libfftw and all other non-standard libraries pointing to @executable_path/../SharedSupport/${LIB_NAME}
If you don't see this, use the following command on each :
install_name_tool -change ${EXISTING_PATH_OUTPUT_FROM_OTOOL_-L} @executable_path/../SharedSupport/${LIB_NAME}

#######################################################################################################################
####### Misc info #####################################################################################################
#######################################################################################################################

Email me regarding anything about the code style or the functionilty of the plugin.
Please note that I am using this for research.
Many of the dependencies are GPL licensed and are thus not fit for commercial use
The plugin is practically capped at 50 sounds (theoretically probably less) and I haven't
actually tested beyond 2 so there may be issues with building densely populated soundscapes.