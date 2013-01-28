//
//  EmotionView.h
//  
//
//  Created by HCII on 1/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface EmotionNubView : UIImageView
{
    CGPoint currentPt;
}


//-(id) initWithImage:(UIImage*) img;

-(void) firstTouch:(NSSet*)touch withEvent:(UIEvent*) event;
-(void) drag:(NSSet*)touch withEvent:(UIEvent*) event;
@end
