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
    
    UIImageView* drivingView;

}

@property (nonatomic, retain) Romibo* romibo;
@property (nonatomic, retain) NSMutableDictionary* romiboCommands;

//silhouette button
-(IBAction)changeShell:(id)sender;
- (void)lockEntered:(NSString*)key;

-(IBAction)buttonClicked:(id)sender;

//driving
-(void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
-(void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;

@end


