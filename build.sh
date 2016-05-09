#!/bin/bash

# This script just uses xcodebuild to build the repo from the terminal, so you don't have to
# open up Xcode
#
# @author Daniel J. Finnegan
# @date June 2015

cd Xcode/
xcodebuild -workspace Unity-Audio-Plugin.xcworkspace -scheme Unity-Audio-Plugin
