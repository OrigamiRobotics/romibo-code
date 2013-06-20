//
//  EmotionNubView.h
//  
//
//  Created by HCII on 1/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CmdDelegate.h"
#import "AppDelegate.h"

@interface EmotionNubView : UIImageView
{
    CGPoint currentPt;
    
    id appDelegate;

}

@property (nonatomic, retain) id <CmdDelegate> cmdDelegate;

@property (nonatomic, assign) id appDelegate;

-(void)calcEmoteCoordinates:(int)x :(int)y;

-(void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
-(void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
@end
