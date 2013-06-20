//
//  EmotionNubView.m
//  
//
//  Created by HCII on 1/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "EmotionNubView.h"

@implementation EmotionNubView

@synthesize cmdDelegate, appDelegate;


- (id)init
{   
    if (self = [super initWithImage:[UIImage imageNamed:@"emotion-nub-02.png"]])
    {
        self.userInteractionEnabled = YES;
        self.appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    }
    
    return self;
}

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*) event
{
    currentPt = [[touches anyObject] locationInView:self];
}

- (void) touchesMoved:(NSSet*)touches withEvent:(UIEvent*) event
{
    CGPoint activePt = [[touches anyObject] locationInView:self];
    
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
    
    /*
    
    float distance = sqrtf( pow(newPt.x - 169, 2) + pow(newPt.y - 169,2) );
     
    NSLog(@"Radius: %f", distance);
    
    if (distance <= 130)
    {
        
    }
     
     */
    
    self.center = newPt;   
}


-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    CGPoint activePt = [[touches anyObject] locationInView:self];
    
    CGPoint newPt = CGPointMake(self.center.x + (activePt.x - currentPt.x), 
                                self.center.y + (activePt.y - currentPt.y));
    
    [self calcEmoteCoordinates:newPt.x:newPt.y];
    
  /*  
    NSString* logStr = [NSString stringWithFormat:@"center: %f %f\r",self.superview.bounds.origin.x, self.superview.bounds.origin.y];
    NSLog( @"%@", logStr );
    
    NSString* logStr2 = [NSString stringWithFormat:@"frame: %f %f\r",self.superview.bounds.size.width, self.superview.bounds.size.width];
    NSLog( @"%@", logStr2 );
    */

}


-(void)calcEmoteCoordinates:(int)x :(int)y
{
    //NSLog(@"%@", [NSString stringWithFormat:@"original %i %i\r", x, y]);
    
    float emoteX = x;
    float emoteY = y;
    
    if (emoteX > 338)
        emoteX = 338;
    
    if (emoteY > 338)
        emoteY = 338;
    
    if (emoteX < 0)
        emoteX = 0;
    
    if (emoteY < 0)
        emoteY = 0;
    
    emoteX = (emoteX - 169) * 0.59;
    emoteY = - (emoteY - 169) * 0.59;  //iOS origin is upper left instead of lower left
    
    [[appDelegate romibo] sendEmoteCmd:roundf(emoteX):roundf(emoteY)];
}



- (void)dealloc
{
    [super dealloc];
}

@end