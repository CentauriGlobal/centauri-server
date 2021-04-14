using System;

namespace CentauriPay
{
	///<summary>
	///CallBack for GetInfo
	///</summary>
	///<remarks>
	///CTIGetInfoResult
	///</remarks>
	public class CTIGetInfoResult : JsonSerializable
	{
		//info type, "mp" or "get_short_openid"
		[JsonProp("type")]
		public string type = "mp";

		//info 
		//[JsonProp("msg")]
		//public string msg;

		//ret code
		[JsonProp("ret")]
		public int ret;

		public CTIGetInfoResult (string param) : base (param) { }

		public CTIGetInfoResult (object json) : base (json) { }
	}
}

