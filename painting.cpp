#include "updategraph.h"
#include "mainwindow.h"
#include "uartdriver.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "metrologicalcalc.h"
#include "mathresolver.h"
#include "options.h"
#include "defines.h"
#include "stackedoptions.h"

extern QColor Channel1Color;
extern QColor Channel2Color;
extern QColor Channel3Color;
extern QColor Channel4Color;
extern QColor Channel1ColorNormal;
extern QColor Channel2ColorNormal ;
extern QColor Channel3ColorNormal;
extern QColor Channel4ColorNormal ;
extern QColor ChannelColorHighState;
extern QColor ChannelColorLowState;
extern QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;

QString Codestring;

void MainWindow::PaintCyfrasBottom()
{
    // задается вручную
    int smallrectingleheight = 80; // высота прямоугольничка в пикселях задается вручную
    //высчитываются
    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях
    int smallrectinglewidth = widgwidth / 4; // ширина прямоугольничка в пикселях высчитывается
    int otstupsnizu = smallrectingleheight + 28;
    int otstupsverhu = widgheight - otstupsnizu;
    int alerttextsize = smallrectingleheight/2;
    int smalltextsize = (smallrectingleheight - alerttextsize ) / 4;
    
#ifdef Q_OS_WIN32
    alerttextsize/=1.5;
    smalltextsize/=1.2;
#endif
    

    // задаем координаты отображения квадратов
    channel1.xposition = 0;
    channel1.yposition = otstupsverhu;
    channel1.w = smallrectinglewidth;
    channel1.h = smallrectingleheight;
    
    channel2.xposition = smallrectinglewidth;
    channel2.yposition = otstupsverhu;
    channel2.w = smallrectinglewidth;
    channel2.h = smallrectingleheight;
    
    channel3.xposition = smallrectinglewidth*2;
    channel3.yposition = otstupsverhu;
    channel3.w = smallrectinglewidth;
    channel3.h = smallrectingleheight;
    
    channel4.xposition = smallrectinglewidth*3;
    channel4.yposition = otstupsverhu;
    channel4.w = smallrectinglewidth;
    channel4.h = smallrectingleheight;
    
    // создаем лист объектов для его отображения на графике
    
    QList<ChannelOptions *> ChannelsObjectsList;
    
    ChannelsObjectsList.append(&channel1);
    ChannelsObjectsList.append(&channel2);
    ChannelsObjectsList.append(&channel3);
    ChannelsObjectsList.append(&channel4);
    
    painter.begin(ui->MessagesWidget);
    
    // здесь собственно рисуем квадрат для каждого канала (в последствии можно будет добавить больше квадратов
    foreach (ChannelOptions * Chanel, ChannelsObjectsList) {
        {
            double channelcurrentvalue =Chanel->GetCurrentChannelValue();
            
            // рисуем прямоугольник  с заполненным цветом

            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));

            if (Chanel->MaximumNow())
            {
                painter.setBrush(QBrush(Chanel->GetMaximumColor(), Qt::SolidPattern));
                painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);
                if  (GetHalfSecFlag())
                    painter.setPen(QPen(Qt::white, 2)); //, Qt::DashDotLine, Qt::RoundCap));
                else
                    painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            }
            else if (Chanel->MinimumNow())
            {
                painter.setBrush(QBrush(Chanel->GetMinimumColor(), Qt::SolidPattern));
                painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);
                if  (GetHalfSecFlag())
                    painter.setPen(QPen(Qt::white, 2)); //, Qt::DashDotLine, Qt::RoundCap));
                else
                    painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            }
            else
            {
                painter.setBrush(QBrush( Chanel->GetNormalColor(), Qt::SolidPattern));
                painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);
            }

            QString ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);

            if (ChannelValueString == NaNMessage)
                ChannelValueString = ObryvErrorMessage;
            else
            {
                if (!ui->percentCheckBox->checkState())
                    ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);
                else
                    ChannelValueString = QString::number( Chanel->GetValuePercent(), 'f', 1) + " %";
            }
            
            // выводим значения каналов большими цифрами
            painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignVCenter,ChannelValueString);
            
            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));

            // подписываем названия каналов
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignTop,Chanel->GetChannelName());
            
            // подписываем единицы измерения
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom,Chanel->GetUnitsName());

            // подписываем math, если канал математически обрабатывается
            painter.setPen(Qt::white);
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            
            if (Chanel->IsChannelMathematical())
                painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignRight | Qt::AlignTop, MathString);
            ////
        }
    }
    painter.end();
}

