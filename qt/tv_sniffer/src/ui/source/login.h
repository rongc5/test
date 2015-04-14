/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             login.h
 * @brief            login interfae
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _LOGIN_H_
#define _LOGIN_H_
#include <QDialog>
#include "../forms/ui_dir/ui_login_dialogue.h"

class CLoginDlg : public QDialog
{
    Q_OBJECT
public:
    CLoginDlg (QWidget * parent = 0);
   
    /** 运行
     * 
     * @return void
     * @see 
     * @note 
     */
    void run ();
protected:
private:
    /** 获取id
     * 
     * @return id
     * @see 
     * @note 
     */
    QString get_id ();
    /** 获取密码
     * 
     * @return 密码
     * @see 
     * @note 
     */
    QString get_password ();
private:
    Ui_login_dlg  m_login_dlg;
};


#endif