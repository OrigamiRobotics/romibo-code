//
//  HeadTiltNubView.h
//  Romibo
//
//  Created by QoLT on 5/28/13.
//
//

#import <UIKit/UIKit.h>
#import "CmdDelegate.h"
#import "AppDelegate.h"

@interface HeadTiltNubView : UIImageView
{
    CGPoint currentPt;
    id appDelegate;
    

}

@property (nonatomic, retain) id <CmdDelegate> cmdDelegate;
@property (nonatomic, assign) id appDelegate;


@end