void MainWindow::PaintCyfrasRight()
{
    //задается вручную
    //высчитываются

    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях
    int smallrectingleheight = widgheight/4-33; // высота прямоугольничка в пикселях задается вручную
    int smallrectinglewidth = widgwidth / (4 *1024/1280)*0.8; // ширина прямоугольничка в пикселях высчитывается
    int otstupsnizu = smallrectingleheight + 37;
    int otstupsverhu = widgheight - otstupsnizu; //  widgheight/12*2-otstupsnizu;
    int otstupsverhu1 = otstupsverhu-smallrectingleheight;
    int otstupsverhu2 = otstupsverhu-smallrectingleheight*2;
    int otstupsverhu3 = otstupsverhu-smallrectingleheight*3;
    int alerttextsize = smallrectingleheight/2;
    int smalltextsize = (smallrectingleheight - alerttextsize ) / 4;
    
#ifdef Q_OS_WIN32
    alerttextsize/=1.5;
    smalltextsize/=1.2;
#endif
    
    // задаем координаты отображения квадратов
    int xpositionall = widgwidth - smallrectinglewidth;

    channel1.xposition = xpositionall;
    channel1.yposition = otstupsverhu3;
    channel1.w = smallrectinglewidth;
    channel1.h = smallrectingleheight;
    
    channel2.xposition = xpositionall;
    channel2.yposition = otstupsverhu2;
    channel2.w = smallrectinglewidth;
    channel2.h = smallrectingleheight;
    
    channel3.xposition = xpositionall;
    channel3.yposition = otstupsverhu1;
    channel3.w = smallrectinglewidth;
    channel3.h = smallrectingleheight;
    
    channel4.xposition = xpositionall;
    channel4.yposition = otstupsverhu;
    channel4.w = smallrectinglewidth;
    channel4.h = smallrectingleheight;
    
    // создаем лист объектов для его отображения на графике
    
    QList<ChannelOptions *> ChannelsObjectsList;
    
    ChannelsObjectsList.append(&channel1);
    ChannelsObjectsList.append(&channel2);
    ChannelsObjectsList.append(&channel3);
    ChannelsObjectsList.append(&channel4);
    
    painter.begin(ui->MessagesWidget);

    // здесь собственно рисуем квадрат для каждого канала (в последствии можно будет добавить больше квадратов
    foreach (ChannelOptions * Chanel, ChannelsObjectsList) {
        {
            double channelcurrentvalue =Chanel->GetCurrentChannelValue();
            double channelstate1value = Chanel->GetState1Value();
            double channelstate2value = Chanel->GetState2Value();
            
            // рисуем прямоугольник  с заполненным цветом
            
            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));

            if (Chanel->MaximumNow())
            {
                painter.setBrush(QBrush(Chanel->GetMaximumColor(), Qt::SolidPattern));
                painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);
                if  (GetHalfSecFlag())
                    painter.setPen(QPen(Qt::white, 2)); //, Qt::DashDotLine, Qt::RoundCap));
                else
                    painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            }
            else if (Chanel->MinimumNow())
            {
                painter.setBrush(QBrush(Chanel->GetMinimumColor(), Qt::SolidPattern));
                painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);
                if  (GetHalfSecFlag())
                    painter.setPen(QPen(Qt::white, 2)); //, Qt::DashDotLine, Qt::RoundCap));
                else
                    painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            }
            else
            {
                painter.setBrush(QBrush(Chanel->GetNormalColor(), Qt::SolidPattern));
                painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);
            }


            QString ChannelValueString ;

            if (!ui->percentCheckBox->checkState())
                ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);
            else
                ChannelValueString = QString::number( Chanel->GetValuePercent(), 'f', 1) + " %";
            
            if (ChannelValueString == NaNMessage)
                ChannelValueString = ObryvErrorMessage;
            else
            {
                if (!ui->percentCheckBox->checkState())
                    ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);
                else
                    ChannelValueString = QString::number( Chanel->GetValuePercent(), 'f', 1) + " %";
            }

            // выводим значения каналов большими цифрами
            painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignVCenter,ChannelValueString);

            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));

            // подписываем названия каналов
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignTop,Chanel->GetChannelName());
            
            //    // подписываем единицы измерения
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom,Chanel->GetUnitsName());
            
            //    // подписываем math, если канал математически обрабатывается
            painter.setPen(Qt::white);
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            
            if (Chanel->IsChannelMathematical())
                painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignRight | Qt::AlignTop, MathString);
        }
    }
    painter.end();
}

