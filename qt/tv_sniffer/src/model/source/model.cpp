#include <vector>
#include <QThread>
#include <QDateTime>
#include <QtNetwork/QUdpSocket>
#include <QProcess>
#include <iostream>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QMutexLocker>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QString>
#include <QImageReader>
#include <QPrinter>
#include <QMessageBox>
#include <QTextCharFormat>
#include <QFont>
#include <QUrl>
#include <QTextTableFormat>
#include <QTextFrameFormat>
#include <QTextLength>
#include <QBrush>

#include <QImage>
#include <stdio.h>
#include <stdlib.h>
#include <python2.7/Python.h>
#include <unistd.h>

#include "data_struct.h"
#include "communicate_data_struct.h"
#include "ictrl.h"
#include "cmd_sender.h"
#include "message_receiver.h"
#include "packets_cache.h"
#include "packets_sender.h"
#include "statistics_thread.h"
#include "baseinfo_thread.h"
#include "ip_filter.h"
#include "imodel.h"
#include "print.h"
#include "filter_file.h"
#include "start_thread.h"

#include "model.h"
#include "iview.h"

class CSimPacketListData : public QThread
{
public:
    CSimPacketListData (ICtrl * ctrl) : m_ctrl (ctrl), stop_sim (true)
    {

    }
    void stop ()
    {
        stop_sim = true;
    }
    void restart ()
    {
        stop_sim = false;
    }
protected:
    virtual void run ()
    {
        std::vector <std::string> v_str_src_ip;
        v_str_src_ip.push_back("21.345.98.34");
        v_str_src_ip.push_back("87.43.87.32");
        v_str_src_ip.push_back("54.36.94.82");
        std::vector <std::string> v_str_dst_ip;
        v_str_dst_ip.push_back("192.168.39");
        v_str_dst_ip.push_back("65.168.128");
        v_str_dst_ip.push_back("68.53.90.35");
        std::vector <std::string> v_str_dst_port;
        v_str_dst_port.push_back("8000");
        v_str_dst_port.push_back("80");
        v_str_dst_port.push_back("10000");
        std::vector <std::string> v_str_packet_size;
        v_str_packet_size.push_back("1024");
        v_str_packet_size.push_back("2048");
        v_str_packet_size.push_back("128");


        for (int i = 1; i <  3600 * 8000; ++i)
        {
            if (stop_sim)
            {
                i = 1;
                while (stop_sim)
                {
                    QThread::msleep (1000 * 1);
                }
            }

            Packet_Record pr;
            pr.no = QString::number(i).toStdString();
            pr.dst_ip = v_str_dst_ip[i % v_str_dst_ip.size()];
            pr.src_ip = v_str_src_ip[i % v_str_src_ip.size()];
            pr.dst_port = v_str_dst_port[i % v_str_dst_port.size()];
            pr.src_port = v_str_dst_port[(i + 1) % v_str_dst_port.size()];;
            pr.packet_size = v_str_packet_size[i % v_str_packet_size.size()];
            QDateTime curr = QDateTime::currentDateTime();
            pr.time = curr.toString("yyyy/MM/dd HH:mm:ss").toStdString();
            pr.protocol = i % 2 == 0 ? "TCP" : "UDP";
            m_ctrl->add_packet(pr);
            if (i % 8 != 0)
            {
                continue;
            }

            m_ctrl->add_packet_count(rand () % 1000);
            QThread::msleep (1000 * 1);


        }
    }
private:
    ICtrl * m_ctrl;
    bool stop_sim;
};

IModel * get_model ()
{
    return new CModel ();
}

