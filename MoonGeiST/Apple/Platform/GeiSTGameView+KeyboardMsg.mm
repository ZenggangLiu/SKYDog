/// Self header
#import "Platform/GeiSTGameView+MouseMsg.hpp"


#if (OS_TYPE == OS_TYPE_MAC)
@implementation GeiSTGameView (MouseMsg)


/// 必须将当前View设置为First Responder，以接收Keyboard消息
- (BOOL) acceptsFirstResponder
{
    return YES;
}


- (void) keyDown: (NSEvent *)event;
{
    NSLog(@"== Keyboard down ==");
}


- (void) keyUp: (NSEvent *)event
{
    
}


@end
#endif /// (OS_TYPE == OS_TYPE_IOS)
