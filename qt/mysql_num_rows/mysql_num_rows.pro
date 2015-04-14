TEMPLATE = app
CONFIG += console
CONFIG -= qt

LIBS +=  -L /usr/lib/mysql/ -lmysqlclient


SOURCES += main.c

