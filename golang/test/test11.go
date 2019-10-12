package main

import "fmt"

func main() {
	kvs := map[string]string{"a":"apple", "b":"banana"}
	for k, v := range kvs {
		fmt.Printf("%s -> %s", k, v)
	}

	for i, c := range "go" {
		fmt.Println(i, c)

	}
}
