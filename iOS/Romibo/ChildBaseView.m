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

@synthesize cmdDelegate;

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
    
    UIImageView* drivingView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"movement-base-03.png"]];
    
    drivingView.frame = CGRectMake(109, 40, 550, 550);
    drivingView.userInteractionEnabled = YES;
    
    [self.view addSubview:drivingView ];
    [drivingView release];
    
    if (!adultBase)
    {
        UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"MainStoryboard_iPad" bundle:nil];
        adultBase = [storyboard instantiateViewControllerWithIdentifier:@"ViewController"];
        [adultBase setModalPresentationStyle:UIModalPresentationFullScreen];

    }
    
    [adultBase retain];

    

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
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Error"
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
    
    [super dealloc];
    
}

@end
