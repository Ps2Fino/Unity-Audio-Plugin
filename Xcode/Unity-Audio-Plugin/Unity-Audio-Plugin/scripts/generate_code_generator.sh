#!/bin/bash

# This script generates a python script that will in turn
# use clang to generate the bundle interfaace file
#
# Generate all the things!!
#
# @author Daniel J. Finnegan
# @date March 2015

# $1 should be the location of the clang installation
# $2 should be the scripts folder

if [[ $# -ne 2 ]]; then
	echo "I need two arguments; the first is the clang directory and the second is the scripts directory";
	exit;
fi

echo "#!/usr/bin/env python
\"\"\" 
    This script will parse a C++ source file, extract all the function
    prototypes declared in the file, and then spit out a C++ header file
    that declares function pointers to all the functions.
    Its intended use is for extracting the C API from an Xcode bundle,
    in my specific instance, the bundle representing the Unity 3D audio plugin
    I went with Python for potential future cross platform support, but I'm no guru
    so if you can make it better, then go ahead!
    @author Daniel J. Finnegan
    @date March 2015
\"\"\"" > $2/code_gen.py

echo "import sys" >> $2/code_gen.py
echo "sys.path.append('$1/bindings/python')
" >> $2/code_gen.py

# Copy the entire funcDecs.py script over to the code_generator
FUNC_DECS_SCRIPT="$2/funcDecs.py"
cat $2/funcDecs.py >> $2/code_gen.py
echo "All Done!"