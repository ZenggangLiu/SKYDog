/***************************************************************************************
                                                                                        
        *          .               *                              .               *     
        ███████╗██╗  ██╗██╗   ██╗        ██████╗  ██████╗  ██████╗         *            
        ██╔════╝██║ ██╔╝╚██╗ ██╔╝        ██╔══██╗██╔═══██╗██╔════╝                      
        ███████╗█████╔╝  ╚████╔╝         ██║  ██║██║   ██║██║  ███╗        .            
        ╚════██║██╔═██╗   ╚██╔╝          ██║  ██║██║   ██║██║   ██║                     
        ███████║██║  ██╗    ██║           ██████╔╝╚██████╔╝╚██████╔╝         *          
        ╚══════╝╚═╝  ╚═╝    ╚═╝           ╚═════╝  ╚═════╝  ╚═════╝                     
                                                                                        
        <~~~               .        SKY Dog Game                      ~~~>        *     
                                Real-Time | Cross-Platform           .                  
----------------------------------------------------------------------------------------
                                                                                        
                                  ,,                                                    
                  __           o-°°|\_____/)                                            
    Author:   (___()'`; Zee...  \_/|_)     )                                            
              /,    /`             \  __  /                                             
              \\"--\\              (_/ (_/                                              
    Created:  12/03/26  @  5:32 PM
    FileName: GeiSTAppDelegate.hpp @ MoonGeiST Project
    History:
             - created by: 12/03/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


/// System headers
#include "Common/PlatformDefines.hpp" /// OS_TYPE
#if (OS_TYPE == OS_TYPE_IOS)
#import <UIKit/UIKit.h>
#elif (OS_TYPE == OS_TYPE_MAC)
#import <Cocoa/Cocoa.h>
#else
#error Not supported Platform!!
#endif /// (OS_TYPE == OS_TYPE_IOS)


#if (OS_TYPE == OS_TYPE_IOS) /// iOS
@interface GeiSTAppDelegate : UIResponder <UIApplicationDelegate>

@property (nonatomic, strong) UIWindow * window;

#elif (OS_TYPE == OS_TYPE_MAC) /// macOS
@interface GeiSTAppDelegate : NSObject <NSApplicationDelegate>
#else
#error Not supported Platform!!
#endif /// (OS_TYPE == OS_TYPE_IOS)

@end
