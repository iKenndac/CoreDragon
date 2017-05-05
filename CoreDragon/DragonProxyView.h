//
//  SPDraggingProxyView.h
//  DragNDropFeature
//
//  Created by Nevyn Bengtsson on 2012-12-14.
//  Copyright (c) 2012 Spotify. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DragonController.h"

/// A simple premade proxy view that shows an icon, and optionally a title.
@interface DragonProxyView : UIView <DragonProxyView>
- (instancetype)initWithIcon:(UIImage*)icon title:(NSString*)title subtitle:(NSString*)subtitle;
@end

// The fallback proxy view that just displays a view snapshot.
@interface DragonScreenshotProxyView : UIImageView <DragonProxyView>
- (instancetype)initWithScreenshot:(UIImage *)screenshot;
@end
