using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace CentauriPay {
	
	///<summary>
	///Request payment base object
	///</summary>
	///<remarks>
	///
	///</remarks>
	[System.Serializable]
	public class CTIBaseRequest : JsonSerializable {

        public const string CTI_WCT = "w"+"ec"+"hat";
        public const string BANK = "bank";
        public const string CTI_QW = "q"+"qwa"+"llet";
        public const string GOOGLEPLAY = "googleplay";  //
        public const string APPLEIAP = "APPLEIAP";  //


        ///<summary>
        ///appId offered by centauri.
        ///[required]
        ///</summary>
        [JsonProp("offerId")]
		public string offerId;


		///<summary>
		///User unique account ID
		/// [required]
		///</summary>
		[JsonProp("openId")]
		public string openId;

		///<summary>
		///Payment token for player
		/// [required]
		///</summary>
		[JsonProp("openKey")]
		public string openKey="openKey";

		///<summary>
		///account zone ID from centauri server configrated. If game don’t have multiple zones, this value is by default zoneId ="1".
		/// [required]
		///</summary>
		[JsonProp("zoneId")]
		public string zoneId;


		///<summary>
		/// login Type，"sessionId" and "sessionType" used in pairs.
		/// Guest：sessionId="hy_gameid"，sessionType=“st_dummy”
		/// Fixed values sesionType="hy_gameid"
		/// [required]
		///</summary>
		[JsonProp("sessionId")]
		public string sessionId="hy_gameid";

		///<summary>
		/// login Type，"sessionId" and "sessionType" used in pairs.
		/// Guest：sessionId="hy_gameid"，sessionType=“st_dummy”;
		/// Fixed values sesionType="st_dummy"
		/// [required]
		///</summary>
		[JsonProp("sessionType")]
		public string sessionType="st_dummy";

		///<summary>
		///Game can retrieve this value with WGPlatform.WGGetPf().
		/// [required]
		///</summary>
		[JsonProp("pf")]
		public string pf;

		///<summary>
		///Game can retrieve this value with MSDKAPI:WGPlatform.WGGetPfKey().
		/// Fixed values pfKey="pfKey"
		/// [required]
		///</summary>
		[JsonProp("pfKey")]
		public string pfKey="pfKey";



        ///<summary>
        /// Values that must be passed on the IOS platform
        /// default values goodsZoneId="1"
        /// must pass in IDIP's partition as the value for the key "app_extra" when called Initialize().
        /// [required]
        ///</summary>
        [JsonProp("goodsZoneId")]
        public string goodsZoneId;

        ///<summary>
        ///The productID of android and ios
        /// [required]
        ///</summary>
        [JsonProp("productId")]
        public string productId;

        ///<summary>
        ///only for iOS：
        /// Goods，“productId*price(unit：jiao)*quantity”；Subscribe，“days”；Game，“quantity”
        /// [required]
        ///</summary>
        [JsonProp("payItem")]
        public string payItem;

        ///<summary>
        ///only for iOS：now only the key "app_extra"
        /// must pass in IDIP's partition as the value for the key "app_extra" when called Initialize().
        /// [required]
        ///</summary>
        //[JsonProp("iapInitExtra")]
        //public Dictionary<string, object> iapInitExtra = new Dictionary<string, object>();


        ///<summary>
        /// buy quantity. if the value is null or "", it will show centauri mall page.
        /// [optional]
        ///</summary>
        [JsonProp("saveValue")]
        public string saveValue;

        ///<summary>
        /// extend object
        /// [optional]
        ///</summary
        //[JsonProp("extendInfo")]
        //public CTIExtendInfo extendInfo = new CTIExtendInfo ();

        ///<summary>
        /// discount object
        /// [optional]
        ///</summary>
        //[JsonProp("mpInfo")]
        //public CTIMPInfo mpInfo = new CTIMPInfo ();

  

        ///<summary>
        /// extend field to pass some unnormal params
        /// such as "remark": extend="remark=xxx"
        /// [optional]
        ///</summary>
        [JsonProp("appExtends")]
		public string appExtends; // remark=xxx&a=x&b＝x&drmInfo=xxx


        ///<summary>
        ///Third-party payment channels: general parameters transparently transmitted to third-party payment channels, generally roleId, roleName, etc., different channel parameters key/value
        ///</summary>
        [JsonProp("channelExtras")]
        public string channelExtras;


        ///<summary>
        ///Product list, optional pass in when garena pulls items.If pass the productList will pull specific products,other pull all products
        ///</summary>
        [JsonProp("productList")]
        public List<string> productList;


        ///<summary>
        ///Specified currency type. The backstage of Master Mi will filter the items according to the currency type.
        ///</summary>
        [JsonProp("currencyType")]
        public string currencyType;

        ///<summary>
        ///Designated country. The backstage of Master Mi will filter payment channels according to the country.
        ///</summary>
        [JsonProp("country")]
        public string country;


        ///<summary>
        ///pay channel 
        ///</summary>
        [JsonProp("payChannel")]
        public string payChannel;

        ///<summary>
        ///account type。
        /// [optional]
        ///</summary>
        //[JsonProp("acctType")]
        private string acctType = "common";

        ///<summary>
        ///quantity of game coin is enable to change in the sdk,only use for centauri mall.
        /// [optional]
        ///</summary>
        //[JsonProp("isCanChange")]
        //public bool isCanChange;

        ///<summary>
        ///mall logo resource id,eg:R.id.xxxx
        /// [optional]
        ///</summary>
        //[JsonProp("mallLogo")]
        //public string mallLogo;

        ///<summary>
        ///mall item icon，need base64 encoding.
        /// [optional]
        ///</summary>
        //[JsonProp("resData")]
        //public string resData = "";

        ///<summary>
        ///mall item icon resource id,only for android.eg: R.id.xxxxx
        /// [optional]
        ///</summary>
        //[JsonProp("resId")]
        //public string resId;


        ///<summary>
        ///extras for custom configuration。The format of this field is json。
        //{
        //  "uiconfig":
        //  {
        //     "unit":"ge",
        //     "isShowNum":false,
        //     "isShowListOtherNum":true,
        //     "isCanChange":true,
        //     "extras":"",
        //     "resData":"",
        //     "resId":0,
        //     "mallLogo":0
        //  },
        //  "drmConfig":
        //  {
        //     "discountType":"",
        //     "discountUrl":"",
        //     "discoutId":""
        //     "drmInfo":""
        //  }
        //  "others"
        //  {
        //      "mallType":"",
        //      "h5Url":""
        //  }
        //}
        ///
        /// 
        /// [optional]
        ///</summary>
        [JsonProp("extras")]
            public string extras;

    }
}
