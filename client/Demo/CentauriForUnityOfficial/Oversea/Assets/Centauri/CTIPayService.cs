using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Linq;

namespace CentauriPay
{
	/// <summary>
	/// The CTIPayService interface 
	/// </summary>
	public class CTIPayService : ICTIPayService
	{
		private static string unityVersion = "1.0.3";
//		private static string UNITY_SDK_VERSION = "V1.0.1";

		private static CTIPayService instance;



		private static CallBackUtils mCallBackUtils;
		private static bool mHasInited = false;

		/// <summary>
		/// Game transparent field
		/// </summary>
		private static string appExtend;

//		private CTIBaseRequest mCTIBasePayRequest;

		//The callback of init.
		private CTIInitCallback mCTIInitCallback;
		//The callback of pay.
		private CTIPayCallback mCTIPayCallback;
		//The callback of get product information.
		private CTIGetLocalPriceCallback mCTIGetProductCallback;
		//The callback of get promotion product information.
		private CTIGetIntroPriceCallback mCTIGetIntroPriceCallback;
		//The callback of get market activties information.
		private CTIGetInfoCallback mCTIGetInfoCallback;
		//The callback of reprovide.
        private CTIReprovideCallback mCTIReprovidetCallback;
		private CTIQueryInventoryCallback mCTIQueryInventoryCallback;
		private CTIQueryPromotionCallback mCTIQueryPromotionCallback;
        

#if UNITY_EDITOR

#elif UNITY_ANDROID
        private string helperClass = "com.centauri.api.UnityPayHelper";
        private AndroidJavaClass helper = null;


#elif UNITY_IOS
		[DllImport ("__Internal")]
		private static extern void centauriSdkSetProcess(string processName);

        [DllImport ("__Internal")]
		private static extern bool centauriSdkInitWithIdcInfo(string idc, string env, string idcInfo, string jsonParams);

		[DllImport ("__Internal")]
		private static extern void centauriSdkPay(string bizType, string jsonParams);

		[DllImport ("__Internal")]
		private static extern void centauriSdkLogEnable(bool enable);

        [DllImport ("__Internal")]
		private static extern string centauriSdkGetVersion();

		[DllImport ("__Internal")]
		private static extern bool centauriSdkPayEnable();

        [DllImport ("__Internal")]
		private static extern void centauriSdkGetProductInfo(string channel,string products);

        [DllImport ("__Internal")]
		private static extern void centauriSdkGetGrnProductInfo(string bizType, string channel,string jsonParams);
		
		[DllImport ("__Internal")]
		private static extern void centauriSdkGetInfo(string type, string bizType, string jsonParams);

		[DllImport ("__Internal")]
		private static extern void centauriSdkGetIntroPrice(string channel,string products);

		[DllImport ("__Internal")]
		private static extern void centauriSdkReprovide();
#endif

		private CTIPayService ()
		{
		}

		public static CTIPayService Instance {
			get {
				if (instance == null) {
					mCallBackUtils = CallBackUtils.Instance;
					instance = new CTIPayService(); 
				}

				return instance;
			}
		}

		/// <summary>
		/// Only for IOS platform
		/// Fixed values processName="sea"
		/// </summary>
		/// <param name="processName"></param>
		public void SetProcess (string processName)
		{
#if UNITY_EDITOR
			ULog.Log ("Emulator SetProcess");
#elif UNITY_ANDROID
			ULog.Log ("Android SetProcess does not support");
#elif UNITY_IOS
			centauriSdkSetProcess(processName);
#endif
		}
		
