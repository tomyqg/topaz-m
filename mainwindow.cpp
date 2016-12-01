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
//    QPixmap pix1("C:/Users/aashmele/untitled2/logo.jpg");
//    ui->label->setPixmap(pix1);


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCaption()));

    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(updatepicture()));

    timer->start(1000);
    timer2->start(25);

    QVector<double> x(size(),y(size();


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
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    /*ui->customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-1, 1);
    customPlot->yAxis->setRange(0, 1);
    customPlot->replot();*/
}
