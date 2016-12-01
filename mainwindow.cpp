#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QPen>
#include <QtMath>


int a=0;

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

    timer->start(1000);
    timer2->start(25);

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


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_dial_sliderMoved(int position)
{
    ui->lcdNumber->display(ui->dial->value());
/*
    QPixmap ship("C:/Users/aashmele/untitled2/logo.jpg");

    QPixmap rotate(ship.size()) ;
    QPainter p(&rotate);
    p.translate(rotate.size().width()/2, rotate.size().height()/2);
    p.rotate();
    p.translate(-rotate.size().width()/2, -rotate.size().height()/2);
    p.drawPixmap(0,0,ship);
    p.end();
    ui->label->setPixmap(rotate);
    a++;*/


//ui->pushButton_2->setText("4");
//ui->label_2->setText("4");


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
}

void MainWindow::on_pushButton_2_pressed()
{
}
