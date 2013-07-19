//
//  ButtonPage.m
//  Romibo
//
//  Created by jmcmillan on 6/4/13.
//
//

#import "ButtonPage.h"
#import "AppDelegate.h"
#import "CommandPickerController.h"

@interface ButtonPage ()

@end


@implementation ButtonPage


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    [self.view setBackgroundColor:[UIColor clearColor]];
    [self.view setOpaque:NO];
    
}



-(IBAction)handleLongPress:(UILongPressGestureRecognizer*)gesture
{
    UIButton* button = (UIButton *) gesture.view;
    [self pickCommand:button];
    
}

- (void)commandSelected:(NSString *)cmd {
    
    [lastButtonClicked setTitle:cmd forState:UIControlStateNormal];
    
    [commandPickerPopover dismissPopoverAnimated:YES];
}

-(void)pickCommand:(id)sender {
    
    lastButtonClicked = sender;
    
    if (commandPicker == nil) {
        
        commandPicker = [[CommandPickerController alloc]
                         initWithStyle:UITableViewStylePlain];
        
        [commandPicker setDelegate:self];
        
        commandPickerPopover = [[UIPopoverController alloc]
                                initWithContentViewController:commandPicker];
        
    }
    
    [commandPickerPopover presentPopoverFromRect:[sender frame]
                                          inView:[self view]
                        permittedArrowDirections:UIPopoverArrowDirectionAny
                                        animated:YES];
    
}



- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    
    
    [commandPicker release];
    commandPicker = nil;
    
    
    [super dealloc];
}
- (void)viewDidUnload {
 
    [super viewDidUnload];
}
@end
