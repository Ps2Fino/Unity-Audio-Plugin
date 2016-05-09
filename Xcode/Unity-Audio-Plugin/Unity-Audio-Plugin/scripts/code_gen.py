#!/usr/bin/env python
""" 
    This script will parse a C++ source file, extract all the function
    prototypes declared in the file, and then spit out a C++ header file
    that declares function pointers to all the functions.
    Its intended use is for extracting the C API from an Xcode bundle,
    in my specific instance, the bundle representing the Unity 3D audio plugin
    I went with Python for potential future cross platform support, but I'm no guru
    so if you can make it better, then go ahead!
    @author Daniel J. Finnegan
    @date March 2015
"""
import sys
sys.path.append('/usr/local/clang/bindings/python')

import clang.cindex
import argparse
import os.path

"""
    This function uses clang to skip through a TU
    and grab all the methods from the class and save their names
    into a list
"""
def find_functions(node, function_map):
    if node.type.kind == clang.cindex.TypeKind.FUNCTIONPROTO and node.kind == clang.cindex.CursorKind.FUNCTION_DECL:
        # First get the argument list
        func_args = node.get_arguments()
        args_list = []
        for arg_node in func_args:
            if node.type.is_const_qualified():
                # function_map[node.spelling].append('const ' + arg_node.type.spelling)
                args_list.append('const ' + arg_node.type.spelling)
            else:
                # function_map[node.spelling].append(arg_node.type.spelling)
                args_list.append(arg_node.type.spelling)

        # Add the node to the map, recording the function name, the return type and the argument list
        if not args_list:
            function_map[node.spelling] = {'ret_type': node.type.get_result().spelling, 'args_list': 0}
        else:
            function_map[node.spelling] = {'ret_type': node.type.get_result().spelling, 'args_list': args_list}

    for child in node.get_children():
        find_functions(child, function_map)

"""
    This function will write a header containing
    type declarations for the functions in the source file
    Best to just pipe this
"""
def write_header(function_map, output_file_name, output_location, assets_directory, bundle_location):
    with open(output_location + output_file_name, 'w') as output_file:
        output_file.write('#ifndef C_API_FUNCTIONS_UNITY_PLUGIN\n')
        output_file.write('#define C_API_FUNCTIONS_UNITY_PLUGIN\n\n')
        output_file.write('/*\n * This file was generated automatically by a python script.\n' + 
                            ' * Modifying it is futile as it will be auto-generated after every build and your changes will be overwritten.\n' +
                            ' * To change the output, add a function declaration inside the extern "C" block in the plugin interface source file.\n' +
                            ' *\n' +
                            ' * To use the functions, setup a function pointer variable and let it equal to the returned value of the set_$(FUNCTION_NAME) functions\n' +
                            ' *\n' +
                            ' * For example (using the initEnv function):\n' +
                            ' *\n *\tinitEnvFunc initEnv = set_initEnvFunc(functionBundle);\n' +
                            ' *\n' +
                            ' * Also, there is a macro for loading the bundle.\n' +
                            ' *\n'
                            ' * @author Daniel J. Finnegan\n' + 
                            ' * @date March 2015\n' +
                            ' */\n\n')

        output_file.write('#import <CoreFoundation/CoreFoundation.h>\n\n')
        output_file.write('// Set the *FULL PATH* to the bundle on your machine. Use pwd to make sure\n')
        output_file.write('#define BUNDLE_LOCATION "' + bundle_location + '"\n\n')
        output_file.write('// Set the test asset path for loading assets\n')
        output_file.write('#define TEST_ASSET_PATH "' + assets_directory + '"\n\n')
        output_file.write('// Bundle macros for loading the bundle.\n')
        output_file.write('// Remember to retain and release the bundle yourself.\n')
        output_file.write('#define API_BUNDLE_VAR apiBundle\n')
        output_file.write('#define RETAIN_API_BUNDLE_VAR CFRetain(API_BUNDLE_VAR);\n')
        output_file.write('#define RELEASE_API_BUNDLE_VAR CFRelease(API_BUNDLE_VAR);\n')
        output_file.write('#define LOAD_API_BUNDLE \\\n')
        output_file.write('\tCFURLRef bundleURL; \\\n' +
                            '\tCFBundleRef API_BUNDLE_VAR; \\\n' +
                            '\tbundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, CFSTR(BUNDLE_LOCATION), kCFURLPOSIXPathStyle, true); \\\n' +
                            '\tAPI_BUNDLE_VAR = CFBundleCreate(kCFAllocatorDefault, bundleURL); \\\n' +
                            '\tCFRelease(bundleURL); \\\n\n')

        for func in function_map:
            func_ret_type = function_map[func]['ret_type']
            func_args_list = function_map[func]['args_list']

            # I can't find a nicer way to do this, but then again I'm not a python programmer
            arg_string = ''
            if func_args_list != 0:
                arg_string = ''
                for arg in func_args_list[:-1]:
                    arg_string += arg + ', '
                arg_string += func_args_list[-1]

            output_file.write('typedef ' + func_ret_type + ' (*' + func + 'Func)(' + arg_string + ');\n') # This line makes the function pointer
            output_file.write(func + 'Func set_' + func + 'Func(CFBundleRef bundle)\n{\n')
            output_file.write('\treturn (' + func + 'Func) CFBundleGetFunctionPointerForName(bundle, CFSTR("' + func + '"));\n}\n\n')

        output_file.write('#endif\n')

# Executable script
if __name__ == "__main__":

    ##################################################
    ########## Set up ################################
    ##################################################

    # Use an argument parser to make sure we have an argument source file
    arg_parser = argparse.ArgumentParser(description='This script will generate a header file that contains all the functions available in the Unity Plugin Bundle')
    arg_parser.add_argument('source_file', help='The source file containing the functions you want to extract')
    arg_parser.add_argument('assets_dir', help='The assets directory. This will be printed as a macro to the header file')
    arg_parser.add_argument('bundle_location', help='Bundle location. Like the assets directory, this will be printed as a macro in the file')

    # Optional arguments
    arg_parser.add_argument('--output-location', help='The location to place the output header file (defaults to the documents in home directory',
                            default=os.path.expanduser('~') + '/')
    arg_parser.add_argument('-o', '--output-filename', help='The name of the output file name (defaults to apiHeader.hpp)',
                                default='apiHeader.hpp')
    args = arg_parser.parse_args()

    # Check if the source file exists and is indeed a file
    if not os.path.isfile(args.source_file):
        print 'I couldn\'t find the source_file named ' + args.source_file
        sys.exit()

    # Check for trailing /
    output_location = ''
    if not args.output_location.endswith('/'):
        output_location = args.output_location + '/'
    else:
        output_location = args.output_location

    # Check if the home shorthand has been used and
    # create the header file from the list of functions
    if '~' in output_location:
        suffix_dir = output_location.split('~')[1]
        output_location = os.path.expanduser('~') + suffix_dir

    ##################################################
    ##################################################
    ##################################################

    # Tell the user what we're going to do
    print 'Using source file:', args.source_file
    print 'Using output file name:', args.output_filename
    print 'Using output location:', output_location

    # Use clang in two lines to do the heavy work (the actual C++ parsing)
    index = clang.cindex.Index.create()
    tu = index.parse(sys.argv[1])

    # Make the list and pass it in
    function_map = {}
    find_functions(tu.cursor, function_map)

    # write the header file output
    write_header(function_map, args.output_filename, output_location, args.assets_dir, args.bundle_location)

    print 'All done!'
