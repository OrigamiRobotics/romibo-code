//
//  DrivingNubView.m
//  
//
//  Created by HCII on 1/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "DrivingNubView.h"
#import "AppDelegate.h"

@implementation DrivingNubView

@synthesize  cmdDelegate, appDelegate;


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
    
    self.center = newPt;
    
    [self calcDriveCoordinates:newPt.x:newPt.y];
    
}

-(void)calcDriveCoordinates :(int)x :(int)y
{
    float fx = x;
    float fy = y;
    
    if (fx > 338)
        fx = 338;
    
    if (fy > 338)
        fy = 338;
    
    if (fx < 0)
        fx = 0;
    
    if (fy < 0)
        fy = 0;
    
    fx = (fx - 169);
    fy = - (fy - 169); //iOS origin is upper left
    
    fx = fx / 108;
    fy = fy / 108;
    
    float driveX = (fx * 100) + (fy * 100);
    float driveY = (fx * -100) + (fy * 100);
    
    
    float larger = [self getLarger:driveX:driveY];
    if (larger >= 99)
    {
        float q = 99/larger;
        driveX = driveX * q;
        driveY = driveY * q;
    }
    
    float smaller = [self getSmaller:driveX:driveY];
    if (smaller <= -99)
    {
        float q = -99/smaller;
        driveX = driveX * q;
        driveY = driveY * q;
    }
        
    [[appDelegate romibo] sendDriveCmd:roundf(driveX):roundf(driveY)];
    
}

-(int)getLarger :(int)i :(int)j
{
    if (i > j)
        return i;
    
    return j;

}
-(int)getSmaller :(int)i :(int)j
{
    if ( i < j)
        return i;
    
    return j;
    
}


-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    self.center = CGPointMake(CGRectGetMidX(self.superview.bounds), CGRectGetMidY(self.superview.bounds));
    [[appDelegate romibo] sendDriveCmd:0:0];
}


- (void)dealloc
{
    [super dealloc];
}

@end