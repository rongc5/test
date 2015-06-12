
/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             check_password.h
 * @brief            检查密码是否正确
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _CHECK_PASSWORD_H_
#define _CHECK_PASSWORD_H_
#include <QString>

class CCheckPassword
{
public:

    /** 审核用户名和密码是否正确
     * 
     * @param  id
     * @param  password
     * @return 0 正确 -1不正确
     * @see 
     * @note 
     */
    int check (const QString & id, const QString & password);
protected:
private:
};


#endif