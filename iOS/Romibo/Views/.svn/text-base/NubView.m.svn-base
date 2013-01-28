//
//  NubView.m
//  
//
//  Created by HCII on 1/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "NubView.h"

@implementation NubView

@synthesize cmdDelegate;
@synthesize nubType;

- (id)initWithString:(NSString*) nt
{   
    [self setNubType:nt];
    
    if ([nubType isEqualToString:@"drive"])
        self = [super initWithImage:[UIImage imageNamed:@"movement-nub-02.png"]];
    else
        self = [super initWithImage:[UIImage imageNamed:@"emotion-nub-02.png"]];
    
    if (self)
    {
        self.userInteractionEnabled = YES;
    }
    
    return self;
}

- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*) event
{
    currentPt = [[touches anyObject] locationInView:self];
    
    NSString* logStr = [NSString stringWithFormat:@"center: %i %i\r",self.center.x, self.center.y];
    NSLog( @"%@", logStr );
    
    NSString* logStr2 = [NSString stringWithFormat:@"frame: %i %i\r",self.superview.bounds.size.width, self.superview.bounds.size.width];
    NSLog( @"%@", logStr2 );

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
    
    //NSString* logStr = [NSString stringWithFormat:@"before: %f %f\r",newPt.x, newPt.y];
    //NSLog( @"%@", logStr );
    
    if ([nubType isEqualToString:@"drive"])
        [self calcCoordinates:newPt.x:newPt.y];
    
    
    
}

-(void)calcCoordinates:(int)x:(int)y
{
    //NSString* logStr = [NSString stringWithFormat:@"after: %i %i\r",x, y];
    //NSLog( @"%@", logStr );

    
    float driveY;
    float driveX;
    
    if (x > 277)
        x = 277;
    
    if (y > 277)
        y = 277;
    
    if (x < 61)
        x = 61;
    
    if (y < 61)
        y = 61;
    
    
    if (x >= 169 && y >= 169)
    {
        driveX = - (277 - x) * 0.92;
        driveY = - ((277 - 169) - (277 - y)) * 0.92;
    }
    
    if (x >= 169 && y < 169)
    {
        driveX = ((277 - 169) - (277 - x)) * 0.92;
        driveY = (y - 61) * 0.92;
    }
    
    if (x < 169 && y < 169)
    {
        driveX = (x - 61) * 0.92;
        driveY = ((169 - 61) - (y - 61)) * 0.92;
    }
    
    if (x < 169 && y >= 169)
    {
        driveX = - (169 - x) * 0.92;
        driveY = ((277 - 169) - (y - 169)) * 0.92;
    }
    
    if ([nubType isEqualToString:@"drive"])
        [cmdDelegate sendDriveCmd:roundf(driveX):roundf(driveY)];
    else
        [cmdDelegate sendEmoteCmd:roundf(driveX):roundf(driveY)];
    
    //[cmdDelegate testDriveCmd:x:y:roundf(driveX):roundf(driveY)];
    //[cmdDelegate sendDriveCmd:roundf(driveX):roundf(driveY)];
    
}

-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if ([nubType isEqualToString:@"drive"]) 
    {
        self.center = CGPointMake(CGRectGetMidX(self.superview.bounds), CGRectGetMidY(self.superview.bounds));
        
        [cmdDelegate sendDriveCmd:0:0];
    }
    else
    {
        CGPoint activePt = [[touches anyObject] locationInView:self];
        
        CGPoint newPt = CGPointMake(self.center.x + (activePt.x - currentPt.x), 
                                    self.center.y + (activePt.y - currentPt.y));
        
        [self calcCoordinates:newPt.x:newPt.y];
    }
}


- (void)dealloc
{
    [super dealloc];    
    
    [nubType release];
    nubType = nil;
}



@end
