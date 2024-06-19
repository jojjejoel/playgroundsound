#import "GamePadViewController.h"
#import "ViewHelper.h"
#import <QuartzCore/QuartzCore.h> // needed by setCornerRadius... otherwise buttons will look ugly
#include "Platform.h"

extern UGBtnState g_btnState;
extern UGStickState g_sticksState[2];

@implementation GamePadViewController
@synthesize  backgroundView;


- (id)initWithButtonSize:(CGFloat) buttonSize marginSize:(CGFloat) marginSize
{
    self = [super init];
    if (self) {
		backgroundView = nil;
    }
    return self;
}

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView
{
	CGRect appRect;
	[ViewHelper getScreenRegion: appRect];
	
	CGRect viewRect = CGRectMake(appRect.origin.x, appRect.origin.y, appRect.size.width, appRect.size.height);

	self.view =[[UIView alloc] initWithFrame:viewRect];
	self.view.alpha = 1.0;
	self.view.opaque = NO;
	self.view.autoresizesSubviews = false;
	self.view.autoresizingMask = false;
	
	// Do not instantiate backgroundView, it is assigned by caller.

	if( backgroundView != nil )
	{
		[self.view addSubview:backgroundView];
	}
	
	for (int d = 0; d < 4; d++)
	{
		m_swipeRecognizer[d] = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(onSwipeGesture:)];
		[m_swipeRecognizer[d] setDirection:(UISwipeGestureRecognizerDirection)(1 << d)];
		[[self view] addGestureRecognizer:m_swipeRecognizer[d]];
	}
	
	m_tapSelectRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onTapSelectGesture:)];
	[m_tapSelectRecognizer setAllowedPressTypes:@[[NSNumber numberWithInteger:UIPressTypeSelect]]];
	[[self view] addGestureRecognizer:m_tapSelectRecognizer];
	
	m_tapMenuRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onTapMenuGesture:)];
	[m_tapMenuRecognizer setAllowedPressTypes:@[[NSNumber numberWithInteger:UIPressTypeMenu]]];
	[[self view] addGestureRecognizer:m_tapMenuRecognizer];
	
	m_tapPlayPauseRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(onTapPlayPauseGesture:)];
	[m_tapPlayPauseRecognizer setAllowedPressTypes:@[[NSNumber numberWithInteger:UIPressTypePlayPause]]];
	[[self view] addGestureRecognizer:m_tapPlayPauseRecognizer];
	
	g_btnState = 0;
	memset(g_sticksState, 0, sizeof(g_sticksState));
}

- (void)onSwipeGesture:(UISwipeGestureRecognizer *)sender
{
	if (sender.state == UIGestureRecognizerStateEnded)
	{
		memset(g_sticksState, 0, sizeof(g_sticksState));
		switch ([sender direction])
		{
			case UISwipeGestureRecognizerDirectionLeft:
				g_btnState |= UG_DPAD_LEFT;
				break;
			case UISwipeGestureRecognizerDirectionRight:
				g_btnState |= UG_DPAD_RIGHT;
				break;
			case UISwipeGestureRecognizerDirectionUp:
				g_btnState |= UG_DPAD_UP;
				break;
			case UISwipeGestureRecognizerDirectionDown:
				g_btnState |= UG_DPAD_DOWN;
				break;
		}
	}
}

- (void)onTapSelectGesture:(UITapGestureRecognizer *)sender
{
	if (sender.state == UIGestureRecognizerStateEnded)
	{
		g_btnState |= UG_BUTTON1; // Confirm button
	}
}

- (void)onTapMenuGesture:(UITapGestureRecognizer *)sender
{
	if (sender.state == UIGestureRecognizerStateEnded)
	{
		g_btnState |= UG_BUTTON2; // Back button
	}
}

- (void)onTapPlayPauseGesture:(UITapGestureRecognizer *)sender
{
	if (sender.state == UIGestureRecognizerStateEnded)
	{
		g_btnState |= UG_BUTTON7; // Start button
	}
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}

- (void)dealloc {
	[backgroundView removeFromSuperview];
	
}

// WG-25720: iOS 8 callback.
- (void)viewWillTransitionToSize:(CGSize)size
	   withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
	[super viewWillTransitionToSize: size withTransitionCoordinator: coordinator];
}

// WG-25720: iOS 8 callback.
- (void)viewWillLayoutSubviews
{
	[super viewWillLayoutSubviews];
}

@end