void MainWindow::PaintCyfrasNew()
{
    // задается вручную
    int smallrectingleheight = 80; // высота прямоугольничка в пикселях задается вручную
    //высчитываются
    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях
    int smallrectinglewidth = widgwidth / 4; // ширина прямоугольничка в пикселях высчитывается
    int otstupsnizu = smallrectingleheight + 28;
    int otstupsverhu = widgheight - otstupsnizu;
    int alerttextsize = smallrectingleheight/2;
    int smalltextsize = (smallrectingleheight - alerttextsize ) / 4;

#ifdef Q_OS_WIN32
    alerttextsize/=1.5;
    smalltextsize/=1.2;
#endif


    // задаем координаты отображения квадратов
    channel1.xposition = 0;
    channel1.yposition = otstupsverhu;
    channel1.w = smallrectinglewidth;
    channel1.h = smallrectingleheight;

    channel2.xposition = smallrectinglewidth;
    channel2.yposition = otstupsverhu;
    channel2.w = smallrectinglewidth;
    channel2.h = smallrectingleheight;

    channel3.xposition = smallrectinglewidth*2;
    channel3.yposition = otstupsverhu;
    channel3.w = smallrectinglewidth;
    channel3.h = smallrectingleheight;

    channel4.xposition = smallrectinglewidth*3;
    channel4.yposition = otstupsverhu;
    channel4.w = smallrectinglewidth;
    channel4.h = smallrectingleheight;

    // создаем лист объектов для его отображения на графике

    QList<ChannelOptions *> ChannelsObjectsList;

    ChannelsObjectsList.append(&channel1);
    ChannelsObjectsList.append(&channel2);
    ChannelsObjectsList.append(&channel3);
    ChannelsObjectsList.append(&channel4);

    painter.begin(ui->MessagesWidget);

    // здесь собственно рисуем квадрат для каждого канала (в последствии можно будет добавить больше квадратов
    foreach (ChannelOptions * Chanel, ChannelsObjectsList) {
        {
            double channelcurrentvalue =Chanel->GetCurrentChannelValue();

            // рисуем прямоугольник  с заполненным цветом

            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));

            if (Chanel->MaximumNow())
            {
                painter.setBrush(QBrush(Chanel->GetMaximumColor(), Qt::SolidPattern));
                painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);
                if  (GetHalfSecFlag())
                    painter.setPen(QPen(Qt::white, 2)); //, Qt::DashDotLine, Qt::RoundCap));
                else
                    painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            }
            else if (Chanel->MinimumNow())
            {
                painter.setBrush(QBrush(Chanel->GetMinimumColor(), Qt::SolidPattern));
                painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);
                if  (GetHalfSecFlag())
                    painter.setPen(QPen(Qt::white, 2)); //, Qt::DashDotLine, Qt::RoundCap));
                else
                    painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            }
            else
            {
                painter.setBrush(QBrush( Chanel->GetNormalColor(), Qt::SolidPattern));
                painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);
            }

            QString ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);

            if (ChannelValueString == NaNMessage)
                ChannelValueString = ObryvErrorMessage;
            else
            {
                if (!ui->percentCheckBox->checkState())
                    ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);
                else
                    ChannelValueString = QString::number( Chanel->GetValuePercent(), 'f', 1) + " %";
            }

            // выводим значения каналов большими цифрами
            painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignVCenter,ChannelValueString);

            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));

            // подписываем названия каналов
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignTop,Chanel->GetChannelName());

            // подписываем единицы измерения
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom,Chanel->GetUnitsName());

            // подписываем math, если канал математически обрабатывается
            painter.setPen(Qt::white);
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));

            if (Chanel->IsChannelMathematical())
                painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignRight | Qt::AlignTop, MathString);
            ////
        }
    }
    painter.end();
}


