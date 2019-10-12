package main

import "fmt"

func printSlice(s string, x []int)  {
	fmt.Printf("%s len=%d cap=%d %v\n", s,len(x), cap(x), x)
}

func main()  {
	a := make([]int, 5)
	printSlice("a", a)
	b := make([]int, 0, 5)
	printSlice("b", b)
	c := b[:2]
	printSlice("c", c)
	d := c[2:5]
	printSlice("d", d)

	var z []int

	fmt.Println(z, len(z), cap(z))

	if z == nil {
		fmt.Println("nil")
	}

	printSlice("z", z)
	z = append(z, 0)
	printSlice("z", z)
	z = append(z, 1)
	printSlice("z", z)
	z = append(z, 2, 3, 4, 5, 6)
	printSlice("z", z)

	var pow = []int{1, 2, 3, 4, 8, 16, 32, 64, 128}

	for i, v := range pow {
		fmt.Printf("2 ** %d = %d\n", i, v)
	}

	for i, _ := range pow {
		fmt.Printf(" %d = \n", i)
	}

	for _, v := range pow {
		fmt.Printf(" %d = \n", v)
	}
}
