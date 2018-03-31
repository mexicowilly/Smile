#import <Cocoa/Cocoa.h>

@interface ConnectController : NSWindowController
{
    IBOutlet NSTextField* m_system;
    IBOutlet NSTextField* m_user;
    IBOutlet NSTextField* m_password;
    
@private
    BOOL m_wasCancelled;
    NSWindow* m_parentWindow;
}

- (id) initWithParentWindow: (NSWindow*)parentWindow;
- (IBAction) cancel: (id)sender;
- (IBAction) okay: (id)sender;
- (NSWindow*) parentWindow;
- (NSString*) password;
- (NSString*) system;
- (NSString*) user;
- (BOOL) wasCancelled;

@end
