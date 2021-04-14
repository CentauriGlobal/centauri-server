using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace CentauriPay {

    /// <summary>
    /// CTI Pay CallBack
    /// </summary>
    public interface CTIPayCallback
    {
        /// <summary>
        /// LoginExpired
        /// </summary>
	    void OnCentauriLoginExpired();
		/// <summary>
		/// 
		/// </summary>
		/// <param name="result"></param>
        void OnCentauriPayFinished(CTIResponse result);
    }
    
	///<summary>
	///Callback for GetInfo
	///</summary>
	public interface CTIGetInfoCallback
	{
		//param type:"mp","short_openid"
		//param retCode: 0 is success; other is failed
		//param json: the market and product info
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="type"></param>
		/// <param name="retCode"></param>
		/// <param name="json"></param>
		void GetInfoFinished (string type, int retCode, string json);
	}


    ///<summary>
    ///初始化的回调。如果涉及到补发货，会通过此回调通知app补发货的信息
    ///</summary>
    public interface CTIInitCallback
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="result"></param>
	    void OnCentauriInitFinished(Dictionary<string, object> result);
    }

    ///<summary>
    ///补发货的回调。业务主动调用补发货接口的情况下，会通过此回调返回结果。
    ///</summary>
    public interface CTIReprovideCallback
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="result"></param>
	    void OnCentauriReprovideFinished(Dictionary<string, object> result);
    }

    ///<summary>
    ///获取物品信息
    ///</summary>
    public interface CTIGetLocalPriceCallback
    {
		/// <summary>
		/// 
		/// </summary>
		/// <param name="result"></param>
		void OnCentauriGetProdcut(Dictionary<string, object> result);
    }

	///<summary>
	///获取推介物品信息
	///</summary>
	public interface CTIGetIntroPriceCallback
	{
		/// <summary>
		/// 
		/// </summary>
		/// <param name="result"></param>
		void OnCentauriGetIntroPrice(Dictionary<string, object> result);
	}
	

	/// <summary>
	/// 
	/// </summary>
	public interface CTIQueryInventoryCallback
	{
		/// <summary>
		/// 
		/// </summary>
		/// <param name="result"></param>
		void OnQueryInventoryCallback (string result);
	}
	/// <summary>
	/// 
	/// </summary>
	public interface CTIQueryPromotionCallback
	{
		/// <summary>
		/// 
		/// </summary>
		/// <param name="result"></param>
		void OnQueryPromotionCallback (string result);
	}
	
	

    /// <summary>
    /// centauri payment interface
    /// </summary>
    
    public interface ICTIPayService
	{
		//Only for iOS
		//param procesName:sea/local, default value is 'local';
		//attention, The 'SetProcess' must be called before all centauri interface method.
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="processName"></param>
		void SetProcess (string processName);

		//is enable log, sugest to set false when after fully tested.
        
		/// <summary>
		/// 
		/// </summary>
		/// <param name="enable"></param>
		void SetLogEnable (bool enable);

		//param env:test: in sandbox environment; release: in payment release environment;Please use release environment only after fully tested in test environment.
		//param req:Request payment base object
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="idc"></param>
		/// <param name="env"></param>
		/// <param name="idcInfo"></param>
		/// <param name="req"></param>
		/// <param name="callback"></param>
		void Initialize (string idc, string env, string idcInfo, CTIBaseRequest req, CTIInitCallback callback);

		//param req:A specific request payment object, such as CTIGameRequest, CTIGoodsRequest, CTIMonthRequest or CTISubscribeRequest
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="req"></param>
		/// <param name="callback"></param>
		void Pay (CTIBaseRequest req, CTIPayCallback callback);

		//is payment allowed, this just valid for ios ,android is always return true
		
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool IsPayEnable ();

		//get market info and productInfo from centauri server:
		//param reqType:"mp",get market info;"short_openid",get the short openid
		//param req:A specific request payment object, such as CTIGameRequest, CTIGoodsRequest, CTIMonthRequest or CTISubscribeRequest
		//param callback: the callback of getInfo
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="reqType"></param>
		/// <param name="req"></param>
		/// <param name="callback"></param>
		void GetInfo (string reqType,CTIBaseRequest req, CTIGetInfoCallback callback);

		//check if it have undeliveried orders and do reprovide 
		//param callback:you can get the callback only when it have undeliveried orders
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="callback"></param>
		void Reprovide (CTIReprovideCallback callback);
		
		


        ///*************************************       optional interfaces below       ********************************************************************************///


        //return sdk version
        //[optional]

        /// <summary>
        ///
        /// </summary>
        /// <returns></returns>
        string GetCTISDKVersion ();

		//only for android
		//[optional]
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="req"></param>
		/// <param name="callback"></param>
		void LaunchWeb (CTIBaseRequest req, CTIPayCallback callback);

		//only for android
		//[optional]
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="path"></param>
		void SetPath (string path);

		//only for android
		//[optional]
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="s"></param>
		void CouponsRollBack (string s);

		//only for iap and googlewallet
		//[optional]
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="channel"></param>
		/// <param name="productList"></param>
		/// <param name="callback"></param>
		void GetLocalPrice(string channel, Dictionary<string,string> productList, CTIGetLocalPriceCallback callback);

		//only for iap and googlewallet
		//[optional]
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="channel"></param>
		/// <param name="productList"></param>
		/// <param name="callback"></param>
		void GetIntroPrice(string channel, Dictionary<string,string> productList, CTIGetIntroPriceCallback callback);
		
		// only for garena
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="request"></param>
		/// <param name="callback"></param>
		void GetGarenaProductInfo(CTIBaseRequest request, CTIGetLocalPriceCallback callback);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="serverId"></param>
		/// <param name="roleId"></param>
		/// <param name="callback"></param>
		///
		void ScanGoogleInventory(int serverId, int roleId, CTIQueryPromotionCallback callback);
    }
}
