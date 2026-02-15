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
    Created:  11/02/26  @  5:44 PM
    FileName: PlatformDefines.hpp @ RedSoUL Project
    History:
             - created by: 11/02/26: Zenggang LIU
                                                                                        
***************************************************************************************/


#pragma once


#if !defined(OS_TYPE_UNKNOWN)
    #define OS_TYPE_UNKNOWN                     0xFFFF
    #define OS_TYPE_TEXT_UNKNOWN                "UNKNOWN"
    #define OS_SUB_TYPE_UNKNOWN                 0xFFFF
    #define OS_SUB_TYPE_TEXT_UNKNOWN            "UNKNOWN"
    #define CPU_TYPE_UNKNOWN                    0xFFFF
    #define CPU_TYPE_TEXT_UNKNOWN               "UNKNOWN"
#endif /// !defined(OS_TYPE_UNKNOWN)

#if !defined(CPU_TYPE_32)
    #define CPU_TYPE_32                         32
    #define CPU_TYPE_32_TEXT                    "32Bits"
#endif /// !defined(CPU_TYPE_32)

#if !defined(CPU_TYPE_64)
    #define CPU_TYPE_64                         64
    #define CPU_TYPE_64_TEXT                    "64Bits"
#endif /// !defined(CPU_TYPE_64)


///-------------------------------------------------------------------------------------
/// WINDOWS DEFINES
///-------------------------------------------------------------------------------------
/// MARK: - Windows 相关定义

/// Windows: 0x0xxx
#if !defined(OS_TYPE_WIN)
    #define OS_TYPE_WIN                         0x0001
    #define OS_TYPE_WIN_TEXT                    "Windows OS"
#endif /// !defined(OS_TYPE_WIN)

/// Windows PC application
#if !defined(OS_SUB_TYPE_WIN10)
    #define OS_SUB_TYPE_WIN10                   0x0100
    #define OS_SUB_TYPE_WIN10_TEXT              "Windows10 OS"
#endif /// !defined(OS_SUB_TYPE_WIN10)

/// Windows store application
#if !defined(OS_SUB_TYPE_WIN10_STORE)
    #define OS_SUB_TYPE_WIN10_STORE             0x0101
    #define OS_SUB_TYPE_WIN10_STORE_TEXT        "Windows Store App"
#endif /// !defined(OS_SUB_TYPE_WIN10_STORE)

/// Windows phone application
#if !defined(OS_SUB_TYPE_WIN_PHONE10)
    #define OS_SUB_TYPE_WIN_PHONE10             0x0102
    #define OS_SUB_TYPE_WIN_PHONE10_TEXT        "Windows Phone App"
#endif /// !defined(OS_SUB_TYPE_WIN_PHONE10)


///-------------------------------------------------------------------------------------
/// APPLE DEFINES
///-------------------------------------------------------------------------------------
/// MARK: - Apple 相关定义

/// Apple: 0x1xxx
#if !defined(OS_TYPE_MAC)
    #define OS_TYPE_MAC                         0x1000
    #define OS_TYPE_MAC_TEXT                    "Mac OS"
#endif /// !defined(OS_TYPE_MAC)

#if !defined(OS_TYPE_IOS)
    #define OS_TYPE_IOS                         0x1A00
    #define OS_TYPE_IOS_TEXT                    "iOS"
#endif /// !defined(OS_TYPE_IOS)

#if !defined(OS_TYPE_TVOS)
    #define OS_TYPE_TVOS                        0x1B00
    #define OS_TYPE_TVOS_TEXT                   "tvOS"
#endif /// !defined(OS_TYPE_TVOS)

#if !defined(OS_SUB_TYPE_MAC_STORE)
    #define OS_SUB_TYPE_MAC_STORE               0xAE  /// MAC STORE
    #define OS_SUB_TYPE_MAC_STORE_TEXT          "MacOS Store App"
#endif /// !defined(OS_SUB_TYPE_MAC_STORE)

