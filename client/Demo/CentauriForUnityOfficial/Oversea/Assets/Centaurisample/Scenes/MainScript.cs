using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;
using CentauriPay;
using System;
using System.Threading;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using CentauriPay;
using UnityEditor;

public class MainScript : MonoBehaviour {
	
	public static string jsonData;
	
	private ICTIPayService mICTIPayService;

	void Start()
	{
		//Get the instance object of Centauri payment
		mICTIPayService = CTIPayService.Instance;
		// open Console log
		ULog.setLevel(ULog.Level.Log);
	}
	
	/// <summary>
	/// Set required fields
	/// </summary>
	/// 
	/// <param name="req">Req.</param> 
	private void SetCommonField(CTIBaseRequest req)
	{
		#if UNITY_EDITOR
		req.offerId = "1450001341";
		req.pf = "IEG_iTOP-2001-android-2011-FB-1130-67137586594922-AppCode";
		#elif UNITY_ANDROID
		//The offerID you had configed on Midas platform.
		req.offerId = "1450005285";

		//The pf you had configed on Midas platform.
		req.pf = @"huyu_m-2001-android";
		
		#elif UNITY_IOS
		//The offerID you had configed on Midas platform.
		req.offerId = "1450000495";

		//The pf you had configed on Midas platform.
		req.pf = "huyu_m-2001-iap";
		#endif
		//The Country of your project
		req.country = "US";
		//The Currency of your project
		req.currencyType = "USD";
		//User unique account ID     
		req.openId = "tedyang";
		//Game server region ID, the default value is "1" for games regardless of region. If the application selects a supported role, then the role is connected to the partition ID number with "_", and the role needs to be urlencoded
		req.zoneId = "1";
	}
	
	/// <summary>
	/// Only for Android.
	/// Pop-up information prompt
	/// </summary>
	/// <param name="message">要弹出的信息</param>
	public static void ShowAndroidToastMessage(string message)
	{
		AndroidJavaClass unityPlayer = new AndroidJavaClass("com.unity3d.player.UnityPlayer");
		AndroidJavaObject unityActivity = unityPlayer.GetStatic<AndroidJavaObject>("currentActivity");
		if (unityActivity != null)
		{
			AndroidJavaClass toastClass = new AndroidJavaClass("android.widget.Toast");
			unityActivity.Call("runOnUiThread", new AndroidJavaRunnable(() =>
			{
				AndroidJavaObject toastObject = toastClass.CallStatic<AndroidJavaObject>("makeText", unityActivity, message, 0);
				toastObject.Call("show");
			}));
		}
	}
	
   /// <summary>
   /// The callback of pay.
   /// </summary>
	private class MyCTIPayCallback : CTIPayCallback
	{
		void CTIPayCallback.OnCentauriLoginExpired()
		{
			jsonData = "CTIPayCallback.OnCentauriLoginExpired";
			Debug.Log("MainScript.OnCentauriLoginExpired");
		}
		
		void CTIPayCallback.OnCentauriPayFinished(CTIResponse result)
		{
			Debug.Log("MainScript.OnCentauriPayFinished: Pay result = " + result.ToString ());
			jsonData = "CentauriPayFinished->\n"+result.ToString ();
			var str = Json.Serialize(result);
			if (result.resultCode.Equals(0))
			{
				Debug.Log("OnCentauriInitFinished success");
				#if UNITY_EDITOR

				#elif UNITY_ANDROID
				ShowAndroidToastMessage("OnCentauriPay Success");
				#elif UNITY_IOS
						
				#endif
			}
		}
	}
	
	/// <summary>
	/// The callback of init.
	/// </summary>
	private class MyCTIInitCallback : CTIInitCallback
	{
		public void OnCentauriInitFinished(Dictionary<string, object> result)
		{
			var str = Json.Serialize(result);
			Debug.Log("OnCentauriInitFinished deserialized: " + str);
			jsonData = "OnCentauriInitFinished deserialized: " + str;
			if (result["ret"].ToString().Equals("0"))
			{
				jsonData = "OnCentauriInitFinished success";
				Debug.Log("OnCentauriInitFinished success");
			}
		}
	}
	/// <summary>
	/// The callback of get market activities information.
	/// </summary>
	private class MyCTIGetInfoCallback : CTIGetInfoCallback
	{
		public void GetInfoFinished (string type, int retCode, string result)
		{
			jsonData = "GetInfoFinish \n"+"type = "+type +";\nretCode = "+retCode+";\nresult = "+result;
			UnityEngine.Debug.Log ("MainScript.GetInfoFinish type = " + type);
			UnityEngine.Debug.Log ("MainScript.GetInfoFinish retCode = " + retCode);
			UnityEngine.Debug.Log ("MainScript.GetInfoFinish result = " + result);
		}
	}
	
