package controllers

import (
	"fmt"
	"github.com/astaxie/beego"
	"login/models"
)

type ResistController struct {
	beego.Controller
}

func (this * ResistController) Get()  {
	this.TplName = "regist.tpl"
}

func (this *ResistController) Post()  {
	var user models.User
	inputs := this.Input()

	user.Username = inputs.Get("username")
	user.Pwd = inputs.Get("pwd")
	err := models.SaveUser(user)
	if err == nil {
		this.TplName = "success.tpl"
	} else {
		fmt.Println(err)
		this.TplName = "error.tpl"
	}
}