	/// <summary>
	/// Init interface
	/// </summary>
	/// <param name="idc"></param> for sandbox test. local：香港      singapore：新加坡      guigu：美国
	/// <param name="env">Env.</param>env:test: in sandbox environment; release: in payment release environment;Please use release environment only after fully tested in test environment.
	/// <param name="idcInfo"></param>When idcInfo is passed in, the payment service will use idcInfo's domain name and ip to request connection.
	/// <param name="req"></param> init request.
	/// <param name="callback"></param> The callback of init. 
		public void Initialize (string idc, string env, string idcInfo, CTIBaseRequest req, CTIInitCallback callback)
		{
			if (mHasInited) 
			{
				return;
			}


            mCTIInitCallback = callback;

            try 
			{
#if UNITY_EDITOR
				ULog.Log ("Emulator Initialize");
				ULog.Log ("CTIUnityV"+unityVersion);

#elif UNITY_ANDROID
				//TODO:设置android版本号
				ULog.Log ("CTIUnityV"+unityVersion);
#elif UNITY_IOS
//				req.iapInitExtra.Add("app_reserve_3","CTIUnityV"+unityVersion);
#endif

				string reqString = req.ToString ();
				ULog.Log ("Initialize req json ＝ " + reqString);

#if UNITY_EDITOR
				ULog.Log ("Emulator Initialize");

#elif UNITY_ANDROID
				mHasInited = true;
				helper = new AndroidJavaClass (helperClass);
				if (helper == null)
				{
					ULog.Log("Cannot get Java helper class");
				}
				else
				{
					helper.CallStatic("Initialize",idc, env,idcInfo, reqString);
				}

#elif UNITY_IOS
				if (idc.Length == 0)
				{
					idc = "local";
				}
				mHasInited = centauriSdkInitWithIdcInfo(idc,env,idcInfo,reqString);

#endif
            }
            catch (System.Exception e) {
				ULog.LogError ("catch exception : " + e.Message);
			}
		}


		/// <summary>
		/// Pay for Game currency or props
		/// </summary>
		/// 
		/// <param name="req">Req.</param> （CTIGameRequest,CTIGoodsRequest,CTISubscribeRequest） 
		/// <param name="callback">Callback.</param> The CallBack of Pay
		public void Pay (CTIBaseRequest req, CTIPayCallback callback)
		{
            if (!mHasInited) 
			{
                Debug.LogError ("you should call Initialize first");
				return;
			}
            try {
				appExtend = req.appExtends;	//Game transparent field
				mCTIPayCallback = callback;
//				mCTIBasePayRequest = req;
				string bizType = req.GetType().Name;//Get The type of Request
				ULog.Log ("PayType = " + bizType);
				string reqString = req.ToString ();
				ULog.Log ("Pay with json : " + reqString);
#if UNITY_EDITOR
				ULog.Log ("Simulator Pay");

#elif UNITY_ANDROID
			helper = new AndroidJavaClass (helperClass);
			if (helper == null) 
			{
			ULog.Log("Cannot get Java helper class");
				}
			else
			{
				helper.CallStatic("Pay", bizType, reqString);
			}

#elif UNITY_IOS
			centauriSdkPay(bizType,reqString);
#endif
            }
            catch (System.Exception e) {
				ULog.LogError ("catch exception : " + e.Message);
			}
		}

		/// <summary>
		/// Get the Market activities information.
		/// </summary>
		/// <param name="reqType"></param> Get The type of Request
		/// <param name="req"></param>req (CTIGameRequest,CTIGoodsRequest,CTISubscribeRequest)
		/// <param name="callback"></param> CTIGetInfo CallBack
		public void GetInfo (string reqType, CTIBaseRequest req, CTIGetInfoCallback callback)
		{
			if (!mHasInited) {
				Debug.LogError ("you should call Initialize first");
				return;
			}
			try {
				mCTIGetInfoCallback = callback;
//				mCTIBasePayRequest = req;
				string bizType = req.GetType ().Name;
				ULog.Log ("PayType = " + bizType);
				string reqString = req.ToString ();
				ULog.Log ("GetInfo with json : " + reqString);

#if UNITY_EDITOR
				ULog.Log ("Simulator GetInfo");

#elif UNITY_ANDROID
				helper = new AndroidJavaClass (helperClass);
				if (helper == null) 
				{
					ULog.Log("Cannot get Java helper class");
				}
				else
				{
					helper.CallStatic("GetInfo", reqType,bizType, reqString);
				}

#elif UNITY_IOS
				// IOS bizTy=CTIGameRequest
				centauriSdkGetInfo(reqType, bizType, reqString);
#endif
            }
            catch (System.Exception e) {
				ULog.LogError ("catch exception : " + e.Message);
			}
		}

