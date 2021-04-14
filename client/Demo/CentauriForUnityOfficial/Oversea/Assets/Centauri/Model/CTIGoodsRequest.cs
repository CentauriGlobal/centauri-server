using UnityEngine;
using System.Collections;
using CentauriPay;

namespace CentauriPay
{
	///<summary>
	///Request payment Goods object
	///</summary>
	///<remarks>
	///
	///</remarks>
	[System.Serializable]
    public class CTIGoodsRequest : CTIBaseRequest
    {	
		public enum TOKEN_TYPE
		{
			GETTOKENTYPE_SERVER = 1,	//make order from game server。note：this is not support for iOS
			GETTOKENTYPE_SDK = 2,		//make order in centauri sdk
			GETTOKENTYPE_CLIENT = 3		//make order in game client
		};

		///<summary>
		///order token type，set value with TOKEN_TYPE from the above。
		/// [required]
		///</summary>
		[JsonProp("tokenType")]
		public int tokenType = -1;

		///<summary>
		///order token from the server,and set tokenType = GETTOKENTYPE_SERVER。
		/// [required]
		///</summary>
		[JsonProp("goodsTokenUrl")]
		public string goodsTokenUrl;

		///<summary>
		///game client provide diamonds
		/// [optional]
		///</summary>
		[JsonProp("developerPayload")]
		public string developerPayload;	

		///<summary>
		///game client provide diamonds
		/// [optional]
		///</summary>
		[JsonProp("isReceiptMode")]
		public bool isReceiptMode = false;
	}
}