CModel::CModel () : m_ctrl (0)
{

}
CModel::~CModel ()
{
    m_udp_socket->close();
}
void CModel::init( ICtrl * ctrl)
{
    m_ctrl = ctrl;
   /* m_test_thread = new  CSimPacketListData(m_ctrl);
    m_test_thread->start ();*/

    m_udp_socket = new QUdpSocket();
    m_cmd_sender = new CMDSender (m_udp_socket);


    m_packets_cache = new CPacketsCache ();
    m_ip_filter = new CIPFilter ();
    m_statistics_thread = new CStatisticsThread (m_ip_filter);
    m_statistics_thread->start ();
     m_message_receiver = new CMessageReceiver (
         m_udp_socket,
         m_packets_cache,
         m_statistics_thread);

     m_packets_sender = new CPacketsSender (
         m_packets_cache,
         m_statistics_thread,
         this);
     m_process= new QProcess ();

    m_base = new BaseThread();
    m_base->start();
    m_start_thread = new CStartThread (this);

    m_filter_file = new CFilterFile ("/home/haier/tv_sniffer/etc/filter_file.ini");
}
void CModel::run ()
{

}
 void CModel::start_capture ()
 {
   m_v_start_thread.push_back(m_start_thread);
   if (m_v_start_thread.size() > 1000)
   {
       CStartThread * tmp = m_v_start_thread.front();
       m_v_start_thread.pop_front();
       delete tmp;
   }
   m_start_thread = new CStartThread (this);
   m_start_thread->start ();

 }
  void CModel::start_capture_impl ()
  {
      QProcess * tmp =  m_process;
      {
          std::cout << " CModel start_capture begin\n";
          QMutexLocker locker (&m_start_stop_mutex);
          //m_ctrl->clear_packet_list();
          //((CSimPacketListData *)m_test_thread)->restart();
          std::cout << " CModel getBasic begin\n";
          BasicInfo baseinfo;
          m_base->getBasic(&baseinfo);
          std::cout << " CModel getBasic end\n";
          //baseinfo.net_name;
          QStringList args;
          args << "-i";
          args <<  "eth1";
          args << "-f";
          args << "ip and ! port 1900";
          args << "-N";
          args << "mntC";
          QStringList::const_iterator cit
              = args.begin();
          for ( ; cit != args.end () ; ++cit)
          {
              std::cout << "arg = " << cit->toStdString().c_str () << "\n";
          }
          std::cout << " CModel args \n";
          std::cout << "start process\n";
          m_process->kill();
          m_process =  new QProcess ();
          m_process->start ("/home/haier/wireshark_hxb/tshark", args);
          std::cout << "start over\n";
          while( !m_process->waitForStarted(500) )
          {
              std::cout << "wait for start\n";
          }
          std::cout << "readAllStandardOutput begin\n";
          m_process->readAllStandardOutput();
          std::cout << "readAllStandardOutput end\n";


          std::cout << "elapsed end\n";
          int state = m_process->state();
          std::cout << "state = " << state << "\n";

          //m_cmd_sender->send(MESSAGE_START_CAPTURE);
          int count = 0;
          while (!m_udp_socket->hasPendingDatagrams())
          {
              //m_cmd_sender->send(MESSAGE_START_CAPTURE);
              QElapsedTimer t;
              t.start();
              std::cout << "elapsed begin\n";
              while(t.elapsed() < 1000)
              {

              }
              m_cmd_sender->send(MESSAGE_START_CAPTURE);
              break;
              if (++count % 6 == 0)
              {
                  m_process->start("/home/haier/wireshark_hxb/tshark", args);
                  break;
              }
          }

          start = QDateTime::currentDateTime();
          m_packets_sender->start();
      }

      delete tmp;
  }
 void CModel::stop_capture ()
 {
     //m_start_thread->quit ();
     std::cout << "stop_capture\n";
    QMutexLocker locker (&m_start_stop_mutex);
    BasicInfo baseinfo;

     end = QDateTime::currentDateTime();

     m_base->getBasic(&baseinfo);

     QString q_time =  start.toString("yyyy-MM-dd hh:mm:ss") + "-" + end.toString("yyyy-MM-dd hh:mm:ss");
     baseinfo.time = q_time.toStdString ();

     baseinfo.sniffer_id = "1000";
     baseinfo.user_id = "admin";
     baseinfo.pf = "m6a801";
     baseinfo.report_id += baseinfo.sniffer_id;
     baseinfo.report_id += start.toString("yyyyMMddhhmmss").toStdString();

     set_basic_info(baseinfo);

     //((CSimPacketListData *)m_test_thread)->stop();
     m_cmd_sender->send(MESSAGE_STOP_CAPTURE);
     //m_process->kill ();
     //delete m_process;
     //m_process = new QProcess ();

     ReportStatInfo stat_info;
     m_statistics_thread->get_report_statistics(stat_info);
     stat_info.start = start;
     stat_info.end = end;
     std::cout << stat_info;
     set_report_statistics(stat_info);

     m_ctrl->grab_pie_wave();
     QTextDocument  document;
     construct_report_document(document,baseinfo, stat_info,"/tmp/wave.jpg",
		     "/tmp/filtered_wave.jpg", "/tmp/pie.jpg", "/tmp/filtered_pie.jpg");
     std::cout << "send_report begin ()\n";
	send_report();
    std::cout << "send_report end ()\n";

    QElapsedTimer t;
    t.start();
    while(t.elapsed() < 1000)
    {
        ;
    }
     m_packets_sender->stop();
 }
  void CModel::add_packet (const Packet_Record & packet_record)
  {
      m_ctrl->add_packet(packet_record);
  }
  void CModel::add_packet_count (int count)
  {
      m_ctrl->add_packet_count (count);
  }
   void CModel::set_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics)
   {
       m_ctrl->set_ip_statistics (ip_statistics);
   }
  void   CModel::set_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics)
  {
      m_ctrl->set_protocol_statistics (protocol_statistics);
  }
  void   CModel::set_frenquency (const std::list <HostFrenquency> & host_frenquency)

  {
      m_ctrl->set_frenquency (host_frenquency);
  }
  void CModel::set_flow_pie_data (const std::list <FlowPieData> & host_frenquency)
  {
      m_ctrl->set_flow_pie_data (host_frenquency);
  }
   void CModel::set_filtered_ip_statistics (const std::list <IpStatisticsRecord> & ip_statistics)
  {
      m_ctrl->set_filtered_ip_statistics (ip_statistics);
  }
  void  CModel::set_filtered_protocol_statistics (const std::list <ProtocolStatistics> & protocol_statistics)
  {
      m_ctrl->set_filtered_protocol_statistics (protocol_statistics);
  }
  void  CModel::set_filtered_frenquency (const std::list <HostFrenquency> & host_frenquency)
  {
      m_ctrl->set_filtered_frenquency (host_frenquency);
  }
  void CModel::set_filtered_flow_pie_data (const std::list <FlowPieData> & flow_pie_data)
  {
      m_ctrl->set_filtered_flow_pie_data (flow_pie_data);
  }

  void  CModel::set_basic_info (const BasicInfo & basic_info)
  {
      m_ctrl->set_basic_info (basic_info);
  }
  void CModel::set_filter (const std::list <IpFilterInfo> & ip_filter_info)
  {
      m_ip_filter->set_filter(ip_filter_info);
  }
   void CModel::set_wave_statistics (const std::list <WaveData> & wave_statistics)
  {
      m_ctrl->set_wave_statistics(wave_statistics);
  }
   void CModel::set_filtered_wave_statistics (const std::list <WaveData> & wave_statistics)
  {
      m_ctrl->set_filtered_wave_statistics(wave_statistics);
  }
   void CModel::set_report_statistics (const ReportStatInfo & report_stat_info)
   {
       m_ctrl->set_report_statistics(report_stat_info);
   }
