//
//  SimpleButtonPage.m
//  Romibo
//
//  Created by jmcmillan on 7/9/13.
//
//

#import "SimpleButtonPage.h"
#import "AppDelegate.h"


@interface SimpleButtonPage ()

@end

@implementation SimpleButtonPage

@synthesize pageName;


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


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    
    
    
    [super dealloc];
}
- (void)viewDidUnload {
    
    [super viewDidUnload];
}
@end

