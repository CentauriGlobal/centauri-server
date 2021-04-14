// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MidasBlueprintFunctionLibrary.generated.h"

/**
 * 游戏可以进行参考的BP demo
 */
UCLASS()
class MIDASSAMPLE_API UMidasBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
    
    //----- sample登录开始 ----
    UFUNCTION(BlueprintCallable, Category="MidasBPLibrary")
    static FString midasSampleLogin();
    //----- sample登录结束 ----
    
	//--------- 支付接口调用开始 ---------
    UFUNCTION(BlueprintCallable, Category="MidasBPLibrary")
    static FString midasSampleInit();
    
    UFUNCTION(BlueprintCallable, Category="MidasBPLibrary")
    static FString midasSampleBuyGame();
    
    UFUNCTION(BlueprintCallable, Category="MidasBPLibrary")
    static FString midasSampleBuyGoods();
    
    UFUNCTION(BlueprintCallable, Category="MidasBPLibrary")
    static FString midasSampleBuySubscribe();
    
    UFUNCTION(BlueprintCallable, Category="MidasBPLibrary")
    static FString midasSampleBuyMonth();
    
    UFUNCTION(BlueprintCallable, Category="MidasBPLibrary")
    static FString midasSampleGetInfo();
    
    UFUNCTION(BlueprintCallable, Category="MidasBPLibrary")
    static FString midasSampleGetLocalPrice();
    
    
    //--------- 支付接口调用结束 ---------
    
#if PLATFORM_IOS
    static void startIOSGamePay();
    static void startIOSGoodsPay();
    static void startIOSSubscribePay();
    static void startIOSMonthPay();
#endif
    

};
