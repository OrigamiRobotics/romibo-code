//
//  Romibo.h
//  
//
//  Created by HCII on 1/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "GCDAsyncSocket.h"
#import "CmdDelegate.h"

@interface Romibo : NSObject <CmdDelegate>

{
    GCDAsyncSocket *socket;
}

@property (nonatomic) BOOL isConnected;

@property (nonatomic, retain) NSString* driveCmd;
@property (nonatomic, retain) NSString* lastSendDriveCmd;

@property (nonatomic, retain) NSString* emoteCmd;
@property (nonatomic, retain) NSString* lastSendEmoteCmd;

@property (nonatomic, retain) NSString* ipAddress;


-(void)connectToIP:(NSString*)ipAddress;
-(void)disconnect;

-(void)sendDriveCmd:(int)x:(int)y;
-(void)sendEmoteCmd:(int)x:(int)y;
-(void)handleCmdTimer;

-(void)sendString:(NSString*)str;

@end
