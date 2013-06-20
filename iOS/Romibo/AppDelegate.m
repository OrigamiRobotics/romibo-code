//
//  AppDelegate.m
//  Romibo
//
//  Created by HCII on 12/31/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import "AppDelegate.h"
#import "Romibo.h"

@implementation AppDelegate

@synthesize window = _window, romiboCommands, buttonLabels, romibo;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // Override point for customization after application launch.
    
    [self setupRomiboCommands];
    
    romibo = [[Romibo alloc] init];
    
    
    return YES;
}
							
- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
     */
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
     */
    
    
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    /*
     Called when the application is about to terminate.
     Save data if appropriate.
     See also applicationDidEnterBackground:.
     */
}

-(void)setupRomiboCommands
{
    
    NSString* commandPath = [[NSBundle mainBundle] pathForResource:@"RomiboCommands" ofType:@"txt"];
    
    NSString* allCommands = [NSString stringWithContentsOfFile:commandPath encoding:NSUTF8StringEncoding error:NULL];
    
    NSArray* commandsArray = [allCommands componentsSeparatedByString:@"\n"];
    
    romiboCommands = [[NSMutableDictionary alloc] init];
    buttonLabels = [[NSMutableArray alloc] init];
    
    for (int i = 0; i < commandsArray.count; i++)
    {
        NSString* command = [commandsArray objectAtIndex:i];
        NSArray* commandFragments = [command componentsSeparatedByString:@","];
        
        if ([commandFragments count] < 2)
        {
            NSLog(@"Invalid command syntax: %@", command);
            continue;
        }
        
        NSString* cmd = [[commandFragments objectAtIndex:0] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
        
        NSString* label = [[commandFragments objectAtIndex:1] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
        
        [romiboCommands  setObject:cmd forKey:label];
        [buttonLabels addObject:label];
    }
}

-(void)dealloc
{
    [romiboCommands release];
    romiboCommands = nil;
    
    [buttonLabels release];
    buttonLabels = nil;
    
    [romibo release];
    romibo = nil;
    
    [super dealloc];
}

@end