		/// <summary>
		/// Reprovide the products
		/// </summary>
		/// <param name="callback"></param>Reprovide CallBack
		public void Reprovide (CTIReprovideCallback callback)
		{
			if (!mHasInited) {
				Debug.LogError ("you should call Initialize first");
				return;
			}
			try {
				mCTIReprovidetCallback = callback;
				
#if UNITY_EDITOR
				ULog.Log ("Simulator Reprovide");

#elif UNITY_ANDROID
			helper = new AndroidJavaClass (helperClass);
			if (helper == null) 
			{
			ULog.Log("Cannot get Java helper class");
				}
			else
			{
				helper.CallStatic("Reprovide", "");
			}
	
#elif UNITY_IOS
				centauriSdkReprovide();
#endif
			}
			catch (System.Exception e) {
				ULog.LogError ("catch exception : " + e.Message);
			}
		}

		/// <summary>
		/// Launch H5 web 
		/// </summary>
		/// <param name="req"></param>
		/// <param name="callback"></param>
		public void LaunchWeb (CTIBaseRequest req, CTIPayCallback callback)
		{
			if (!mHasInited) {
				Debug.LogError ("you should call Initialize first");
				return;
			}

			try {
				mCTIPayCallback = callback;
//				mCTIBasePayRequest = req;
				string reqString = req.ToString ();
				ULog.Log ("LaunchWeb with json : " + reqString);

#if UNITY_EDITOR
				ULog.Log ("Simulator LaunchWeb");

#elif UNITY_ANDROID
				helper = new AndroidJavaClass (helperClass);
				if (helper == null) 
				{
					ULog.Log ("Cannot get Java helper class");
				}
				else
				{
					helper.CallStatic ("LaunchWeb", reqString);
				}

#elif UNITY_IOS
				ULog.Log("IOS LaunchWeb does not support");

#endif
			} catch (System.Exception e) {
				ULog.LogError ("catch exception : " + e.Message);
			}
		}


		/// <summary>
		/// The Switch for pay.
		/// </summary>
		/// <returns></returns>
		public bool IsPayEnable ()
		{  
			bool enable = true;

			try {

#if UNITY_EDITOR
				ULog.Log ("simulator IsIAPEnable");

#elif UNITY_ANDROID
				ULog.Log("android IsIAPEnable");
				// Android不作处理，返回默认值True
				enable = true;

#elif UNITY_IOS
				enable = centauriSdkPayEnable();

#endif
			} catch (System.Exception e) {
				ULog.LogError ("catch exception : " + e.Message);
			}

			return enable;
		}
		/// <summary>
		/// Set the log Enable or not.
		/// </summary>
		/// <param name="enable"></param>
		public void SetLogEnable (bool enable)
		{  
			try {
				ULog.setLevel (ULog.Level.Log);
				ULog.Log ("SetLogEnable enable:" + enable);

#if UNITY_EDITOR
				ULog.Log ("simulator LogEnable");

#elif UNITY_ANDROID
				helper = new AndroidJavaClass (helperClass);
				if (helper == null) 
				{
					ULog.Log("Cannot get Java helper class");
				}
				else
				{
		
					helper.CallStatic("SetLogEnable", enable);
				}

#elif UNITY_IOS
				centauriSdkLogEnable(enable);

#endif
			} catch (System.Exception e) {
				ULog.LogError ("catch exception : " + e.Message);
			}
		}

		/// <summary>
		/// Gets the centauri SDK version.
		/// </summary>
		/// 
		/// <returns>The centauri SDK version.</returns>
		public string GetCTISDKVersion ()
		{  
			string version = "";
			
			try {

#if UNITY_EDITOR
				ULog.Log ("simulator GetCTISDKVersion");

#elif UNITY_ANDROID
				helper = new AndroidJavaClass (helperClass);
				if (helper == null) 
				{
					ULog.Log("Cannot get Java helper class");
				}
				else
				{
					version = helper.CallStatic<string>("GetCentauriSDKVersion");
				}

#elif UNITY_IOS
				version = centauriSdkGetVersion();

#endif
			} catch (System.Exception e) {
				ULog.LogError ("catch exception : " + e.Message);
			}

			return version;
		}


