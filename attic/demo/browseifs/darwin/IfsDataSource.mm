#import "IfsDataSource.h"

@implementation IfsDataSource

- (id) initWithSystem: (NSString*)system user: (NSString*)user password: (NSString*)password
{
    [super init];
    m_as400 = new smile::AS400(smile::Text([system UTF8String], "UTF-8"),
                               smile::Text([user UTF8String], "UTF-8"),
                               smile::Text([password UTF8String], "UTF-8"));
    m_root = [[IfsItem alloc] initWithObject:
        new smile::IfsDirectory(*m_as400, smile::IfsObjectName("/"))];
    return self;
}

- (void) dealloc
{
    [m_root release];
    delete m_as400;
    [super dealloc];
}

- (id) outlineView: (NSOutlineView*)outlineView child: (int)idx ofItem: (id)item
{
    return (item == nil) ? m_root : [(IfsItem*)item childAtIndex: idx];
}

- (BOOL) outlineView: (NSOutlineView*)outlineView isItemExpandable: (id)item
{
    return (item == nil) ? YES : [(IfsItem*)item canHaveChildren];
}

- (int) outlineView: (NSOutlineView*)outlineView numberOfChildrenOfItem: (id)item
{
    return (item == nil) ? 1 : [(IfsItem*)item numberOfChildren];
}

- (id) outlineView: (NSOutlineView*)outlineView objectValueForTableColumn: (NSTableColumn*)column byItem: (id)item
{
    return (item == nil) ? [m_root name] : [(IfsItem*)item name];
}

@end