	/// <summary>
	/// The callback of get product information.
	/// The format of the returned json is as follows：
	/// {"ret":"0","msg":"success",
	/// "productInfo":[{"productId":"midas_product_1","price":"HK$7.75","currency":"HKD","microprice":"7750000"}]}
	/// </summary>
	private class MyCTIGetProductCallback : CTIGetLocalPriceCallback
	{
		public void OnCentauriGetProdcut(Dictionary<string, object> result)
		{
			var str = Json.Serialize(result);
			jsonData = "GetProduct Finish result = " + str;
			Debug.Log("OnCentauriGetProdcut deserialized: " + str);
			if (result["ret"].ToString().Equals("0"))
			{
				Debug.Log("OnCentauriGetProdcut success");
				List<object> arr = (List<object>)result["productInfo"];
				foreach (object o in arr)
				{
					Dictionary<string, object> dic = (Dictionary<string, object>)o;
					Debug.Log("productId："+ dic["productId"].ToString());
					Debug.Log("price：" + dic["price"].ToString());
					Debug.Log("microprice：" + dic["microprice"].ToString());
					Debug.Log("currency：" + dic["currency"].ToString());
				}
			}
		}
	}

	/// <summary>
	/// The callback of get promotion product information.
	/// </summary>
	private class MyCTIGetIntroPriceCallback : CTIGetIntroPriceCallback
	{
		public void OnCentauriGetIntroPrice(Dictionary<string, object> result)
		{
			var str = Json.Serialize(result);
			jsonData = "GetPrice Finish result = " + str;
			Debug.Log("OnCTIGetIntroPrice deserialized: " + str);
		}
	}

	/// <summary>
	/// The callback of reprovide.
	/// </summary>
	private class MyCTIReprovideCallback : CTIReprovideCallback
	{
		public void OnCentauriReprovideFinished(Dictionary<string, object> result)
		{
			var str = Json.Serialize(result);
			jsonData = "Reprovide Finish result = " + str;
			Debug.Log("OnCentauriReprovideFinished deserialized: " + str);
		}
	}
	
