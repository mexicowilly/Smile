#import "ConnectController.h"

@implementation ConnectController

- (id) initWithParentWindow: (NSWindow*)parentWindow;
{
    [super initWithWindowNibName: @"Connect" owner: self];
    m_parentWindow = parentWindow;
    return self;
}

- (IBAction) cancel: (id)sender
{
    m_wasCancelled = YES;
    [NSApp endSheet: [self window]];
}

- (IBAction) okay: (id)sender
{
    m_wasCancelled = NO;
    [NSApp endSheet: [self window]];
}

- (NSWindow*) parentWindow
{
    return m_parentWindow;
}
    
- (NSString*) password
{
    return [m_password stringValue];
}

- (NSString*) system
{
    return [m_system stringValue];
}

- (NSString*) user
{
    return [m_user stringValue];
}

- (BOOL) wasCancelled
{
    return m_wasCancelled;
}

@end
