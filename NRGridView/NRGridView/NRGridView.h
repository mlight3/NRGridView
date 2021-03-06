//
//  NRGridView.h
//  Grid
//
//  Created by Louka Desroziers on 05/01/12.

/***********************************************************************************
 *
 * Copyright (c) 2012 Louka Desroziers
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 ***********************************************************************************
 *
 * Referencing this project in your AboutBox is appreciated.
 * Please tell me if you use this class so we can cross-reference our projects.
 *
 ***********************************************************************************/


#import <UIKit/UIKit.h>
#import "NRGridViewDataSource.h"
#import "NRGridViewDelegate.h"
#import "NRGridViewCell.h"


@interface NSIndexPath (NRGridViewIndexPath)
+ (NSIndexPath*)indexPathForItemIndex:(NSInteger)itemIndex
                            inSection:(NSInteger)section;
@property (readonly) NSInteger itemIndex;
@end

typedef enum{
    NRGridViewLayoutStyleVertical,
    NRGridViewLayoutStyleHorizontal
} NRGridViewLayoutStyle;

typedef enum{
    NRGridViewStylePlain,
    NRGridViewStyleGrouped
} NRGridViewStyle;


typedef enum{
    NRGridViewScrollPositionNone,   // Please refer to UITableViewScrollPositionNone's description.
    NRGridViewScrollPositionAtTop,
    NRGridViewScrollPositionAtLeft  = NRGridViewScrollPositionAtTop, // for horizontal layout convention
    NRGridViewScrollPositionAtMiddle,
    NRGridViewScrollPositionAtBottom,
    NRGridViewScrollPositionAtRight = NRGridViewScrollPositionAtBottom // for horizontal layout convention
} NRGridViewScrollPosition;


/** Possible options that can be combined together to determine when a pressured cell must but un-highlighted. 
 * (cf NRGridViewDelegate if you wish to support the long press gesture on any cell)
 */
typedef enum{
    NRGridViewLongPressUnhighlightUponPressGestureEnds = 0x01,              // Un-highlights the cell when the user's finger lefts the screen
    NRGridViewLongPressUnhighlightUponScroll           = 0x02,              // Un-highlights the cell when the user scrolls the gridView.
    NRGridViewLongPressUnhighlightUponAnotherTouch     = 0x04               // Un-highlights the cell when the user touches the same or another cell
} NRGridViewLongPressUnhighlightOptions;


typedef enum {
    NRGridViewCellAnimationFade,
    NRGridViewCellAnimationRight,
    NRGridViewCellAnimationLeft,
    NRGridViewCellAnimationTop,
    NRGridViewCellAnimationBottom,
    NRGridViewCellAnimationNone,
    NRGridViewCellAnimationAutomatic = 100
} NRGridViewCellAnimation;


static CGSize const kNRGridViewDefaultCellSize = {50, 70};

@interface NRGridView : UIScrollView
{
    @private
    NSMutableArray  *_sectionLayouts;
    NSMutableSet    *_reusableCellsSet;
    NSMutableSet    *_visibleCellsSet;
    NRGridViewCell  *_highlightedCell, *_longPressuredCell;
    NSMutableArray  *_selectedCellsIndexPaths;
    
    struct {
        unsigned int willDisplayCell:1;  
        unsigned int willSelectCell:1;  
        unsigned int didSelectCell:1;  
        unsigned int didLongPressCell:1;  
        unsigned int didSelectHeader:1;  
    } _gridViewDelegateRespondsTo;
    
    struct {
        unsigned int numberOfSections:1;  
        unsigned int titleForHeader:1;  
        unsigned int viewForHeader:1;  
        unsigned int heightForHeader:1;  
        unsigned int widthForHeader:1;  
        unsigned int titleForFooter:1;  
        unsigned int viewForFooter:1;  
        unsigned int heightForFooter:1;  
        unsigned int widthForFooter:1;
        unsigned int hasTranslucentNavigationBar:1;
        unsigned int gridCellSize:1;
        unsigned int backgroundView:1;
    } _gridViewDataSourceRespondsTo;
}

- (id)initWithLayoutStyle:(NRGridViewLayoutStyle)layoutStyle;
- (id)initWithLayoutStyle:(NRGridViewLayoutStyle)layoutStyle style:(NRGridViewStyle)style;

@property (nonatomic, assign) NRGridViewLayoutStyle layoutStyle;
@property (nonatomic, assign) NRGridViewStyle style;

