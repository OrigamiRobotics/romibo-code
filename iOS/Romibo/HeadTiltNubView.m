//
//  HeadTiltNubView.m
//  Romibo
//
//  Created by QoLT on 5/28/13.
//
//

#import "HeadTiltNubView.h"

@implementation HeadTiltNubView

@synthesize  cmdDelegate;


- (id)init
{
    if (self = [super initWithImage:[UIImage imageNamed:@"movement-nub-02.png"]])
    {
        self.userInteractionEnabled = YES;
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
    
    self.center = newPt;
    
    [self calcDriveCoordinates:newPt.x:newPt.y];
    
}

-(void)calcDriveCoordinates:(int)x:(int)y
{
    float fx = x;
    float fy = y;
    
    float span = 277 - 61;
    
    float tiltX = 100 * ((fx - 61)/span);
    float tiltY = 100 * ((fy - 61)/span);
    
    
    NSString* tCmd = [NSString stringWithFormat:@"current coords %i %i\r", x, y];
    
    NSLog(@"%@", tCmd);
    
    
    [cmdDelegate sendHeadTiltCmd:roundf(tiltX):roundf(tiltY)];
    
}


-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    self.center = CGPointMake(CGRectGetMidX(self.superview.bounds), CGRectGetMidY(self.superview.bounds));
    
    
    [cmdDelegate sendHeadTiltCmd:50:50];
}


- (void)dealloc
{
    [super dealloc];
}

@end
