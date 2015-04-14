
#include <QtNetwork/QUdpSocket>
#include <iostream>
#include "cmd_sender.h"


CMDSender::CMDSender (QUdpSocket * udp_socket)
    : m_udp_socket (udp_socket)
{
    std::cout << "CMDSender ()\n";
}   
int CMDSender::send (int message_type, char * content, int length)
{
    std::cout << "send " << message_type << "\n";
    std::cout << "111\n";
    m_udp_socket->writeDatagram ((char *)&message_type, 4, QHostAddress::LocalHost, 9501);
    return 0;
}