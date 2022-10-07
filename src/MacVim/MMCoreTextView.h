/* vi:set ts=8 sts=4 sw=4 ft=objc:
 *
 * VIM - Vi IMproved		by Bram Moolenaar
 *				MacVim GUI port by Bjorn Winckler
 *
 * Do ":help uganda"  in Vim to read copying and usage conditions.
 * Do ":help credits" in Vim to see a list of people who contributed.
 * See README.txt for an overview of the Vim source code.
 */

#import <Cocoa/Cocoa.h>

@class MMTextViewHelper;


@interface MMCoreTextView : NSView <
    NSTextInput
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_14
    , NSFontChanging
    , NSMenuItemValidation
#endif
    >
{
    // From MMTextStorage
    int                         maxRows, maxColumns;
    NSColor                     *defaultBackgroundColor;
    NSColor                     *defaultForegroundColor;
    NSSize                      cellSize;
    NSFont                      *font;
    NSFont                      *fontWide;
    float                       linespace;
    float                       columnspace;

    // From NSTextView
    NSSize                      insetSize;

    CGFloat                     fontDescent;
    CGFloat                     fontAscent;
    CGFloat                     fontXHeight;
    BOOL                        antialias;
    BOOL                        ligatures;
    BOOL                        thinStrokes;

    BOOL                        forceRefreshFont; // when true, don't early out of setFont calls.

    MMTextViewHelper            *helper;

    NSMutableDictionary<NSNumber *, NSFont *> *fontVariants;
    NSMutableSet<NSString *> *characterStrings;
    NSMutableDictionary<NSNumber *,NSCache<NSString *,id> *> *characterLines;
    
    // These are used in MMCoreTextView+ToolTip.m
    id trackingRectOwner_;              // (not retained)
    void *trackingRectUserData_;
    NSTrackingRectTag lastToolTipTag_;
    NSString* toolTip_;
}

- (id)initWithFrame:(NSRect)frame;

//
// NSFontChanging methods
//
- (void)changeFont:(id)sender;

//
// NSMenuItemValidation
//
- (BOOL)validateMenuItem:(NSMenuItem *)item;

//
// Public macaction's
// Note: New items here need to be handled in validateMenuItem: as well.
//
- (IBAction)cut:(id)sender;
- (IBAction)copy:(id)sender;
- (IBAction)paste:(id)sender;
- (IBAction)undo:(id)sender;
- (IBAction)redo:(id)sender;
- (IBAction)selectAll:(id)sender;

//
// MMTextStorage methods
//
- (int)maxRows;
- (int)maxColumns;
- (void)getMaxRows:(int*)rows columns:(int*)cols;
- (void)setMaxRows:(int)rows columns:(int)cols;
- (void)setDefaultColorsBackground:(NSColor *)bgColor
                        foreground:(NSColor *)fgColor;
- (NSColor *)defaultBackgroundColor;
- (NSColor *)defaultForegroundColor;
- (NSRect)rectForRowsInRange:(NSRange)range;
- (NSRect)rectForColumnsInRange:(NSRange)range;

- (void)setFont:(NSFont *)newFont;
- (void)setWideFont:(NSFont *)newFont;
- (NSFont *)font;
- (NSFont *)fontWide;
- (NSSize)cellSize;
- (void)setLinespace:(float)newLinespace;

//
// MMTextView methods
//
- (void)deleteSign:(NSString *)signName;
- (void)setShouldDrawInsertionPoint:(BOOL)on;
- (void)setPreEditRow:(int)row column:(int)col;
- (void)setMouseShape:(int)shape;
- (void)setAntialias:(BOOL)state;
- (void)setLigatures:(BOOL)state;
- (void)setThinStrokes:(BOOL)state;
- (void)setImControl:(BOOL)enable;
- (void)activateIm:(BOOL)enable;
- (void)checkImState;
- (BOOL)convertPoint:(NSPoint)point toRow:(int *)row column:(int *)column;
- (NSRect)rectForRow:(int)row column:(int)column numRows:(int)nr
          numColumns:(int)nc;
- (void)updateCmdlineRow;

//
// NSTextView methods
//
- (void)keyDown:(NSEvent *)event;
- (void)insertText:(id)string;
- (void)doCommandBySelector:(SEL)selector;

//
// NSTextContainer methods
//
- (void)setTextContainerInset:(NSSize)inset;

//
// MMCoreTextView methods
//
- (void)performBatchDrawWithData:(NSData *)data;
- (NSSize)desiredSize;
- (NSSize)minSize;
- (NSSize)constrainRows:(int *)rows columns:(int *)cols toSize:(NSSize)size;
@end


//
// This category is defined in MMCoreTextView+ToolTip.m
//
@interface MMCoreTextView (ToolTip)
- (void)setToolTipAtMousePoint:(NSString *)string;
@end