void CModel::send_report()
	{
    //char * string;
    PyObject *pModule, *pDict, *pFunc, *pArgs, *pRetVal;

	char buf[64];
	if (!getcwd(buf, sizeof(buf)))
		return ;

	chdir("/home/haier/tv_sniffer/script");

    Py_Initialize();
    if ( !Py_IsInitialized() ){
		chdir(buf);
        return;
	}

    /*chdir("/mnt/hgfs/D/code/linux-virtual/myproj/python/");*/
    /*PyRun_SimpleString("execfile('pytest.py')");*/
    /*return 0;*/
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
	try {
    pModule = PyImport_ImportModule("semail");
    /*pName = PyString_FromString("pytest");  */
    /*pModule = PyImport_Import(pName);  */
    if ( !pModule )
    {
		chdir(buf);
        printf("can't find semail.py\n");
        //getchar();
        return;
    }
    pDict = PyModule_GetDict(pModule);
    if ( !pDict )
    {
		chdir(buf);
        return;
    }

    pFunc = PyDict_GetItemString(pDict, "main");
    if ( !pFunc || !PyCallable_Check(pFunc) )
    {
        printf("can't find function [add]\n");
        //getchar();
		chdir(buf);
        return;
    }

    pArgs = PyTuple_New(1);

    PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", "/tmp/hello.pdf"));
    /*PyTuple_SetItem(pArgs, 1, Py_BuildValue("i",4));*/

    pRetVal = PyObject_CallObject(pFunc, pArgs);
    printf("function return value : %ld\r\n", PyInt_AsLong(pRetVal));

    /*Py_DECREF(pName);  */
    if (pArgs)
    	Py_DECREF(pArgs);

    if (pModule)
    	Py_DECREF(pModule);
    if (pRetVal)
    	Py_DECREF(pRetVal);

    Py_Finalize();
	}catch (...){
		printf("send mail failed\n");
	}

	chdir(buf);
	}


