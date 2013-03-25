//
//  ChildBaseView.h
//  Romibo
//
//  Created by QoLT on 3/22/13.
//
//

#import <UIKit/UIKit.h>
#import "CmdDelegate.h"
#import "ViewController.h"

@interface ChildBaseView : UIViewController
{
    
    
    DrawPatternLockViewController *lockVC;
    UIViewController* adultBase;

}


@property (nonatomic, retain) id <CmdDelegate> cmdDelegate;

//silhouette button
-(IBAction)changeShell:(id)sender;
- (void)lockEntered:(NSString*)key;

@end


