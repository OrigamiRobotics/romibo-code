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
    IBOutlet UILabel *nameLabel;

    id appDelegate;
    
    UIButton* lastButtonClicked;
}

@property (nonatomic, assign) id appDelegate;

@property (retain, nonatomic) NSString* pageName;

-(void)setupPage:(NSString*)filePath;
//-(void)setupButtons:(NSString*)fileName:(NSString*) directory;
-(IBAction)buttonClicked:(id)sender;

@end
