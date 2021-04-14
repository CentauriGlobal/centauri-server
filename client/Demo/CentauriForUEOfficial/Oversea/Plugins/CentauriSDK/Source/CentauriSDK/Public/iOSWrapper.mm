//
//  iOSWrapper.m
//  
//
//  Created by cti on 2017/11/13.
//


#if PLATFORM_IOS

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

const void * getRootViewController()
{
    for (UIWindow *win in [UIApplication sharedApplication].windows) {
        if (win.hidden == NO && win.rootViewController) {
            return (__bridge const void *)win.rootViewController;
        }
    }
    return [UIApplication sharedApplication].keyWindow.rootViewController;
}
#endif

