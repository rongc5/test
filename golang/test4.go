package main

import "fmt"

func main() {

    var a = [...]int{0, 1, 2, 3, 4, 5, 6, 7}

    fmt.Printf("type of x :%d",a[6])                

    var x interface{}

    switch i := x.(type) {
        case nil:    
            fmt.Printf("type of x :%T",i)                
        case int:     
                fmt.Printf("x is int")                       
        case float64:
                    fmt.Printf("x is float64")           
        case func(int) float64:
                        fmt.Printf("x is func(int)")                      
        case bool, string:
                            fmt.Printf("x is bool or string")       
        default:
                                fmt.Printf("don't know the type")     
    }   
}
