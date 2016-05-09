#ifndef C_API_FUNCTIONS_UNITY_PLUGIN
#define C_API_FUNCTIONS_UNITY_PLUGIN

/*
 * This file was generated automatically by a python script.
 * Modifying it is futile as it will be auto-generated after every build and your changes will be overwritten.
 * To change the output, add a function declaration inside the extern "C" block in the plugin interface source file.
 *
 * To use the functions, setup a function pointer variable and let it equal to the returned value of the set_$(FUNCTION_NAME) functions
 *
 * For example (using the initEnv function):
 *
 *	initEnvFunc initEnv = set_initEnvFunc(functionBundle);
 *
 * Also, there is a macro for loading the bundle.
 *
 * @author Daniel J. Finnegan
 * @date March 2015
 */

#import <CoreFoundation/CoreFoundation.h>

// Set the *FULL PATH* to the bundle on your machine. Use pwd to make sure
#define BUNDLE_LOCATION "/Users/danieljfinnegan/Documents/Research-Projects/uap/Xcode/Unity-Audio-Plugin/../../Plugin-Build-Directory/Unity-Audio-Plugin.bundle"

// Set the test asset path for loading assets
#define TEST_ASSET_PATH "/Users/danieljfinnegan/Documents/Research-Projects/uap/Xcode/Unity-Audio-Plugin/Unity-Audio-Plugin-Test-Application/assets"

// Bundle macros for loading the bundle.
// Remember to retain and release the bundle yourself.
#define API_BUNDLE_VAR apiBundle
#define RETAIN_API_BUNDLE_VAR CFRetain(API_BUNDLE_VAR);
#define RELEASE_API_BUNDLE_VAR CFRelease(API_BUNDLE_VAR);
#define LOAD_API_BUNDLE \
	CFURLRef bundleURL; \
	CFBundleRef API_BUNDLE_VAR; \
	bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, CFSTR(BUNDLE_LOCATION), kCFURLPOSIXPathStyle, true); \
	API_BUNDLE_VAR = CFBundleCreate(kCFAllocatorDefault, bundleURL); \
	CFRelease(bundleURL); \

typedef void (*initEnvFunc)();
initEnvFunc set_initEnvFunc(CFBundleRef bundle)
{
	return (initEnvFunc) CFBundleGetFunctionPointerForName(bundle, CFSTR("initEnv"));
}

typedef void (*addSoundToRendererBankAtPositionFunc)(char *, float, float);
addSoundToRendererBankAtPositionFunc set_addSoundToRendererBankAtPositionFunc(CFBundleRef bundle)
{
	return (addSoundToRendererBankAtPositionFunc) CFBundleGetFunctionPointerForName(bundle, CFSTR("addSoundToRendererBankAtPosition"));
}

typedef bool (*activateSoundWithNameFunc)(char *);
activateSoundWithNameFunc set_activateSoundWithNameFunc(CFBundleRef bundle)
{
	return (activateSoundWithNameFunc) CFBundleGetFunctionPointerForName(bundle, CFSTR("activateSoundWithName"));
}

typedef void (*destroyEnvFunc)();
destroyEnvFunc set_destroyEnvFunc(CFBundleRef bundle)
{
	return (destroyEnvFunc) CFBundleGetFunctionPointerForName(bundle, CFSTR("destroyEnv"));
}

typedef bool (*orientListenerInDirectionFunc)(float);
orientListenerInDirectionFunc set_orientListenerInDirectionFunc(CFBundleRef bundle)
{
	return (orientListenerInDirectionFunc) CFBundleGetFunctionPointerForName(bundle, CFSTR("orientListenerInDirection"));
}

typedef bool (*setSoundWithNameToPositionFunc)(char *, float, float);
setSoundWithNameToPositionFunc set_setSoundWithNameToPositionFunc(CFBundleRef bundle)
{
	return (setSoundWithNameToPositionFunc) CFBundleGetFunctionPointerForName(bundle, CFSTR("setSoundWithNameToPosition"));
}

typedef bool (*deactivateSoundInRendererBankFunc)(char *);
deactivateSoundInRendererBankFunc set_deactivateSoundInRendererBankFunc(CFBundleRef bundle)
{
	return (deactivateSoundInRendererBankFunc) CFBundleGetFunctionPointerForName(bundle, CFSTR("deactivateSoundInRendererBank"));
}

typedef void (*copyStringFromManagedCallerFunc)(char *);
copyStringFromManagedCallerFunc set_copyStringFromManagedCallerFunc(CFBundleRef bundle)
{
	return (copyStringFromManagedCallerFunc) CFBundleGetFunctionPointerForName(bundle, CFSTR("copyStringFromManagedCaller"));
}

typedef void (*stopJuceFunc)();
stopJuceFunc set_stopJuceFunc(CFBundleRef bundle)
{
	return (stopJuceFunc) CFBundleGetFunctionPointerForName(bundle, CFSTR("stopJuce"));
}

typedef void (*addSoundToRendererBankFunc)(char *);
addSoundToRendererBankFunc set_addSoundToRendererBankFunc(CFBundleRef bundle)
{
	return (addSoundToRendererBankFunc) CFBundleGetFunctionPointerForName(bundle, CFSTR("addSoundToRendererBank"));
}

typedef int (*checkStatusFunc)();
checkStatusFunc set_checkStatusFunc(CFBundleRef bundle)
{
	return (checkStatusFunc) CFBundleGetFunctionPointerForName(bundle, CFSTR("checkStatus"));
}

typedef void (*initEnvWithHRIRSFunc)(char *);
initEnvWithHRIRSFunc set_initEnvWithHRIRSFunc(CFBundleRef bundle)
{
	return (initEnvWithHRIRSFunc) CFBundleGetFunctionPointerForName(bundle, CFSTR("initEnvWithHRIRS"));
}

typedef void (*startJuceFunc)();
startJuceFunc set_startJuceFunc(CFBundleRef bundle)
{
	return (startJuceFunc) CFBundleGetFunctionPointerForName(bundle, CFSTR("startJuce"));
}

typedef bool (*moveListenerToPositionFunc)(float, float);
moveListenerToPositionFunc set_moveListenerToPositionFunc(CFBundleRef bundle)
{
	return (moveListenerToPositionFunc) CFBundleGetFunctionPointerForName(bundle, CFSTR("moveListenerToPosition"));
}

#endif
