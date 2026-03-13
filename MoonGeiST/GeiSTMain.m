#import "Platform/GeiSTAppDelegate.hpp"


int
main (
    int           argc,
    const char ** argv)
{
    /// 创建Application以及ApplicationDelegate
#if (OS_TYPE == OS_TYPE_IOS)
    NSString * app_delegate_class_name;
    @autoreleasepool
    {
        app_delegate_class_name = NSStringFromClass([GeiSTAppDelegate class]);
    }

    return UIApplicationMain(argc, (char**)argv, nil, app_delegate_class_name);

#elif (OS_TYPE == OS_TYPE_MAC)
    return NSApplicationMain(argc, argv);
#else
    #error Not supported Platform!!
#endif /// (OS_TYPE == OS_TYPE_IOS)
}

