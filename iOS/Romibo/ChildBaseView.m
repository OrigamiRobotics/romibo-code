//
//  ChildBaseView.m
//  Romibo
//
//  Created by QoLT on 3/22/13.
//
//

#import "ChildBaseView.h"
#import "CmdDelegate.h"

@implementation ChildBaseView


@synthesize romibo;

-(void)setRomibo:(Romibo *)_romibo
{
    romibo = _romibo;
    [romibo retain];

}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        
    
        
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    self.view.backgroundColor = [[UIColor alloc] initWithPatternImage:[UIImage imageNamed:@"background-02.png"]];
    
    
    drivingView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"movement-base-03.png"]];
    
    drivingView.frame = CGRectMake(109, 40, 550, 550);
    drivingView.userInteractionEnabled = YES;
    
    [self.view addSubview:drivingView ];
    [drivingView release];

    
    if (!adultBase)
    {
        UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"MainStoryboard_iPad" bundle:nil];
        adultBase = [storyboard instantiateViewControllerWithIdentifier:@"ViewController"];
        [adultBase setModalPresentationStyle:UIModalPresentationFullScreen];
        adultBase.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;

    }
    
    [adultBase retain];

    

}


- (void) touchesBegan:(NSSet*)touches withEvent:(UIEvent*) event
{
    [romibo endStopDrivingTimer];
    
    CGPoint currentPt = [[touches anyObject] locationInView:drivingView];
    
    //NSLog(@"X: %f", currentPt.x);
    //NSLog(@"Y: %f", currentPt.y);
    
    //for the kids' view, we drive by quadrant - overlay a standard grid and rotate by 45 degrees to match with the arrows
    //line A is the rotated y-axis, line B the rotated x; also note that iOS grid has the origin in the upper left
    bool leftOfA = ( (500 - 50) * (currentPt.y - 40) - (520 - 40) * (currentPt.x - 50) ) > 0;
    bool rightOfB = ( (500 - 50) * (currentPt.y - 520) - (40 - 520) * (currentPt.x - 50) ) > 0;
    
    int quadrant;
    
    if (leftOfA)
    {
        if (rightOfB)
            quadrant = 3;
        else quadrant = 2;
    }
    else
    {
        if (rightOfB)
            quadrant = 4;
        else quadrant = 1;
    }
    
    NSLog(@"Quadrant: %d", quadrant);
    
    switch (quadrant) {
        case 1:
            [romibo driveForward];
            break;
        case 2:
            [romibo driveLeft];
            break;
        case 3:
            [romibo driveBackward];
            break;
        case 4:
            [romibo driveRight];
            break;
            
        default:
            break;
    }
    
}


-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [romibo setStopDrivingTimer];
}


-(IBAction)changeShell:(UILongPressGestureRecognizer*)gesture
{
    if (!lockVC)
    {
        lockVC = [[DrawPatternLockViewController alloc] init];
        [lockVC setTarget:self withAction:@selector(lockEntered:)];
        
    }
    
    if (![lockVC isBeingPresented])
    {
        [self presentModalViewController:lockVC animated:YES];
        
    }
    
    
}

- (void)lockEntered:(NSString*)key {
    NSLog(@"key: %@", key);
    
    if (![key isEqualToString:@"070809"]) {
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Wrong pattern"
                                                            message:@"Wrong pattern for exiting child view"
                                                           delegate:nil
                                                  cancelButtonTitle:nil
                                                  otherButtonTitles:@"OK", nil];
        [alertView show];
        [self dismissModalViewControllerAnimated:YES];
    }
    else
    {
        [self dismissViewControllerAnimated:YES
                                 completion:^{
                                     if (![adultBase isBeingPresented])
                                         [self presentModalViewController:adultBase animated:YES];
                                     
                                 }
         
         ];
        
    }
}



- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void) dealloc
{
    [adultBase release];
    adultBase = nil;
    
    [romibo release];
    romibo = nil;
    
    [super dealloc];
    
}

@end
