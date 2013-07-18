//
//  ButtonPageBase.h
//  Romibo
//
//  Created by jmcmillan on 7/9/13.
//
//

#import <UIKit/UIKit.h>
#import "CommandPickerController.h"

@interface ButtonPageBase : UIViewController
{

    CommandPickerController *commandPicker;
    UIPopoverController *commandPickerPopover;
    UIButton* lastButtonClicked;

    id appDelegate;
}


@end
