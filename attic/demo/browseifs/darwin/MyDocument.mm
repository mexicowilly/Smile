#import "MyDocument.h"
#import "ConnectController.h"

@implementation MyDocument

- (id)init
{
    self = [super init];
    if (self)
        m_dataSource = nil;
    return self;
}

- (void) dealloc
{
    [m_dataSource release];
    [super dealloc];
}

- (void)windowControllerDidLoadNib:(NSWindowController *) aController
{
    [super windowControllerDidLoadNib:aController];
    // Add any code here that needs to be executed once the windowController has loaded the document's window.
}

- (void) sheetDidEnd: (NSWindow*)sheet returnCode: (int)rc contextInfo: (void*)ctx
{
    ConnectController* connect = (ConnectController*)ctx;
    [sheet orderOut: self];
    if ([connect wasCancelled])
    {
        [[connect parentWindow] close];
    }
    else
    {
        m_dataSource = [[IfsDataSource alloc] initWithSystem: [connect system]
            user: [connect user] password: [connect password]];
        [m_outlineView setDataSource: m_dataSource];
        [m_outlineView expandItem: [m_outlineView itemAtRow: 0]];
    }
    [connect release];
}

- (void) windowDidBecomeMain: (NSNotification*)notification
{
    if (m_dataSource == nil)
    {
        NSWindow* window = (NSWindow*)[notification object];
        ConnectController* connect = [[ConnectController alloc] initWithParentWindow: window];
        [NSApp beginSheet: [connect window]
            modalForWindow: window
            modalDelegate: self
            didEndSelector: @selector(sheetDidEnd:returnCode:contextInfo:) 
            contextInfo: connect];
    }
}

- (NSString*) windowNibName
{
    return @"MyDocument";
}

- (NSData *)dataRepresentationOfType:(NSString *)aType
{
    // Insert code here to write your document from the given data.  You can also choose to override -fileWrapperRepresentationOfType: or -writeToFile:ofType: instead.
    
    // For applications targeted for Tiger or later systems, you should use the new Tiger API -dataOfType:error:.  In this case you can also choose to override -writeToURL:ofType:error:, -fileWrapperOfType:error:, or -writeToURL:ofType:forSaveOperation:originalContentsURL:error: instead.

    return nil;
}

- (BOOL)loadDataRepresentation:(NSData *)data ofType:(NSString *)aType
{
    // Insert code here to read your document from the given data.  You can also choose to override -loadFileWrapperRepresentation:ofType: or -readFromFile:ofType: instead.
    
    // For applications targeted for Tiger or later systems, you should use the new Tiger API readFromData:ofType:error:.  In this case you can also choose to override -readFromURL:ofType:error: or -readFromFileWrapper:ofType:error: instead.
    
    return YES;
}

@end
