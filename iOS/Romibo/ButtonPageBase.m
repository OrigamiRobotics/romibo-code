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

@synthesize appDelegate, pageName;

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
	// Do any additional setup after loading the view.
    
    [self.view setBackgroundColor:[UIColor clearColor]];
    [self.view setOpaque:NO];
    
    [nameLabel setText:pageName];
    
    self.appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}




-(void)setupPage:(NSString*)filePath
{
    
//    NSString* screenPath = [[NSBundle mainBundle] pathForResource:fileName ofType:@"txt" inDirectory:directory];
    
    NSString* allCommands = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:NULL];
    
    NSMutableArray* commandsArray = [NSMutableArray arrayWithArray:[allCommands componentsSeparatedByString:@"\n"]];
    
    pageName = [commandsArray objectAtIndex:0];
    
    [commandsArray removeObjectAtIndex:0];
    
    for (int i = 1; i < [commandsArray count] + 1; i++)
    {
        NSString* command = [commandsArray objectAtIndex:i-1];
        NSArray* commandFragments = [command componentsSeparatedByString:@","];
        
        if ([commandFragments count] < 2)
        {
            NSLog(@"Invalid command syntax: %@", command);
            continue;
        }
        
        NSString* cmd = [[commandFragments objectAtIndex:0] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
        
        NSString* label = [[commandFragments objectAtIndex:1] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
        
        if ([[appDelegate romiboCommands] objectForKey:label] == nil)
        {
            [[appDelegate romiboCommands] setObject:cmd forKey:label];
            [[appDelegate buttonLabels] addObject:label];
        }
        
        UIButton* button = (UIButton*)[self.view viewWithTag:i];
        [button setTitle:label forState:UIControlStateNormal];
        
        //NSLog(@"%@", [[button titleLabel] text]);
    }
}


//-(void)setupPage:(NSString*)filePath:(NSString*) directory;
//{
  //  NSString* fileName = [[filePath lastPathComponent] stringByDeletingPathExtension];
    //[self setupButtons:fileName:directory];
//}


-(IBAction)buttonClicked:(id)sender
{
    lastButtonClicked = (UIButton *)sender;
    
    NSString* btnText = [((UIButton *)sender).titleLabel text];
    NSLog(@"Button text: %@", btnText);
    
    NSString* btnCommand = [[appDelegate romiboCommands] objectForKey:btnText];
    NSLog(@"Button command: %@", btnCommand);
    
    NSString* command;
    if ([btnCommand hasSuffix:@".wav"] || [btnCommand hasSuffix:@".WAV"])
        command = [@"say " stringByAppendingString:btnCommand];
    else command = btnCommand;
    
    NSString* fullCommand = [NSString stringWithFormat:@"%@\r", command];
    NSLog(@"Full command: %@", fullCommand);
    
    [[appDelegate romibo] sendString:fullCommand];
    
}

-(void)dealloc
{
    [nameLabel release];
    nameLabel = nil;
    
    [super dealloc];
    
    
}

@end
