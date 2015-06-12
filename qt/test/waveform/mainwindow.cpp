#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPixmap>
#include <unistd.h>
#include <QPen>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setGeometry(400, 250, 542, 390);
    setGeometry(400, 250, 1366, 800);
    customPlot = new QCustomPlot();
    demoPlay();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete customPlot;
}

#if 0
void MainWindow::demoPlay()
{
    //demoName = "Quadratic Demo";
    // generate some data:
    this->setCentralWidget(customPlot);
    int n = 101;
    //double phase=0;
    double k = 5;
    srand(1);
    QVector<double> x(n), y(n); // initialize with entries 0..100
    for (int i=0; i<n; ++i)
    {
      x[i] = i/(double)(n - 1)*34; // x goes from -1 to 1
      //y[i] = 5*x[i]*x[i];  // let's plot a quadratic function
      //y[i] = qExp(-x[i]*x[i]/20.0)*qSin(k*x[i]+phase);  // let's plot a quadratic function
      k *= rand()%100;
          y[i] = qExp(-x[i]*x[i]/20.0)*qSin(k);  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    //Qpen pen;

   QVector<double> x1(10), y1(10);
   for (int k=0; k<10; ++k)
    {
      x[k] = k/10.0 * 4*3.14 + 0.01;
      y[k] = 7*sin(x[k])/x[k] + k*5;
    }

    customPlot->addGraph();
    customPlot->graph(0)->setAntialiasedFill(false);
    customPlot->graph(0)->setData(x, y);
    customPlot->graph(0)->setPen(QPen(Qt::green));
    customPlot->graph(0)->setName("192.168.1.2");
	
	customPlot->addGraph();
    customPlot->graph(1)->setData(x1, y1);
    customPlot->graph(1)->setPen(QPen(Qt::blue));
    customPlot->graph(1)->setName("192.168.1.3");
    customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));
	

    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, 30);
    customPlot->yAxis->setRange(-1, 1);

    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    //timer = new QTimer(this);
    //timer->singleShot(200, this, SLOT(shootScreen()));
}

#endif


void MainWindow::demoPlay()
{
	//demoName = "Date Demo";
    this->setCentralWidget(customPlot);
    //customPlot->show();
  // set locale to english, so we get english month names:
  customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
  // seconds of current time, we'll use it as starting point in time for data:
  double now = QDateTime::currentDateTime().toTime_t();
  srand(8); // set the random seed, so we always get the same random data
  // create multiple graphs:
  char buf[32];
  for (int gi=0; gi<5; ++gi)
  {
    customPlot->addGraph();
    //QPen pen;
    //pen.setColor(QColor(0, 0, 255, 200));
    customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    customPlot->graph()->setPen(QPen(QColor(255/4.0*gi,160,50,150)));
	sprintf(buf, "192.168.1.%d", gi+1);
     customPlot->graph()->setName(buf);
    //customPlot->graph()->setBrush(QBrush(QColor(255/4.0*gi,160,50,150)));
    // generate random walk data:
    QVector<double> time(250), value(250);
    for (int i=0; i<250; ++i)
    {
      time[i] = now + 24*3600*i;
      if (i == 0)
        value[i] = (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
      else
        value[i] = fabs(value[i-1])*(1+0.02/4.0*(4-gi)) + (i/50.0+1)*(rand()/(double)RAND_MAX-0.5);
    }
    customPlot->graph()->setData(time, value);
  }
  // configure bottom axis to show date and time instead of number:
  //customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  //customPlot->xAxis->setDateTimeFormat("MMMM\nyyyy");
  // set a more compact font size for bottom and left axis tick labels:
  //customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
  //customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
  // set a fixed tick-step to one tick per month:
  //customPlot->xAxis->setAutoTickStep(false);
  //customPlot->xAxis->setTickStep(2628000); // one month in seconds
  //customPlot->xAxis->setSubTickCount(3);
  // apply manual tick and tick label for left axis:
  customPlot->xAxis->setAutoTicks(true);
  customPlot->xAxis->setAutoTickLabels(true);
  customPlot->yAxis->setAutoTicks(true);
  customPlot->yAxis->setAutoTickLabels(true);
  //customPlot->yAxis->setTickVector(QVector<double>() << 5 << 55);
  //customPlot->yAxis->setTickVectorLabels(QVector<QString>() << "Not so\nhigh" << "Very\nhigh");
  // set axis labels:
  //customPlot->xAxis->setLabel("Date");
  //customPlot->yAxis->setLabel("Random wobbly lines value");
  // make top and right axes visible but without ticks and labels:
  //customPlot->xAxis2->setVisible(true);
  //customPlot->yAxis2->setVisible(true);
  customPlot->xAxis2->setTicks(false);
  customPlot->yAxis2->setTicks(false);
  customPlot->xAxis2->setTickLabels(false);
  customPlot->yAxis2->setTickLabels(false);
  // set axis ranges to show all data:
  customPlot->xAxis->setRange(now, now+24*3600*249);
  customPlot->yAxis->setRange(0, 60);
  // show legend:
  //customPlot->legend->setVisible(true);
	    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
	timer = new QTimer(this);
    timer->singleShot(200, this, SLOT(shootScreen()));
}

void MainWindow::shootScreen()
{
    
    QPixmap originalPixmap = QPixmap(); // clear image for low memory situations
                                // on embedded devices.
    //originalPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    //originalPixmap = QPixmap::grabWindow(QWidget::winId());
    originalPixmap = QPixmap::grabWindow(this->winId());

    originalPixmap.save(QString("/mnt/hgfs/D/code/linux-virtual/myproj/qt/test/screenshot1/hello.jpg"), "JPG");
}

