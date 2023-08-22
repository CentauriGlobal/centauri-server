package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"net/http"

	sdk "github.com/CentauriGlobal/centauri-server/sdk/go"
)

const (
	HOST   = "http://presandbox.centauriglobal.com"
	METHOD = "POST"
	PATH   = "/billing/v3/payments/order"
)

func main() {
	// Prepare the materials needed to calculate the signature
	appId := "1460000404"
	serialNo := "39393631313439343138"

	// read file to get the private key
	privateKey, err := ioutil.ReadFile("../../data/private.pem")
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
	req, err := http.NewRequest(METHOD, HOST+PATH, payload)
	if err != nil {
		fmt.Println(err)
		return
	}

	// calculate signature and add to header
	signParam := sdk.SignParam{
		Method:     METHOD,
		Path:       PATH,
		Body:       body,
		AppID:      appId,
		SerialNo:   serialNo,
		PrivateKey: privateKey,
		// hero we use app_id, in real production environment, you don't need to specify this field
		AuthIdType: "app_id",
	}
	authorization, err := sdk.Sign(req.Context(), signParam)
	if err != nil {
		fmt.Println(err)
		return
	}

	// add http request header
	req.Header.Add("Authorization", authorization)
	req.Header.Add("Content-Type", "application/json")

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
