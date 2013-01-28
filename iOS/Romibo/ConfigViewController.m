//
//  ConfigViewController.m
//  
//
//  Created by HCII on 1/5/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ConfigViewController.h"

@implementation ConfigViewController

//@synthesize ipAddress;
@synthesize popDelegate;
@synthesize ipAddress;

-(IBAction)connectClicked:(id)sender
{   
    //NSString* IPstart = [[NSString alloc] initWithString:@"169.254."];
    //ipAddress = [[NSString alloc] initWithString:[IPstart stringByAppendingString:[ipTail text]]];

    [self.popDelegate connectClicked:[ipTail text]];
}

- (IBAction)disconnectClicked:(id)sender 
{
    [self.popDelegate disconnectClicked];
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        self.contentSizeForViewInPopover = CGSizeMake(302, 250);
    }
    return self;
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void) configureButtonState:(bool) isConnected
{   
    if (isConnected)
    {
        [disconnectBtn setEnabled:true];
        [disconnectBtn setTitleColor:[UIColor redColor] forState:UIControlStateNormal];
        
        [connectBtn setEnabled:NO];
        [connectBtn setTitleColor:[UIColor grayColor] forState:UIControlStateNormal];        
    }
    else
    {
        [disconnectBtn setEnabled:NO];
        [disconnectBtn setTitleColor:[UIColor grayColor] forState:UIControlStateNormal];
        
        [connectBtn setEnabled:true];
        [connectBtn setTitleColor:[UIColor greenColor] forState:UIControlStateNormal];
    }
}

-(void) setTextBoxText:(NSString *)text
{
    [ipTail setText:text];
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
     
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)dealloc {
    [ipTail release];
    ipTail = nil;
    
    [connectBtn release];
    connectBtn = nil;
    
    [disconnectBtn release];
    disconnectBtn = nil;

    [super dealloc];
}
@end
