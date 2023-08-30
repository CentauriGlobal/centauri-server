#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include "crypto.h"

// g++ -std=c++11 -g -Wall main.cpp -lssl -lcrypto 

int main() {
    // sign
    std::string method = "POST";
    std::string path = "/billing/v3/payments/order";
    std::string body = "{\"app_id\":\"1450000000\",\"reference_id\":\"2021092610483\"}";
    std::string auth_id = "1450000000";
    std::string serial_no = "1";
    std::string private_key = "-----BEGIN RSA PRIVATE KEY-----\nMIIEpQIBAAKCA*************************OFXYBWD*************************2HyLVHqT323ECgYEA1/TG\ntU6gaMQFv9nw8ueEmNOcvmiCWBM0WVjp*************************zcOQJLED0bT2lbF/yp5g2WQSxTJ0m3L5\nzSpcePCRkY5G7foVGFzrnSd53i44yL0O1LsMF/FYiKh1XsXNzbKaIrVHwI2U/DKP\nGi9iDM7d8V5MYXJTF0QgON4Jghkgwv9RkGF5GR0CgYEAqVfyj/R3VJVw4gvuIcED\naRSY/Mo2j5X/DudKK2tOc3JnStVZP/UkxpZb8U3FQkmXl0aQeCbBstsuixd1Qty6\n0gW8EeQMdNNWleYXoStoZwfr7+kx3XLHgftvPCGMVmn4Dl4jZ15b0YGDgxBKBf2g\nSxk8C+OGCYcCF6JG8To3B9ECgYEAjREOBD1aN1Vk1A/PfzyQtyv0BcVK77GLzplw\nEMMgk3c2IXRuZxZNfdO7PAZM5Jey6CBQ4R48zZuH22bvGmD0CrgYCM5SoznaSfaF\ni5yrp7A1vv8bvE7FwwpZ+TYlqrhRi/SS2e39xJQVEqggiL8fW+Fakwsndt5nMjmJ\n1CB39FkCgYEAz3CAvYHedkLTjmUfUFyFW8lrt6fa98+j6Oxd3TbQAW/LJlMCKz2g\nOVIijyKVH2JlZksFAlCu8tB7axgdOn3ZfIIqg7Dd6TVc7PiwNg501EWJIPNt7jdU\nXuSKSHWPUQFcpYejFMWqISRLni5ng90YAHopyt2Zjtp4AQW7leeVSCQ=\n-----END RSA PRIVATE KEY-----"; 
    std::string authorization, err_msg;
    if (sign(method, path, body, auth_id, serial_no, private_key, authorization, err_msg) != true) {
        std::cerr << err_msg << std::endl;
    } else {
        std::cerr << authorization << std::endl;
    }

    // verify
    std::map<std::string, std::string> webhook_header;  // webhook event header
    webhook_header["Timestamp"] = "1690270063";
    webhook_header["Nonce"] = "yXrTu4r6dIwhhfYm1LeBLb2t1ba4Ps7n";
    webhook_header["Signature"] = "FkZKRZPX63Ztg/oDn51p/eM79rtjsMwQxnvZNOsJYKp+WgjROhJzA/kBmx17ETOAUb5fsYzl3T8BCG6ev1cjRl0zO3nD46WNdM06N6WMmGN52n3ZU0m2UHVJElGZnUB65rBBJuEas+CljpVi2pKmO8VzIm1Wjh7217IF7assr6UbY8vRHcQB0rMLTMloY+1H7vsdB+wfJ8K6y47rnjuuZUhD8B/Zq3CkvPTSS1LxWr92J1Lwaj+9/Ey2V5K0PtDmTmd40yOHw+MHz7Ijm5R5CvCijWYkvZUToUrClpxIw9sxqUeet6FhVl9bbLYhpuKof8Rw+JJRm+mvPvh5fIhqqQ==";
    std::string webhook_body = "{\"app_id\":\"1450000000\",\"reference_id\":\"2021092610483\"}";
    std::string public_key = "-----BEGIN PUBLIC KEY-----\nMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAy9VMx9kQJyepVJWtEC4a\nHgS5hOt38bb6xVDGovvyhswhy9vbOLirZiYGo0/fTNNJEBM39/lRS13X77m/K49Z\nJvVLTd03aDTYDz2eRBJx2fL8wXSFSE1spqW4QrC9mPttZZmW2d7y6bt0W2OgMw5w\nt+3CFcX5WVMCOZaXgzeM6G+Cszglnx8kWqNtxghU2J/1CdnnUZIT/BXTSiLTu4oq\nrrbLYy/tVqXLDbS43kP8LV7iNDf6hEf25sHWqC/Q//VrnqGOenqEGqygfqLjki5f\nN3YGM2bTc2AQJxg0VcOtfXH/kyFs3fI4tHEufVnr7pGLBb3eHFyRIOG1KTZxberk\nzQIDAQAB\n-----END PUBLIC KEY-----";
    if (verify(webhook_body, webhook_header, public_key, err_msg) != true) {
        std::cerr << err_msg << std::endl;
    }
    return 0;
}
