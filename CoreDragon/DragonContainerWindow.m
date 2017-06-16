//
//  SPDraggingContainerView.m
//  DragNDropFeature
//
//  Created by Nevyn Bengtsson on 2012-12-10.
//  Copyright (c) 2012 Spotify. All rights reserved.
//

#import "DragonContainerWindow.h"

@implementation DragonContainerWindow
- (instancetype)initWithFrame:(CGRect)frame
{
    if(!(self = [super initWithFrame:frame]))
        return nil;

    self.userInteractionEnabled = NO;
    self.backgroundColor = [UIColor clearColor];
    self.opaque = NO;
	self.windowLevel = UIWindowLevelStatusBar;
	
    self.rootViewController = [DragonContainerViewController new];
    self.rootViewController.view.backgroundColor = [UIColor clearColor];//[UIColor colorWithHue:0.4 saturation:1 brightness:1 alpha:0.2];

    return self;
}

-(void)setStatusBarStyle:(UIStatusBarStyle)style {
    DragonContainerViewController *root = (DragonContainerViewController *)self.rootViewController;
    [root setStatusBarStyle:style];
}

@end

@interface DragonContainerViewController()
@property (nonatomic) UIStatusBarStyle style;
@end

@implementation DragonContainerViewController

-(void)setStatusBarStyle:(UIStatusBarStyle)style {
    self.style = style;
    [self setNeedsStatusBarAppearanceUpdate];
}

-(UIStatusBarStyle)preferredStatusBarStyle {
    return self.style;
}

@end
