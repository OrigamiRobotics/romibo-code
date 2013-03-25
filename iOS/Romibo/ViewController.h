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
#import "CommandPickerController.h"
#import "DrawPatternLockViewController.h"
#import "DrawPatternLockView.h"

@class ChildBaseView;

@interface ViewController : UIViewController <UIPopoverControllerDelegate, DismissPopoverDelegate, CommandPickerDelegate>

{
    IBOutlet UILabel *connectionLabel;
    UIPopoverController* configPopover;
    Romibo* romibo; 
    NSTimer* cmdTimer;
    NSTimer* connectionStatusTimer;
    
    EmotionNubView* eNub;
    DrivingNubView* dNub;

    CommandPickerController *commandPicker;
    UIPopoverController *commandPickerPopover;
    UIButton* lastButtonClicked;
    
    NSMutableDictionary* romiboCommands;
    NSMutableArray* buttonLabels;
    
    UILongPressGestureRecognizer* longPressRecognizer;
    
    DrawPatternLockViewController *lockVC;
    ChildBaseView* childView;

}


//handles the 8 action buttons
-(IBAction)buttonClicked:(id)sender;
-(IBAction)handleLongPress:(id)sender;
-(void)pickCommand:(id)sender;

//gear button
-(IBAction)configClicked:(id)sender;
-(void)closePopup;
-(void)connectClicked:(NSString*) ipaddr;
-(void)disconnectClicked;
-(void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController;

//silhouette button
-(IBAction)changeShell:(id)sender;
- (void)lockEntered:(NSString*)key;

-(void)setupEmotionSubview;
-(void)setupDrivingSubview;

-(void)setConnectionStatus;

@end
