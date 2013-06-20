//
//  DrivingNubView.h
//  
//
//  Created by HCII on 1/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CmdDelegate.h"

@interface DrivingNubView : UIImageView
{
    CGPoint currentPt;
    
    id appDelegate;
    
}


@property (nonatomic, assign) id appDelegate;

@property (nonatomic, retain) id <CmdDelegate> cmdDelegate;

-(void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
-(void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;

-(void)calcDriveCoordinates :(int)x :(int)y;
-(int)getLarger :(int)i :(int)j;
-(int)getSmaller :(int)i :(int)j;

@end

