//
//  EmotionView.m
//  
//
//  Created by HCII on 1/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "EmotionNubView.h"

@implementation EmotionNubView

- (id)initwithImage:(UIImage*) img
{
    
    if (self = [super initWithImage:img])
    {
        self.userInteractionEnabled = TRUE;
    }
    
    return self;
}

- (void) firstTouch:(NSSet*)touch withEvent:(UIEvent*) event
{
    currentPt = [[touch anyObject] locationInView:self];
}

- (void) drag:(NSSet*)touch withEvent:(UIEvent*) event
{
    CGPoint activePt = [[touch anyObject] locationInView:self];
    
    CGPoint newPt = CGPointMake(self.center.x + (activePt.x - currentPt.x), 
                                self.center.y + (activePt.y - currentPt.y));
    
    float midPointX = CGRectGetMidX(self.bounds);
    
    if (newPt.x > self.superview.bounds.size.width - midPointX)
        newPt.x = self.superview.bounds.size.width - midPointX;
    else if (newPt.x < midPointX)
        newPt.x = midPointX;
    
    float midPointY = CGRectGetMidY(self.bounds);
    
    if (newPt.y > self.superview.bounds.size.height - midPointY)
        newPt.y = self.superview.bounds.size.height - midPointY;
    else if (newPt.y < midPointY)
        newPt.y = midPointY;
    
    self.center = newPt;
}



- (void)dealloc
{
    [super dealloc];
}

@end
