package main

import (
	//"fmt"
	"github.com/astaxie/beego"
	//"login/controllers"
)

type MainController struct {
	beego.Controller
}

func main() {
	//beego.Sess = true
	//beego.UnregisterFixedRoute("/", &controllers.IndexController{})
	//beego.UnregisterFixedRoute("/regist", &controllers.RegistController{})
	beego.Run()
}
