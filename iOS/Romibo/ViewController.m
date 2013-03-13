//
//  ViewController.m
//  Romibo
//
//  Created by HCII on 12/31/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import "ViewController.h"
#import "EmotionNubView.h"
#import "DrivingNubView.h"
#import "ConfigViewController.h"
#import "Romibo.h"
#import "CmdDelegate.h"


@implementation ViewController 

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    self.view.backgroundColor = [[UIColor alloc] initWithPatternImage:[UIImage imageNamed:@"background-01.png"]];
    
    
    romibo = [[Romibo alloc] init];
    [self closePopup];
    
    if (!cmdTimer)
    {
        cmdTimer = [NSTimer scheduledTimerWithTimeInterval:0.1 target:romibo selector:@selector(handleCmdTimer) userInfo:nil repeats:true];
    }
    
    [self setupEmotionSubview];
    [eNub setCmdDelegate:romibo];
    
    [self setupDrivingSubview];
    [dNub setCmdDelegate:romibo];
    
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil
    
    [eNub release];
    [dNub release];
    [romibo release];
    [cmdTimer release];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
	[super viewWillDisappear:animated];
}

- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    
    /*if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
     return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
     } else {
     return YES;
     }*/
    
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

-(void)setupEmotionSubview
{
    UIImageView* emotionView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"emotion-base-03.png"]];
    
    eNub = [[EmotionNubView alloc] init];
    
    emotionView.frame = CGRectMake(390, 230, 338, 338);
    
    eNub.center = CGPointMake(CGRectGetMidX(emotionView.bounds), CGRectGetMidY(emotionView.bounds));
    
    eNub.userInteractionEnabled = YES;
    emotionView.userInteractionEnabled = YES;
    
    [emotionView addSubview:eNub];
    [eNub release];
    
    [self.view addSubview:emotionView ];
    [emotionView release];
}

-(void)setupDrivingSubview
{
    UIImageView* drivingView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"movement-base-03.png"]];
    
    dNub = [[DrivingNubView alloc] init];
    
    drivingView.frame = CGRectMake(20, 230, 338, 338);
    
    dNub.center = CGPointMake(CGRectGetMidX(drivingView.bounds), CGRectGetMidY(drivingView.bounds));
    
    dNub.userInteractionEnabled = YES;
    drivingView.userInteractionEnabled = YES;
    
    [drivingView addSubview:dNub];
    [dNub release];
    
    [self.view addSubview:drivingView ];
    [drivingView release];
}

- (IBAction)stopClicked:(id)sender {
    
    [romibo sendString:@"drive 0 0\r"];
}

-(IBAction)configClicked:(id)sender
{
    ConfigViewController* configVC = [[ConfigViewController alloc] init];
    [configVC view];
    [configVC setPopDelegate:self];
    [configVC configureButtonState:[romibo isConnected]];
    
    if ([romibo isConnected])
        [configVC setTextBoxText:[romibo ipAddress]];
    else [configVC setTextBoxText:@"169.254.1.1"];      
    
    
    configPopover = [[UIPopoverController alloc] initWithContentViewController:configVC];
    [configVC release];
    
    configPopover.delegate = self;
    
    CGRect popoverRect = [self.view convertRect:[sender frame] toView:[sender superview]];
    
    [configPopover presentPopoverFromRect:popoverRect inView:self.view permittedArrowDirections:UIPopoverArrowDirectionUp animated: YES];
}

-(void)connectClicked:(NSString*)ipaddr
{
    [romibo connectToIP:ipaddr];
    [self closePopup];
}

-(void)disconnectClicked
{
    [romibo disconnect];
    [self closePopup];
}

-(void)setConnectionStatus
{
    if (romibo.isConnected)
        [connectionLabel setText:@"Connection OK"];
    else
        [connectionLabel setText:@"Not Connected"];
}

-(void)closePopup
{    
    if ([configPopover isPopoverVisible])
        [configPopover dismissPopoverAnimated:YES];
    
    [configPopover release];
}


-(IBAction)buttonClicked:(id)sender
{
    int btnTag = ((UIButton *)sender).tag;
    
    switch(btnTag)
    {
        case 1:
            [romibo sendString:@"say TWINKLE\r"];
            break;
            
        case 2:
            [romibo sendString:@"say AWESOME\r"];
            break;
            
        case 3:           
            [romibo sendString:@"count"];
            break;
            
        case 4:
            [romibo sendString:@"say SAD4\r"];
            break;
            
        case 5:
            [romibo sendString:@"dance\r"];
            break;
            
        case 6:
            [romibo sendString:@"nod\r"];
            break;
            
        case 7:
            [romibo sendString:@"rock\r"];
            break;
            
        case 8:
            [romibo sendString:@"bob\r"];
            break;
            
    }
}


-(void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController
{
    
}


- (void)dealloc {
    
    [romibo release];
    romibo = nil;
    
    [connectionLabel release];
    connectionLabel = nil;
    
    [cmdTimer invalidate];
    cmdTimer = nil;
    
    [super dealloc];
}
@end
