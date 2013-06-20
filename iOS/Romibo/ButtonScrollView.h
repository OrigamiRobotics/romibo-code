//
//  ButtonScrollView.h
//  Romibo
//
//  Created by jmcmillan on 6/4/13.
//
//

#import <UIKit/UIKit.h>

@interface ButtonScrollView : UIViewController <UIScrollViewDelegate>

@property (retain, nonatomic) NSMutableArray *viewControllers;
@property (nonatomic) int totalPages;

@property (retain, nonatomic) IBOutlet UIScrollView *scroll;
@property (retain, nonatomic) IBOutlet UIPageControl *pager;

-(void)loadButtonPages;

@end
