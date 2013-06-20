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

@synthesize pageName, appDelegate;

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
    
    [nameLabel setText:pageName];
    
    self.appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    
    
}


-(void)setupButtons:(NSString*)fileName
{
    
    NSString* screenPath = [[NSBundle mainBundle] pathForResource:fileName ofType:@"txt" inDirectory:@"/screens"];
    
    NSString* allCommands = [NSString stringWithContentsOfFile:screenPath encoding:NSUTF8StringEncoding error:NULL];
    
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

-(void)setupPage:(NSString*)filePath
{
    NSString* fileName = [[filePath lastPathComponent] stringByDeletingPathExtension];
    [self setupButtons:fileName];
}


-(IBAction)handleLongPress:(UILongPressGestureRecognizer*)gesture
{
    UIButton* button = (UIButton *) gesture.view;
    [self pickCommand:button];
    
}

-(IBAction)buttonClicked:(id)sender
{
    
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
    
    [nameLabel release];
    nameLabel = nil;
    
    
    [commandPicker release];
    commandPicker = nil;
    
    
    [super dealloc];
}
- (void)viewDidUnload {
 
    [super viewDidUnload];
}
@end