	void OnGUI()
	{
		// 设置按钮中文字的颜色
		GUI.color = Color.white;
		// 设置按钮的背景色
		GUI.backgroundColor = Color.blue;
		
		// 设置GUI控件的字体大小
		GUI.skin.button.fontSize = 30;
		GUI.skin.textArea.fontSize = 30;
		
		
		// Initialize entry
		if (GUI.Button(new Rect(100,100,350,100),"初始化")) 
		{	
			#if(SEA)
			mICTIPayService.SetProcess("sea");//sdk走海外流程
			#elif(LOCAL)
			mICTIPayService.SetProcess("local");//sdk走国内流程
			#endif
			
			CTIBaseRequest req = new CTIBaseRequest ();
			// Set required fields.
			SetCommonField(req);
			
			//Set idip partition.
			req.goodsZoneId = "1";
			
			jsonData = "已执行初始化！";
			
			MyCTIInitCallback callback = new MyCTIInitCallback();
			/// init SDK 
			mICTIPayService.Initialize ("local", "test", "", req, callback);
			
			//Note: The following interfaces need to be called after init.
			//Optional: Get the version number of the SDK.
			string version = mICTIPayService.GetCTISDKVersion ();
			ULog.Log ("GetCTISDKVersion = " + version);
			
			/// Set the log enable or not.
			mICTIPayService.SetLogEnable (true);
			
			//Optional: Whether it is possible to pay, mainly due to iOS jailbreak or the in-app purchase function in the phone settings is closed, it will return false, indicating that the payment conditions are not met.
			bool enable = mICTIPayService.IsPayEnable ();
			ULog.Log ("IsPayEnable = " + enable);
		}
		
		// Game currency Pay entry
		if (GUI.Button(new Rect(100,250,350,100),"游戏币支付"))  
		{
			jsonData = GUI.TextArea (new Rect (600, 400, 300, 200), jsonData);
			
			CTIPayCallback callback = new MyCTIPayCallback();
			
			CTIGameRequest req = new CTIGameRequest();
			// Set required fields.
			SetCommonField(req);
			
			#if UNITY_EDITOR
			#elif UNITY_ANDROID   
			//The game productID had configed on Midas and googleplay platform.
			req.productId = "midas_product_1";
			#elif UNITY_IOS 
			//The game productID had configed on Midas and iap platform.
			req.productId = "com.tencent.pay.test1";
			//The value of payItem: props, pass "item ID unit price (unit: angle) quantity"; monthly pass, pass "days"; game currency, pass "quantity"
			req.payItem = "100";
			//Game transparent field.
			req.appExtends = "aaa=1234";
			#endif
			req.payChannel = "os_offical";
			jsonData = "准备拉起游戏币支付！";        
			
			//Transfer payment interface.
			mICTIPayService.Pay(req, callback);
		}
		
		// Goods Pay entry.
		if (GUI.Button(new Rect(550, 100, 350, 100), "道具直购"))
		{
			CTIPayCallback callback = new MyCTIPayCallback();
			
			CTIGoodsRequest req = new CTIGoodsRequest();
			
			// Set required fields.
			SetCommonField(req);
			
			#if UNITY_EDITOR
			req.offerId = "1450001341";
			#elif UNITY_ANDROID
			//The offerID you had configed on Midas platform.
			req.offerId = "1450005285";
			//The game productID had configed on Midas and googleplay platform.
			req.productId = "midas_product_2";
			
			#elif UNITY_IOS
			//The offerID you had configed on Midas platform.
			req.offerId = "1450005935";
			//The value of payItem: props, pass "item ID unit price (unit: angle) quantity"; monthly pass, pass "days"; game currency, pass "quantity"
			req.payItem = "com.tencent.qpaytest.qqvip*120*1";
			//The game productID had configed on Midas and iap platform.
			req.productId = "com.tencent.qpaytest.qqvip";
			#endif
			
			Debug.Log ("mICTIPayService.PayGoods string = " + req.ToString());
			req.payChannel = "os_offical";
			jsonData = "准备拉起道具支付！";
			//Transfer payment interface.
			mICTIPayService.Pay(req, callback);
		}
		
		// Month card entry
		if (GUI.Button(new Rect(550, 250, 350, 100), "月卡支付"))
		{
  		        jsonData = "准备拉起月卡支付！";
			CTIPayCallback callback = new MyCTIPayCallback();
			
			CTISubscribeRequest req = new CTISubscribeRequest();
			// Set required fields.
			SetCommonField (req);
			#if UNITY_EDITOR
			req.offerId = "1450000766";
			#elif UNITY_ANDROID
			// jsonData = "暂时不支持非自动续费月卡测试！";
			req.offerId = "1450005285";
			req.payItem = "31";
			req.productId = "midas_product_2";
						
			#elif UNITY_IOS
			req.offerId = "1450011344";
			//Monthly card business code
			req.serviceCode = "test";
			//Monthly card business name
            req.serviceName = "test";
			req.productId = "com.tencent.qpaytest.nonautorenew.monthcard_31";
			#endif
			req.payChannel = "os_offical";
			
			//Transfer payment interface.
			mICTIPayService.Pay(req, callback);
		}
		

		
		// Get market activities informations entry
		if (GUI.Button(new Rect(100, 400, 350, 100), "GetInfo"))
		{
			CTIGetInfoCallback callback = new MyCTIGetInfoCallback();
			
			CTIBaseRequest req = new CTIBaseRequest();
			// Set required fields.
			SetCommonField (req);
			
			jsonData = "已选择GetInfo!";
			
			// The first parameter is reqType, used to identify the type of getInfo,
			// Passing "mp" here means to pull mpInfo marketing information(Fixed values "mp")
			mICTIPayService.GetInfo("mp", req, callback);
			//mICTIPayService.GetInfo("mp", req, callback);
		}
		
		// Get product information entry
		if (GUI.Button(new Rect(550, 400, 350, 100), "GetPrice"))
		{
			jsonData = "start getproduct";
			
			#if UNITY_EDITOR
			ULog.Log("use simulator");
#elif UNITY_ANDROID
            Dictionary<string,string> dict = new Dictionary<string,string>();
			dict.Add("midas_product_1","inapp");
			dict.Add("com.tencent.pay.test3","");
			//fixed values channel="os_offical",if dict is not empty,get the Specified product information，other get the all product information. 
			mICTIPayService.GetLocalPrice("os_offical", dict,new MyCTIGetProductCallback());
#elif UNITY_IOS
			Dictionary<string,string> dict = new Dictionary<string,string>();
			dict.Add("com.tencent.pay.test1","");
			dict.Add("com.tencent.pay.test3","");
			//Fixed values channel="os_offical",if dict is not empty,get the Specified promotion product information，other get the all product information. 
			//mICTIPayService.GetIntroPrice("iap", dict,new MyCTIGetIntroPriceCallback());
			//Fixed values channel="os_offical",if dict is not empty,get the Specified product information，other get the all product information. 
			mICTIPayService.GetLocalPrice("iap", dict,new MyCTIGetProductCallback());
#endif
		}

		// Month card entry
		if (GUI.Button(new Rect(100, 550, 350, 100), "自动续费月卡"))
		{
			CTIPayCallback callback = new MyCTIPayCallback();
			
			CTISubscribeRequest req = new CTISubscribeRequest();
			// Set required fields.
			SetCommonField (req);
			#if UNITY_EDITOR
			#elif UNITY_ANDROID
			req.productId = "testsub1";
			req.serviceCode = "test";
                        req.serviceName = "test";
			#elif UNITY_IOS
			req.offerId = "1450008464";
			req.payItem = "31";
			req.productId = "com.tencent.qpaytest.otherautorenew1month";
			#endif
			req.autoPay = true;
			req.payChannel = "os_offical";
			
			jsonData = "准备拉起自动续费月卡支付！";
			//Transfer payment interface.
			mICTIPayService.Pay(req, callback);
		}
		
		//Reprovide entry
		if (GUI.Button(new Rect(550, 550, 350, 100), "Reprovide"))
        {
            jsonData = "Reprovide";
			mICTIPayService.Reprovide(new MyCTIReprovideCallback());
         }
		
		// 包月支付
		if (GUI.Button(new Rect(100, 700, 800, 100), "包月支付"))
		{
			jsonData = "暂时没有包月物品支持测试";
		}
		
		jsonData = GUI.TextArea (new Rect (100, 850, 800, 400), jsonData);	
		
	}
}