void MainWindow::PaintCyfrasFullScreen()
{
    QPainter painter;
    // задаётся параметры вручную
    
    // отступ  сверху и слева в пикселях
    int borderwidth = 0 ;
    //высчитываются
    int widgwidth = ui->MessagesWidget->width()-borderwidth;// высота всей области построения в пикселях
    int widgheight = ui->MessagesWidget->height()- borderwidth; // ширина всей области построения в пикселях
    int bigrectingleheight = (widgheight)/2; // высота прямоугольничка в пикселях задается вручную
    int bigrectinglewidth = (widgwidth)/2; // ширина прямоугольничка в пикселях высчитывается
    int alerttextsize = bigrectingleheight/2.8;
    int smalltextsize = (bigrectingleheight - alerttextsize ) / 5;
    
#ifdef Q_OS_WIN32
    alerttextsize/=1.5;
    smalltextsize/=1.2;
#endif

    // задаем координаты отображения квадратов
    channel1.xposition = borderwidth;
    channel1.yposition = borderwidth;
    channel1.w = bigrectinglewidth;
    channel1.h = bigrectingleheight;
    
    channel2.xposition = borderwidth+bigrectinglewidth;
    channel2.yposition = borderwidth;
    channel2.w = bigrectinglewidth;
    channel2.h = bigrectingleheight;
    
    channel3.xposition = borderwidth;
    channel3.yposition = borderwidth+bigrectingleheight;
    channel3.w = bigrectinglewidth;
    channel3.h = bigrectingleheight;
    
    channel4.xposition = borderwidth+bigrectinglewidth;
    channel4.yposition = borderwidth+bigrectingleheight;
    channel4.w = bigrectinglewidth;
    channel4.h = bigrectingleheight;
    
    // создаем лист объектов для его отображения на графике
    
    QList<ChannelOptions *> ChannelsObjectsList;
    
    ChannelsObjectsList.append(&channel1);
    ChannelsObjectsList.append(&channel2);
    ChannelsObjectsList.append(&channel3);
    ChannelsObjectsList.append(&channel4);
    
    painter.begin(ui->MessagesWidget);

    int index = 1;
    
    // здесь собственно рисуем квадрат для каждого канала (в последствии можно будет добавить больше квадратов
    foreach (ChannelOptions * Chanel, ChannelsObjectsList) {
        {
            double channelcurrentvalue =Chanel->GetCurrentChannelValue();
            
            // рисуем прямоугольник  с заполненным цветом
            
            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            if (Chanel->MaximumNow())
            {
                painter.setBrush(QBrush(Chanel->GetMaximumColor(), Qt::SolidPattern));
                if  (GetHalfSecFlag())
                    painter.setPen(QPen(Qt::white, 2)); //, Qt::DashDotLine, Qt::RoundCap));
                else
                    painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            }
            else if (Chanel->MinimumNow())
            {
                painter.setBrush(QBrush(Chanel->GetMinimumColor(), Qt::SolidPattern));
                if  (GetHalfSecFlag())
                    painter.setPen(QPen(Qt::white, 2)); //, Qt::DashDotLine, Qt::RoundCap));
                else
                    painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            }
            else
            {
                painter.setBrush(QBrush(Chanel->GetNormalColor(), Qt::SolidPattern));
            }

            painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);

            QString ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);

            if (ChannelValueString == NaNMessage)
                ChannelValueString = ObryvErrorMessage;
            else
            {
                if (!ui->percentCheckBox->checkState())
                    ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);
                else
                    ChannelValueString = QString::number( Chanel->GetValuePercent(), 'f', 1) + " %";
            }
            
            // выводим значения каналов большими цифрами
            painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignVCenter,ChannelValueString);

            // подписываем названия каналов
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignTop,Chanel->GetChannelName());
            
            //    // подписываем единицы измерения
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom,Chanel->GetUnitsName());
            
            //    // подписываем math, если канал математически обрабатывается
            painter.setPen(Qt::white);
            painter.setFont(QFont(Font, smalltextsize/2, QFont::ExtraBold));
            
            if (Chanel->IsChannelMathematical())
                painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignRight | Qt::AlignTop, MathString);
            
            painter.setFont(QFont(Font, smalltextsize/2, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignLeft | Qt::AlignBottom, QString::number(index) );
            ++index;
            ////
        }
    }
    painter.end();
}

