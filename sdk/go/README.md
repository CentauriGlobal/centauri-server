# How to use SDK for signing and verifying

## Signing

here is an example for calculate signature

```golang
package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"net/http"

	sdk "github.com/CentauriGlobal/centauri-server/sdk/go"
)

func main() {

	// Prepare the materials needed to calculate the signature
	host := "http://127.0.0.1:8001"
	path := "/billing/v3/payments/order"
	method := "POST"
	appId := "145961149415000000000"
	serialNo := "39393631313439343138"
	timestamp := 1692153442
	nonce := "7742fbb4-1eae-4265-a226-63f8db6923dc"

	// read file to get the private key
	privateKey, err := ioutil.ReadFile("./private.pem")
	if err != nil {
		fmt.Println(err)
		return
	}

	// request body in json format
	body, err := ioutil.ReadFile("./request.json")
	if err != nil {
		fmt.Println(err)
		return
	}

	// construct http request
	payload := bytes.NewReader(body)
	client := &http.Client{}
	req, err := http.NewRequest(method, host+path, payload)
	if err != nil {
		fmt.Println(err)
		return
	}

	// add http request header
	req.Header.Add("Content-Type", "application/json")

	// calculate signature and add to header
	signParam := sdk.SignParam{
		Method:     method,
		Path:       path,
		Body:       body,
		AppID:      appId,
		SerialNo:   serialNo,
		PrivateKey: privateKey,
		Timestamp:  int64(timestamp),
		Nonce:      nonce,
		AuthIdType: "MERCHANT_ID",
	}
	authorization, err := sdk.Sign(req.Context(), signParam)
	if err != nil {
		fmt.Println(err)
		return
	}
	req.Header.Add("Authorization", authorization)

	// send request
	res, err := client.Do(req)
	if err != nil {
		fmt.Println(err)
		return
	}
	defer res.Body.Close()

	// read from response body
	body, err = ioutil.ReadAll(res.Body)
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println(string(body))
}
```

## Verifying

Similar to computing a signature, the key part is as follows:

```golang
verifyParam := sdk.VerifyParam{
	Signature: signature,
	Nonce:     nonce,
	Timestamp: timestamp,
	PublicKey: publicKey,
	Body:      body,
}
err := sdk.Verify(context.TODO(), verifyParam)
if err != nil {
	log.Println(err)
}
```

The materials required for verifying can be obtained from the request header.
