#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QPen>
#include <QVector>
#include <QtMath>


int a=0;
int b=0;

QVector<double> xx1;
QVector<double> yy1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pix("/usr/inc/logo.jpg");
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCaption()));

    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(updatepicture()));

    QTimer *timer3 = new QTimer(this);
    connect(timer3, SIGNAL(timeout()), this, SLOT(updategraph()));

    timer->start(1000);
    timer2->start(20);
    timer3->start(100);

    int n = 50; // number of points in graph
    double xScale = (rand()/(double)RAND_MAX + 0.5)*2;
    double yScale = (rand()/(double)RAND_MAX + 0.5)*2;
    double xOffset = (rand()/(double)RAND_MAX - 0.5)*4;
    double yOffset = (rand()/(double)RAND_MAX - 0.5)*5;
    double r1 = (rand()/(double)RAND_MAX - 0.5)*2;
    double r2 = (rand()/(double)RAND_MAX - 0.5)*2;
    double r3 = (rand()/(double)RAND_MAX - 0.5)*2;
    double r4 = (rand()/(double)RAND_MAX - 0.5)*2;
    QVector<double> x(n), y(n);
    for (int i=0; i<n; i++)
    {
        x[i] = (i/(double)n-0.5)*10.0*xScale + xOffset;
        y[i] = (qSin(x[i]*r1*5)*qSin(qCos(x[i]*r2)*r4*3)+r3*qCos(qSin(x[i])*r4*2))*yScale + yOffset;
    }

    ui->customPlot->xAxis->setRange(-8, 200);
    ui->customPlot->yAxis->setRange(-5, 100);

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName(QString("New graph %1").arg(ui->customPlot->graphCount()-1));
    ui->customPlot->graph()->setData(x, y);
    ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(rand()%5+1));
    if (rand()%100 > 50)
        ui->customPlot->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(rand()%14+1)));
    QPen graphPen;
    graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->replot();

    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_dial_sliderMoved(int position)
{
    ui->lcdNumber->display(ui->dial->value());

}

void MainWindow::on_lcdNumber_overflow()
{

}

void MainWindow::on_dial_actionTriggered(int action)
{

}

void MainWindow::updateCaption()
{
    QDateTime local(QDateTime::currentDateTime());
    ui->textEdit_2->setText(local.toString());

}


void MainWindow::updatepicture()
{
    //QPixmap ship("C:/Users/aashmele/untitled2/logo.jpg");

    QPixmap ship("/usr/inc/logo.jpg");
    QPixmap rotate(ship.size()) ;
    QPainter p(&rotate);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    p.translate(rotate.size().width()/2, rotate.size().height()/2);
    p.rotate(a+(ui->dial->value()-45)*2);
    p.translate(-rotate.size().width()/2, -rotate.size().height()/2);
    p.drawPixmap(0,0,ship);
    p.end();
    ui->label->setPixmap(rotate);
    a++;

}

void MainWindow::updategraph()
{

    xx1.append(b);
    yy1.append(ui->dial_2->value());


    ui->customPlot->addGraph();
    ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(1));
    ui->customPlot->graph()->setData(xx1, yy1);

    ui->customPlot->replot();
    ///////////////
    b++;


    b++;
    if (b==200)
    {
        b=0;
        xx1.clear();
        yy1.clear();
        ui->customPlot->clearGraphs();
    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{/*
    QPainter painter(this);

    QPen pen1(Qt::black);
    pen1.setWidth(6);

painter.rotate(-45);
    QRect rec(20,60,20,60);
    painter.setPen(pen1);
    painter.drawRect(rec);*/

}

void MainWindow::on_pushButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_pushButton_2_clicked()
{


    ui->customPlot->clearGraphs();

    int n = 50; // number of points in graph
    double xScale = (rand()/(double)RAND_MAX + 0.5)*2;
    double yScale = (rand()/(double)RAND_MAX + 0.5)*2;
    double xOffset = (rand()/(double)RAND_MAX - 0.5)*4;
    double yOffset = (rand()/(double)RAND_MAX - 0.5)*5;
    double r1 = (rand()/(double)RAND_MAX - 0.5)*2;
    double r2 = (rand()/(double)RAND_MAX - 0.5)*2;
    double r3 = (rand()/(double)RAND_MAX - 0.5)*2;
    double r4 = (rand()/(double)RAND_MAX - 0.5)*2;
    QVector<double> x(n), y(n);
    for (int i=0; i<n; i++)
    {
        x[i] = (i/(double)n-0.5)*10.0*xScale + xOffset;
        y[i] = (qSin(x[i]*r1*5)*qSin(qCos(x[i]*r2)*r4*3)+r3*qCos(qSin(x[i])*r4*2))*yScale + yOffset;
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName(QString("New graph %1").arg(ui->customPlot->graphCount()-1));
    ui->customPlot->graph()->setData(x, y);
    ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(rand()%5+1));
    if (rand()%100 > 50)
        ui->customPlot->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(rand()%14+1)));
    QPen graphPen;
    graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
    graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->replot();




    ///////////////
    QVector<double> x1(200), y1(200);
    QVector<double> x2(201), y2(201);
    //int tre=a%100;

    for (int i=0; i<201; i++)
    {
        x2[i] = i ; // x goes from -1 to 1
        y2[i] = x2[i]/2; // let's plot a quadratic function
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setLineStyle((QCPGraph::LineStyle)(1));
    //    ui->customPlot->graph()->setData({10,20,22,25,27,28,29,30,31,90}, {50,15,30,20,10,90,80,70,60,50});
    //    ui->customPlot->graph()->setData({10,20,30,40,50,60,70,80,90,100},y1);// {50,15,30,20,10,90,80,70,60,50});
    ui->customPlot->graph()->setData(x2, y2);
    //ui->customPlot->graph()->addData(1,10);

    ui->customPlot->replot();
    ///////////////
    b++;
    if(b==200)
    {
        b=0;
        ui->customPlot->clearGraphs();

    }
}

void MainWindow::on_pushButton_2_pressed()
{
}
