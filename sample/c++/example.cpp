#include <curl/curl.h>

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "crypto.h"

size_t write_call_back(char* data, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    response->append(data, totalSize);
    return totalSize;
}

int post(const std::string& url, const std::string body, const std::vector<std::string>* headers, std::string &response) {
    int ret = 0;
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
        struct curl_slist* header = NULL;
        if (headers != NULL) {
            for (unsigned int i = 0; i < headers->size(); i++) {
                header = curl_slist_append(header, (*headers)[i].c_str());
            }
        }

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_call_back);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl request failed: " << curl_easy_strerror(res) << std::endl;
            ret = -1;
        } else {
            long res_code;
            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res_code);
            if (res_code != 200)  // HTTP OK
            {
                std::cerr << "curl response http code not 200: " << curl_easy_strerror(res) << std::endl;
                ret = -1;
            }
        }
        curl_slist_free_all(header);
        curl_easy_cleanup(curl);
    }
    return ret;
}

void order() {
    // sign
    std::string method = "POST";
    std::string host_name = "https://presandbox.centauriglobal.com";
    std::string path = "/billing/v3/payments/order";
    std::string url = host_name + path;
    std::string body = "{\"app_id\":\"1460000404\",\"user_id\":\"1\",\"reference_id\":\"123123111\",\"transaction\":{\"payment_method\":\"STEAM\",\"currency_code\":\"USD\",\"region_code\":\"US\"},\"platform_type\":\"WINDOWS\",\"product_info\":{\"unified_product_id\":\"10086\",\"provider_app_id\":\"900000934\",\"server_id\":\"1\",\"role_id\":\"xxx\"},\"payload\":\"\",\"user_info\":{\"user_ip\":\"192.168.0.1\",\"age\":19}}";
    std::string auth_id = "1460000404";
    std::string serial_no = "1";
    std::string private_key = "-----BEGIN RSA PRIVATE KEY-----\nMII*************************tfXH/kyFs\n3fI4tHEufVnr7pGLBb3eHFyRIOG1KTZxberkzQIDAQABAoIBAAGO5uBcO3BhMD0y\nS7lk8NQ*************************r3CE7tFiECM1anoAY\nWeAqCxiM27/43HgfV1QDX7MkGJKqTBD6cGPJlGJibkGl02+Z/IGbo7c2Y0KRpC3i\nC0uC/y528RcAvZxlPAA8oBO5Tv571IK47jkjQDZ0vfGNnZOYeh5XakKngKCcGWT2\nDWGGxs81AfVOhItpdGk2Ounh+QpxhOkS+o5m6FpbVltgeKlN2Yf1SkTYs/Eveb+D\nElRiLUECgYEA8aEPJJLwFQcgKYnxBDblJDYqFpdAi+ZgU9yrZDcwWAXP0Vdxv03Z\nqda8doxj8hqvhE0GgiODlZ3QAHq1uQPOE3ooGB+D01flye3OInkIjtG0p8sVTNuP\nSMHVYcr0C2IJSZbKuCwG1/YVrzcoF6FIwYXsOFXYBWD2HyLVHqT323ECgYEA1/TG\ntU6gaMQFv9nw8ueEmNOcvmiCWBM0WVjpzcOQJLED0bT2lbF/yp5g2WQSxTJ0m3L5\nzSpcePCRkY5G7foVGFzrnSd53i44yL0O1LsMF/FYiKh1XsXNzbKaIrVHwI2U/DKP\nGi9iDM7d8V5MYXJTF0QgON4Jghkgwv9RkGF5GR0CgYEAqVf*************************XLHgftvPCGMVmn4Dl4jZ15b0YGDgxBKBf2g\nSxk8C+OGCYcCF6JG8To3B9ECgYEAjREOBD1aN1Vk1A/PfzyQtyv0BcVK77GLzplw\nEMMgk3c2IXRuZxZNfdO7PAZM5Jey6CBQ4R48zZuH22bvGmD0CrgYCM5SoznaSfaF\ni5yrp7A1vv8bvE7FwwpZ+TYlqrhRi/SS2e39xJQVEqggiL8fW+Fakwsndt5nMjmJ\n1CB39FkCgYEAz3CAvYHedkLTjmUfUFyFW8lrt6fa98+j6Oxd3TbQAW/LJlMCKz2g\nOVIijyKVH2JlZksFAlCu8tB7axgdOn3ZfIIqg7Dd6TVc7PiwNg501EWJIPNt7jdU\nXuSKSHWPUQFcpYejFMWqISRLni5ng90YAHopyt2Zjtp4AQW7leeVSCQ=\n-----END RSA PRIVATE KEY-----"; 
   
    std::string authorization, err_msg;
    if (sign(method, path, body, auth_id, serial_no, private_key, authorization, err_msg) != true) {
        std::cerr << err_msg << std::endl;
        return;
    } else {
        std::cerr << authorization << std::endl;
    }
    std::vector<std::string> headers;
    headers.push_back("Content-type: application/json");
    headers.push_back("Authorization: " + authorization);

    // order request
    std::string response;
    if (post(url, body, &headers, response) != 0) {
        return;
    }
    std::cerr << "response: " << response << std::endl;
}

