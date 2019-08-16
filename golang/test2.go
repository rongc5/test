package main

import (
    "io"
    "log"
    "net/http"
    "time"
)

var mux map[string]func(http.ResponseWriter, *http.Request)

func main() {
    server := http.Server{
        Addr:        ":8081",
        Handler:     &MyHandle{},
        ReadTimeout: 6 * time.Second,
    }
    mux = make(map[string]func(http.ResponseWriter, *http.Request))
    mux["/hello"] = hello
    mux["/bye"] = bye
    err := server.ListenAndServe()

    if err != nil {
        log.Fatal(err)
    }
}

type MyHandle struct{}

func (*MyHandle) ServeHTTP(w http.ResponseWriter, r *http.Request) {
    if h, ok := mux[r.URL.String()]; ok {
        h(w, r)
    }
    io.WriteString(w, "URL"+r.URL.String())
}

func hello(w http.ResponseWriter, r *http.Request) {
    io.WriteString(w, "hello 模块")
}

func bye(w http.ResponseWriter, r *http.Request) {
    io.WriteString(w, "bye 模块")
}
