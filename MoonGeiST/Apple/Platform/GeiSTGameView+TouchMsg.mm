/// Self header
#import "Platform/GeiSTGameView+TouchMsg.hpp"


#if (OS_TYPE == OS_TYPE_IOS)
@implementation GeiSTGameView (TouchMsg)


- (void) touchesBegan: (NSSet<UITouch *> *)touches withEvent: (nullable UIEvent *)event
{
    /// UIView的坐标系是TopSide Down的
    /// --- UIWindow/UIView坐标系 ---
    /// +-------------->  x
    /// |
    /// |
    /// |
    /// |
    /// v  Y
    UITouch * const touch_msg = touches.anyObject;
    const CGPoint pos_in_view = [touch_msg locationInView:touch_msg.view];
    NSLog(@"== Touch has begon ==");
    NSLog(@"[time]: %f, [pos]: < %f, %f >",
          touch_msg.timestamp,
          pos_in_view.x, pos_in_view.y);
}


//- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event;
//- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event;
//- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event;


@end
#endif /// (OS_TYPE == OS_TYPE_IOS)
