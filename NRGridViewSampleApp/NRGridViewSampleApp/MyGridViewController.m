//
//  MyGridViewController.m
//  NRGridViewSampleApp
//
//  Created by Louka Desroziers on 04/02/12.
//  Copyright (c) 2012 Novedia Regions. All rights reserved.
//

#import "MyGridViewController.h"

static BOOL const _kNRGridViewSampleCrazyScrollEnabled = NO; // For the lulz.
@implementation MyGridViewController
{
    BOOL _firstSectionReloaded; // For reloading sections/cells demo
}

#pragma mark - Crazy Scroll LULZ

- (void)__beginGeneratingCrazyScrolls
{
    if(_kNRGridViewSampleCrazyScrollEnabled==NO)return;
    
    NSInteger randomSection = arc4random() % ([[[self gridView] dataSource] respondsToSelector:@selector(numberOfSectionsInGridView:)]
                                              ? [[[self gridView] dataSource] numberOfSectionsInGridView:[self gridView]]
                                              : 1);
    NSInteger randomItemIndex = arc4random() % [[[self gridView] dataSource] gridView:[self gridView] 
                                                               numberOfItemsInSection:randomSection];
    
    
    [[self gridView] selectCellAtIndexPath:[NSIndexPath indexPathForItemIndex:randomItemIndex inSection:randomSection]
                                autoScroll:YES 
                            scrollPosition:NRGridViewScrollPositionAtMiddle
                                  animated:YES];
    
    /*
    [[self gridView] scrollRectToSection:randomSection 
                                animated:YES 
                          scrollPosition:NRGridViewScrollPositionAtBottom];
    */
    
    [self performSelector:@selector(__beginGeneratingCrazyScrolls) 
               withObject:nil 
               afterDelay:2.5 
                  inModes:[NSArray arrayWithObject:NSRunLoopCommonModes]];
}

- (void)__endGeneratingCrazyScrolls
{
    [NSObject cancelPreviousPerformRequestsWithTarget:self 
                                             selector:@selector(__beginGeneratingCrazyScrolls) 
                                               object:nil];
}

- (void)__reloadFirstSection:(id)sender
{
    _firstSectionReloaded = !_firstSectionReloaded;
    [[self gridView] reloadSections:[NSIndexSet indexSetWithIndex:0]
                  withCellAnimation:NRGridViewCellAnimationLeft];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    [self __beginGeneratingCrazyScrolls];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self __endGeneratingCrazyScrolls];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    UIBarButtonItem *reloadSectionButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh
                                                                                             target:self
                                                                                             action:@selector(__reloadFirstSection:)];
    [[self navigationItem] setRightBarButtonItem:reloadSectionButtonItem animated:animated];
    [reloadSectionButtonItem release];
}

#pragma mark -

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

#pragma mark - View lifecycle
- (BOOL)canBecomeFirstResponder{return YES;}

- (void)loadView
{ 
    [super loadView];
    [[self gridView] setCellSize:CGSizeMake(100, 75)];
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
    [super viewDidLoad];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    // Return YES for supported orientations
    return YES;
}


#pragma mark - NRGridView Data Source



- (NSInteger)numberOfSectionsInGridView:(NRGridView *)gridView
{
    return 5;
}

- (NSInteger)gridView:(NRGridView *)gridView numberOfItemsInSection:(NSInteger)section
{
    return 50;
}

- (NSString*)gridView:(NRGridView *)gridView titleForHeaderInSection:(NSInteger)section
{
    return [NSString stringWithFormat:@"Section %i %@", section, (section == 0 && _firstSectionReloaded ? @"(Reloaded)" : @"")];
}

- (NSString*)gridView:(NRGridView *)gridView titleForFooterInSection:(NSInteger)section
{
   if(section%2)
       return [NSString stringWithFormat:@"Footer %i", section];
    return nil;
}  


- (NRGridViewCell*)gridView:(NRGridView *)gridView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *MyCellIdentifier = @"MyCellIdentifier";
    
    NRGridViewCell* cell = [gridView dequeueReusableCellWithIdentifier:MyCellIdentifier];
    
    if(cell == nil){
        cell = [[[NRGridViewCell alloc] initWithReuseIdentifier:MyCellIdentifier] autorelease];
        
        [[cell textLabel] setFont:[UIFont boldSystemFontOfSize:11.]];
        [[cell detailedTextLabel] setFont:[UIFont systemFontOfSize:11.]];
        cell.backgroundColor = [UIColor clearColor];
        cell.selectionBackgroundView.backgroundColor = [UIColor clearColor];
    }
    
    cell.imageView.image = (indexPath.section == 0 && _firstSectionReloaded ? nil : [UIImage imageNamed:[NSString stringWithFormat:@"%i.png", (indexPath.row%7)]]);
    cell.textLabel.text = [NSString stringWithFormat:@"Item %i %@", indexPath.itemIndex, (indexPath.section == 0 && _firstSectionReloaded ? @"(Reloaded)" : @"")];
    cell.detailedTextLabel.text = @"Some details";

    return cell;
}

#pragma mark - NRGridView Delegate

- (void)gridView:(NRGridView *)gridView didSelectCellAtIndexPath:(NSIndexPath *)indexPath
{
    MyGridViewController *gridViewController = [[MyGridViewController alloc] initWithGridLayoutStyle:([self gridLayoutStyle] == NRGridViewLayoutStyleVertical
                                                                                                      ? NRGridViewLayoutStyleHorizontal
                                                                                                      : NRGridViewLayoutStyleVertical)];
    [[self navigationController] pushViewController:gridViewController 
                                           animated:YES];
    [gridViewController release];
}

- (void)gridView:(NRGridView *)gridView didLongPressCellAtIndexPath:(NSIndexPath *)indexPath
{
    UIMenuController* menuController = [UIMenuController sharedMenuController];
    NRGridViewCell* cell = [gridView cellAtIndexPath:indexPath];

    [self becomeFirstResponder];
    [menuController setMenuItems:[NSArray arrayWithObject:[[[UIMenuItem alloc] initWithTitle:@"Hooorayyyy!" 
                                                                                     action:@selector(handleHooray:)] autorelease]]];
    [menuController setTargetRect:[cell frame] 
                           inView:[self view]];
    
    [menuController setMenuVisible:YES animated:YES];
    
}

- (UIView *)gridView:(NRGridView *)gridView backgroundViewInSection:(NSInteger)section {
    if (section%2 == 0) {
        UIView * view = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 0, 0)];
        view.backgroundColor = [UIColor cyanColor];
        return [view autorelease];
    }
    return nil;
}

#pragma mark - UIMenuController Actions

- (void)handleHooray:(id)sender
{
    [[self gridView] unhighlightPressuredCellAnimated:YES];
}

- (BOOL)canPerformAction:(SEL)action withSender:(id)sender
{
    return (action == @selector(handleHooray:));
}

#pragma mark - Memory Management


- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}
- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)dealloc
{
    [super dealloc];
}

@end
