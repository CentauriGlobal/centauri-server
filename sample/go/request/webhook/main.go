package main

import (
	"context"
	"fmt"
	"io/ioutil"
	"strconv"

	sdk "github.com/CentauriGlobal/centauri-server/sdk/go"
)

func main() {
	// timestamp retrived from the http header
	timestamp := 1692154465
	// nonce retrived from the http header
	nonce := "kmoIFQORnAzvTOVg3wPsed1UxxYyO4KM"
	// signature retrived from the http header
	signature := "E8Jm5N4jMQUrhMu8ecR/94GNpo6qG3cOTINaZzrwNFGmajdqhWwXS+KW0AMpYkajYDKm2KE8tEvz9Elltlq9vk0cuaJ2hTUGYgAHiVOtCwAJd1+CMK3R6tduA9aItwgjlkhePsMCHR0RsT6nAWV46+S1g+O3QZIISfnBG7pQveWGtY1t4u0Jm8gWjhrORGUiuo5qebUajQoJHLySbDW8J1jrmwvvW5yk/JPDUYwS0epTLmpUUJ8zkKTfbXZ+21Id/UtTcW6ZMauUoSLYHPPae0Epr/CYYfuT7tsgFsH4SRLL44qS32z0ILh4kRAKxbMHkfkFmXR1uQMMrpsbsrzoNg=="

	// read file to get the public key
	publicKey, err := ioutil.ReadFile("../../data/public.pem")
	if err != nil {
		fmt.Println(err)
		return
	}

	// request body in json format, that is request body which Midas send to you with Webhook interface
	body, err := ioutil.ReadFile("./request.json")
	if err != nil {
		fmt.Println(err)
		return
	}

	verifyParam := sdk.VerifyParam{
		Signature: signature,
		Nonce:     nonce,
		Timestamp: strconv.FormatInt(int64(timestamp), 10),
		PublicKey: publicKey,
		Body:      body,
	}
	err = sdk.Verify(context.Background(), verifyParam)
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println("verify ok")
}
