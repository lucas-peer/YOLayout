//
//  YOLayout.h
//  YOLayout
//
//  Created by Gabriel Handford on 10/29/13.
//  Copyright (c) 2014 YOLayout. All rights reserved.
//

@import UIKit;

typedef enum {
  YOLayoutOptionsSizeToFit = 1 << 0, // Size to fit the view
  YOLayoutOptionsVariableWidth = 1 << 1,
  
  // Alignment
  // Center the view in the specified size (horizontal + vertical). If you use this with YOLayoutOptionsSizeToFit then the origin width is maintained.
  YOLayoutOptionsCenter = 1 << 2,
  YOLayoutOptionsCenterVertical = 1 << 3, // Center vertically
  YOLayoutOptionsRightAlign = 1 << 4, // After sizing, aligns the view with the right of the passed in rect
  
  // Whether sizeToFit is constrained to the width specified.
  // For example UILabel sizeToFit may return a larger width than was specified,
  // and this will constrain it.
  YOLayoutOptionsSizeToFitConstrainWidth = 1 << 8,
  
  // Whether the size specified is the default. Using this option will
  // use the specified width and/or height (if not 0) when the sizeToFit
  // returns a 0 width.
  // This is useful for an image view that has to load its data and may
  // initially return 0 for sizeThatFits.
  // This option is only available when using
  YOLayoutOptionsSizeToFitDefaultSize = 1 << 9,
  
  // Whether sizeToFit is constrained to the size specified.
  // This is useful for an image view that may need to be constrained to a max
  // size, but still maintain its aspect ratio.
  YOLayoutOptionsSizeToFitConstrainSizeMaintainAspectRatio = 1 << 10,
  
  // Whether width specified will be set as the width. Using this option will
  // use the specified width regardless of the width returned by sizeThatFits.
  YOLayoutOptionsFixedWidth = 1 << 11,
  // Whether width specified will be set as the height. Using this option will
  // use the specified width regardless of the height returned by sizeThatFits.
  YOLayoutOptionsFixedHeight = 1 << 12,
  
} YOLayoutOptions;


@protocol YOLayout <NSObject>

/*!
 Layout the subviews.
 
 @param size Size to layout in
 */
- (CGSize)layoutSubviews:(CGSize)size;

/*!
 Size that fits.
 
 @param size Size to layout in; Should have a width specified.
 @result The size needed to display the view
 */
- (CGSize)sizeThatFits:(CGSize)size;

/*!
 Set frame for the (sub)view.
 If we are calculating sizeThatFits, this doesn't actually set the views frame.
 
 @param frame Frame
 @param view View should conform to YOLView informal protocol.
 */
- (CGRect)setFrame:(CGRect)frame view:(id)view;

/*!
 Set frame for the (sub)view.
 If we are calculating sizeThatFits, this doesn't actually set the views frame.
 
 @param frame Frame
 @param view View should conform to YOLView informal protocol.
 @param needsLayout If YES, calls setNeedsLayout on view.
 */
- (CGRect)setFrame:(CGRect)frame view:(id)view needsLayout:(BOOL)needsLayout;

/*!
 Set the (sub)view frame, then size to fit the view.
 If we are calculating sizeThatFits, this doesn't actually set the views frame.
 Use this value instead of view.frame since the views frame might not have been set.
 
 @param frame Frame
 @param view View should conform to YOLView informal protocol.
 @param sizeToFit Size to fit
 @result The view frame.
 */
- (CGRect)setFrame:(CGRect)frame view:(id)view sizeToFit:(BOOL)sizeToFit;

/*!
 Set the (sub)view frame.
 If we are calculating sizeThatFits, this doesn't actually set the views frame.
 Use this value instead of view.frame since the views frame might not have been set.
 
 @param frame Frame
 @param view View should conform to YOLView informal protocol.
 @param options Options for setFrame; See YOLayoutOptions for more info
 @result The view frame.
 */
- (CGRect)setFrame:(CGRect)frame view:(id)view options:(YOLayoutOptions)options;

/*!
 Set the (sub)view frame.
 If we are calculating sizeThatFits, this doesn't actually set the views frame.
 Use this value instead of view.frame since the views frame might not have been set.
 
 @param frame Frame
 @param inRect Rect to optionally position in for YOLayoutOptionsCenter, YOLayoutOptionsCenterVertical, YOLayoutOptionsRightAlign, etc.
 @param view View
 @param options Options for setFrame; See YOLayoutOptions for more info
 @result The view frame.
 */
- (CGRect)setFrame:(CGRect)frame inRect:(CGRect)inRect view:(id)view options:(YOLayoutOptions)options;

/*!
 Set the (sub)view frame.
 Calls sizeThatFits on the view and then centers it. If inRect.height == 0, we only center horizontally.
 @param inRect The containing rectangle.
 @param view View
 */
- (CGRect)setFrameInRect:(CGRect)inRect view:(id)view;

/*!
 Set origin.
 */
- (CGRect)setOrigin:(CGPoint)origin view:(id)view;
- (CGRect)setOrigin:(CGPoint)origin view:(id)view sizeToFit:(BOOL)sizeToFit;

/*!
 Set size.
 */
- (CGRect)setSize:(CGSize)size view:(id)view;
- (CGRect)setSize:(CGSize)size view:(id)view sizeToFit:(BOOL)sizeToFit;

/*!
 Set origin, x position.
 Use this value instead of view.frame since the views frame might not have been set.
 
 @param x X position
 @param frame Frame
 @param view View should conform to YOLView informal protocol.
 @result The view frame.
 */
