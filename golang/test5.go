// test5 project main.go
package main

import (
	"fmt"
)

type User struct {
	UserId uint64
	Name   string
}

func (u *User) String() string {
	return fmt.Sprintf("%v	 ::::::::: %v\n", u.Name, u.UserId)
}

func main() {

	var u = User{
		UserId: 11111111,
		Name:   "hello world",
	}

	var str = u.String()
	fmt.Printf("%v", str)
	//fmt.Println("Hello World!")
}
