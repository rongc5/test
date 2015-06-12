
#include "check_password.h"

int CCheckPassword::check (const QString & id, const QString & password)
{
    if (id == "admin" && password == "admin")
    {
        return 0;
    }

    return -1;
}