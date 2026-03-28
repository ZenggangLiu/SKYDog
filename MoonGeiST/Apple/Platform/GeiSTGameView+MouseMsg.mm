/// Self header
#import "Platform/GeiSTGameView+MouseMsg.hpp"


#if (OS_TYPE == OS_TYPE_MAC)
@implementation GeiSTGameView (MouseMsg)


- (void) mouseDown: (NSEvent *)event;
{
    NSLog(@"== Mouse down ==");
}


- (void) mouseUp: (NSEvent *)event
{

}


- (void) rightMouseDown: (NSEvent *)event
{

}


- (void) rightMouseUp: (NSEvent *)event
{

}


- (void) mouseMoved: (NSEvent *)event
{

}


- (void) mouseDragged: (NSEvent *)event
{

}


- (void) scrollWheel: (NSEvent *)event
{

}


@end
#endif /// (OS_TYPE == OS_TYPE_IOS)
