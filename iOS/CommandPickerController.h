//
//  CommandPickerController.h
//  Romibo
//
//  Created by QoLT on 2/15/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol CommandPickerDelegate
- (void)commandSelected:(NSString *)color;
@end


@interface CommandPickerController : UITableViewController {
  
}

@property (nonatomic, assign) id<CommandPickerDelegate> delegate;
@property (nonatomic, retain) NSArray* commands;


@end