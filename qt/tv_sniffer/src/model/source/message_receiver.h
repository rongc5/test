
/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             message_receiver.h
 * @brief            消息接收器
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef  _RECEIVER_MSG_THREAD_H_
#define  _RECEIVER_MSG_THREAD_H_
#include <QObject>
class QUdpSocket;
class IModel;
class CPacketsCache;
class CStatisticsThread;

class  CMessageReceiver : public QObject
{
    Q_OBJECT
public:
    CMessageReceiver (
        QUdpSocket * udp_socket,
        CPacketsCache * cache,
        CStatisticsThread * statistics_thread
        );
    ~CMessageReceiver ();

protected:

private:
    /** 处理报文
     * 
     * @param  datagram 报文
     * @return void
     * @see 
     * @note 
     */
    void processTheDatagram (const QByteArray & datagram);

private:
    QUdpSocket * m_udp_socket;
    CPacketsCache * m_packets_cache;
    CStatisticsThread * m_statistics_thread;
private slots:

    /** 阅读挂起的报文
     * 
     * @return void
     * @see 
     * @note 
     */
    void readPendingDatagrams ();
    
};

#endif