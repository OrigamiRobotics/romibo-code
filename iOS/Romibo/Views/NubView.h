//
//  NubView.h
//  
//
//  Created by HCII on 1/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CmdDelegate.h"

@interface NubView : UIImageView
{
    CGPoint currentPt;
}

@property (nonatomic, retain) id <CmdDelegate> cmdDelegate;
@property (nonatomic, retain) NSString* nubType;

-(id)initWithString:(NSString*) nt;

-(void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
-(void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;

-(void)calcCoordinates:(int)x:(int)y;


@end
