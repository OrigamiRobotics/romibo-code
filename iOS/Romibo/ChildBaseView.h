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
    
    UIImageView* drivingView;
    
    id appDelegate;

}

@property (nonatomic, assign) id appDelegate;



//silhouette button
-(IBAction)changeShell:(id)sender;
- (void)lockEntered:(NSString*)key;

-(IBAction)buttonClicked:(id)sender;

//driving
-(void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;

@end