void MainWindow::PaintStatesAndAlertsAtTop() // отрисовывает события на уставки
{
    QPainter painter;

    //высчитываются
    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях
    
    //размеры самого прямоугольничка
    int alertwindowwidth = widgwidth/2; // ширина
    int alertwindowheight  = widgheight/12; // высота
    int alerttextsize = alertwindowheight/2; // размер алерт-текста
    int smalltextsize = (alertwindowheight - alerttextsize)/2.5; // мал.размер текста
    
#ifdef Q_OS_WIN32
    // если компилируем под виндой, уменьшаем текст еще в полтора раза чтоб не  наезжал друг на друга
    alerttextsize/=1.5;
    smalltextsize/=1.5;
#endif

    // задаем координаты отображения квадратов
    channel1.xposition = 0;
    channel1.yposition = 0;
    channel1.w = alertwindowwidth;
    channel1.h = alertwindowheight;
    
    channel2.xposition = alertwindowwidth;
    channel2.yposition = 0;
    channel2.w = alertwindowwidth;
    channel2.h = alertwindowheight;
    
    channel3.xposition = 0;
    channel3.yposition = alertwindowheight;
    channel3.w = alertwindowwidth;
    channel3.h = alertwindowheight;
    
    channel4.xposition = alertwindowwidth;
    channel4.yposition = alertwindowheight;
    channel4.w = alertwindowwidth;
    channel4.h = alertwindowheight;
    
    // создаем лист объектов для его отображения на графике
    
    QList<ChannelOptions *> ChannelsObjectsList;
    
    ChannelsObjectsList.append(&channel1);
    ChannelsObjectsList.append(&channel2);
    ChannelsObjectsList.append(&channel3);
    ChannelsObjectsList.append(&channel4);
    
    painter.begin(ui->MessagesWidget);

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));
    
    int confirmwindowwidth = widgwidth/3;
    int confirmwindowheight  = widgheight/3;
    int confirmwindowposx = (widgwidth -  confirmwindowwidth)/2;
    int confirmwindowposy = (widgheight -  confirmwindowheight)/2;
    int index = 0;
    
    // здесь собственно рисуем квадрат для каждого канала (в последствии можно будет добавить больше квадратов
    foreach (ChannelOptions * Chanel, ChannelsObjectsList) {
        {

            // рисуем прямоугольник  с заполненным цветом

            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            if (Chanel->MaximumNow())
            {
                painter.setBrush(QBrush(Chanel->GetMaximumColor(), Qt::SolidPattern));
                painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);
            }
            else if (Chanel->MinimumNow())
            {
                painter.setBrush(QBrush(Chanel->GetMinimumColor(), Qt::SolidPattern));
                painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);
            }
            else
            {
                painter.setBrush(QBrush(Chanel->GetNormalColor(), Qt::SolidPattern));
                painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);
            }

            // рисуем текстовую инфу
            painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignTop, Chanel->GetChannelName());
            painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));

            if ( GetHalfSecFlag() && ( Chanel->MinimumNow() || Chanel->MaximumNow()) )
            {
                painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
            }
            else
                painter.setPen(QPen(Qt::black, 1)); //, Qt::DashDotLine, Qt::RoundCap));
            
            if (Chanel->MaximumNow())
            {
                painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom, Chanel->GetState1HighMessage());
                
                // часть квитирования
                // если превысила уставку
                //                if ( (Chanel->GetConfirmationNeed() == true) && (ui->ConfirmBox->isChecked()) )

                if (0)
                {
                    painter.setBrush(QBrush(ChannelColorHighState, Qt::SolidPattern));

                    if  (GetHalfSecFlag())
                        painter.setPen(QPen(Qt::white, 2)); //, Qt::DashDotLine, Qt::RoundCap));
                    else
                        painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));

                    painter.drawRect(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight);
                    painter.drawText(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight, Qt::AlignHCenter | Qt::AlignVCenter,Chanel->GetState1HighMessage());
                }
            }
            // уменьшение уставки  Channel 1
            else if (Chanel->MinimumNow())
            {
                painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom, Chanel->GetState2LowMessage());
                
                // если ниже уставки
                //                if ( (Chanel->GetConfirmationNeed() == true) && (ui->ConfirmBox->isChecked()) )

                if (0)
                {
                    painter.setBrush(QBrush(ChannelColorLowState, Qt::SolidPattern));
                    if  (GetHalfSecFlag())
                        painter.setPen(QPen(Qt::white, 2)); //, Qt::DashDotLine, Qt::RoundCap));
                    else
                        painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
                    painter.drawRect(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight);
                    painter.drawText(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight, Qt::AlignHCenter | Qt::AlignVCenter,Chanel->GetState2LowMessage());
                }
                
            }
            else
            {
                painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
                painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom, tr(OKMessage));
            }
            
            if  (GetHalfSecFlag())
            {
                painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
                painter.setFont(QFont(Font, alerttextsize*2, QFont::ExtraBold));
            }
        }
        index++;
    }
    
    return;
}

