//
//  ButtonPageBase.m
//  Romibo
//
//  Created by jmcmillan on 7/9/13.
//
//

#import "ButtonPageBase.h"
#import "AppDelegate.h"

@interface ButtonPageBase ()

@end

@implementation ButtonPageBase

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
	// Do any additional setup after loading the view.
    
    [self.view setBackgroundColor:[UIColor clearColor]];
    [self.view setOpaque:NO];
    
    
    
    //self.appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
