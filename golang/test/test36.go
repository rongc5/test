package main

import "os"

func main() {
	panic("a problen")

	_, err := os.Create("/tmp/file")
	if err != nil {
		panic(err)
	}
}
