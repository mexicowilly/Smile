#import <Cocoa/Cocoa.h>
#import "IfsItem.h"

@interface IfsDataSource : NSObject
{
@private
    IfsItem* m_root;
    smile::AS400* m_as400;
}

- (id) initWithSystem: (NSString*)system user: (NSString*)user password: (NSString*)password;
- (void) dealloc;

- (id) outlineView: (NSOutlineView*)outlineView child: (int)idx ofItem: (id)item;
- (BOOL) outlineView: (NSOutlineView*)outlineView isItemExpandable: (id)item;
- (int) outlineView: (NSOutlineView*)outlineView numberOfChildrenOfItem: (id)item;
- (id) outlineView: (NSOutlineView*)outlineView objectValueForTableColumn: (NSTableColumn*)column byItem: (id)item;

@end
