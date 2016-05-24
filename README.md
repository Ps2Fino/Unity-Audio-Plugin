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

I'd greatly appreciate it if you could include the following referral or citation if you ever use this work:

For LaTeX users:

@inproceedings{Finnegan:2016:CDC:2858036.2858065,
 author = {Finnegan, Daniel J. and O'Neill, Eamonn and Proulx, Michael J.},
 title = {Compensating for Distance Compression in Audiovisual Virtual Environments Using Incongruence},
 booktitle = {Proceedings of the 2016 CHI Conference on Human Factors in Computing Systems},
 series = {CHI '16},
 year = {2016},
 isbn = {978-1-4503-3362-7},
 location = {Santa Clara, California, USA},
 pages = {200--212},
 numpages = {13},
 url = {http://doi.acm.org/10.1145/2858036.2858065},
 doi = {10.1145/2858036.2858065},
 acmid = {2858065},
 publisher = {ACM},
 address = {New York, NY, USA},
 keywords = {binaural audio, distance perception, head mounted display, incongruent display, spatial audio, virtual environment},
}

For everyone else:
**Daniel J. Finnegan, Eamonn O'Neill, Michael J. Proulx**</br>
Compensating for Distance Compression in Audiovisual Virtual Environments Using Incongruence</br>
*Proceedings of the 2016 CHI Conference on Human Factors in Computing Systems*

Daniel J. Finnegan
April 2015
