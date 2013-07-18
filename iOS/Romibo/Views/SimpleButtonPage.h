//
//  SimpleButtonPage.h
//  Romibo
//
//  Created by jmcmillan on 7/9/13.
//
//

#import <UIKit/UIKit.h>
#import "CommandPickerController.h"

@interface SimpleButtonPage : UIViewController
{
    IBOutlet UILabel *nameLabel;
    
    CommandPickerController *commandPicker;
    UIPopoverController *commandPickerPopover;
    UIButton* lastButtonClicked;
    
    id appDelegate;
}

@property (nonatomic, assign) id appDelegate;

-(void)setupButtons:(NSString*)fileName:(NSString*)directory;
-(void)setupPage:(NSString*)filePath:(NSString*)directory;

@property (retain, nonatomic) NSString* pageName;

//handles the 6 action buttons
-(IBAction)buttonClicked:(id)sender;

@end