void CModel::construct_report_document (
                QTextDocument & document,
                const BasicInfo & basic_info,
                const ReportStatInfo & report_stat_info,
                const std::string & wave_file,
                const std::string & filtered_wave_file,
                const std::string & pie_file,
                const std::string & filtered_pie_file)
{
        QTextCursor cursor (&document);
        //ursor.insertText(QString::fromLocal8Bit("统计报告\n"));
        //QString str;
        //str.clear();
        //str.append("统计报告\n\n");
        //str.resize()

        QFont font("Times", 22, QFont::Bold);
        QTextImageFormat format;
        format.setFont(font);

        cursor.insertText(QString("统计报告\n\n"), format);
        //cursor.clearSelection();
        font.setPointSize(16);
        format.setFont(font);
        cursor.insertText(QString("基本信息\n"), format);

        append_basic_info(cursor, basic_info);

        fprintf(stdout, "---------------basic\n");
        cursor.insertText(QString("\n\n所有ip统计\n"), format);
        cursor.insertText(QString("1.1 协议统计\n"), format);
        //append_protocol_statistic_info (cursor, report_stat_info.protocol_statistics);
        append_protocol_statistic_info (&document, report_stat_info.protocol_statistics);

        fprintf(stdout, "---------------xieyitongji\n");
        cursor.insertText(QString("\n\n1.2 频率图\n"), format);
        //append_wave_statistic_info (cursor, wave_file);
        append_wave_statistic_info (&document, wave_file);

        fprintf(stdout, "---------------wave\n");
        cursor.insertText(QString("\n\n1.3 流量饼图\n"), format);
        append_flow_pie_statistic_info (cursor, pie_file);

        fprintf(stdout, "---------------pie\n");
        cursor.insertText(QString("\n\n1.4 对不同ip的访问频次\n"), format);
        //append_frenquency_statistic_info (cursor, report_stat_info.host_frenquency);
        append_frenquency_statistic_info (&document, report_stat_info.host_frenquency);

        fprintf(stdout, "---------------pinci\n");
        cursor.insertText(QString("\n\n1.5 host/packets/size列表\n"), format);
        //append_ip_statistic_info (cursor, report_stat_info.ip_statistics);
        append_ip_statistic_info (&document, report_stat_info.ip_statistics);
	    fprintf(stdout, "---------------lie biao\n");
        cursor.insertText(QString("\n\n1.6 dns信息\n"), format);
        //append_dns_statistic_info (cursor, report_stat_info.dns_statistics);
        append_dns_statistic_info (&document, report_stat_info.dns_statistics);

        fprintf(stdout, "---------------dns\n");
        cursor.insertText(QString("\n\n被关注的ip统计\n\n"), format);
        cursor.insertText(QString("1.1 协议统计\n"), format);
        //append_protocol_statistic_info (cursor, report_stat_info.filtered_protocol_statistics);
        append_protocol_statistic_info (&document, report_stat_info.filtered_protocol_statistics);

        //cursor.movePosition(QTextCursor::End);
        fprintf(stdout, "---------------filter\n");
        cursor.insertText(QString("\n\n1.2 频率图\n"), format);
        //append_wave_statistic_info (cursor, filtered_wave_file);
        append_wave_statistic_info (&document, filtered_wave_file);

        cursor.insertText(QString("\n\n1.3 流量饼图\n"), format);
        append_flow_pie_statistic_info (cursor, filtered_pie_file);

        cursor.insertText(QString("\n\n1.4 对不同ip的访问频次\n"), format);
        //append_frenquency_statistic_info (cursor, report_stat_info.host_frenquency);
        append_frenquency_statistic_info (&document, report_stat_info.filtered_host_frenquency);

        cursor.insertText(QString("\n\n1.5 host/packets/size列表\n"), format);
        //append_ip_statistic_info (cursor, report_stat_info.filtered_ip_statistics);
        append_ip_statistic_info (&document, report_stat_info.filtered_ip_statistics);

        cursor.insertText(QString("\n\n1.6 dns信息\n"), format);
        //append_dns_statistic_info (cursor, report_stat_info.filtered_dns_statistics);
        append_dns_statistic_info (&document, report_stat_info.filtered_dns_statistics);


        QPrinter printer(QPrinter::HighResolution);

    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("/tmp/hello.pdf");//filename表示导出文件的文件名（包含路径）
  document.print(&printer);

}



