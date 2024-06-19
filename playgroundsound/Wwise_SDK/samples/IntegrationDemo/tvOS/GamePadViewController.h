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
#import "InputMgr.h"
#import "GLView.h"

// Used by both IntegratoinDemo and GameSim
@interface GamePadViewController : UIViewController {
	UITapGestureRecognizer* m_tapSelectRecognizer;
	UITapGestureRecognizer* m_tapMenuRecognizer;
	UITapGestureRecognizer* m_tapPlayPauseRecognizer;
	UISwipeGestureRecognizer* m_swipeRecognizer[4];
	UIView* backgroundView; // Inserted view from caller, can be IntegrationDemo or GameSim main view.
}

@property (nonatomic, strong) UIView *backgroundView;

- (id)initWithButtonSize:(CGFloat) buttonSize marginSize:(CGFloat) marginSize;
@end