#if !defined(OS_SUB_TYPE_IOS_DEVICE)
    #define OS_SUB_TYPE_IOS_DEVICE              0x1ADE
    #define OS_SUB_TYPE_IOS_DEVICE_TEXT         "iOS Device"
#endif /// !defined(OS_SUB_TYPE_IOS_DEVICE)

#if !defined(OS_SUB_TYPE_TVOS_DEVICE)
    #define OS_SUB_TYPE_TVOS_DEVICE             0x1BDE
    #define OS_SUB_TYPE_TVOS_DEVICE_TEXT        "tvOS Device"
#endif /// !defined(OS_SUB_TYPE_TVOS_DEVICE)

#if !defined(OS_SUB_TYPE_IOS_SIMULATOR)
    #define OS_SUB_TYPE_IOS_SIMULATOR           0x1A21 /// SI --> 2I --> 21
    #define OS_SUB_TYPE_IOS_SIMULATOR_TEXT      "iOS Simulator"
#endif /// !defined(OS_SUB_TYPE_IOS_SIMULATOR)

#if !defined(OS_SUB_TYPE_TVOS_SIMULATOR)
    #define OS_SUB_TYPE_TVOS_SIMULATOR          0x1B21 /// SI --> 2I --> 21
    #define OS_SUB_TYPE_TVOS_SIMULATOR_TEXT     "tvOS Simulator"
#endif /// !defined(OS_SUB_TYPE_TVOS_SIMULATOR)


///-------------------------------------------------------------------------------------
/// LINUX DEFINES
///-------------------------------------------------------------------------------------
/// MARK: - Linux 相关定义

/// Linux: 0x2xxx
#if !defined(OS_TYPE_LINUX)
    #define OS_TYPE_LINUX                       0x2000
    #define OS_TYPE_LINUX_TEXT                  "Linux OS"
#endif /// !defined(OS_TYPE_LINUX)


///-------------------------------------------------------------------------------------
/// ANDROID DEFINES
//--------------------------------------------------------------------------------------
/// MARK: - Android 相关定义

/// Android: 0x3xxx
#if !defined(OS_TYPE_ANDROID)
    #define OS_TYPE_ANDROID                     0x3000
    #define OS_TYPE_ANDROID_TEXT                "Andriod OS"
#endif /// !defined(OS_TYPE_ANDROID)


///-------------------------------------------------------------------------------------
/// DEFAULTS
///-------------------------------------------------------------------------------------
/// MARK: - Defaults 缺省定义

/// defines all defaults
#if !defined(OS_TYPE)
    #undef  OS_TYPE
    #define OS_TYPE             OS_TYPE_UNKNOWN
    #undef  OS_TYPE_TEXT
    #define OS_TYPE_TEXT        OS_TYPE_TEXT_UNKNOWN
    #undef  OS_SUB_TYPE
    #define OS_SUB_TYPE         OS_SUB_TYPE_UNKNOWN
    #undef  OS_SUB_TYPE_TEXT
    #define OS_SUB_TYPE_TEXT    OS_SUB_TYPE_TEXT_UNKNOWN
    #undef  CPU_TYPE
    #define CPU_TYPE            CPU_TYPE_UNKNOWN
    #undef  CPU_TYPE_TEXT
    #define CPU_TYPE_TEXT       CPU_TYPE_TEXT_UNKNOWN
#endif /// !defined(OS_TYPE)


///-------------------------------------------------------------------------------------
/// WINDOWS CHECKING
///-------------------------------------------------------------------------------------
/// MARK: - Windows 检测

/// ------------ Windows Family ------------ ///
/// --- Checks OS_TYPE --- ///
/// 如果 OS_TYPE 使用缺省值（OS_TYPE_UNKNOWN）: 表示无意义的OS类型
#if (OS_TYPE == OS_TYPE_UNKNOWN)
    #if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
        #undef  OS_TYPE
        #define OS_TYPE OS_TYPE_WIN
        #undef  OS_TYPE_TEXT
        #define OS_TYPE_TEXT OS_TYPE_WIN_TEXT
    #endif /// defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#endif /// (OS_TYPE == OS_TYPE_UNKNOWN)

