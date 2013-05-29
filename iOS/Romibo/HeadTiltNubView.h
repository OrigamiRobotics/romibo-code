//
//  HeadTiltNubView.h
//  Romibo
//
//  Created by QoLT on 5/28/13.
//
//

#import <UIKit/UIKit.h>
#import "CmdDelegate.h"

@interface HeadTiltNubView : UIImageView
{
    CGPoint currentPt;

}

@property (nonatomic, retain) id <CmdDelegate> cmdDelegate;

@end
