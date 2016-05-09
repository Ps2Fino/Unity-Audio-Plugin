#!/bin/bash
EXECFILE=${BUILT_PRODUCTS_DIR}/${EXECUTABLE_PATH}
LIBPATH=${BUILT_PRODUCTS_DIR}/${SHARED_SUPPORT_FOLDER_PATH}
NEWLIBPATH="@loader_path/../SharedSupport"

# echo `ls ${EXECFILE}`

# space separated list of libraries
TARGETS="libsndfile.1.dylib libFLAC.8.dylib libogg.0.dylib libvorbis.0.dylib libvorbisenc.2.dylib libfftw3.3.dylib libfftw3f_threads.3.dylib libfftw3f.3.dylib libfftw3l_threads.3.dylib libfftw3l.3.dylib"

# Chane where the plugin expects to load libraries at run time from
for TARGET in ${TARGETS} ; do
	# CURR_VAL=`otool -L ${EXECFILE} | grep ${TARGET} | awk -F'[ (]' '{print $1}'`
	CURR_VAL=`otool -DX ${LIBPATH}/${TARGET}`
	echo 'Using CURR_VAL' ${CURR_VAL}
	install_name_tool -change ${CURR_VAL} ${NEWLIBPATH}/${TARGET} ${EXECFILE}
done