/// --- Checks OS_SUB_TYPE --- ///
/// 如果 OS_SUB_TYPE 为缺省值（OS_SUB_TYPE_UNKNOWN）进行如下诸多检测
#if (OS_SUB_TYPE == OS_SUB_TYPE_UNKNOWN)
    #if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
        #undef  OS_SUB_TYPE
        #define OS_SUB_TYPE OS_SUB_TYPE_WIN10_STORE
        #undef  OS_SUB_TYPE_TEXT
        #define OS_SUB_TYPE_TEXT OS_SUB_TYPE_WIN10_STORE_TEXT
    #endif /// defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#endif /// (OS_SUB_TYPE == OS_TYPE_UNKNOWN)

#if (OS_SUB_TYPE == OS_TYPE_UNKNOWN)
    #if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
        #undef  OS_SUB_TYPE
        #define OS_SUB_TYPE OS_SUB_TYPE_WIN_PHONE10
        #undef  OS_SUB_TYPE_TEXT
        #define OS_SUB_TYPE_TEXT OS_SUB_TYPE_WIN_PHONE10_TEXT
    #endif /// defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
#endif /// (OS_SUB_TYPE == OS_TYPE_UNKNOWN)

#if (OS_SUB_TYPE == OS_TYPE_UNKNOWN)
    #if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
        #undef  OS_SUB_TYPE
        #define OS_SUB_TYPE OS_SUB_TYPE_WIN10
        #undef  OS_SUB_TYPE_TEXT
        #define OS_SUB_TYPE_TEXT OS_SUB_TYPE_WIN10_TEXT
    #endif /// defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#endif /// (OS_SUB_TYPE == OS_TYPE_UNKNOWN)

/// --- Checks CPU_TYPE --- ///
/// 首先检测 WIN64，由于 Win64 以及 Win32 在 x64 平台上同时定义
#if (CPU_TYPE == CPU_TYPE_UNKNOWN)
    #if defined(WIN64) || defined(_WIN64)
        #undef  CPU_TYPE
        #define CPU_TYPE CPU_TYPE_64
        #undef  CPU_TYPE_TEXT
        #define CPU_TYPE_TEXT CPU_TYPE_64_TEXT
    #elif defined(WIN32) || defined(_WIN32)
        #undef  CPU_TYPE
        #define CPU_TYPE CPU_TYPE_32
        #undef  CPU_TYPE_TEXT
        #define CPU_TYPE_TEXT CPU_TYPE_32_TEXT
    #endif /// defined(WIN64) || defined(_WIN64)
#endif /// (CPU_TYPE ==  CPU_TYPE_UNKNOWN)


///-------------------------------------------------------------------------------------
/// APPLE CHECKING
///-------------------------------------------------------------------------------------
/// MARK: - Apple 检测

/// 使用如下方式 DUMP 出所有预定义的宏
/// How to dump all predefined marcos:
/// -x <language>           Treat subsequent input files as having type <language>
/// -dM                     Print macro definitions in -E mode instead of normal output
/// -E                      Only run the preprocessor
/// ARCH 参数可以取如下数值: arm64, x86_64, i386
///
/// /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang
/// -x c++ -arch ARCH -std=c++11 -stdlib=libc++ -fno-exceptions -fno-rtti -dM -E /dev/null
///
/// --- arm 64 ---
/// /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -x c++ -arch arm64 -std=c++11 -stdlib=libc++ -fno-exceptions -fno-rtti -dM -E /dev/null > predefined-macros_arm64.txt
/// --- x64 ---
/// /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -x c++ -arch x86_64 -std=c++11 -stdlib=libc++ -fno-exceptions -fno-rtti -dM -E /dev/null > predefined-macros_x64.txt
/// --- i386 --
/// /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -x c++ -arch i386 -std=c++11 -stdlib=libc++ -fno-exceptions -fno-rtti -dM -E /dev/null > predefined-macros_i386.txt
///
/// 参考:
/// - Documents/Compiler/XCode/Predefined_Macros/*.*

