package main

import (
	"fmt"
	"net/url"
	"net"
)

func main() {
	//s := "https://www.yiibai.com/go/golang-url-parsing.html#article-start"
	s := "postgres://user:pass@host.com:5432/path?k=v#f"
	p := fmt.Println

	u, e := url.Parse(s)
	if e != nil {
		panic(e)
	}

	p(u.Scheme)
	p(u.User)
	p(u.User.Username())
	f, _ := u.User.Password()
	p(f)

	p(u.Host)
	host, port, _ := net.SplitHostPort(u.Host)
	p(host)
	p(port)
	p(u.Path)
	p(u.Fragment)

	p(u.RawQuery)
	m, _ := url.ParseQuery(u.RawQuery)
	p(m)
	p(m["k"][0])
}
