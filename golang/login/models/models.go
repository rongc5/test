package models

import (
	"database/sql"
	"errors"
	"fmt"
	"github.com/astaxie/beedb"
	_ "github.com/ziutek/mymysql/godrv"
)

type User struct {
	Id int `PK`
	Username string
	Pwd string
}

func  getLink() beedb.Model  {
	db,err := sql.Open("mysql", "root:root@tcp(192.168.1.81:3306)/test_my?charset=utf8")
	if err != nil {
		panic(err)
	}

	orm := beedb.New(db)
	return orm
}

func SaveUser(user User) error {
	orm := getLink()
	fmt.Println(user)
	err := orm.Save(&user)
	return err
}

func ValidateUser(user User) error {
	orm := getLink()
	orm.Where("username=? and pwd=?", user.Username, user.Pwd).Find(&user)
	if user.Username == "" {
		return errors.New("用户名或密码错误！")
	}
	return nil
}