int CModel::append_basic_info (QTextCursor & cursor,
		const BasicInfo & basic_info)
{
        QString str;

        QFont font;
        //font.setPointSize(12);
        QTextImageFormat format;
        format.setFont(font);

        str.clear();
        //cursor.clearSelection();
        //cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
        str =  "测试时间:       ";
        str.append(basic_info.time.c_str());
        str.append("\n");
        cursor.insertText(str, format);

        str.clear();
        //cursor.clearSelection();
        //cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
        str =  "SNIFFER ID:     ";
        str.append(basic_info.sniffer_id.c_str());
        str.append("\n");
        cursor.insertText(str, format);


        str.clear();
        //cursor.clearSelection();
        //cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
        str =  "USERID: ";
        str.append(basic_info.user_id.c_str());
        str.append("\n");
        cursor.insertText(str, format);

        str.clear();
        //cursor.clearSelection();
        //cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
        str =  "报告id: ";
        str.append(basic_info.report_id.c_str());
        str.append("\n");
        cursor.insertText(str, format);
#if 0
        str.clear();
        //cursor.clearSelection();
        //cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
        str =  "TV信息: ";
        str.append("\n");
        cursor.insertText(str, format);

        str.clear();
        //cursor.clearSelection();
        //cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
        str =  "MAC:    ";
        //str.append(basic_info.tv_Mac.c_str());
        str.append("\n");
        cursor.insertText(str, format);


        str.clear();
        //cursor.clearSelection();
        //cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
        str =  "IP:     ";
        //str.append(basic_info.tv_ip.c_str ());
        str.append("\n");
        cursor.insertText(str, format);
#endif
        return 0;
}


