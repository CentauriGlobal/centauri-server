//
//  CTIIAPPayDelegate.h
//  CTIIAPSDK
//
//  Created by seahubhuang on 2017/8/3.
//  Copyright © 2017年 centauri. All rights reserved.
//

#import <Foundation/Foundation.h>

@class CTIIAPPayResp;

/*!
 @protocol APCTIPayDelegate
 @abstract 米大师支付业务回调
 @author bladebao
 */
@protocol CTIIAPPayDelegate <NSObject>

/// 通过APCTIBaseReq及其子类传入的登录态失效，需要重新登录
/// 注意：回调并不保证一定在主线程执行，进行 UI 更新等相关操作时，一定要进行主线程分发
/// @author bladebao
- (void)needLogin;

/// 米大师支付服务的返回结果接口
/// 注意：回调并不保证一定在主线程执行，进行 UI 更新等相关操作时，一定要进行主线程分发
/// @param resp 返回的结果对象，可参见 APCTIResp 定义
/// @author bladebao
- (void)onResp:(CTIIAPPayResp *)resp;

@end
