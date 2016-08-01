//
//  UIView+YOView.m
//  YOLayoutExample
//
//  Created by Lucas Yan on 8/1/16.
//  Copyright © 2016 YOLayout. All rights reserved.
//

#import "UIView+YOView.h"
#import <objc/runtime.h>
#import "YOLayout.h"
#import "YOCGUtils.h"

static char kAssociatedLayoutKey;

@implementation UIView (YOView)

+ (void)initLayout {
  [self exchangeImplementations:@selector(initWithFrame:) replacementImplementation:@selector(_initWithFrame:)];
  [self exchangeImplementations:@selector(initWithCoder:) replacementImplementation:@selector(_initWithCoder:)];
  [self exchangeImplementations:@selector(setFrame:) replacementImplementation:@selector(_setFrame:)];
  [self exchangeImplementations:@selector(layoutSubviews) replacementImplementation:@selector(_layoutSubviews)];
  [self exchangeImplementations:@selector(sizeThatFits:) replacementImplementation:@selector(_sizeThatFits:)];
  [self exchangeImplementations:@selector(setNeedsLayout) replacementImplementation:@selector(_setNeedsLayout)];
}

+ (void)exchangeImplementations:(SEL)originalImplementation replacementImplementation:(SEL)replacementImplementation {
  Method originalMethod = class_getInstanceMethod(self, originalImplementation);
  Method swizzledMethod = class_getInstanceMethod(self, replacementImplementation);
  
  BOOL didAddMethod =
  class_addMethod(self,
      originalImplementation,
      method_getImplementation(swizzledMethod),
      method_getTypeEncoding(swizzledMethod));
  
  if (didAddMethod) {
    class_replaceMethod(self,
      replacementImplementation,
      method_getImplementation(originalMethod),
      method_getTypeEncoding(originalMethod));
  } else {
    method_exchangeImplementations(originalMethod, swizzledMethod);
  }
}

- (void)viewInit {
  
}

- (CGSize)layoutWithLayout:(YOLayout *)layout size:(CGSize)size {
  return CGSizeZero;
}

- (void)layoutView {
  [self.layout setNeedsLayout];
  [self.layout layoutSubviews:self.frame.size];
}

- (void)setupDefaultLayout {
  __weak typeof(self) weakSelf = self;
  self.layout = [YOLayout layoutWithLayoutBlock:^CGSize(YOLayout *layout, CGSize size) {
    return [weakSelf layoutWithLayout:layout size:size];
  }];
}

#pragma mark Overriden Methods

- (id)_initWithFrame:(CGRect)frame {
  if ((self = [self _initWithFrame:frame])) {
    [self setupDefaultLayout];
    [self viewInit];
  }
  return self;
}

- (id)_initWithCoder:(NSCoder *)aDecoder {
  if ((self = [self _initWithCoder:aDecoder])) {
    [self setupDefaultLayout];
    [self viewInit];
  }
  return self;
}

- (void)_setFrame:(CGRect)frame {
  if (!YOCGSizeIsEqual(self.frame.size, frame.size)) [self.layout setNeedsLayout];
  [self _setFrame:frame];
}

- (void)_setNeedsLayout {
  [self.layout setNeedsLayout];
  [self.layout layoutSubviews:self.frame.size];
}

#pragma mark Layout

- (void)_layoutSubviews {
  [self _layoutSubviews];
  [self.layout layoutSubviews:self.frame.size];
}

- (CGSize)_sizeThatFits:(CGSize)size {
  if (self.layout) {
    return [self.layout sizeThatFits:size];
  }
  return [self _sizeThatFits:size];
}

#pragma mark Getter/Setter

- (void)setLayout:(YOLayout *)layout {
  objc_setAssociatedObject(self, &kAssociatedLayoutKey, layout, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (YOLayout *)layout {
  return objc_getAssociatedObject(self, &kAssociatedLayoutKey);
}

@end
