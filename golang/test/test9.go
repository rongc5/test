package main

import (
	"fmt"
	"strings"
)

type Vertex struct {
	x, y int
}


func main() {
	i, j := 42, 2701

	p := &i
	fmt.Println(*p)
	*p = 21
	fmt.Println(i)

	p = &j
	*p = *p
	fmt.Println(j)

	fmt.Println(Vertex{1, 2})

	v := Vertex{3, 5}
	v.x = 9

	q := &v
	q.x = 1e9
	fmt.Println(v)

	v1 := Vertex{1,2}
	v2 := Vertex{x:1}
	v3 := Vertex{}
	ff := &Vertex{1,2}

	fmt.Println(v1, ff, v2, v3)

	var a [10]int

	fmt.Println(a)

	s := []int{2, 3, 5, 7, 11, 13, 15}
	fmt.Println("s == s[1:4], s[:3]", s, s[1:4], s[:3])
	fmt.Println("s[4:]", s[4:])


	for i := 0; i < len(s); i++ {
		fmt.Printf("s[%d] == %d", i, s[i])
	}

	game := [][]string{
		[]string{"_", "_", "_"},
		[]string{"_", "_", "_"},
		[]string{"_", "_", "_"},
	}

	game[0][0] = "X"
	game[2][2] = "O"
	game[2][0] = "X"
	game[1][0] = "O"
	game[0][2] = "X"

	pritBoard(game)

}

func pritBoard(s [][]string) {
	for i := 0; i< len(s); i++{
		fmt.Printf("%s\n", strings.Join(s[i], " "))
	}
}