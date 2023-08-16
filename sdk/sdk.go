package sdk

import (
	"context"
	"crypto"
	crand "crypto/rand"
	"crypto/rsa"
	"crypto/sha256"
	"crypto/x509"
	"encoding/base64"
	"encoding/pem"
	"errors"
	"fmt"
	srand "math/rand"
	"time"
)

const letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

const (
	// 6 bits to represent a letter index
	letterIdBits = 6
	// All 1-bits as many as letterIdBits
	letterIdMask = 1<<letterIdBits - 1
	letterIdMax  = 63 / letterIdBits
)

// SignParam parameters required to calculate signature
type SignParam struct {
	// http request method, eg. POST/GET
	Method string
	// request path with leading slash
	Path string
	// request body
	Body []byte
	// app id
	AppID string
	// serial_no
	SerialNo string
	// private key
	PrivateKey []byte
	// UTC timestamp in second, if you don't want to provide, we will generate for you
	Timestamp int64
	// random string, if you don't want to provide, we will generate for you
	Nonce string
	// auth id type, if you don't want to provide, we will generate for you, default is "app_id"
	AuthIdType string
}

// VerifyParam parameters required to verify signature
type VerifyParam struct {
	// signature
	Signature string
	// nonce str
	Nonce string
	// UTC timestamp in seconds
	Timestamp string
	// public key
	PublicKey []byte
	// request body
	Body []byte
}

// Sign calculate signature for request to midas
func Sign(ctx context.Context, param SignParam) (authorization string, err error) {
	timestamp := time.Now().UTC().Unix()
	if param.Timestamp != 0 {
		timestamp = param.Timestamp
	}
	nonce := randStr(16)
	if param.Nonce != "" {
		nonce = param.Nonce
	}
	authIdType := "app_id"
	if param.AuthIdType != "" {
		authIdType = param.AuthIdType
	}
	signStr := fmt.Sprintf("%s\n%s\n%d\n%s\n%s\n", param.Method, param.Path, timestamp, nonce, string(param.Body))
	privateKey, err := parseRSAPrivateKey(param.PrivateKey)
	if err != nil {
		return "", err
	}

	hashed := sha256.Sum256([]byte(signStr))

	signature, err := rsa.SignPKCS1v15(crand.Reader, privateKey, crypto.SHA256, hashed[:])
	if err != nil {
		return "", err
	}
	authorization = fmt.Sprintf("SHA256-RSA2048 auth_id=%s,auth_id_type=%s,nonce_str=%s,timestamp=%d,signature=%s,serial_no=%s",
		param.AppID, authIdType, nonce, timestamp, base64.StdEncoding.EncodeToString(signature), param.SerialNo)
	return authorization, nil
}

// Verify verify signature from midas
func Verify(ctx context.Context, param VerifyParam) (err error) {
	signature, err := base64.StdEncoding.DecodeString(param.Signature)
	if err != nil {
		return err
	}
	publicKey, err := parseRSAPublicKey(param.PublicKey)
	if err != nil {
		return err
	}
	signStr := fmt.Sprintf("%s\n%s\n%s\n", param.Timestamp, param.Nonce, string(param.Body))
	hash := sha256.Sum256([]byte(signStr))
	err = rsa.VerifyPKCS1v15(publicKey, crypto.SHA256, hash[:], signature)
	if err != nil {
		return err
	}
	return nil
}

// randStr generate random string of n bytes
func randStr(n int) string {
	src := srand.NewSource(time.Now().UnixNano())
	b := make([]byte, n)
	// A rand.Int63() generates 63 random bits, enough for letterIdMax letters!
	for i, cache, remain := n-1, src.Int63(), letterIdMax; i >= 0; {
		if remain == 0 {
			cache, remain = src.Int63(), letterIdMax
		}
		if idx := int(cache & letterIdMask); idx < len(letters) {
			b[i] = letters[idx]
			i--
		}
		cache >>= letterIdBits
		remain--
	}
	return string(b)
}

func parseRSAPrivateKey(privateKeyData []byte) (*rsa.PrivateKey, error) {
	block, _ := pem.Decode(privateKeyData)
	if block == nil {
		return nil, errors.New("invalid private key")
	}

	rsaPrivateKey, err := x509.ParsePKCS1PrivateKey(block.Bytes)
	if err != nil {
		return nil, err
	}
	return rsaPrivateKey, nil
}

func parseRSAPublicKey(publicKeyData []byte) (*rsa.PublicKey, error) {
	block, _ := pem.Decode(publicKeyData)
	if block == nil {
		return nil, errors.New("invalid public key")
	}
	publicKey, err := x509.ParsePKIXPublicKey(block.Bytes)
	if err != nil {
		return nil, err
	}
	rsaPublicKey, ok := publicKey.(*rsa.PublicKey)
	if !ok {
		return nil, errors.New("failed to convert publicKey to *rsa.PublicKe")
	}
	return rsaPublicKey, nil
}
