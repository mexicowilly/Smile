#import "IfsItem.h"

@interface IfsItem (PrivateMethods)

- (void) initializeChildren: (smile::IfsDirectory*)dir;

@end

@implementation IfsItem

- (id) initWithObject: (smile::IfsObject*)object
{
    [super init];
    m_object = object;
    m_children = nil;
    return self;
}

- (void) dealloc
{
    delete m_object;
    [m_children release];
    [super dealloc];
}

- (IfsItem*) childAtIndex: (unsigned)idx
{
    IfsItem* result = nil;
    smile::IfsDirectory* dir = dynamic_cast<smile::IfsDirectory*>(m_object);
    if (dir != NULL)
    {
        [self initializeChildren: dir];
        if (idx < [m_children count])
            result = [m_children objectAtIndex: idx];
    }
    return result;
}

- (BOOL) canHaveChildren
{
    return dynamic_cast<smile::IfsDirectory*>(m_object) != NULL;
}

- (unsigned) numberOfChildren
{
    unsigned result = 0;
    smile::IfsDirectory* dir = dynamic_cast<smile::IfsDirectory*>(m_object);
    if (dir != NULL)
    {
        [self initializeChildren: dir];
        result = [m_children count];
    }
    return result;
}

- (NSString*) name
{
    const smile::Text text = m_object->getName().getObjectName();
    return [NSString stringWithCharacters: (const unichar*)text.getUChars() length: text.getLength()];
}

@end

@implementation IfsItem (PrivateMethods)

- (void) initializeChildren: (smile::IfsDirectory*)dir
{
    if (m_children == nil)
    {
        std::vector<smile::IfsObject*> cppChildren = dir->listObjects();
        m_children = [[NSMutableArray alloc] initWithCapacity: cppChildren.size()];
        for (std::vector<smile::IfsObject*>::iterator itor = cppChildren.begin();
             itor != cppChildren.end();
             itor++)
        {
            IfsItem* curItem = [[IfsItem alloc] initWithObject: *itor];
            [m_children addObject: curItem];
            [curItem release];
        }
    }
}

@end