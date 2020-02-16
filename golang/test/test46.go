package main

import (
	"fmt"
	"time"
)

func main() {
	p := fmt.Println

	t := time.Now()
	p(t.Format(time.RFC3339))

	t1, _ := time.Parse(time.RFC3339, "2019-08-19")
	p(t1)
	p(t.Format("3:04PM"))
	form := "3 04 PM"
	t2, _ := time.Parse(form, "8 41 PM")
	p(t2)

}
