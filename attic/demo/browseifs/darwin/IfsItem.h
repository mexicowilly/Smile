#import <Cocoa/Cocoa.h>
#import <smile/IfsDirectory.hpp>

@interface IfsItem : NSObject
{
@private
    smile::IfsObject* m_object;
    NSMutableArray* m_children;
}

- (id) initWithObject: (smile::IfsObject*)object;
- (void) dealloc;

- (IfsItem*) childAtIndex: (unsigned)idx;
- (BOOL) canHaveChildren;
- (unsigned) numberOfChildren;
- (NSString*) name;

@end