int CModel::append_protocol_statistic_info (QTextDocument * doc,
		const std::list <ProtocolStatistics> & protocol_statistics)
  {
      QTextTableFormat tableFormat;
      tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
      tableFormat.setBorderBrush(QBrush(Qt::black));
      tableFormat.setCellPadding(1);
      tableFormat.setBorder(2);
      tableFormat.setCellSpacing(5);
      tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 80));
      tableFormat.setAlignment(Qt::AlignLeft|Qt::AlignBottom);

	QTextCursor cursor (doc);
	cursor.movePosition(QTextCursor::End);
	QTextTable * table = cursor.insertTable (protocol_statistics.size(), 2, tableFormat);
	std::list <ProtocolStatistics>::const_iterator cit
		= protocol_statistics.begin();
	int row = 0;
	for ( ; cit != protocol_statistics.end (); ++cit)
	{
		{
			QTextTableCell  cell = table->cellAt (row, 0);
			QTextCursor cursor = cell.firstCursorPosition ();
			cursor.insertText(cit->protocol.c_str ());
		}
		{
			QTextTableCell  cell = table->cellAt (row, 1);
			QTextCursor cursor = cell.firstCursorPosition ();
			char str_num[24] = {0};
			sprintf(str_num, "%d", cit->count);
			cursor.insertText(str_num);
		}

		++row;
	}

	return 0;
}

int CModel::append_wave_statistic_info (QTextDocument * doc, const std::string & file)
{

        QUrl Uri(QString("file://%1").arg (file.c_str()));
        QImage image = QImageReader(file.c_str()).read();

        QTextCursor cursor(doc);
        cursor.movePosition(QTextCursor::End);

        doc->addResource(QTextDocument::ImageResource, Uri, QVariant(image));
        QTextImageFormat imageFormat;
       imageFormat.setWidth(600);
       //printf("11111111image.width():%d\n", image.width());
				//imageFormat.setWidth(image.width());
        imageFormat.setHeight( image.height() );

        //imageFormat.setWidth(doc->pageSize().width());

        imageFormat.setName( Uri.toString() );
        cursor.insertImage(imageFormat);

        return 0;
}


int CModel::append_flow_pie_statistic_info (QTextCursor & cursor, const std::string & file)
{
  //QImage image = QImageReader(file.c_str()).read();
        //QUrl Uri(QString("file://%1").arg (file.c_str()));

        //cursor.movePosition(QTextCursor::End);

        //doc->addResource(QTextDocument::ImageResource, Uri, QVariant(image));
        //QTextImageFormat imageFormat;
        //imageFormat.setWidth( image.width() );
        //imageFormat.setHeight( image.height() );
        //imageFormat.setName( Uri.toString() );

        QImage image;
        image.load(QString::fromStdString(file));
        cursor.insertImage(image);

        return 0;
}


int CModel::append_frenquency_statistic_info (QTextDocument * doc,
		const std::list <HostFrenquency> & frenquency_statistics)
{
  QTextTableFormat tableFormat;
  tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  tableFormat.setBorderBrush(QBrush(Qt::black));
  tableFormat.setCellPadding(1);
  tableFormat.setBorder(2);
  tableFormat.setCellSpacing(5);
  tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 80));
  tableFormat.setAlignment(Qt::AlignLeft|Qt::AlignBottom);


  std::list <HostFrenquency>::const_iterator cit = frenquency_statistics.begin();
        if(cit == frenquency_statistics.end())
        {
                return 0;
        }
        int column_num = cit->count.size();

        QTextCursor cursor (doc);
        cursor.movePosition(QTextCursor::End);
        QTextTable * table = cursor.insertTable (frenquency_statistics.size(), column_num+1, tableFormat);
        int row = 0;
        for ( ; cit != frenquency_statistics.end (); ++cit)
        {

                {
                QTextTableCell  cell = table->cellAt (row, 0);
                QTextCursor cursor = cell.firstCursorPosition ();
                cursor.insertText(cit->host.c_str ());
                }
                std::list<int>::const_iterator it = cit->count.begin();
                for(int i=1; it != cit->count.end(); it++,i++)
                {
                        //fprintf(stdout, "i=%d, size=%d\n", i, cit->count.size());
                        {
						if(i == column_num+1) break;
                        QTextTableCell  cell = table->cellAt (row, i);
                        QTextCursor cursor = cell.firstCursorPosition ();
                        char str_num[24] = {0};
                        sprintf(str_num, "%d", *it);
                        cursor.insertText(str_num);
                        }

                }

                ++row;
        }

        return 0;
}


