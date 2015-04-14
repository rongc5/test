
#include "login.h"
#include "check_password.h"
#include <QMessageBox>
#include <QPushButton>

CLoginDlg::CLoginDlg (QWidget * parent)
    : QDialog(parent)
{
    m_login_dlg.setupUi(this);
    m_login_dlg.buttonBox->button(QDialogButtonBox::Ok)->setText (QString::fromLocal8Bit("确定"));
    //m_login_dlg.buttonBox->button(QDialogButtonBox::Cancel)->setText (QString::fromLocal8Bit("取消"));
    //setWindowFlags (windowFlags() & (~Qt::WindowCloseButtonHint));
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowTitleHint);
    //setWindowFlags (windowFlags() & (~Qt::));
}

QString CLoginDlg::get_id ()
{
    return m_login_dlg.line_edit_id->text();
}
QString CLoginDlg::get_password ()
{
    return m_login_dlg.line_edit_password->text();
}

void CLoginDlg::run ()
{
    CCheckPassword check_password;
    do 
    {
        int ret = this->exec();
        if (ret == QDialog::Rejected)
        {
            exit(0);
        }
        if (check_password.check(get_id(), get_password()) == 0)
        {
            break;
        }
         QMessageBox::warning (this, QString::fromLocal8Bit("提示") , QString::fromLocal8Bit("用户名或密码错误") );
    } while (true);
}