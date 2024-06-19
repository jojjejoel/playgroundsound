#import "IntegrationDemoAppDelegate.h"
#import "GamePadViewController.h"
#include <AK/SoundEngine/Platforms/iOS/AkiOSSoundEngine.h>    // Platform-specific Sound engine
#include "IntegrationDemo.h"
#include "Helpers.h"
#import "GameRenderer.h"
#import "ViewHelper.h"
#include "Platform.h"
#include "SoundInput.h"

@implementation IntegrationDemoAppDelegate

@synthesize window;
@synthesize viewController;

#pragma mark -
#pragma mark Application lifecycle

extern GameRenderer* g_pOpenGLRenderer;

bool g_bEnableMicDemo = false;

AkAudioSessionCategoryOptions GetAudioSessionCategoryOptionBitMask(bool in_bMixWithOthers, bool in_bDuckOthers, bool in_bAllowBluetooth, bool in_bDefaultToSpeaker)
{
	AkInt32 mask = 0;
	if (in_bMixWithOthers)
	{
		mask |= (1 << 0);
	}
	else
	{
		mask |= (0 << 0);
	}
	
	if (in_bDuckOthers)
	{
		mask |= (1 << 1);
	}
	else
	{
		mask |= (0 << 1);
	}
	
	if (in_bAllowBluetooth)
	{
		mask |= (1 << 2);
	}
	else
	{
		mask |= (0 << 2);
	}
	
	if (in_bDefaultToSpeaker)
	{
		mask |= (1 << 3);
	}
	else
	{
		mask |= (0 << 3);
	}
	
	return (AkAudioSessionCategoryOptions) mask;
}


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    m_bIsInit = false;
	m_gameView = nil;
	
	AkOSChar szError[500];

	AkPlatformInitSettings &platformInitSettings = IntegrationDemo::Instance().GetPlatformInitSettings();
	// Allow other applications to play sounds while the integration demo executes.
//	platformInitSettings.audioSession.eCategory = AkAudioSessionCategoryPlayAndRecord;
	platformInitSettings.audioSession.eCategory = AkAudioSessionCategorySoloAmbient;
//	platformInitSettings.audioSession.eCategory = AkAudioSessionCategoryAmbient;
	
	if (platformInitSettings.audioSession.eCategory == AkAudioSessionCategoryPlayAndRecord)
	{
		g_bEnableMicDemo = true;
		platformInitSettings.audioSession.eCategoryOptions = GetAudioSessionCategoryOptionBitMask(true, false, true, true);
	}
	else
	{
		g_bEnableMicDemo = false;
		platformInitSettings.audioSession.eCategoryOptions = GetAudioSessionCategoryOptionBitMask(true, true, false, false);
	}
	
	// Optimization.
	platformInitSettings.uSampleRate = AUDIO_SAMPLE_RATE;
	g_uSamplesPerFrame = IntegrationDemo::Instance().GetInitSettings().uNumSamplesPerFrame;
	
	platformInitSettings.audioCallbacks.inputCallback = SoundInput::AudioInputCallback;
	platformInitSettings.audioCallbacks.inputCallbackCookie = (void*)&SoundInput::Instance();
	
	// Apple suggests that game apps should ignore route changes unless it is crucial to the gameplay.
	// If your app needs to respond to certain output/input device change scenarios,
	// e.g., Karaoke games may need to respond to a mic change.
	// You can register your own route change notifications, as shown here.
	CGRect appRect;
	[ViewHelper getScreenRegion: appRect];
	
	BOOL bFailedToInitialize = !IntegrationDemo::Instance().Init( NULL,
																 szError,
																 (AkInt32)IntegrationDemoHelpers::AK_ARRAYSIZE(szError),
																 appRect.size.width,
																 appRect.size.height);
	
	// Override point for customization after application launch.
	window = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
	
	viewController = [[GamePadViewController alloc] initWithButtonSize:100 marginSize:40];

	m_gameView = [[GLView alloc] initWithFrame:appRect];
	viewController.backgroundView = m_gameView; // Insert before viewController.loadView() is called.
	
	[[self window] setRootViewController:viewController];
	[[self window] makeKeyAndVisible];

	if ( bFailedToInitialize )
	{
		NSString* message =
			[NSString stringWithFormat:@"Failed to initialize the Integration Demo\n%@", [NSString stringWithCString:szError encoding:[NSString defaultCStringEncoding]]];

		UIAlertController* alert = [UIAlertController alertControllerWithTitle:@"Error"
									   message:message
									   preferredStyle:UIAlertControllerStyleAlert];
		 
		UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault
															  handler:^(UIAlertAction * action) {
			
		}];
		 
		[alert addAction:defaultAction];
		[viewController presentViewController:alert animated:YES completion:nil];
		
		IntegrationDemo::Instance().SetGamePaused(true);
		return NO;
	}
	
	m_bIsInit = YES;
	
    return YES;
}



- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
	
	if( m_gameView != nil )
	{
		[m_gameView stopRendering];
	}
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
   
	/*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
	
	if( m_gameView != nil )
	{
		[m_gameView startRendering];
	}
}


- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
	
	IntegrationDemo::Instance().Term();
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
	[g_pOpenGLRenderer freeTextureCache];
}




@end