		/// <summary>
		/// Only for android.
		/// Set the Path.
		/// </summary>
		/// <param name="path"></param>
		public void SetPath (string path)
		{
			try {
				#if UNITY_EDITOR
				ULog.Log ("Emulator SetPath");

				#elif UNITY_ANDROID
		helper = new AndroidJavaClass (helperClass);
		if (helper == null)
		{
		ULog.Log("Cannot get Java helper class");
		}
		else
		{
		helper.CallStatic("SetPath", path);
		}
				#elif UNITY_IOS
				#endif
			} catch (System.Exception e) {
				ULog.LogError ("catch exception : " + e.Message);
			}
		}

		/// <summary>
		/// Only for android.
		/// </summary>
		/// <param name="s"></param>
		public void CouponsRollBack (string s)
		{
			try {
				ULog.Log ("CouponsRollBack s = " + s);

#if UNITY_EDITOR
				ULog.Log ("Simulator CouponsRollBack");

#elif UNITY_ANDROID
				helper = new AndroidJavaClass (helperClass);
				if (helper == null) 
				{
					ULog.Log("Cannot get Java helper class");
				}
				else
				{
					helper.CallStatic("CouponsRollBack", s);
				}

#elif UNITY_IOS
#endif
			} catch (System.Exception e) {
				ULog.LogError ("catch exception : " + e.Message);
			}
		}

		/// <summary>
		/// Get the product informations. 
		/// </summary>
		/// <param name="channel"></param> pay channel. 
		/// <param name="productList"></param> List of product information to be obtained：If it is empty, get all products information.
		/// <param name="callback"></param> Get product information callback.
        public void GetLocalPrice(string channel, Dictionary<string,string> productList, CTIGetLocalPriceCallback callback)
        {
            if (!mHasInited)
            {
                Debug.LogError("you should call Initialize first");
                return;
            }

            mCTIGetProductCallback = callback;

            if (productList == null || productList.Count == 0)
            {
                ULog.LogError("productList is empty");
                return;
            }

#if UNITY_EDITOR
            ULog.Log("use simulator");
#elif UNITY_ANDROID
				helper = new AndroidJavaClass (helperClass);
				if (helper == null) {
					 ULog.LogError("Unity helperClass is empty");
				}else{
                    string products = Json.Serialize(productList);
                    ULog.Log("productList:" + products);
					AndroidJavaObject mapProducts = new AndroidJavaObject("java.util.HashMap");  
					foreach(KeyValuePair<string, string> product in productList)  {  
						mapProducts.Call<string>("put", product.Key, product.Value);  
					} 
					helper.CallStatic("GetProductInfo",channel,mapProducts);
				}
#elif UNITY_IOS
                List<string> prdList = productList.Keys.ToList<string>();
                string products = Json.Serialize(prdList);
                centauriSdkGetProductInfo(channel,products);
#endif
		}
		/// <summary>
		///Get the  Promotion Product Information.
		/// </summary>
		/// <param name="channel"></param> Pay channel.
		/// <param name="productList"></param>List of product information to be obtained：If it is empty, get all products information.
		/// <param name="callback"></param>Get Promotion product information callback.
		public void GetIntroPrice(string channel, Dictionary<string,string> productList, CTIGetIntroPriceCallback callback)
		{
			if (!mHasInited)
			{
				Debug.LogError("you should call Initialize first");
				return;
			}

			mCTIGetIntroPriceCallback = callback;

			if (productList == null || productList.Count == 0)
			{
				ULog.LogError("productList is empty");
				return;
			}

#if UNITY_EDITOR
			ULog.Log("use simulator");
#elif UNITY_ANDROID
				helper = new AndroidJavaClass (helperClass);
				if (helper == null) {
					 ULog.LogError("Unity helperClass is empty");
				}else{
                    string products = Json.Serialize(productList);
                    ULog.Log("productList:" + products);
					AndroidJavaObject mapProducts = new AndroidJavaObject("java.util.HashMap");  
					foreach(KeyValuePair<string, string> product in productList)  {  
						mapProducts.Call<string>("put", product.Key, product.Value);  
					} 
					helper.CallStatic("GetProductInfo",channel,mapProducts);
				}
#elif UNITY_IOS
            List<string> prdList = productList.Keys.ToList<string>();
            string products = Json.Serialize(prdList);
            ULog.Log("productList:" + products);
			centauriSdkGetIntroPrice(channel,products);
#endif
		}