int CModel::append_ip_statistic_info (QTextDocument * doc,
		const std::list <IpStatisticsRecord> & ip_statistics)
{
  QTextTableFormat tableFormat;
  tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  tableFormat.setBorderBrush(QBrush(Qt::black));
  tableFormat.setCellPadding(1);
  tableFormat.setBorder(2);
  tableFormat.setCellSpacing(5);
  tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 80));
  tableFormat.setAlignment(Qt::AlignLeft|Qt::AlignBottom);


  QTextCursor cursor (doc);
        cursor.movePosition(QTextCursor::End);
        QTextTable * table = cursor.insertTable (ip_statistics.size(), 3, tableFormat);
        std::list <IpStatisticsRecord>::const_iterator cit
                = ip_statistics.begin();
        int row = 0;
        for ( ; cit != ip_statistics.end (); ++cit)
        {
                {
                        QTextTableCell  cell = table->cellAt (row, 0);
                        QTextCursor cursor = cell.firstCursorPosition ();
                        cursor.insertText(cit->ip.c_str ());
                }
                {
                        QTextTableCell  cell = table->cellAt (row, 1);
                        QTextCursor cursor = cell.firstCursorPosition ();
                        cursor.insertText(cit->packets.c_str ());
                }

                {
                        QTextTableCell  cell = table->cellAt (row, 2);
                        QTextCursor cursor = cell.firstCursorPosition ();
                        cursor.insertText(cit->bytes.c_str ());
                }
                ++row;
        }

        return 0;

}


int CModel::append_dns_statistic_info (QTextDocument * doc,
		const std::list <DnsInfo> & frenquency_statistics)
{
  QTextTableFormat tableFormat;
  tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
  tableFormat.setBorderBrush(QBrush(Qt::black));
  tableFormat.setCellPadding(1);
  tableFormat.setBorder(2);
  tableFormat.setCellSpacing(5);
  tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 80));
  tableFormat.setAlignment(Qt::AlignLeft|Qt::AlignBottom);


	QTextCursor cursor (doc);
        cursor.movePosition(QTextCursor::End);
        QTextTable * table = cursor.insertTable (frenquency_statistics.size(), 2, tableFormat);
	std::list <DnsInfo>::const_iterator cit
		= frenquency_statistics.begin();
	int row = 0;
	for ( ; cit != frenquency_statistics.end (); ++cit)
	{
		{
			QTextTableCell  cell = table->cellAt (row, 0);
			QTextCursor cursor = cell.firstCursorPosition ();
			cursor.insertText(cit->domain_name.c_str ());
		}
		{
			QTextTableCell  cell = table->cellAt (row, 1);
			QTextCursor cursor = cell.firstCursorPosition ();
			cursor.insertText(cit->ip.c_str ());
		}

		++row;
	}
}

 bool CModel::save_filter_record_to_file(const std::vector<FilterRecord> & v_filter_record)
   {
       return m_filter_file->save_filter_record_to_file (v_filter_record);
   }
   bool CModel::get_filter_record_from_file (std::vector<FilterRecord> & v_filter_record)
   {
       std::cout << "CModel::get_filter_record_from_file\n";
       return m_filter_file->get_filter_record_from_file(v_filter_record);
   }