/// ------------ Apple Family ------------ ///
#if (OS_TYPE == OS_TYPE_UNKNOWN)
    /// --- Checks OS_TYPE and OS_SUB_TYPE --- ///
    #if defined(__APPLE__)
        #include <TargetConditionals.h>
        /// +----------------------------------------------------------------+
        /// |                TARGET_OS_MAC                                   |
        /// | +---+  +-----------------------------------------------------+ |
        /// | |   |  |          TARGET_OS_IPHONE                           | |
        /// | |OSX|  | +-----+ +----+ +-------+ +--------+ +-------------+ | |
        /// | |   |  | | IOS | | TV | | WATCH | | BRIDGE | | MACCATALYST | | |
        /// | |   |  | +-----+ +----+ +-------+ +--------+ +-------------+ | |
        /// | +---+  +-----------------------------------------------------+ |
        /// +----------------------------------------------------------------+

        #if (TARGET_OS_IOS && TARGET_OS_SIMULATOR)
            /// iOS 模拟器（Simulator）
            #undef  OS_TYPE
            #define OS_TYPE OS_TYPE_IOS
            #undef  OS_TYPE_TEXT
            #define OS_TYPE_TEXT OS_TYPE_IOS_TEXT
            #undef  OS_SUB_TYPE
            #define OS_SUB_TYPE OS_SUB_TYPE_IOS_SIMULATOR
            #undef  OS_SUB_TYPE_TEXT
            #define OS_SUB_TYPE_TEXT OS_SUB_TYPE_IOS_SIMULATOR_TEXT

        #elif TARGET_OS_IOS
            /// iOS 硬件设备（device）
            #undef  OS_TYPE
            #define OS_TYPE OS_TYPE_IOS
            #undef  OS_TYPE_TEXT
            #define OS_TYPE_TEXT OS_TYPE_IOS_TEXT
            #undef  OS_SUB_TYPE
            #define OS_SUB_TYPE OS_SUB_TYPE_IOS_DEVICE
            #undef  OS_SUB_TYPE_TEXT
            #define OS_SUB_TYPE_TEXT OS_SUB_TYPE_IOS_DEVICE_TEXT

        #elif (TARGET_OS_TV && TARGET_OS_SIMULATOR)
            /// tvOS 模拟器（Simulator）
            #undef  OS_TYPE
            #define OS_TYPE OS_TYPE_TVOS
            #undef  OS_TYPE_TEXT
            #define OS_TYPE_TEXT OS_TYPE_TVOS_TEXT
            #undef  OS_SUB_TYPE
            #define OS_SUB_TYPE OS_SUB_TYPE_TVOS_SIMULATOR
            #undef  OS_SUB_TYPE_TEXT
            #define OS_SUB_TYPE_TEXT OS_SUB_TYPE_TVOS_SIMULATOR_TEXT

        #elif TARGET_OS_TV
            /// tvOS 硬件设备（device）
            #undef  OS_TYPE
            #define OS_TYPE OS_TYPE_TVOS
            #undef  OS_TYPE_TEXT
            #define OS_TYPE_TEXT OS_TYPE_TVOS_TEXT
            #undef  OS_SUB_TYPE
            #define OS_SUB_TYPE OS_SUB_TYPE_TVOS_DEVICE
            #undef  OS_SUB_TYPE_TEXT
            #define OS_SUB_TYPE_TEXT OS_SUB_TYPE_TVOS_DEVICE_TEXT

        #else
            /// macOS
            #undef  OS_TYPE
            #define OS_TYPE OS_TYPE_MAC
            #undef  OS_TYPE_TEXT
            #define OS_TYPE_TEXT OS_TYPE_MAC_TEXT
            #undef  OS_SUB_TYPE
            #define OS_SUB_TYPE OS_SUB_TYPE_MAC_STORE
            #undef  OS_SUB_TYPE_TEXT
            #define OS_SUB_TYPE_TEXT OS_SUB_TYPE_MAC_STORE_TEXT
        #endif /// (TARGET_OS_IOS && TARGET_IPHONE_SIMULATOR)
    #endif /// defined(__APPLE__)

    /// --- Checks CPU_TYPE --- ///
    #if defined(_LP64) || defined(__LP64__)
        #undef  CPU_TYPE
        #define CPU_TYPE CPU_TYPE_64
        #undef  CPU_TYPE_TEXT
        #define CPU_TYPE_TEXT CPU_TYPE_64_TEXT
    #else
        #undef  CPU_TYPE
        #define CPU_TYPE CPU_TYPE_32
        #undef  CPU_TYPE_TEXT
        #define CPU_TYPE_TEXT CPU_TYPE_32_TEXT
    #endif /// defined(__x86_64) || defined(__x86_64__)
