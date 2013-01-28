//
//  Romibo.m
//  
//
//  Created by HCII on 1/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Romibo.h"
#import "AppDelegate.h"
#import "ViewController.h"


@implementation Romibo 

@synthesize isConnected;
@synthesize driveCmd;
@synthesize emoteCmd;
@synthesize lastSendEmoteCmd;
@synthesize lastSendDriveCmd;
@synthesize ipAddress;


-(id)init
{
    self = [super init];
    
    [self setIsConnected:false];
    [self setDriveCmd:@""];
    [self setLastSendDriveCmd:@""];
    [self setEmoteCmd:@""];
    [self setLastSendEmoteCmd:@""];
    
    return self;
}

-(void)connectToIP:(NSString*)_ipAddress
{   
    [self setIpAddress:_ipAddress];
    
    socket = [[GCDAsyncSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
    
    NSError *err = nil;
    
    
    [socket connectToHost:ipAddress onPort:2000 error:&err];
    NSLog(@"Attempting connection to: %@\r", ipAddress); 
   
}

-(void)onSocket:(GCDAsyncSocket *)sock willDisconnectWithError:(NSError *)err
{
    NSLog(@"Disconnection error: %@", err);
    
    self.isConnected = false;
    
    [self disconnect];
    
}

- (void)socket:(GCDAsyncSocket *)sender didConnectToHost:(NSString *)host port:(UInt16)port
{
    NSLog(@"Connected to: %@", host);
    
    self.isConnected = true;
    [(ViewController*)[[(AppDelegate *)[[UIApplication sharedApplication] delegate] window] rootViewController] setConnectionStatus];
    
    [self sendString:@"\rbegin\r"];
    [self sendString:@"test\r"];
}

-(void)disconnect
{
    [self sendString:@"end\r"];
    
    [self setIsConnected:false];
    [(ViewController*)[[(AppDelegate *)[[UIApplication sharedApplication] delegate] window] rootViewController] setConnectionStatus];
    
    [socket setDelegate:nil delegateQueue:NULL];
    [socket disconnect];
    [socket release];
    socket = nil;
}


-(void)handleCmdTimer
{   
    //get rid of reams of "drive 0 0"    
    if (driveCmd.length > 0)
    {
        if (! ( [driveCmd isEqualToString:@"drive 0 0\r"] && [lastSendDriveCmd isEqualToString:@"drive 0 0\r"]) )
        {
            [self setLastSendDriveCmd:driveCmd];
            [self sendString:driveCmd];
        }
    }
    
    if (![emoteCmd isEqualToString:lastSendEmoteCmd]) 
    {
        [self sendString:emoteCmd];
        [self setLastSendEmoteCmd:emoteCmd];
    }
    
    
    
}

-(void)testDriveCmd:(int)startX:(int)startY:(int)x:(int)y
{
    NSString* dCmd = [NSString stringWithFormat:@"start %i %i drive %i %i\r", startX, startY, x, y];
    [self setDriveCmd:dCmd];
}

-(void)sendDriveCmd:(int)x:(int)y
{
    NSString* dCmd = [NSString stringWithFormat:@"drive %i %i\r", x, y];
    
    NSLog(@"%@", dCmd);
    
    [self setDriveCmd:dCmd];
}

-(void)sendEmoteCmd:(int)x:(int)y
{
    NSString* eCmd = [NSString stringWithFormat:@"emote %i %i\r", x, y];
    [self setEmoteCmd:eCmd];
}

-(void)sendString:(NSString*)str
{
    NSLog(@"Send string: %@", str);
    
    NSData* data = [str dataUsingEncoding:NSUTF8StringEncoding];
    
    [socket writeData:data withTimeout:1000 tag:1];
    
    
}

-(void)dealloc
{
    [self disconnect];
        
    [super dealloc];
}

@end