// полярные координаты
void MainWindow::PaintPolarDiagramm()
{
    QPainter painter;
    double maximumradius;
    maximumradius = 400;
    
    painter.begin(ui->MessagesWidget);

    painter.setRenderHint(QPainter::Antialiasing, true);

    int channel1value = X_Coordinates.last();
    int channel2value = X_Coordinates.last();
    int channel3value = X_Coordinates.last();
    int channel4value = X_Coordinates.last();

    float channel1length = channel1.GetValuePercent() * 5;
    float channel2length = channel2.GetValuePercent() * 5;
    float channel3length = channel3.GetValuePercent() * 5;
    float channel4length = channel4.GetValuePercent() * 5;

    QColor color1,color2,color3,color4;

    color1 = channel1.GetStateDependentColor();
    color2 = channel2.GetStateDependentColor();
    color3 = channel3.GetStateDependentColor();
    color4 = channel4.GetStateDependentColor();


    
    int centerx1,centerx2,centerx3,centerx4;
    int centery1,centery2,centery3,centery4;
    int newxcenter = 0, newycenter = 500;
    
    centerx1 = centerx2 = centerx3 = centerx4 = 1;
    centery1 = centery2 = centery3 = centery4 = 1;
    
    painter.translate(newxcenter, newycenter);
    /* Create the line object: */
    
    painter.setPen(QPen(Qt::black, 1));
    if (GetEcoMode()){
        painter.setBrush(QBrush(EcoColor, Qt::SolidPattern));
        painter.setPen(QPen(NotEcoColor,1,  Qt::DashLine));
    }
    else
    {
        painter.setBrush(QBrush(NotEcoColor, Qt::SolidPattern));
        painter.setPen(QPen(EcoColor,1,  Qt::DashLine));
    }
    painter.drawRect(0, -newycenter,ui->MessagesWidget->width()-1,ui->MessagesWidget->height());
    
    // Рисуем пять гругов разметочки
    
    painter.drawEllipse(QPointF(centerx1,centery1), 500, 500); // 100%
    painter.drawEllipse(QPointF(centerx1,centery1), 400, 400);// 80%
    painter.drawEllipse(QPointF(centerx1,centery1), 300, 300);// 60%
    painter.drawEllipse(QPointF(centerx1,centery1), 200, 200);// 40%
    painter.drawEllipse(QPointF(centerx1,centery1), 100, 100);// 20%

    QLineF Channel1Line;
    
    if (channel1value>=90) // если больше 90 градусов то поворачиваем диск
        
    {
        painter.rotate(channel1value-90);
        // оставляем только 180 точек (на 180 градусов)
        if (channel1value>=180) // если больше 90 градусов то поворачиваем диск
        {
            PolarChartPointsChannel1.removeFirst();
            PolarChartPointsChannel2.removeFirst();
            PolarChartPointsChannel3.removeFirst();
            PolarChartPointsChannel4.removeFirst();
        }
    }
    
    QLineF Line1,Line2,Line3,Line4,Line5,Line6,Line7,Line8,Line9,Line10,Line11,Line12;
    
    QList<QLineF> LineList;
    
    LineList.append(Line1);
    LineList.append(Line2);
    LineList.append(Line3);
    LineList.append(Line4);
    LineList.append(Line5);
    LineList.append(Line6);
    LineList.append(Line7);
    LineList.append(Line8);
    LineList.append(Line9);
    LineList.append(Line10);
    LineList.append(Line11);
    LineList.append(Line12);
    
    int ind = 0;

    foreach (QLineF Line, LineList) {
        
        Line.setP1(QPointF(centerx1, centery1));
        Line.setAngle(30*ind);
        Line.setLength(500);
        painter.drawLine(Line);
        ++ind;
    }
    
    // отсюда начинаем рисовать значения на круговой диаграмме
    painter.setPen(QPen(Qt::green, 1));
    /* Set the origin: */
    Channel1Line.setP1(QPointF(centerx1, centery1));
    Channel1Line.setAngle(channel1value);
    Channel1Line.setLength(channel1length);
    
    int x1 = Channel1Line.x2(); // мы берем координаты `1 точки
    int y1 = Channel1Line.y2(); // мы берем координаты второй точки
    
    /* Create the line object: */
    QLineF Channel2Line;
    painter.setPen(QPen(Qt::green, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel2Line.setP1(QPointF(centerx2, centery2));
    Channel2Line.setAngle(channel2value);
    Channel2Line.setLength(channel2length);
    
    int x2 = Channel2Line.x2(); // мы берем координаты `1 точки
    int y2 = Channel2Line.y2(); // мы берем координаты второй точки
    
    /* Create the line object: */
    QLineF Channel3Line;
    painter.setPen(QPen(Qt::green, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel3Line.setP1(QPointF(centerx3, centery3));
    Channel3Line.setAngle(channel3value);
    Channel3Line.setLength(channel3length);
    int x3 = Channel3Line.x2(); // мы берем координаты `1 точки
    int y3 = Channel3Line.y2(); // мы берем координаты второй точки
    
    /* Create the line object: */
    QLineF Channel4Line;
    painter.setPen(QPen(Qt::green, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel4Line.setP1(QPointF(centerx4, centery4) );
    Channel4Line.setAngle(channel4value);
    Channel4Line.setLength(channel4length);
    
    int x4 = Channel4Line.x2(); // мы берем координаты `1 точки
    int y4 = Channel4Line.y2(); // мы берем координаты второй точки
    
    painter.setPen(QPen(Qt::green,2,  Qt::DashLine)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.drawLine(Channel1Line);
    
    //painter.drawLine(Channel2Line);
    //painter.drawLine(Channel3Line);
    //painter.drawLine(Channel4Line);
    
    QPoint NewPolarPointChannel1,NewPolarPointChannel2,NewPolarPointChannel3,NewPolarPointChannel4;
    
    NewPolarPointChannel1.setX(x1);
    NewPolarPointChannel1.setY(y1);
    
    NewPolarPointChannel2.setX(x2);
    NewPolarPointChannel2.setY(y2);
    
    NewPolarPointChannel3.setX(x3);
    NewPolarPointChannel3.setY(y3);
    
    NewPolarPointChannel4.setX(x4);
    NewPolarPointChannel4.setY(y4);
    
    PolarChartPointsChannel1.append(NewPolarPointChannel1);
    PolarChartPointsChannel2.append(NewPolarPointChannel2);
    PolarChartPointsChannel3.append(NewPolarPointChannel3);
    PolarChartPointsChannel4.append(NewPolarPointChannel4);
    
    painter.setPen(QPen(color1, 2));
    painter.drawPolyline(PolarChartPointsChannel1);
    painter.setPen(QPen(color2, 2));
    painter.drawPolyline(PolarChartPointsChannel2);
    painter.setPen(QPen(color3, 2));
    painter.drawPolyline(PolarChartPointsChannel3);
    painter.setPen(QPen(color4, 2));
    painter.drawPolyline(PolarChartPointsChannel4);

    painter.resetTransform(); // все что дальше - не поворачивается динамически

    painter.setFont(QFont(Font, 10, QFont::Bold));

    painter.translate(newxcenter, newycenter);
    painter.drawText(5, -40,40, 40, Qt::AlignHCenter | Qt::AlignVCenter,    "0%");
    painter.drawText(105, -40, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "20%");
    painter.drawText(205, -40,40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "40%");
    painter.drawText(305, -40,40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "60%");
    painter.drawText(405, -40,40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "80%");
    painter.drawText(505, -40,40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "100%");

    painter.drawText(5, -40-100, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "20%");
    painter.drawText(5, -40-200,40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "40%");
    painter.drawText(5, -40-300,40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "60%");
    painter.drawText(5, -40-400,40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "80%");
    painter.drawText(5, -40-500,40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "100%");

    painter.end();
}

void MainWindow::PaintOnWidget()
{
    switch( StackedOptions::GetCurrentDisplayParametr())
    {
    case Options::Cyfra:
        PaintCyfrasFullScreen();
        break;
    case Options::TrendsCyfra:
        //PaintStatesAndAlertsAtTop();
        PaintCyfrasNew();
        //PaintCyfrasRight();
        break;
    case Options::Trends:
        PaintStatesAndAlertsAtTop();
        break;
    case Options::TrendsCyfraBars:
        PaintStatesAndAlertsAtTop();
        PaintCyfrasBottom();
        break;
    case Options::BarsCyfra:
        PaintCyfrasBottom();
        PaintStatesAndAlertsAtTop();
        break;
    case Options::Polar:
        //        PaintStatesAndAlertsAtTop();
        PaintPolarDiagramm();
        PaintCyfrasRight();
        break;
    default:
        break;
    }
}

void MainWindow::ReactOnMouseSlide()
{
    int  y   =  QCursor::pos().y() ;
    int ky = 20 + y*3;
    
    ui->customPlot->yAxis->setRange(-ky, ky);
}

void MainWindow::ReactOnTouch()
{
    //пока просто возвращаем

    return;
    // подкрашиваем квадраты куда коснулись в желтый цвет
    // отступ  сверху и слева в пикселях
    // высчитываются
    
    int xcenter  = 652;// общая точка четырех квадратов по иксу в пикселях
    int ycenter  = 384; // ширина всей области построения в пикселях
    
    int xpos = QCursor::pos().x();
    int ypos = QCursor::pos().y();
    
    QString x = QString::number(xpos);
    QString y = QString::number(ypos);

    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях
    int confirmwindowwidth = widgwidth/4;
    int confirmwindowheight  = widgheight/4;
    int confirmwindowposx = (widgwidth -  confirmwindowwidth)/2;
    int confirmwindowposy = (widgheight -  confirmwindowheight)/2;
    int confirmwindowposx2 = confirmwindowposx  +  confirmwindowwidth;
    int confirmwindowposy2 = confirmwindowposy + confirmwindowheight ;

    if      ((xpos>confirmwindowposx) &&
             (xpos<confirmwindowposx2) &&
             (ypos>confirmwindowposy) &&
             (ypos<confirmwindowposy2) )
    {
        if (channel4.GetConfirmationNeed() == true) {
            channel4.SetConfirmationNeed(false);
        }
        if (channel3.GetConfirmationNeed() == true) {
            channel3.SetConfirmationNeed(false);
        }
        if (channel2.GetConfirmationNeed() == true) {
            channel2.SetConfirmationNeed(false);
        }
        if (channel1.GetConfirmationNeed() == true) {
            channel1.SetConfirmationNeed(false);
        }
    }
    
    int x1right  = 940;
    int x2right  = 1240;
    int y0right  = 162;
    int y1right  = 258;
    
    if      ((xpos>x1right) &&
             (xpos<x2right) &&
             (ypos>y0right) &&
             (ypos<y1right) )
    {
        {
            QPainter painter;
            painter.begin(ui->MessagesWidget);
            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            int confirmwindowposx = (widgwidth -  confirmwindowwidth)/2;
            int confirmwindowposy = (widgheight -  confirmwindowheight)/2;
            painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
            painter.drawRect(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight);
            painter.drawText(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight, Qt::AlignHCenter | Qt::AlignVCenter,"Allelua");
            painter.end();
        }
    }
}
