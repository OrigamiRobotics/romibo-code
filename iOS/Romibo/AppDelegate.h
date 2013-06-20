//
//  AppDelegate.h
//  Romibo
//
//  Created by HCII on 12/31/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Romibo.h"

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (retain, nonatomic) NSMutableDictionary* romiboCommands;
@property (retain, nonatomic) NSMutableArray* buttonLabels;
@property (retain, nonatomic) Romibo* romibo;


-(void)setupRomiboCommands;

@end
