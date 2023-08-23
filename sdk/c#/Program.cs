using System.Text;
using Midas;

/********************************************************
      The API request signing process starts here.
*********************************************************/

// Construct the POST body here; please note that you should be careful with "\r\n" due to OS differences. 
// It is highly recommended to trim all "\r" or "\n" characters before signing and requesting.
var postBody = "";
var priKey = "\"-----BEGIN RSA PRIVATE KEY-----\\n....\\n-----END RSA PRIVATE KEY-----\"";
var serialNo = ""; // Serials corresponding to the private key.
var appId = "";    // app_id provided by Midas.

var signParam = new APISignParam
{
    Method = "POST",
    Path = "/billing/v3/payments/order", // for example, order.
    Body = postBody,
    AppID = appId,
    SerialNo = serialNo,
    // Your private key of this APP provided by Midas.
    PrivateKey = Encoding.ASCII.GetBytes(priKey),
};

// Place the generated authorization string into the API call request's HTTP header, using the key "Authorization."
// Then send the request.
var authorization = MidasSignVerifySDK.Sign(signParam);
Console.WriteLine($"Authorization: {authorization}");



/********************************************************
  The webhook request verification process starts here.
*********************************************************/

/* The Authorization header sample
 * 
 * SHA256-RSA2048 auth_id="1460000404",auth_id_type="app_id",nonce_str="be961d8b-87bc-430f-9a18-3e058772c3b5",timestamp="1692690034",signature="XM7TcmBGpNvSkgOQDs6fwWevu8O/xSZIyyQOXVbDqtI6GyHfr9hqo3KE2ibe+wk0uVjOz/gKtJjidEJbaCpaMPBccUpplMMdAq42BL+MOpAEidopuMSLjtX84ouLD9cImhKsav15SWqM/p+vhT7ZbtQ5uKa6JyqlSOV3Jga0+j85JoMMiMlhgg52Hm7HSvHj1TGKqgZ+T3dX4SYLPoCE/pYOtZ/W+/eUrsLhmdsAT85vySHWdA/t+/NlEFXcUTBlOOOq6b4OL8lzLj//5ZEedshfU8uTryytYdCJKMLEbWRi5TlB3M+VXTTDjHT4ufUkAmCwEizsnV0xj8JvVK1ZbQ==",serial_no="39393631313439343138"
 * 
 */

var signature = "";  // Get signature string from webhook Authorization header
var nonce_str = "";  // Get nonce string from webhook Authorization header
var timestamp = "";  // Get timestamp string from webhook Authorization header
var pubKey = "-----BEGIN PUBLIC KEY-----\n........\n-----END PUBLIC KEY-----";
var body = "";       // Get signature from webhook request

var verifyParam = new WebhookVerifyParam
{
    Signature = signature,
    Nonce = nonce_str,
    Timestamp = timestamp,
    PublicKey = Encoding.ASCII.GetBytes(pubKey),
    Body = body,
};

var isValid = MidasSignVerifySDK.Verify(verifyParam);
Console.WriteLine($"Signature is valid: {isValid}");