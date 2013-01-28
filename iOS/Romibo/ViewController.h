//
//  ViewController.h
//  Romibo
//
//  Created by HCII on 12/31/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Romibo.h"
#import "ConfigViewController.h"
#import <Foundation/Foundation.h>

#import "EmotionNubView.h"
#import "DrivingNubView.h"

@interface ViewController : UIViewController <UIPopoverControllerDelegate, DismissPopoverDelegate>

{
    IBOutlet UILabel *connectionLabel;
    UIPopoverController* configPopover;
    Romibo* romibo; 
    NSTimer* cmdTimer;
    NSTimer* connectionStatusTimer;
    EmotionNubView* eNub;
    DrivingNubView* dNub;
    //NubView* eNub;
    //NubView* dNub;
}


//handles the 8 action buttons
-(IBAction)buttonClicked:(id)sender;

//gear button
-(IBAction)configClicked:(id)sender;
-(void)closePopup;
-(void)connectClicked:(NSString*) ipaddr;
-(void)disconnectClicked;
-(void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController;


-(void)setupEmotionSubview;
-(void)setupDrivingSubview;


-(void)setConnectionStatus;

@end
