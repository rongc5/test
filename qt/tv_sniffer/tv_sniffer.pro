
CONFIG +=   debug
CONFIG -= debug_and_release
QT += widgets network printsupport core

TARGET = tvsniffer-alpha
DESTDIR  = bin
MOC_DIR  = tmp
RCC_DIR = tmp
OBJECTS_DIR += tmp
LIBS += -L/usr/local/log4cxx/lib -llog4cxx \
				-L/usr/lib/python2.7/config -lpython2.7
INCLUDEPATH  += src/ui/interface \
							src/ctrl/interface \
							src/model/interface \
							src/common \
							/usr/local/log4cxx/include

HEADERS += src/ui/source/main_window.h \
           src/ui/source/login.h \
           src/ui/source/check_password.h \
           src/ui/source/packet_list_model.h \
					 src/ctrl/source/ctrl.h \
					 src/ctrl/interface/ictrl.h \
					 src/model/interface/imodel.h \
					 src/model/source/model.h \
					 src/ui/interface/iview.h \
					 src/ui/source/view.h \
					 src/ui/source/histogram_widget.h \
					 src/ui/source/nightcharts.h \
					 src/ui/source/pie_chart_widget.h \
					 src/ui/source/ip_statistics_model.h \
					 src/ui/source/packet_list_view.h \
					 src/ui/source/config_filter_dlg.h \
					 src/ui/source/config_filter_model.h \
					 src/model/source/statistics_thread.h  \
					 src/model/source/message_receiver.h  \
					 src/model/source/cmd_sender.h  \
					 src/model/source/packets_sender.h  \
					 src/model/source/packets_cache.h \
					 src/ui/source/report_widget.h \
					 src/ui/source/filter_delegate.h \
					 src/ui/source/ipctrl.h \
					 src/ui/source/ip_frenquency_model.h \
					 src/ui/source/protocol_statistic_model.h \
					 src/model/source/ip_filter.h \
					  src/model/source/packets_cache.h \
					  src/ui/source/qcustomplot.h \
					  src/model/source/print.h  \
					   src/model/source/baseinfo_thread.h  \
					   src/ui/source/dns_info_model.h \
					   src/ui/source/start_button.h \
					   src/ui/source/start_page.h \
					   src/model/source/tvsnifferlog.h \
					   src/model/source/filter_file.h \
					   src/model/source/start_thread.h
					 					 
						
					 
					 
SOURCES = src/ui/source/main_window.cpp \
          src/ui/source/main.cpp \
          src/ui/source/login.cpp \
          src/ui/source/check_password.cpp \
          src/ui/source/packet_list_model.cpp \
          src/ctrl/source/ctrl.cpp \
          src/model/source/model.cpp \
          src/ui/source/view.cpp \
          src/ui/source/histogram_widget.cpp \
          src/ui/source/nightcharts.cpp \
          src/ui/source/pie_chart_widget.cpp \
          src/ui/source/ip_statistics_model.cpp \
          src/ui/source/packet_list_view.cpp \
          src/ui/source/config_filter_dlg.cpp \
          src/ui/source/config_filter_model.cpp \
          src/model/source/statistics_thread.cpp \
          src/model/source/message_receiver.cpp \
          src/model/source/cmd_sender.cpp \
          src/model/source/packets_sender.cpp  \
					src/ui/source/report_widget.cpp \
					src/ui/source/filter_delegate.cpp \
					src/ui/source/ipctrl.cpp \
					src/ui/source/ip_frenquency_model.cpp \
					src/ui/source/protocol_statistic_model.cpp \
					 src/model/source/ip_filter.cpp \
					 src/model/source/packets_cache.cpp \
					 src/ui/source/qcustomplot.cpp \
					 src/model/source/baseinfo_thread.cpp \
					 src/ui/source/dns_info_model.cpp \
					src/ui/source/start_button.cpp \
					 src/ui/source/start_page.cpp \
					 src/model/source/filter_file.cpp \
					 src/model/source/tvsnifferlog.cpp \
					 src/model/source/start_thread.cpp
          
RESOURCES += src/ui/resource/resource.qrc

UI_DIR = src/ui/forms/ui_dir
FORMS += src/ui/forms/main_window.ui \
         src/ui/forms/login_dialogue.ui \
         src/ui/forms/pie_chart_dlg.ui \
         src/ui/forms/ip_statistics_dlg.ui \
         src/ui/forms/config_filter.ui \
         src/ui/forms/report.ui \
         src/ui/forms/start_page.ui

