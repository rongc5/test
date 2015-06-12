

#include <QtNetwork/QUdpSocket>
#include <QByteArray>
#include <iostream>
#include "communicate_data_struct.h"
#include "imodel.h"
#include "message_receiver.h"
#include "qdebug.h"
#include "data_struct.h"
#include "statistics_thread.h"
#include "packets_cache.h"

CMessageReceiver::CMessageReceiver (
    QUdpSocket * udp_socket,
    CPacketsCache * cache,
    CStatisticsThread * statistics_thread)
    : 
    m_udp_socket (udp_socket),
    m_packets_cache (cache),
    m_statistics_thread (statistics_thread)
{
    int result = m_udp_socket->bind(QHostAddress::LocalHost, m_udp_socket->localPort(), QUdpSocket::ShareAddress);
    std::cout << "result = " << result << "\n";
    connect(m_udp_socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    
    std::cout << "socket id = " << m_udp_socket << "\n";
}
CMessageReceiver::~CMessageReceiver ()
{

}

void CMessageReceiver::readPendingDatagrams ( )
{
    std::cout << "has pending data\n";
    qDebug() <<  "has pending data\n";
    while (m_udp_socket->hasPendingDatagrams()) 
    {
        QByteArray datagram;
        datagram.resize(m_udp_socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_udp_socket->readDatagram(datagram.data(), datagram.size(),
            &sender, &senderPort);

        processTheDatagram(datagram);
    }
}
void CMessageReceiver::processTheDatagram (const QByteArray & datagram)
{
    int length = datagram.length();
    std::cout << "lenth = " << length << "\n";
    const char * data = datagram.data();
    int * cmd = (int *)data;
    std::cout << "*cmd = " << *cmd << "\n";
    if (*cmd == 0)
    {
        std::cout << "*cmd = " << *cmd << "\n";
        if (length == 4 + sizeof (tv_sniffer_column))
        {
            std::cout << "begin\n";
           tv_sniffer_column * col = (tv_sniffer_column *)(data + 4);
          std::cout << col->no << " " << col->time <<" " <<
                col->src_ip << " " << col->src_port << " " << col->src_dns << " " 
                << col->dst_dns << " "<< col->dst_ip << " " << col->dst_port
                << " " << col->protocol << " " << col->packet_size << "\n";
            Packet_Record record;
            record.no = col->no;
            record.time = col->time;
            record.src_ip = col->src_ip;
            record.src_port = col->src_port;
            record.dst_ip= col->dst_ip;
            record.src_dns = col->src_dns;
            record.dst_dns = col->dst_dns;
            record.dst_port = col->dst_port;
            record.packet_size = col->packet_size;
            record.protocol = col->protocol;
            std::cout << "add\n";
            m_statistics_thread->add_packet (record);
            m_packets_cache->add_packet(record);
        }
    }
    
}
