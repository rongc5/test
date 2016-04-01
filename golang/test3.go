package main

import "encoding/json" 
import "fmt" 
import "os"

type Response1 struct { 
    Page   int 
        Fruits []string 
}

type Response2 struct { 
    Page   int      `json:"page"` 
        Fruits []string `json:"fruits"` 
}

type CommonArg struct { 
    SessionId int64  `json:",string"` 
        Op        string `json:"Op,omitempty"` 
        AppId     string `json:"appId,omitempty"` 
        Online    bool 
}

func main() { 
    bolB, _ := json.Marshal(true) 
        fmt.Println(string(bolB)) // true

        intB, _ := json.Marshal(1) 
        fmt.Println(string(intB)) // 1

        fltB, _ := json.Marshal(2.34) 
        fmt.Println(string(fltB)) // 2.34

        strB, _ := json.Marshal("gopher") 
        fmt.Println(string(strB)) // "gopher"

        slcD := []string{"apple", "peach", "pear"} 
    slcB, _ := json.Marshal(slcD) 
        fmt.Println(string(slcB)) //  ["apple","peach","pear"]

        mapD := map[string]int{"apple": 5, "lettuce": 7} 
    mapB, _ := json.Marshal(mapD) 
        fmt.Println(string(mapB)) // {"apple":5,"lettuce":7}

res1D := &Response1{ 
Page:   1, 
            Fruits: []string{"apple", "peach", "pear"}} 
       res1B, _ := json.Marshal(res1D) 
           fmt.Println(string(res1B)) // {"Page":1,"Fruits":["apple","peach","pear"]}

res2D := &Response2{ 
Page:   1, 
            Fruits: []string{"apple", "peach", "pear"}} 
       res2B, _ := json.Marshal(res2D) 
           fmt.Println(string(res2B)) // {"Page":1,"Fruits":["apple","peach","pear"]}

aaa := &CommonArg{ 
SessionId: 12345, 
               Op:        "werdfsdde22233", 
               AppId:     "654343dfddd33424dd", 
               Online:    false, 
     } 
     aaa1, _ := json.Marshal(aaa) 
         fmt.Println(string(aaa1)) // {"SessionId":"12345","Op":"werdfsdde22233","appId":"654343dfddd33424dd","Online":false}

byt := []byte(`{"num":6.0,"strs":["a","b"]}`)

         var dat map[string]interface{}

     if err := json.Unmarshal(byt, &dat); err != nil { 
         panic(err) 
     } 
     fmt.Println(dat) // map[strs:[a b] num:6]

         num := dat["num"].(float64) 
         fmt.Println(num) // 6

         strs := dat["strs"].([]interface{}) 
         str1 := strs[0].(string) 
         fmt.Println(str1) // a

         str := `{"page": 1, "fruits": ["apple", "peach"]}` 
         res := &Response2{} 
     json.Unmarshal([]byte(str), &res) 
         fmt.Println(res)           // &{1 [apple peach]} 
         fmt.Println(res.Fruits[0]) // apple

         enc := json.NewEncoder(os.Stdout) 
         d := map[string]int{"apple": 5, "lettuce": 7} 
     enc.Encode(d) // {"apple":5,"lettuce":7} 
}


