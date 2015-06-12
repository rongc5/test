#ifndef BINFO_H
#define BINFO_H
#include <QMutex>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QString>

typedef struct Baseinfo{
    QString haierIp;
    QString haierMac;
    QString snifferIp;
    QString snifferNetface;
}Binfo;

class Basic
{
public:
    void getBasic(Binfo * info);
    void setBasic();
    void parse_network_info(char *buf);
    void getNetworkInterface(char *buf, size_t size);
    void prase_haier_info(char *buf);
    void getHaierInfo(char *buf, size_t size);

private:
    int prase_str(char *buf, const char *name, char *save);
    Binfo base;
    QMutex mutex;
};

#endif // BINFO_H
