using UnityEngine;
using System.Collections;

/// <summary>
/// namespace for centauri
/// </summary>
namespace CentauriPay {

	/// <summary>
	/// Response object for payment callback
	/// </summary>
	[System.Serializable]
	public class CTIResponse : JsonSerializable {

        ///<summary>
        ///payment result,0 is success,other is fail。
		/// 
        ///</summary>
		[JsonProp("resultCode")]
        public int resultCode;

        ///<summary>
        ///inner error code for payment。
        ///</summary>
		[JsonProp("resultInerCode")]
        public string resultInerCode ;

        ///<summary>
		///game coin quantity。
        ///</summary>
		[JsonProp("realSaveNum")]
		public string realSaveNum;

        ///<summary>
        ///pay channel。
        ///</summary>
		[JsonProp("payChannel")]
        public string payChannel;

        ///<summary>
        ///error message for payment
        ///</summary>
		[JsonProp("resultMsg")]
        public string resultMsg;

		///<summary>
		///extend info, this will return the original value that passed in Request
		///</summary>
		[JsonProp("appExtends")]
		public string appExtends; 


		///<summary>
		///business type, game or goods or month or subscribe
		///</summary>
		[JsonProp("reqType")]
		public string reqType; 

		///<summary>
		/// Reserved field
		///</summary>
		[JsonProp("payReserve1")]
		public string payReserve1;

		///<summary>
		/// Reserved field
		///</summary>
		[JsonProp("payReserve2")]
		public string payReserve2;

		///<summary>
		/// Reserved field
		///</summary>
		[JsonProp("payReserve3")]
		public string payReserve3;

		public CTIResponse (string param) : base (param) { }

		public CTIResponse (object json) : base (json) { }
    }
}
