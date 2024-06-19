/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

  Copyright (c) 2023 Audiokinetic Inc.
*******************************************************************************/

#import <UIKit/UIKit.h>
#import "GLView.h"
#import <AVFoundation/AVAudioSession.h>

@class GamePadViewController;

@interface IntegrationDemoAppDelegate : UIResponder <UIApplicationDelegate> {
    UIWindow *window;
	GLView* m_gameView; // Background view to instert into reusable view controller.
	
	bool m_bIsInit;
	
}
@property (nonatomic, strong) UIWindow *window;
@property (nonatomic, strong) GamePadViewController *viewController;
@property (readwrite, strong) AVAudioSession* session; // Use atomic since it is used by multiple threads.

@end

