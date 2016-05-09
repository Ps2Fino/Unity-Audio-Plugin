Unity Spatial Audio Plugin
==========================

This repository contains everything needed to build a plugin for Unity3D to render spatial audio environments.
The plugin is written in C++, with dependencies on JUCE, the SSR, libsndfile, and libfftw.
The plugin exports a very thin and simple interface to clients which currently consists of:

- The ability to add sounds to the spatializer
- The ability to activate and deactivate sounds in the sound bank
- The ability to place a sound on the horizontal plane all around the listener

After cloning the repo, the first port of call should be to build the project with Xcode.
Just open up the workspace file located in ./Xcode/Unity-Audio-Plugin.
There is a user defined build setting in Unity-Audio-Plugin project file
called CLANG_INSTALL_DIRECTORY.
This is required if you want to generate a small api header file for using the plugin with
console applications or native mac applications (Obj-C apps) as well as for building the test console application bundled with the source.

If you just want the plugin built, just remove the Run Script phase in the project
settings that uses clang to generate the api header (or just ignore it; the plugin
should still build but Xcode might generate a warning regarding the missing clang installation)

Included in this repository, is a small demo project, zipped up, for Unity.
This project just houses 2 cubes and draws an aerial map of the environment
as the player moves around, FPS style.

On the Unity side, just create a plugins directory and drop the plugin from finder into the directory from within Unity (or directly in to the assets folder).
You then need to import the C# interface package located in the Unity folder.

Finally, this may not be obvious, but the plugin only works with OSX.
It may build for linux, but definitely not Windows (pthreads dependency for starters...I haven't looked at
the pthreads for windows port yet)

This plugin is licensed for Open Source distribution using the GNU GPL license.
A copy of the GPL is included in the repository.

@author Daniel J. Finnegan
@date April 2015
