//
//  SPDraggingContainerView.h
//  DragNDropFeature
//
//  Created by Nevyn Bengtsson on 2012-12-10.
//  Copyright (c) 2012 Spotify. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface DragonContainerWindow : UIWindow
- (instancetype)initWithFrame:(CGRect)frame;
- (void)setStatusBarStyle:(UIStatusBarStyle)style;
@end

@interface DragonContainerViewController: UIViewController
- (void)setStatusBarStyle:(UIStatusBarStyle)style;
@end
