//
//  ButtonPage.h
//  Romibo
//
//  Created by jmcmillan on 6/4/13.
//
//

#import <UIKit/UIKit.h>
#import "CommandPickerController.h"
#import "AppDelegate.h"
#import "ButtonPageBase.h"

@interface ButtonPage : ButtonPageBase <CommandPickerDelegate>
{
    //IBOutlet UILabel *nameLabel;
    
    CommandPickerController *commandPicker;
    UIPopoverController *commandPickerPopover;

    
}

//-(void)setupButtons:(NSString*)fileName;
//-(void)setupPage:(NSString*)filePath;



//handles the 16 action buttons
//-(IBAction)buttonClicked:(id)sender;
-(IBAction)handleLongPress:(id)sender;
-(void)pickCommand:(id)sender;


@end
