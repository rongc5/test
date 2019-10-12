package main

import (
	"encoding/base64"
	"fmt"
)

func main() {
	data := "abc123!?$*&()'-=@~"
	sEnc := base64.StdEncoding.EncodeToString([]byte(data))

	p := fmt.Println
	p(sEnc)

	sDec, _ := base64.StdEncoding.DecodeString(sEnc)
	p(string(sDec))

	uEnc := base64.URLEncoding.EncodeToString([]byte(data))
	p(uEnc)
	uDec, _ := base64.URLEncoding.DecodeString(uEnc)
	p(string(uDec))
}
