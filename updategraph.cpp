#include "updategraph.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QPen>
#include <QVector>
#include <QtMath>
#include <QFile>
#include <QDataStream>
#include <QtMath>
#include <QtScript/QScriptEngine>
#include <QtSerialPort/QtSerialPort>
#include <QtScript/QScriptEngine>
#include <QPainterPath>
#include <QPainter>
#include <QDateTime>
#include <QFile>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QMutex>

int a=0;
int b=0;
int color1rgb[]={153, 0, 153}; //фиолетовый темный
int color2rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color3rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color4rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color5rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color6rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color7rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color8rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color9rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color10rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};

int color11rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color12rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color13rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color14rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color15rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color16rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color17rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color18rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color19rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};
int color20rgb[]={rand()%245+10, rand()%245+10, rand()%245+10};

QVector<double> xx1;
QVector<double> yy1;

QVector<double> xx2;
QVector<double> yy2;

QVector<double> xx3;
QVector<double> yy3;

QVector<double> xx4;
QVector<double> yy4;

QVector<double> xx5;
QVector<double> yy5;

QVector<double> xx6;
QVector<double> yy6;

QVector<double> xx7;
QVector<double> yy7;

QVector<double> xx8;
QVector<double> yy8;

QVector<double> xx9;
QVector<double> yy9;

QVector<double> xx10;
QVector<double> yy10;

QVector<double> xx11;
QVector<double> yy11;

QVector<double> xx12;
QVector<double> yy12;

QVector<double> xx13;
QVector<double> yy13;

QVector<double> xx14;
QVector<double> yy14;

QVector<double> xx15;
QVector<double> yy15;

QVector<double> xx16;
QVector<double> yy16;

QVector<double> xx17;
QVector<double> yy17;

QVector<double> xx18;
QVector<double> yy18;

QVector<double> xx19;
QVector<double> yy19;

QVector<double> xx20;
QVector<double> yy20;


void MainWindow::updategraph()
{
    int countgraph = ui->horizontalSlider->value();
    
    xx1.append(b);
    if(countgraph>=2) xx2.append(b);
    if(countgraph>=3) xx3.append(b);
    if(countgraph>=4) xx4.append(b);
    if(countgraph>=5) xx5.append(b);
    if(countgraph>=6) xx6.append(b);
    if(countgraph>=7) xx7.append(b);
    if(countgraph>=8) xx8.append(b);
    if(countgraph>=9) xx9.append(b);
    if(countgraph>=10) xx10.append(b);
    
    if(countgraph>=11) xx11.append(b);
    if(countgraph>=12) xx12.append(b);
    if(countgraph>=13) xx13.append(b);
    if(countgraph>=14) xx14.append(b);
    if(countgraph>=15) xx15.append(b);
    if(countgraph>=16) xx16.append(b);
    if(countgraph>=17) xx17.append(b);
    if(countgraph>=18) xx18.append(b);
    if(countgraph>=19) xx19.append(b);
    if(countgraph>=20) xx20.append(b);
    
    double argument = ui->dial->value();
    
    yy1.append(returnmathresult(argument)*1);
    
    if(countgraph>=2)
        yy2.append(returnmathresult(argument)*1+5);
    if(countgraph>=3)
        yy3.append(returnmathresult(argument)*1+10);
    if(countgraph>=4)
        yy4.append(returnmathresult(argument)*1+15);
    if(countgraph>=5)
        yy5.append(returnmathresult(argument)*1+20);
    if(countgraph>=6)
        yy6.append(returnmathresult(argument)*1+25);
    if(countgraph>=7)
        yy7.append(returnmathresult(argument)*1+30);
    if(countgraph>=8)
        yy8.append(returnmathresult(argument)*1+35);
    if(countgraph>=9)
        yy9.append(returnmathresult(argument)*1+40);
    if(countgraph>=10)
        yy10.append(returnmathresult(argument)*1+45);
    if(countgraph>=11)
        yy11.append(returnmathresult(argument)*1+50);
    if(countgraph>=12)
        yy12.append(returnmathresult(argument)*1+55);
    
    if(countgraph>=13) yy13.append(returnmathresult(argument)*1+60);
    if(countgraph>=14) yy14.append(returnmathresult(argument)*1+65);
    if(countgraph>=15) yy15.append(returnmathresult(argument)*1+70);
    if(countgraph>=16) yy16.append(returnmathresult(argument)*1+75);
    if(countgraph>=17) yy17.append(returnmathresult(argument)*1+80);
    if(countgraph>=18) yy18.append(returnmathresult(argument)*1+85);
    if(countgraph>=19) yy19.append(returnmathresult(argument)*1+90);
    if(countgraph>=20) yy20.append(returnmathresult(argument)*1+95);
    
    ///////////////
    b++;
    
    if (b>=300&&b<900)
    {
        ui->customPlot->xAxis->setRange(b-300, b+300);
    }
    
    if (b==1200)
    {
        /* ui->customPlot->xAxis->setRange(0, 600);
        
        b=0;
       xx1.clear();
        xx2.clear();
        xx3.clear();
        xx4.clear();
        xx5.clear();
        xx6.clear();
        xx7.clear();
        xx8.clear();
        xx9.clear();
        xx10.clear();
        
        xx11.clear();
        xx12.clear();
        xx13.clear();
        xx14.clear();
        xx15.clear();
        xx16.clear();
        xx17.clear();
        xx18.clear();
        xx19.clear();
        xx20.clear();
        
        yy1.clear();
        yy2.clear();
        yy3.clear();
        yy4.clear();
        yy5.clear();
        yy6.clear();
        yy7.clear();
        yy8.clear();
        yy9.clear();
        yy10.clear();
        yy11.clear();
        yy12.clear();
        yy13.clear();
        yy14.clear();
        yy15.clear();
        yy16.clear();
        yy17.clear();
        yy18.clear();
        yy19.clear();
        yy20.clear();*/
    }
}

