// IntegrationDemoAppDelegate.mm
// Copyright (C) 2020 Audiokinetic Inc.
/// \file 
/// Application delegate for the demo.

#import "IntegrationDemoAppDelegate.h"
#include "IntegrationDemo.h"
#include "Helpers.h"
#include "Drawing.h"


@interface IntegrationDemoAppDelegate(PrivateMethods)
-(void) InitDrawing;
@end

@implementation IntegrationDemoAppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification 
{	
	AkOSChar szError[500];
		
	g_uSamplesPerFrame = IntegrationDemo::Instance().GetInitSettings().uNumSamplesPerFrame;
	
	if ( !IntegrationDemo::Instance().Init( NULL, szError, (unsigned int)IntegrationDemoHelpers::AK_ARRAYSIZE(szError) ))
	{
		// Something went bad in the initialization
		// Post a modal alert and quit!
		NSAlert *alert =
			[NSAlert
				alertWithMessageText:@"Failed to initialize the Integration Demo"
				defaultButton:NSLocalizedString(@"OK", @"")
				alternateButton:nil
				otherButton:nil
				informativeTextWithFormat:@"%@", [NSString stringWithCString:szError encoding:[NSString defaultCStringEncoding]]];
		
		[alert runModal];
		[alert release];
		
		[NSApp terminate:self];
		
		return;
	}
	[self InitDrawing];
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
	// Release the gameview
	// Important because this is the one that call render frame on the IntegrationDemo instance
	[m_gameView removeFromSuperview];
	[m_gameView release];
	
	// Unregister to notifications... otherwise applicationWillTerminate might be called twice
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	
	// Terminate the integration demo (will terminate the wwise soundengine)
	IntegrationDemo::Instance().Term();	
}

-(void) InitDrawing
{
	
	CGFloat width = IntegrationDemo::Instance().GetWindowWidth();
	CGFloat height = IntegrationDemo::Instance().GetWindowHeight();

	
	NSRect windowRect = NSMakeRect(0, 0, width, height);
	
	m_window = [[NSWindow alloc] initWithContentRect:windowRect
										   styleMask:NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask backing:NSBackingStoreBuffered
											   defer:false];
	
	[m_window setDelegate:self];
	
	m_gameView = [[NSGLView alloc] initWithFrame:windowRect];
	
	[m_window setContentView:m_gameView];
	[m_window makeFirstResponder:m_gameView];
	
	// Remember window position
	[m_window setFrameAutosaveName:@"IntegrationDemoWindow"];
	
	m_window.isVisible = YES;
	
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(windowWillClose:) name:NSWindowWillCloseNotification
											   object:nil];

	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(windowsDidMiniaturize:) name:NSWindowDidMiniaturizeNotification
											   object:nil];

	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(windowsDidDeminiaturize:) name:NSWindowDidDeminiaturizeNotification
											   object:nil];
}

- (void)windowsDidMiniaturize:(NSNotification *)NSNotification
{
	IntegrationDemo::Instance().PauseAllSounds();
}

- (void)windowsDidDeminiaturize:(NSNotification *)NSNotification
{
	IntegrationDemo::Instance().ResumeAllSounds();
}

- (void)windowWillClose:(NSNotification *)notification
{
	[[NSRunLoop currentRunLoop] performSelector:@selector(terminate:) 
										 target:NSApp
									   argument:self 
										  order:100 
										  modes: [NSArray arrayWithObject:NSDefaultRunLoopMode]];
}

@end