void webhook(){
    // webhook verify
    std::string err_msg;
    std::map<std::string, std::string> webhook_header;  // webhook event header
    webhook_header["Timestamp"] = "1692154465";
    webhook_header["Nonce"] = "kmoIFQORnAzvTOVg3wPsed1UxxYyO4KM";
    webhook_header["Signature"] = "E8Jm5N4jMQUrhMu8ecR/94GNpo6qG3cOTINaZzrwNFGmajdqhWwXS+KW0AMpYkajYDKm2KE8tEvz9Elltlq9vk0cuaJ2hTUGYgAHiVOtCwAJd1+CMK3R6tduA9aItwgjlkhePsMCHR0RsT6nAWV46+S1g+O3QZIISfnBG7pQveWGtY1t4u0Jm8gWjhrORGUiuo5qebUajQoJHLySbDW8J1jrmwvvW5yk/JPDUYwS0epTLmpUUJ8zkKTfbXZ+21Id/UtTcW6ZMauUoSLYHPPae0Epr/CYYfuT7tsgFsH4SRLL44qS32z0ILh4kRAKxbMHkfkFmXR1uQMMrpsbsrzoNg==";
    std::string webhook_body = "{\"app_id\":\"900000934\",\"reference_id\":\"-APPDJSX200671-20230816-AsP3JXxnk0Bf0\",\"user_id\":\"12644935570913858931\",\"platform_type\":\"WINDOWS\",\"shipping_id\":\"\",\"event_type\":\"CHECK\",\"payload\":\"\",\"transaction\":{\"payment_method\":\"STEAM\",\"amount\":10,\"region_code\":\"\",\"currency_code\":\"USD\"},\"product_info\":{\"unified_product_id\":\"10086\",\"quantity\":1,\"server_id\":\"1\",\"role_id\":\"xxx\",\"original_price\":10,\"current_price\":10}}";
    std::string public_key = "-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAteHfnW3zf9iYzAHiCEmH\ncg2Pd8aM54jERHMEqyjiJLgRHMPF5cHc5Thxob0M9wvDz8OEhfw/fUJirpqXdW/D\n2CAQPXJQHlQoZfsCrwHixiYc9Q5ZAB2tMKOiT84vumXYF4RYMeTOWsCxTD1m4K/x\nmUGfh3vFlozwwY77MDBeIO+q+uX5mjl5Roi8No2mkpQa0DOK8U4DYtOOlLi9KrCH\nQ3xzA/2aqWRLmApf4H6l9DoynH3z9RFEROD0YtqzaFolYhJb86qc9XA/NUrqV+iH\nS5tUpVMqU007Amn872Y9HUYautVc+fXKQMlMes/lCp9KwrCFDpoXD4x8WiOprKTx\nDQIDAQAB\n-----END PUBLIC KEY-----";
    if (verify(webhook_body, webhook_header, public_key, err_msg) != true) {
        std::cerr << err_msg << std::endl;
        return;
    }
    std::cerr << "verify ok" << std::endl;
}

// g++ -std=c++11 -g -Wall example.cpp -lssl -lcrypto -lcurl

int main(){
    std::cerr << "order example" << std::endl;
    order();
    std::cerr << "webhook verify example" << std::endl;
    webhook();
}