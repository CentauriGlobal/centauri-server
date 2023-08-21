#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <sys/time.h>

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string base64_encode(const std::string& input) {
    std::vector<unsigned char> data(input.begin(), input.end());
    std::string output;

    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    for (std::vector<unsigned char>::const_iterator it = data.begin(); it != data.end(); ++it) {
        char_array_3[i++] = *it;
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; i < 4; i++) {
                output += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    if (i > 0) {
        for (j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; j < i + 1; j++) {
            output += base64_chars[char_array_4[j]];
        }

        while (i++ < 3) {
            output += '=';
        }
    }

    return output;
}

std::string base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') &&
           (std::isalnum(encoded_string[in_]) || (encoded_string[in_] == '+') || (encoded_string[in_] == '/'))) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++) char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++) ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++) char_array_4[j] = 0;

        for (j = 0; j < 4; j++) char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

bool rsa_sha256_verify(const std::string& plaintext, const std::string& signature, const std::string& key_str,
                       std::string& err_msg) {
    std::string sign = base64_decode(signature);
    BIO* bio = BIO_new_mem_buf(key_str.c_str(), -1);
    if (bio == NULL) {
        err_msg = "Failed to create BIO";
        return false;
    }
    RSA* rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);
    if (rsa == NULL) {
        err_msg = "Failed to load public key";
        BIO_free(bio);
        return false;
    }
    BIO_free(bio);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)(plaintext.c_str()), plaintext.length(), hash);

    int result = RSA_verify(NID_sha256, hash, SHA256_DIGEST_LENGTH, (const unsigned char*)(sign.c_str()), sign.length(), rsa);
    RSA_free(rsa);
    if (result != 1) {
        err_msg = "Failed to verify signature";
        return false;
    }
    return true;
}

bool rsa_sha256_sign(const std::string& plaintext, const std::string& key_str, std::string& sign, std::string& err_msg) {
    BIO* bio = BIO_new_mem_buf(key_str.c_str(), -1);
    if (bio == NULL) {
        err_msg = "Failed to create BIO";
        return false;
    }
    RSA* rsa_key = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);
    if (rsa_key == NULL) {
        err_msg = "Failed to load private key";
        BIO_free(bio);
        return false;
    }
    BIO_free(bio);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)plaintext.c_str(), plaintext.length(), hash);

    unsigned char signature[256];
    unsigned int signature_length;
    if (RSA_sign(NID_sha256, hash, SHA256_DIGEST_LENGTH, signature, &signature_length, rsa_key) != 1) {
        err_msg = "Signing failed";
        return false;
    }
    std::string message = std::string((const char*)signature, signature_length);
    sign = base64_encode(message);
    return true;
}

bool sign(const std::string& method, const std::string& url, const std::string& body, const std::string& auth_id,
          const std::string& serial_no, const std::string& key_str, std::string& authorization, std::string& err_msg) {
    std::stringstream ss;
    ss << time(NULL);
    std::string timestamp = ss.str();
    struct timeval tvNow;
    gettimeofday(&tvNow, NULL);
    srand(tvNow.tv_sec * 1000000 + tvNow.tv_usec);
    const char CCH[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string nonce;
    for (int i = 0; i < 32; i++) {
        int x = rand() % (sizeof(CCH) - 1);
        nonce += CCH[x];
    }
    std::string message = method + "\n" + url + "\n" + timestamp + "\n" + nonce + "\n" + body + "\n";
    std::string signature;
    if (rsa_sha256_sign(message, key_str, signature, err_msg) != true) {
        return false;
    }
    authorization = "SHA256-RSA2048 auth_id=\"" + auth_id + "\",auth_id_type=\"app_id\",nonce_str=\"" + nonce + "\",timestamp=\"" + timestamp +
                    "\",serial_no=\"" + serial_no + "\",signature=\"" + signature + "\"";
    return true;
}

bool verify(const std::string& body, std::map<std::string, std::string>& header, const std::string& key_str,
            std::string& err_msg) {
    const std::string timestamp = header["Timestamp"];
    const std::string nonce = header["Nonce"];
    const std::string signature = header["Signature"];
    const std::string message = timestamp + "\n" + nonce + "\n" + body + "\n";
    if (rsa_sha256_verify(message, signature, key_str, err_msg) != true) {
        return false;
    }
    return true;
}