		/// <summary>
		/// Only for garena channel.
		/// </summary>
		/// <param name="request"></param>
		/// <param name="callback"></param>
		public void GetGarenaProductInfo(CTIBaseRequest request, CTIGetLocalPriceCallback callback)
		{
			if (!mHasInited)
			{
				ULog.LogError("you should call Initialize first");
				return;
			}

			mCTIGetProductCallback = callback;
			string reqString = request.ToString();
			ULog.Log("GetGarenaProductInfo with json : " + reqString);
#if UNITY_EDITOR
			ULog.Log("use simulator");
#elif UNITY_ANDROID
			helper = new AndroidJavaClass (helperClass);
			if (helper == null) {
			}
			else{
				helper.CallStatic("GetProductInfo",reqString);
			}
#elif UNITY_IOS

            string bizType = request.GetType().Name;
			ULog.Log ("PayType = " + bizType);

			centauriSdkGetGrnProductInfo(bizType, "os_garena", reqString);
#endif
		}
		
		/// <summary>
		/// Only for Android 
		/// </summary>
		/// <param name="serverId"></param> ServerID 
		/// <param name="roleId"></param> roleId
		/// <param name="callback"></param> 
		public void ScanGoogleInventory(int serverId, int roleId, CTIQueryPromotionCallback callback)
		{
			mCTIQueryPromotionCallback = callback;

#if UNITY_EDITOR
			ULog.Log("use simulator");
#elif UNITY_ANDROID
			helper = new AndroidJavaClass (helperClass);
			if (helper == null) {
			}
			else{
				helper.CallStatic("ScanGoogleInventory",serverId, roleId);
			}
#elif UNITY_IOS
#endif
		}

		/// <summary>
		/// After receiving the message from Java/OC, this method will be called to call back the game of C# layer
		/// </summary>
		/// <param name="result"></param>
		public  void CentauriPayCallback (string result)
		{
			if (mCTIPayCallback != null) {
				CTIResponse resp = new CTIResponse (result);
				resp.appExtends = appExtend;
				mCTIPayCallback.OnCentauriPayFinished (resp);
				// 用完即废，防止影响后续逻辑
				mCTIPayCallback = null;
			}
		}
		/// <summary>
		/// LoginExpiredCallBack
		/// </summary>
		public  void CentauriLoginExpiredCallback ()
		{
			if (mCTIPayCallback != null) {
				mCTIPayCallback.OnCentauriLoginExpired ();
				// 用完即废，防止影响后续逻辑
				mCTIPayCallback = null;
			}
		}
		/// <summary>
		/// Init CallBack
		/// </summary>
		/// <param name="result"></param>
        public void CentauriInitCallback(string result)
        {
            if (mCTIInitCallback != null)
            {
                Dictionary<string, object> jsonResult = Json.Deserialize(result) as Dictionary<string, object>;
                mCTIInitCallback.OnCentauriInitFinished(jsonResult);
            }
        }
		
        /// <summary>
        /// GetInfo Callback
        /// </summary>
        /// 
        /// <param name="type">Type.</param>
        /// <param name="result">Result.</param>
        public  void CentauriGetInfoFinishCallback (string type, int retCode, string json)
		{
			if (mCTIGetInfoCallback != null) {
				mCTIGetInfoCallback.GetInfoFinished (type, retCode, json);
				// 用完即废，防止影响后续逻辑
				mCTIGetInfoCallback = null;
			}
		}


        /// <summary>
        /// Get product information callback.
        /// </summary>
        /// 
        /// <param name="result">Result.</param>
        public void CentauriGetLocalPriceCallback(string resul)
        {
            if (mCTIGetProductCallback != null)
            {
                Dictionary<string, object> jsonResult = Json.Deserialize(resul) as Dictionary<string, object>;
                mCTIGetProductCallback.OnCentauriGetProdcut(jsonResult);
				mCTIGetProductCallback = null;
            }
        }

