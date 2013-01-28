//
//  CmdDelegate.h
//  
//
//  Created by HCII on 1/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol CmdDelegate
-(void)sendDriveCmd:(int)x:(int)y;
-(void)sendEmoteCmd:(int)x:(int)y;
-(void)testDriveCmd:(int)startX:(int)startY:(int)x:(int)y;
@end



