//
//  ButtonScrollView.h
//  Romibo
//
//  Created by jmcmillan on 6/4/13.
//
//

#import <UIKit/UIKit.h>

@interface ButtonScrollView : UIViewController <UIScrollViewDelegate>
{
    
    NSArray* pagePaths;
}
@property (retain, nonatomic) NSMutableArray *viewControllers;
@property (nonatomic) int totalPages;

@property (retain, nonatomic) IBOutlet UIScrollView *scroll;
@property (retain, nonatomic) IBOutlet UIPageControl *pager;

@property (retain, nonatomic) NSString* directoryName;

-(void)loadButtonPages:(NSString*) dirName;
-(void)addAdultButtonPage:(int)idx;
-(void)addSimpleButtonPage:(int)idx;

@end