		/// <summary>
		/// The promotion callback.
		/// </summary>
		/// 
		/// <param name="result">Result.</param>
		public void CentauriGetIntroPriceCallback(string resul)
		{
			if (mCTIGetIntroPriceCallback != null)
			{
				Dictionary<string, object> jsonResult = Json.Deserialize(resul) as Dictionary<string, object>;
				mCTIGetIntroPriceCallback.OnCentauriGetIntroPrice(jsonResult);
				mCTIGetIntroPriceCallback = null;
			}
		}


        /// <summary>
        /// Reprovide callback.
        /// </summary>
        /// 
        /// <param name="result">Result.</param>
        public void CentauriReProvidetCallback(string result)
        {
            if (mCTIReprovidetCallback != null)
            {
                Dictionary<string, object> jsonResult = Json.Deserialize(result) as Dictionary<string, object>;
                mCTIReprovidetCallback.OnCentauriReprovideFinished(jsonResult);
            }
        }
		
		public void CentauriQueryPurchaseInventoryCallback(string result)
		{
			if (mCTIQueryInventoryCallback != null)
			{
				mCTIQueryInventoryCallback.OnQueryInventoryCallback(result);
			}
		}
	
		public void CentauriQueryPromotionCallback(string result)
		{
			if (mCTIQueryPromotionCallback != null)
			{
				mCTIQueryPromotionCallback.OnQueryPromotionCallback(result);
			}
		}


    }

    // Used to receive messages from the Java/Object C layer SendMessage
    public class CallBackUtils : Singleton<CallBackUtils>
	{
		public void CentauriPayCallback (string result)
		{
			ULog.Log ("CallBackUtils.CTIPayCallback message from Java/OC = " + result);
			// After receiving the message sent by the Java/OC layer, call the corresponding method of CTIPayService
			CTIPayService.Instance.CentauriPayCallback (result);
		}

		public void CentauriLoginExpiredCallback ()
		{
			ULog.Log ("Got CentauriLoginExpiredCallback message from Java/OC");
			// After receiving the message sent by the Java/OC layer, call the corresponding method of CTIPayService
			CTIPayService.Instance.CentauriLoginExpiredCallback ();
		}

		public void CentauriGetLocalPriceCallback (string json)
		{
			CTIPayService.Instance.CentauriGetLocalPriceCallback(json);
        }

		public void CentauriGetIntroPriceCallback (string json)
		{
			CTIPayService.Instance.CentauriGetIntroPriceCallback(json);
		}

        public void CentauriInitCallback(string result)
        {
	        CTIPayService.Instance.CentauriInitCallback(result);
        }
        public void CentauriGetInfoCallback(string json)
        {
            ULog.Log("Got CentauriGetInfoFinish message from Java/OC, json = " + json);
            // After receiving the message sent by the Java/OC layer, call the corresponding method of CTIPayService
            CTIGetInfoResult result2 = new CTIGetInfoResult(json);
            ULog.Log("CallBackUtils.CentauriGetInfoFinish result.type = " + result2.type);
            ULog.Log("CallBackUtils.CentauriGetInfoFinish result.ret = " + result2.ret);
//            ULog.Log("CallBackUtils.CentauriGetInfoFinish result.msg = " + result2.msg);

	        CTIPayService.Instance.CentauriGetInfoFinishCallback(result2.type, result2.ret, json);
        }
		public void CentauriGetShortOpenidCallback(string json)
		{
			ULog.Log("Got CentauriGetInfoFinish message from Java/OC, json = " + json);
			// After receiving the message sent by the Java/OC layer, call the corresponding method of CTIPayService
			CTIGetInfoResult result2 = new CTIGetInfoResult(json);
			result2.type = "get_short_openid";
			//ULog.Log("CallBackUtils.CentauriGetInfoFinish result.type = " + result2.type);
			//ULog.Log("CallBackUtils.CentauriGetInfoFinish result.ret = " + result2.ret);
			//ULog.Log("CallBackUtils.CentauriGetInfoFinish result.msg = " + result2.msg);

			CTIPayService.Instance.CentauriGetInfoFinishCallback(result2.type, result2.ret, json);
		}
        public void CentauriReProvidetCallback(string result)
        {
	        CTIPayService.Instance.CentauriReProvidetCallback(result);
        }
		public void CentauriQueryPromotionCallback(string result)
		{	
			CTIPayService.Instance.CentauriQueryPromotionCallback(result);
		}
		
    }
	
	
	
}
