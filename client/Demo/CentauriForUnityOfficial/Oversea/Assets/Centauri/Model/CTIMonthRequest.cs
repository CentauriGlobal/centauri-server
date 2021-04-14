using System;

namespace CentauriPay
{
	///<summary>
	///Request payment Month object
	///</summary>
	///<remarks>
	///
	///</remarks>
	public class CTIMonthRequest : CTIBaseRequest
	{
		///<summary>
		///service code for month bussiness
		/// [required]
		///</summary>
		[JsonProp("serviceCode")]
		public string serviceCode = "";

		///<summary>
		///service name for month bussiness
		/// [required]
		///</summary>
		[JsonProp("serviceName")]
		public string serviceName = "";

		///<summary>
		///service type for month bussiness, see SERVICE_TYPE
		/// [optional]
		///</summary>
		[JsonProp("serviceType")]
		public int serviceType = 1;

		///<summary>
		/// is enable auto continue pay. 
		/// note:For iOS, it's not allowed to buy a month service, if you wan to use this for subscribe, please contact with centauri
		/// [optional]
		///</summary>
		[JsonProp("autoPay")]
		public bool autoPay = false;	

		public enum SERVICE_TYPE
		{
			SERVICETYPE_NORMAL = 1,	//开通包月
			SERVICETYPE_RENEW = 2,	//续费
			SERVICETYPE_UPGRADE = 3	//升级
		};
	}
}

