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

@interface ButtonPage : UIViewController <CommandPickerDelegate>
{
    IBOutlet UILabel *nameLabel;
    
    CommandPickerController *commandPicker;
    UIPopoverController *commandPickerPopover;
    UIButton* lastButtonClicked;
    
    id appDelegate;
}

@property (nonatomic, assign) id appDelegate;

-(void)setupButtons:(NSString*)fileName;
-(void)setupPage:(NSString*)filePath;

@property (retain, nonatomic) NSString* pageName;

//handles the 16 action buttons
-(IBAction)buttonClicked:(id)sender;
-(IBAction)handleLongPress:(id)sender;
-(void)pickCommand:(id)sender;


@end
