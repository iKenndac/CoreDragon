#import <UIKit/UIKit.h>
@protocol DragonDelegate, DragonDropDelegate, DragonProxyView;

NS_ASSUME_NONNULL_BEGIN

/// Controller for managing drag an drop between views (possibly between applications).
@interface DragonController : NSObject <UIGestureRecognizerDelegate>

/*! Get the shared DragonController. Only use this singleton: don't instantiate
	more of them. */
+ (instancetype)sharedController;

#pragma mark Gesture handling

/*! To enable drag'n'drop within your application, some gesture must be used to start
	a dragging operation. You can enable the default 'long-press-then-drag' gesture
	by calling `allowLongPressDraggingInWindow:` with your app's main window
	(or any other window).
	
	@see also `dragGesture:`
	*/
- (void)enableLongPressDraggingInWindow:(UIWindow*)window;
/*! Uninstall the 'long-press-then-drag' gesture from your window. */
- (void)disableLongPressDraggingInWindow:(UIWindow*)window;

/*! To enable a custom drag'n'drop gesture, set this method as your gesture
	recognizer's action method. */
- (void)dragGesture:(UIGestureRecognizer*)grec;

#pragma mark Drag sources

/*! Allow drags to be started from the 'draggable' UIView. The given delegate
	will be asked to customize this drag (by providing the data to be dragged, etc)
	if/when a drag starts. */
- (void)registerDragSource:(UIView *)draggable delegate:(id<DragonDelegate>)delegate;
/*! Stop allowing drags from this view.*/
- (void)unregisterDragSource:(UIView *)draggable;

#pragma mark Drop targets

/*! Allow drags to end up in the 'droppable' UIView. The given delegate
	will be asked to accept the drop data if/when a drag ends over this view. */
- (void)registerDropTarget:(UIView *)droppable delegate:(id<DragonDropDelegate>)delegate;
/*! Stop allowing drops to this view. */
- (void)unregisterDropTarget:(id)droppableOrDelegate;

#pragma mark Misc
/*!	Is the user currently dragging something, in this app or any other app? */
- (BOOL)draggingOperationIsInProgress;

/*! Recalculates active drop targets, e.g. to allow for refreshing cells in scrolling table view. */
- (void)recalculateActiveDropTargets;

/*! Explicitely invalidates and redraws a highlight for a specific view. */
- (void)invalidateHighlightForView:(UIView *)view;

@end

/// Information about a dragging operation that is about to start or is in progress.
@protocol DragonInfo <NSObject>
@property(nonatomic,readonly) UIPasteboard *pasteboard;

/*! Optional drag context. Not given to other apps — only used within the local app.
         Examples to use this would be when two closely-coupled parts of your app (or a
         single part) want to do drag and drop and having the original object is important.
         For example, when reordering items in a collection view, or dropping things onto
         a map. To pass data further away or to other apps, use the pasteboard instead.
 */

@property(nonatomic,readwrite,strong) id localContext;

// Can only be set during 'beingDragOperation:fromView:'
/*! An icon to represent the data you just put in pasteboard. If not set (and no localProxyView
         is provided), the drag will be represented by a screenshot of the dragged view. */
@property(nonatomic,strong,nullable) UIImage *draggingIcon;
/*! If draggingIcon is set, you can optionally also set a title to be shown next
	to the icon while dragging. */
@property(nonatomic,copy,nullable) NSString *title;
/*! And additionally, a subtitle can be displayed below the title. */
@property(nonatomic,copy,nullable) NSString *subtitle;


/**
 If provided, this view will be used to represent the drag during the operation.
 
 @note It's important to also set a draggingIcon image, as your custom view will
 not make it over to the other application when dragging across the splitscreen divide 
 on iPad. The draggingIcon and title etc will be used to construct a new image.
 */
@property(nonatomic,strong,nullable) UIView<DragonProxyView> *localProxyView;
@end