- (CGRect)setX:(CGFloat)x frame:(CGRect)frame view:(id)view;

/*!
 Set origin, y position.
 
 Use this value instead of view.frame since the views frame might not have been set.
 
 @param y Y position
 @param frame Frame
 @param view View should conform to YOLView informal protocol.
 @result The view frame.
 */
- (CGRect)setY:(CGFloat)y frame:(CGRect)frame view:(id)view;

/*!
 @deprecated Use setY:frame:view:
 */
- (CGRect)setY:(CGFloat)y view:(UIView *)view;

/*!
 If layout is required. Otherwise cached value may be returned.
 This should be called when a views data changes.
 */
- (void)setNeedsLayout;

/*!
 For subclasses, in rare cases, if they need to know whether the layout will
 be applied or not via setFrame:view:
 
 @result YES if we are only sizing, NO if we are setting frames
 */
- (BOOL)isSizing;

@end


/*
 UIViews can implement this protocol, to enable custom layouts.
 */
@protocol YOLayoutView <NSObject>

/*!
 Layout object belonging to the class implementing this protocol.
 */
@property id <YOLayout>layout;

@end


/*!
 UIViews can also use custom layouts even if they aren't in the view hierarchy (can't do that with Auto Layout!). This protocol presents a convention for how to create drawable views. To create a drawable view, simply create a YOView that implements drawInRect:. Then superviews can draw the view in their drawRect method.
 */
@protocol YODrawableView <YOLayoutView>

/*!
 @result Draw the drawable
 */
- (void)drawInRect:(CGRect)rect;

@end


/*!
 Block containing logic to layout or size the current view, with the specified
 size as a hint. Return the size used.

 You should never setFrame on subviews in this block. Instead use the methods
 in layout in order to setFrame, and use what those methods return to layout other
 subviews. This is because setFrame calls are no-ops when the view is only sizing.

 This block must be implemented.

 @param layout Layout
 @param size Size to layout in
 @result size Size of the view being laid out
 */
typedef CGSize (^YOLayoutBlock)(id<YOLayout> layout, CGSize size);


/*!
 YOLayout is a way to size and layout views without having to implement layoutSubview and
 sizeToFit separately.
 
 It also provides a basic cache to avoid layout when the view is unchanged.
 
 YOLayout calculates a size that best fits the receiver’s subviews,
 without altering the subviews frames, or affecting layoutSubviews call hierarchy.
 
 Instead of defining both sizeThatFits: and layoutSubviews, you create a block named layoutBlock.
 In this block you use the layout instance to set the subview frames (if sizing).

 This prevents your code from altering subviews when you are sizing (for sizeThatFits:).
 
 For example,
 
 CGRect titleLabelFrame = [layout setFrame:CGRectMake(x, y, 400, 0) view:_titleLabel options:YOLayoutOptionsSizeToFit];
 // titleLabelFrame may have a different width and will have a valid height
 
 You can combine YOLayoutOptionsSizeToFit and YOLayoutOptionsCenter to have it be centered with a variable height.
 For example,
 
 CGRect titleLabelFrame = [layout setFrame:CGRectMake(x, y, 400, 0) view:_titleLabel options:YOLayoutOptionsSizeToFit|YOLayoutOptionsCenter];
 // titleLabelFrame may have a different width and will have a valid height and will have an x position so it is centered
 
 You can also combine YOLayoutOptionsSizeToFit with YOLayoutOptionsConstraintWidth
 to make sure the width isn't set larger than expected. For example,
 
 CGRect titleLabelFrame = [layout setFrame:CGRectMake(x, y, 400, 0) view:_titleLabel options:YOLayoutOptionsSizeToFit|YOLayoutOptionsConstraintWidth];
 // titleLabelFrame may have a different width but it won't be larger than 400
 
 You can combine YOLayoutOptionsSizeToFit, YOLayoutOptionsConstraintWidth, and YOLayoutOptionsDefaultWidth to make sure
 a view sizes to fit with max and default width (when 0).
 */
@interface YOLayout : NSObject <YOLayout> {
  BOOL _needsLayout;
  BOOL _needsSizing;
  CGSize _cachedSize;
  CGSize _cachedLayoutSize;
  
  CGSize _sizeThatFits;
}

@property (readonly, nonatomic, getter=isSizing) BOOL sizing;
//! Block containing logic to layout or size the current view. See the discussion above the YOLayoutBlock typedef for more info.
@property (nonatomic, copy) YOLayoutBlock layoutBlock;

/*!
 Set a custon/fixed size that fits.
 Override the size that is returned by sizeThatFits:(CGSize)size.
 Defaults to CGSizeZero, which is unset.
 If height is not set (is 0), then we will use this size value for sizeThatFits:.
 */
@property (assign, nonatomic) CGSize sizeThatFits;

/*!
 Create layout.

 @param layoutBlock Block containing layout code. See the discussion above the YOLayoutBlock typedef for more info.
 */
- (id)initWithLayoutBlock:(YOLayoutBlock)layoutBlock;

/*!
 Default layout.

 @param view View for layout (weak reference).
 @param layoutBlock Block containing layout code. See the discussion above the YOLayoutBlock typedef for more info.
 @result Layout
 */
+ (YOLayout *)layoutWithLayoutBlock:(YOLayoutBlock)layoutBlock;

/*!
 Assert layout parameters are correct.
 */
void YOLayoutAssert(UIView *view, id<YOLayout> layout);

@end
