#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCursor>
#include <QPrinter>
#include <QFile>
#include <QDebug>
#include <QTextDocument>
#include <QString>
#include <QTextImageFormat>
#include <QImage>
#include <QImageReader>
#include <QUrl>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QPrinter printer(QPrinter::HighResolution);

    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("hello.pdf");//filename表示导出文件的文件名（包含路径）
    QFile file("hello.pdf");
    if (!file.open(QFile::WriteOnly)){
    qDebug() << "error to open file";
    return;
    }
    
    QString file1("./hello.jpg");
    QImage image = QImageReader(file1).read();
    QUrl Uri(QString("file://%1").arg (file1));

    QTextDocument *test = new QTextDocument(this);
    QTextCursor cursor(test);
    cursor.movePosition(QTextCursor::End);

    test->addResource(QTextDocument::ImageResource, Uri, QVariant(image));
    QTextImageFormat imageFormat;
    imageFormat.setWidth( image.width() );
    imageFormat.setHeight( image.height() );
    imageFormat.setName( Uri.toString() );
    cursor.insertImage(imageFormat);

    //QTextImageFormat imageFormat;
    //imageFormat.setName("mydata://hello.jpg");
    //cursor.insertImage(imageFormat);

    //QFont Font;
    //Font.setFamily("宋体");
    //test->setDefaultFont(Font);
    //test->setHtml("hello world");
    test->print(&printer);
    file.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
