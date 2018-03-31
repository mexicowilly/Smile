#import <Cocoa/Cocoa.h>
#import "IfsDataSource.h"

@interface MyDocument : NSDocument
{
    IBOutlet NSOutlineView* m_outlineView;
    
@private
    IfsDataSource* m_dataSource;
}

- (id) init;
- (void) dealloc;
- (void) sheetDidEnd: (NSWindow*)sheet returnCode: (int)rc contextInfo: (void*)ctx;
- (void) windowDidBecomeMain: (NSNotification*)notification;

@end
