//
//  ConfigViewController.h
//  
//
//  Created by HCII on 1/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Romibo.h"

@protocol DismissPopoverDelegate
-(void)connectClicked:(NSString*) ipaddr;
-(void)disconnectClicked;
@end

@interface ConfigViewController : UIViewController

{
    IBOutlet UITextField *ipTail;
    IBOutlet UIButton *connectBtn;
    IBOutlet UIButton *disconnectBtn;
}

@property (nonatomic, retain) id <DismissPopoverDelegate> popDelegate;
@property (nonatomic, retain) NSString* ipAddress;

-(IBAction)connectClicked:(id)sender;
-(IBAction)disconnectClicked:(id)sender;
-(void)configureButtonState:(bool) isConnected;
-(void)setTextBoxText:(NSString*) text;

@end
