using System.Security.Cryptography;
using System.Text;

namespace Midas
{
    // The following classes are designed to be compatible with .NET 6.x.


    // This class is designed to encapsulate parameters that can be used to generate Midas API signatures.
    public class APISignParam
    {
        public string Method { get; set; }       // "POST", for most cases.
        public string Path { get; set; }         // API paths, such as "/billing/v3/payments/order", can usually be obtained from an HTTP request.
        public string Body { get; set; }         // The POST body text, generallly.
        public string AppID { get; set; }        // The app_id provided by Midas.
        
        // Serials associated with the private key (or a certificate), provided by Midas,
        // used for identifying different keys during a transition period caused by an impending certificate expiration.
        public string SerialNo { get; set; }     
        public byte[] PrivateKey { get; set; }   // Acquired from Midas, or you can maintain your own private key, and send the corresponding public key to Midas.
        public long Timestamp { get; set; }      // A Unix-style timestamp since 1970-01-01 00:00:00; if omitted, the current timestamp will be used.
        public string Nonce { get; set; }       // A random string, can be omitted.
        public string AuthIdType { get; set; }  // Type of AppID, in most cases it is "app_id", can be ommited.
    }

    // This class is designed to encapsulate parameters that can be used to verify Midas webhook signatures,
    // enable developers avoiding fake requests or attacks.
    public class WebhookVerifyParam
    {
        public string Signature { get; set; }     // The "Signature" string obtained from the Midas webhook request header.
        public string Nonce { get; set; }        // The "Nonce" string obtained from the Midas webhook request header.
        public string Timestamp { get; set; }    // The "Timestamp" string obtained from the Midas webhook request header.
        public byte[] PublicKey { get; set; }    // Acquired from Midas, that's Midas's public key corresponding to this APP (app_id).

        public string Body { get; set; }         // The body text of Midas webhook request.
    } 

    // Before calling Midas backend service API, use this class to identify developers.
    // Please note that the two public methods of this class, "Sign" and "Verify",
    // are used separately for API calling and webhook verification.
    // The algorithms of the two methods are not exactly the same due to procedural differences.
    public static class MidasSignVerifySDK
    {
        // The main signing procedure.
        // The algorithm of this method can be referred to in the Midas Developer Documentation if interested.
        public static string Sign(APISignParam param)
        {
            if (null == param.PrivateKey)
            {
                return "";
            }

            var timestamp = DateTimeOffset.UtcNow.ToUnixTimeSeconds();
            if (param.Timestamp > 0)
            {
                timestamp = param.Timestamp;
            }

            var nonce = "";
            if (!string.IsNullOrEmpty(param.Nonce))
            {
                nonce = param.Nonce;
            }
            else
            {
                nonce = GenerateUUID();
            }

            var authIdType = "app_id";
            if (!string.IsNullOrEmpty(param.AuthIdType))
            {
                authIdType = param.AuthIdType;
            }

            var privateKey = ParseRSAPrivateKey(param.PrivateKey);
            var signStr = $"{param.Method}\n{param.Path}\n{timestamp}\n{nonce}\n{param.Body}\n";
            var signature = privateKey.SignData(Encoding.UTF8.GetBytes(signStr), HashAlgorithmName.SHA256, RSASignaturePadding.Pkcs1);
            var authorization = $"SHA256-RSA2048 auth_id=\"{param.AppID}\",auth_id_type=\"{authIdType}\",nonce_str=\"{nonce}\",timestamp=\"{timestamp}\",signature=\"{Convert.ToBase64String(signature)}\",serial_no=\"{param.SerialNo}\"";
            return authorization;
        }

        // The main verification procedure.
        // The algorithm of this method can be referred to in the Midas Developer Documentation if interested.
        public static bool Verify(WebhookVerifyParam param)
        {
            var signature = Convert.FromBase64String(param.Signature);
            var publicKey = ParseRSAPublicKey(param.PublicKey);
            var signStr = $"{param.Timestamp}\n{param.Nonce}\n{param.Body}\n";
            var result = publicKey.VerifyData(Encoding.UTF8.GetBytes(signStr), signature, HashAlgorithmName.SHA256, RSASignaturePadding.Pkcs1);
            return result;
        }

        private static string GenerateUUID()
        {
            Guid uuid = Guid.NewGuid();
            string uuidString = uuid.ToString();
            return uuidString;
        }

        private static RSA ParseRSAPrivateKey(byte[] privateKeyData)
        {
            var privateKeyPem = Encoding.ASCII.GetString(privateKeyData);
            var rsa = RSA.Create();
            rsa.ImportFromPem(privateKeyPem);
            return rsa;
        }

        private static RSA ParseRSAPublicKey(byte[] publicKeyData)
        {
            var publicKeyPem = Encoding.ASCII.GetString(publicKeyData);
            var rsa = RSA.Create();
            rsa.ImportFromPem(publicKeyPem);
            return rsa;
        }
    }
}
