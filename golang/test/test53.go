package main

import (
	"bufio"
	"os"
	"strings"
	"fmt"
)

func main() {
	sc := bufio.NewScanner(os.Stdin)

	for sc.Scan() {
		ucl := strings.ToUpper(sc.Text())

		fmt.Println(ucl)
	}

	if err := sc.Err(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}