#endif /// (OS_TYPE == OS_TYPE_UNKNOWN)


///-------------------------------------------------------------------------------------
/// LINUX CHECKING
///-------------------------------------------------------------------------------------
/// MARK: - Linux检测

/// ------------ Linux Family ------------ ///
#if (OS_TYPE == OS_TYPE_UNKNOWN)
    #if defined(__linux__)
        #undef  OS_TYPE
        #define OS_TYPE OS_TYPE_LINUX
        #undef  OS_TYPE_TEXT
        #define OS_TYPE_TEXT OS_TYPE_LINUX_TEXT
    #endif /// defined(__linux__)

    /// --- Checks CPU_TYPE --- ///
    #if defined(__x86_64__) || defined(_M_X64)
        #undef  CPU_TYPE
        #define CPU_TYPE CPU_TYPE_64
        #undef  CPU_TYPE_TEXT
        #define CPU_TYPE_TEXT CPU_TYPE_64_TEXT
    #elif defined(__i386__) || defined(_M_IX86)
        #undef  CPU_TYPE
        #define CPU_TYPE CPU_TYPE_32
        #undef  CPU_TYPE_TEXT
        #define CPU_TYPE_TEXT CPU_TYPE_32_TEXT
    #else
        #error Unknown CPU type
    #endif /// defined(__x86_64__) || defined(_M_X64)
#endif /// (OS_TYPE == OS_TYPE_UNKNOWN)


///-------------------------------------------------------------------------------------
/// ANDROID CHECKING
///-------------------------------------------------------------------------------------
/// MARK: - Android检测

/// ------------ Android Family ------------ ///
#if (OS_TYPE == OS_TYPE_UNKNOWN)
    #if defined(PLATFORM_ANDROID)
        #undef  OS_TYPE
        #define OS_TYPE OS_TYPE_ANDROID
        #undef  OS_TYPE_TEXT
        #define OS_TYPE_TEXT OS_TYPE_ANDROID_TEXT
    #endif /// defined(PLATFORM_ANDROID)
#endif /// (OS_TYPE == OS_TYPE_UNKNOWN)


/// 判断所需要的Apple OS版本
#if defined(__APPLE__)
    #if (OS_TYPE == OS_TYPE_IOS)
        /// 至少需要iOS 13.0
        #if (__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ < 130000)
            #error We need iOS 13+ to run this library
        #endif
    #elif (OS_TYPE == OS_TYPE_MAC)
        /// 至少需要macOS 10.15
        #if (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 101500 /* __MAC_10_15 */)
            #error We need macOS 10.15+ to run this library
        #endif
    #elif (OS_TYPE == OS_TYPE_TVOS)
        /// 至少需要tvOS 13.0
        #if (__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ < 130000)
            #error We need tvOS 13+ to run this library
        #endif
    #else
        #error Unknown Apple platform
    #endif /// (OS_TYPE == OS_TYPE_IOS)
#endif /// defined(__APPLE__)
