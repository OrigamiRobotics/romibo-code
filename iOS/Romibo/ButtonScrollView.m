//
//  ButtonScrollView.m
//  Romibo
//
//  Created by jmcmillan on 6/4/13.
//
//

#import "ButtonScrollView.h"
#import "ButtonPage.h"
#import "SimpleButtonPage.h"

@implementation ButtonScrollView

@synthesize scroll;
@synthesize pager;
@synthesize viewControllers;
@synthesize totalPages;
@synthesize directoryName;


-(void)loadButtonPages:(NSString*) dirName
{
    NSString* directory = [NSString stringWithFormat:@"/%@", dirName];
    directoryName = directory;
    
    pagePaths = [[NSBundle mainBundle] pathsForResourcesOfType:@"txt" inDirectory:directoryName];
    
    totalPages = [pagePaths count];
    
    viewControllers = [[NSMutableArray alloc] initWithCapacity:totalPages];
    
    [scroll setPagingEnabled:YES];
    [scroll setShowsHorizontalScrollIndicator:NO];
    [scroll setShowsVerticalScrollIndicator:NO];
    [scroll setScrollsToTop:NO];
    [scroll setDelegate:self];
    
    [scroll setContentSize:CGSizeMake(scroll.frame.size.width * totalPages, scroll.frame.size.height)];
    
    
    for (int i=0; i<totalPages; i++) {
        
        NSLog(@"%@", pagePaths[i]);
        
        if ([dirName isEqualToString:@"screens"])
            [self addAdultButtonPage:i];
        else [self addSimpleButtonPage:i];
    }
    
    [pager setNumberOfPages:totalPages];
    [pager setCurrentPage:0];
    
}

-(void)addAdultButtonPage:(int)index
{
    ButtonPage *testView = [[ButtonPage alloc] initWithNibName:@"ButtonPage" bundle:[NSBundle mainBundle]];
    [testView setupPage:pagePaths[index]];
    
    CGRect viewFrame = CGRectMake(scroll.frame.size.width * index, 0.0, scroll.frame.size.width, scroll.frame.size.height);
    [testView.view setFrame:viewFrame];
    
    [scroll addSubview:testView.view];
    
    [viewControllers addObject:testView];
    
    [testView release];
    

}

-(void)addSimpleButtonPage:(int)index
{
    SimpleButtonPage *testView = [[SimpleButtonPage alloc] initWithNibName:@"SimpleButtonPage" bundle:[NSBundle mainBundle]];
    [testView setupPage:pagePaths[index]:directoryName];
    
    CGRect viewFrame = CGRectMake(scroll.frame.size.width * index, 0.0, scroll.frame.size.width, scroll.frame.size.height);
    [testView.view setFrame:viewFrame];
    
    [scroll addSubview:testView.view];
    
    [viewControllers addObject:testView];
    
    [testView release];
    
    
}

-(void)scrollViewDidScroll:(UIScrollView *)scrollView{
    int page = floor(scrollView.contentOffset.x / [UIScreen mainScreen].bounds.size.width);
    
    [pager setCurrentPage:page];
}


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
    
    [self.view setBackgroundColor:[UIColor clearColor]];
    [self.view setOpaque:NO];
    
    
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc {
    [scroll release];
    [pager release];
    [viewControllers release];
    [super dealloc];
}
- (void)viewDidUnload {
    [self setScroll:nil];
    [super viewDidUnload];
}
@end