void MainWindow::updatepicture()
{
    
    QByteArray arr;
    
    QPen graphPen;

    ui->customPlot->clearGraphs();
    
    int countgraph = ui->horizontalSlider->value();
    
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setName("graph #1");
    ui->customPlot->graph()->setData(xx1, yy1);
    
    graphPen.setColor(QColor(color1rgb[0],color1rgb[1],color1rgb[2]));
    ui->customPlot->graph()->setPen(graphPen);
    ui->customPlot->addGraph();
    
    if(countgraph>=2){ui->customPlot->graph()->setData(xx2, yy2);
        graphPen.setColor(QColor(color2rgb[0],color2rgb[1],color2rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    
    if(countgraph>=3){ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx3, yy3);
        graphPen.setColor(QColor(color3rgb[0],color3rgb[1],color3rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);}
    
    if(countgraph>=4){ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx4, yy4);
        graphPen.setColor(QColor(color4rgb[0],color4rgb[1],color4rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    
    if(countgraph>=5){ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx5, yy5);
        graphPen.setColor(QColor(color5rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    
    if(countgraph>=6){
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx6, yy6);
        graphPen.setColor(QColor(color6rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    
    if(countgraph>=7){
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx7, yy7);
        graphPen.setColor(QColor(color7rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    
    if(countgraph>=8){
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx8, yy8);
        graphPen.setColor(QColor(color8rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    
    
    if(countgraph>=9){
        ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx9, yy9);
        graphPen.setColor(QColor(color9rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);}
    
    if(countgraph>=10){   ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx10, yy10);
        graphPen.setColor(QColor(color10rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    
    if(countgraph>=11){   ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx11, yy11);
        graphPen.setColor(QColor(color11rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    
    if(countgraph>=12){   ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx12, yy12);
        graphPen.setColor(QColor(color12rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    
    if(countgraph>=13){   ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx13, yy13);
        graphPen.setColor(QColor(color13rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    if(countgraph>=14){    ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx14, yy14);
        graphPen.setColor(QColor(color14rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    
    if(countgraph>=15){   ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx15, yy15);
        graphPen.setColor(QColor(color15rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    
    if(countgraph>=16){   ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx16, yy16);
        graphPen.setColor(QColor(color16rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    
    if(countgraph>=17){    ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx17, yy17);
        graphPen.setColor(QColor(color17rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    
    if(countgraph>=18){   ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx18, yy18);
        graphPen.setColor(QColor(color18rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    if(countgraph>=19)
    {   ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx19, yy19);
        graphPen.setColor(QColor(color19rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    if(countgraph>=20)
    {   ui->customPlot->addGraph();
        ui->customPlot->graph()->setData(xx20, yy20);
        graphPen.setColor(QColor(color20rgb[0],color5rgb[1],color5rgb[2]));
        ui->customPlot->graph()->setPen(graphPen);
    }
    ui->customPlot->replot();
}

void MainWindow::mousePress()
{
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->xAxis->orientation());
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis->orientation());
}



void MainWindow::WriteArchiveToFile()
{

    QJsonObject archivechannel1;
    QJsonObject archivechannel2;
    QJsonObject archivechannel3;
    QJsonObject archivechannel4;


    QJsonObject archive;

    QJsonArray archives;

    QJsonArray array;

    QJsonArray valuesarray;

    for(int y=0; y<yy1.size(); y++)
    {
        //qDebug()<< yy1.at(y);
        valuesarray.append(yy1.at(y)*0.11);
    }

    //    qDebug()<< settings;

    int a;
    QDateTime end(QDateTime::currentDateTime());

    archivechannel1["arraysize"] = valuesarray.size();
    archivechannel1["values"] = valuesarray;
    archivechannel1["channelname"] = "channel_1";



    archivechannel2["arraysize"] = valuesarray.size();
    archivechannel2["values"] = valuesarray;
    archivechannel2["channelname"] = "channel_2";

    archivechannel3["arraysize"] = valuesarray.size();
    archivechannel3["values"] = valuesarray;
    archivechannel3["channelname"] = "channel_3";

    archivechannel4["arraysize"] = valuesarray.size();
    archivechannel4["values"] = valuesarray;
    archivechannel4["channelname"] = "channel_4";

    archives.append(archivechannel1);
    archives.append(archivechannel2);
    archives.append(archivechannel3);
    archives.append(archivechannel4);

    archive["archives"] = archives;
    archive["StartTime"] = MainWindow::starttime;
    archive["EndTime"] = end.toString();

    QString setstr = QJsonDocument(archive).toJson(QJsonDocument::Compact);

    //qDebug() << setstr;

    QFile file("C:/Work/archive.txt");

    file.open(QIODevice::ReadWrite);

    file.resize(0); // clear file

    QTextStream out(&file);

    //    ui->textEdit_3->setText(setstr);

    out << setstr;

    file.close();
}