@property (nonatomic, assign) IBOutlet id<NRGridViewDelegate> delegate;
@property (nonatomic, assign) IBOutlet id<NRGridViewDataSource> dataSource;

@property (nonatomic, retain) UIView    *gridHeaderView;
@property (nonatomic, retain) UIView    *gridFooterView;

/** When the NRGridView's header/footer view is set to be sticky, it will always be visible on screen. */
@property (nonatomic, assign, getter = isGridHeaderViewSticky) BOOL stickyGridHeaderView;
@property (nonatomic, assign, getter = isGridFooterViewSticky) BOOL stickyGridFooterView;

- (void)setGridHeaderView:(UIView *)gridHeaderView sticky:(BOOL)sticky;
- (void)setGridFooterView:(UIView *)gridFooterView sticky:(BOOL)sticky;

/** Determines the size of every cells passed into the gridView. Default value is kNRGridViewDefaultCellSize */
@property (nonatomic, assign) CGSize cellSize;

@property (nonatomic, readonly) NSArray     *visibleCells;
@property (nonatomic, readonly) NSArray     *indexPathsForVisibleCells;

@property (nonatomic, assign) CGFloat spacingBetweenSections;
//@property (nonatomic, assign) CGFloat spacingBetweenCells;

/** Reloading the GridView */
- (void)reloadData;
- (void)reloadCellsAtIndexPaths:(NSArray *)indexPaths withCellAnimation:(NRGridViewCellAnimation)cellAnimation;
- (void)reloadSections:(NSIndexSet *)sections withCellAnimation:(NRGridViewCellAnimation)cellAnimation;

/** Requesting cells */
- (NRGridViewCell*)dequeueReusableCellWithIdentifier:(NSString*)identifier;
- (NRGridViewCell*)cellAtIndexPath:(NSIndexPath*)indexPath; // returns nil if cell is not visible.
- (NSArray*)visibleCellsAtIndexPaths:(NSArray*)indexPaths;

/** Handling (de)selection */
@property (nonatomic, retain)       NSIndexPath *selectedCellIndexPath __attribute__ ((deprecated));

@property (nonatomic, assign)       BOOL        allowsMultipleSelections;

/** Returns the indexPath for the selected cell. 
 * @discussion If the gridView is allowed to perform multiple selections, the latest selected indexPath is returned.
 */
- (NSIndexPath *)indexPathForSelectedCell;

/** Returns the indexPaths for selected cells. */
- (NSArray *)indexPathsForSelectedCells;


- (void)selectCellAtIndexPath:(NSIndexPath*)indexPath 
                     animated:(BOOL)animated;
- (void)selectCellAtIndexPath:(NSIndexPath*)indexPath 
                   autoScroll:(BOOL)autoScroll
               scrollPosition:(NRGridViewScrollPosition)scrollPosition
                     animated:(BOOL)animated;

- (void)deselectCellAtIndexPath:(NSIndexPath*)indexPath 
                       animated:(BOOL)animated;



/** Getting rects, and scroll to specific section/indexPath */
- (CGRect)rectForHeaderInSection:(NSInteger)section;
- (CGRect)rectForSection:(NSInteger)section;
- (CGRect)rectForItemAtIndexPath:(NSIndexPath*)indexPath;
- (CGRect)rectForFooterInSection:(NSInteger)section;

/** Scrolls to the given 'section' in the grid view. The scrollPosition determines which part of the section will be visible.
 * E.g.: using NRGridViewScrollPositionAtBottom/Right should display the end of the section. */
- (void)scrollRectToSection:(NSInteger)section 
                   animated:(BOOL)animated
             scrollPosition:(NRGridViewScrollPosition)scrollPosition;

- (void)scrollRectToItemAtIndexPath:(NSIndexPath*)indexPath 
                           animated:(BOOL)animated
                     scrollPosition:(NRGridViewScrollPosition)scrollPosition;


/** Long Pressure options (cf NRGridViewDelegate if you wish to support the long press gesture on any cell) 
 * Default value is (NRGridViewLongPressUnhighlightUponScroll|NRGridViewLongPressUnhighlightUponAnotherTouch)
 */
@property (nonatomic, assign) NRGridViewLongPressUnhighlightOptions longPressOptions; 

// You can either manually deselect the pressured cell like we do in our sample app.
- (void)unhighlightPressuredCellAnimated:(BOOL)animated;
- (NSIndexPath*)indexPathForLongPressuredCell;

- (UIView *)headerViewForSection:(NSInteger)section;
- (UIView *)footerViewForSection:(NSInteger)section;

@end
