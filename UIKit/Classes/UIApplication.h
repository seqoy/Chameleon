/*
 * Copyright (c) 2011, The Iconfactory. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of The Iconfactory nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE ICONFACTORY BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "UIResponder.h"
#import "UIDevice.h"
#import "UIApplicationDelegate.h"
#import "UIApplicationAppKitIntegration.h"

extern NSString *const UIApplicationWillChangeStatusBarOrientationNotification;
extern NSString *const UIApplicationDidChangeStatusBarOrientationNotification;
extern NSString *const UIApplicationWillEnterForegroundNotification;
extern NSString *const UIApplicationWillTerminateNotification;
extern NSString *const UIApplicationWillResignActiveNotification;
extern NSString *const UIApplicationDidEnterBackgroundNotification;
extern NSString *const UIApplicationDidBecomeActiveNotification;
extern NSString *const UIApplicationDidFinishLaunchingNotification;

extern NSString *const UIApplicationLaunchOptionsURLKey;
extern NSString *const UIApplicationLaunchOptionsSourceApplicationKey;
extern NSString *const UIApplicationLaunchOptionsRemoteNotificationKey;
extern NSString *const UIApplicationLaunchOptionsAnnotationKey;
extern NSString *const UIApplicationLaunchOptionsLocalNotificationKey;
extern NSString *const UIApplicationLaunchOptionsLocationKey;

extern NSString *const UIApplicationDidReceiveMemoryWarningNotification;

extern NSString *const UITrackingRunLoopMode;

typedef enum {
  UIStatusBarStyleDefault,
  UIStatusBarStyleBlackTranslucent,
  UIStatusBarStyleBlackOpaque
} UIStatusBarStyle;

/**
 * The animation applied to the status bar as it is hidden or made visible.
 */
typedef enum {
    UIStatusBarAnimationNone,       /*!< No animation is applied to the status bar as it is shown or hidden. */
    UIStatusBarAnimationFade,       /*!< The status bar fades in and out as it is shown or hidden, respectively. */
    UIStatusBarAnimationSlide,      /*!< The status bar slides in or out as it is shown or hidden, respectively. */
} UIStatusBarAnimation;

typedef enum {
    UIInterfaceOrientationPortrait           = UIDeviceOrientationPortrait,
    UIInterfaceOrientationPortraitUpsideDown = UIDeviceOrientationPortraitUpsideDown,
    UIInterfaceOrientationLandscapeLeft      = UIDeviceOrientationLandscapeRight,
    UIInterfaceOrientationLandscapeRight     = UIDeviceOrientationLandscapeLeft
} UIInterfaceOrientation;

#define UIInterfaceOrientationIsPortrait(orientation) \
((orientation) == UIInterfaceOrientationPortrait || \
(orientation) == UIInterfaceOrientationPortraitUpsideDown)

#define UIInterfaceOrientationIsLandscape(orientation) \
((orientation) == UIInterfaceOrientationLandscapeLeft || \
(orientation) == UIInterfaceOrientationLandscapeRight)

// push is not gonna work in mac os, unless you are apple (facetime)
typedef enum {
  UIRemoteNotificationTypeNone    = 0,
  UIRemoteNotificationTypeBadge   = 1 << 0,
  UIRemoteNotificationTypeSound   = 1 << 1,
  UIRemoteNotificationTypeAlert   = 1 << 2
} UIRemoteNotificationType;

// will always be UIApplicationStateActive (for now)
typedef enum {
  UIApplicationStateActive,
  UIApplicationStateInactive,
  UIApplicationStateBackground
} UIApplicationState;

typedef NSUInteger UIBackgroundTaskIdentifier;

extern const UIBackgroundTaskIdentifier UIBackgroundTaskInvalid;
extern const NSTimeInterval UIMinimumKeepAliveTimeout;

@class UIWindow, UIApplication, UILocalNotification;

@interface UIApplication : UIResponder {
@private
    UIEvent *_currentEvent;
    __weak UIWindow *_keyWindow;
    NSMutableSet *_visibleWindows;
    id<UIApplicationDelegate> _delegate;
    BOOL _idleTimerDisabled;
    BOOL _networkActivityIndicatorVisible;
    BOOL _applicationSupportsShakeToEdit;
    NSUInteger _ignoringInteractionEvents;
    NSInteger _applicationIconBadgeNumber;
}

+ (UIApplication *)sharedApplication;

- (BOOL)sendAction:(SEL)action to:(id)target from:(id)sender forEvent:(UIEvent *)event;
- (void)sendEvent:(UIEvent *)event;

- (BOOL)openURL:(NSURL *)url;
- (BOOL)canOpenURL:(NSURL *)URL;

- (void)setStatusBarStyle:(UIStatusBarStyle)statusBarStyle animated:(BOOL)animated;  // no effect

- (void)beginIgnoringInteractionEvents;
- (void)endIgnoringInteractionEvents;
- (BOOL)isIgnoringInteractionEvents;

- (void)presentLocalNotificationNow:(UILocalNotification *)notification;
- (void)cancelLocalNotification:(UILocalNotification *)notification;
- (void)cancelAllLocalNotifications;

@property (nonatomic, readonly) __weak UIWindow *keyWindow;
@property (nonatomic, readonly) NSArray *windows;
@property (nonatomic, getter=isStatusBarHidden, readonly) BOOL statusBarHidden;
@property (nonatomic, readonly) CGRect statusBarFrame;
@property (nonatomic, getter=isNetworkActivityIndicatorVisible) BOOL networkActivityIndicatorVisible;	// does nothing, always returns NO
@property (nonatomic) UIInterfaceOrientation statusBarOrientation;
@property (nonatomic, readonly) NSTimeInterval statusBarOrientationAnimationDuration;
@property (nonatomic, assign) id<UIApplicationDelegate> delegate;
@property (nonatomic, getter=isIdleTimerDisabled) BOOL idleTimerDisabled;	// has no actual affect
@property (nonatomic) BOOL applicationSupportsShakeToEdit;					// no effect
@property (nonatomic) UIStatusBarStyle statusBarStyle;                      // always returns UIStatusBarStyleDefault
@property (nonatomic, readonly) UIApplicationState applicationState;        // always returns UIApplicationStateActive
@property (nonatomic, readonly) NSTimeInterval backgroundTimeRemaining;     // always 0
@property (nonatomic) NSInteger applicationIconBadgeNumber;                 // no effect, but does set/get the number correctly
@property (nonatomic, copy) NSArray *scheduledLocalNotifications;           // no effect, returns nil


/**
 * Hides or shows the status bar, optionally animating the transition.
 @param hidden YES to hide the status bar, NO to show the status bar.
 @param animation A constant that indicates whether there should be an animation and, 
 if one is requested, whether it should fade the status bar in or out or whether 
 it should slide the status bar in or out.
 See the descriptions of the constants of the UIStatusBarAnimation type for more information.
 */
- (void)setStatusBarHidden:(BOOL)hidden withAnimation:(UIStatusBarAnimation)animation;

@end


@interface UIApplication(UIApplicationDeprecated)
- (void)setStatusBarHidden:(BOOL)hidden animated:(BOOL)animated __attribute__((deprecated)); // use -setStatusBarHidden:withAnimation:
@end