/// A view that's placed onscreen to show the contents of the drag during the drag operation.
@protocol DragonProxyView <NSObject>

/**
 Called just before a drag operation will begin. 
 
 @note The view is unlikely to have a superview at this point.

 @param dragOperation The operation that's about to begin.
 */
- (void)prepareForDragOperation:(id<DragonInfo>)dragOperation;

/**
 Called when the view needs to animate in at the beginning of a drag. A simple example is a fade in.
 
 @note This view will have a superview when this method is called, and the view's layer will be positioned such that
 its anchor point is underneath the user's finger.

 @note If you intend to animate your custom drag view in, it's important to set initial state in -prepareForDragOperation:. 
 For instance, if you're going to fade in, set your view's alpha to 0 beforehand.

 @param suggestedDuration A suggested animation duration.
 @param completion A block to be called when the animation is complete.
 */
- (void)animateInWithSuggestedDuration:(NSTimeInterval)duration completion:(dispatch_block_t)completion;

/**
 Called when the view needs to animate out at the end of a drag. A simple example is a fade out.

 @param dragWasSuccessful `YES` if the drag was successfully dropped, `NO` if the drag was cancelled.
 @param suggestedDuration A suggested animation duration.
 @param completion A block to be called when the animation is complete.
 */
- (void)animateOutForSuccess:(BOOL)dragWasSuccessful suggestedDuration:(NSTimeInterval)duration completion:(dispatch_block_t)completion;
@end

@protocol DragonDelegate <NSObject>
@required
/*!
	Dragging was just initiated from `draggable`. Put the object(s) to be dragged onto
	the pasteboard in `drag`. Not doing so will cancel the drag.
*/
- (void)beginDragOperation:(id<DragonInfo>)drag fromPoint:(CGPoint)point inView:(UIView*)draggable;

@optional

/// Return `NO` to disable the "snap back" animation when a drag is cancelled.
- (BOOL)shouldSnapBackCancelledDragOperation:(id<DragonInfo>)drag;

/// Delegate methods for the source to react to cancelled and completed drags.
- (void)dragOperationDidCancel:(id<DragonInfo>)drag;
- (void)dragOperationDidComplete:(id<DragonInfo>)drag;

@end

@protocol DragonDropDelegate <NSObject>
@required
/// Whether you can drop the items on on `droppable` or not (either for accepting the drop, or for springloading)
- (BOOL)dropTarget:(UIView*)droppable canAcceptDrag:(id<DragonInfo>)drag;

@optional
/// Default 'YES' if `dropTarget:acceptDrag:atPoint:` is implemented, otherwise NO
- (BOOL)dropTarget:(UIView*)droppable shouldAcceptDrag:(id<DragonInfo>)drag;
- (void)dropTarget:(UIView*)droppable acceptDrag:(id<DragonInfo>)drag atPoint:(CGPoint)p;

/// Default 'YES' if `dropTarget:springload:atPoint:` is implemented, otherwise NO
- (BOOL)dropTarget:(UIView*)droppable shouldSpringload:(id<DragonInfo>)drag;
- (void)dropTarget:(UIView*)droppable springload:(id<DragonInfo>)drag atPoint:(CGPoint)p;

// If you want to customize the highlight shown for a drag based on where the finger is right now
// (e g for doing out-of-edit drag rearrangement in table views), implement this method.
- (void)dropTarget:(UIView *)droppable updateHighlight:(UIView*)highlightContainer forDrag:(id<DragonInfo>)drag atPoint:(CGPoint)p;
@end

/// Sent when a drag operation starts.
static NSString *const DragonDragOperationStartedNotificationName = @"eu.thirdcog.dragon.dragStarted";
/// Sent when a drag operation stops.
static NSString *const DragonDragOperationStoppedNotificationName = @"eu.thirdcog.dragon.dragStopped";

NS_ASSUME_NONNULL_END
