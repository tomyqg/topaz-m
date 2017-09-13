#include "updategraph.h"
#include "mainwindow.h"
#include "uartdriver.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "metrologicalcalc.h"
#include "mathresolver.h"
#include "options.h"
#include "defines.h"

extern QColor Channel1Color;
extern QColor Channel2Color;
extern QColor Channel3Color;
extern QColor Channel4Color;
extern QColor ChannelColorNormal;
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
    int smallrectingleheight = 100; // высота прямоугольничка в пикселях задается вручную
    //высчитываются
    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях
    int smallrectinglewidth = widgwidth / 4; // ширина прямоугольничка в пикселях высчитывается
    int otstupsnizu = smallrectingleheight + 24;
    int otstupsverhu = widgheight - otstupsnizu;
    int alerttextsize = smallrectingleheight/2.5;
    int smalltextsize = (smallrectingleheight - alerttextsize ) / 5;

#ifdef Q_OS_WIN32
    alerttextsize/=1.5;
    smalltextsize/=1.2;
#endif

    channel1object.SetCurrentChannelValue( UartDriver::channelinputbuffer[0]);
    channel2object.SetCurrentChannelValue( UartDriver::channelinputbuffer[1]);
    channel3object.SetCurrentChannelValue( UartDriver::channelinputbuffer[2]);
    channel4object.SetCurrentChannelValue( UartDriver::channelinputbuffer[3]);

    // задаем координаты отображения квадратов
    channel1object.xposition = 0;
    channel1object.yposition = otstupsverhu;
    channel1object.w = smallrectinglewidth;
    channel1object.h = smallrectingleheight;

    channel2object.xposition = smallrectinglewidth;
    channel2object.yposition = otstupsverhu;
    channel2object.w = smallrectinglewidth;
    channel2object.h = smallrectingleheight;

    channel3object.xposition = smallrectinglewidth*2;
    channel3object.yposition = otstupsverhu;
    channel3object.w = smallrectinglewidth;
    channel3object.h = smallrectingleheight;

    channel4object.xposition = smallrectinglewidth*3;
    channel4object.yposition = otstupsverhu;
    channel4object.w = smallrectinglewidth;
    channel4object.h = smallrectingleheight;

    // создаем лист объектов для его отображения на графике

    QList<ChannelOptions *> ChannelsObjectsList;

    ChannelsObjectsList.append(&channel1object);
    ChannelsObjectsList.append(&channel2object);
    ChannelsObjectsList.append(&channel3object);
    ChannelsObjectsList.append(&channel4object);


    painter.begin(ui->MessagesWidget);
    painter.setRenderHint(QPainter::Antialiasing, false);


    // здесь собственно рисуем квадрат для каждого канала (в последствии можно будет добавить больше квадратов
    foreach (ChannelOptions * Chanel, ChannelsObjectsList) {
        {
            double channelcurrentvalue =Chanel->GetCurrentChannelValue();
            double channelstate1value = Chanel->GetState1Value();
            double channelstate2value = Chanel->GetState2Value();
            QColor channelcolor = Chanel->GetCurrentColor();

            // рисуем прямоугольник  с заполненным цветом

            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            if (channelcurrentvalue>channelstate1value)
            {
                if  (GetHalfSecFlag())
                    painter.setBrush(QBrush(ChannelColorHighState, Qt::SolidPattern));
                else
                    painter.setBrush(QBrush(channelcolor, Qt::SolidPattern));
            }
            else if (channelcurrentvalue<channelstate2value)
            {
                if  (GetHalfSecFlag())
                    painter.setBrush(QBrush(ChannelColorLowState, Qt::SolidPattern));
                else
                    painter.setBrush(QBrush(channelcolor, Qt::SolidPattern));
            }
            else
            {
                painter.setBrush(QBrush(channelcolor, Qt::SolidPattern));
            }

            painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);

            QString ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);

            if (ChannelValueString == NaNMessage)
                ChannelValueString = ObryvErrorMessage;

            // выводим значения каналов большими цифрами
            painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignVCenter,ChannelValueString);

            // подписываем названия каналов
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignTop,Chanel->GetChannelName());

            //    // подписываем единицы измерения
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom,channel1object.GetUnitsName());

            //    // подписываем math, если канал математически обрабатывается
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
    // задается вручную
    //высчитываются
    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях
    int smallrectingleheight = widgheight/4-30; // высота прямоугольничка в пикселях задается вручную
    int smallrectinglewidth = widgwidth / 4; // ширина прямоугольничка в пикселях высчитывается
    int otstupsnizu = smallrectingleheight + 24;
    int otstupsverhu = widgheight - otstupsnizu;
    int otstupsverhu1 = otstupsverhu-smallrectingleheight;
    int otstupsverhu2 = otstupsverhu-smallrectingleheight*2;
    int otstupsverhu3 = otstupsverhu-smallrectingleheight*3;
    int alerttextsize = smallrectingleheight/2;
    int smalltextsize = (smallrectingleheight - alerttextsize ) / 4;

#ifdef Q_OS_WIN32
    alerttextsize/=1.5;
    smalltextsize/=1.2;
#endif

    channel1object.SetCurrentChannelValue( UartDriver::channelinputbuffer[0]);
    channel2object.SetCurrentChannelValue( UartDriver::channelinputbuffer[1]);
    channel3object.SetCurrentChannelValue( UartDriver::channelinputbuffer[2]);
    channel4object.SetCurrentChannelValue( UartDriver::channelinputbuffer[3]);

    // задаем координаты отображения квадратов
    channel1object.xposition = smallrectinglewidth*3-5;
    channel1object.yposition = otstupsverhu3;
    channel1object.w = smallrectinglewidth;
    channel1object.h = smallrectingleheight;

    channel2object.xposition = smallrectinglewidth*3-5;
    channel2object.yposition = otstupsverhu2;
    channel2object.w = smallrectinglewidth;
    channel2object.h = smallrectingleheight;

    channel3object.xposition = smallrectinglewidth*3-5;
    channel3object.yposition = otstupsverhu1;
    channel3object.w = smallrectinglewidth;
    channel3object.h = smallrectingleheight;

    channel4object.xposition = smallrectinglewidth*3-5;
    channel4object.yposition = otstupsverhu;
    channel4object.w = smallrectinglewidth;
    channel4object.h = smallrectingleheight;

    // создаем лист объектов для его отображения на графике

    QList<ChannelOptions *> ChannelsObjectsList;

    ChannelsObjectsList.append(&channel1object);
    ChannelsObjectsList.append(&channel2object);
    ChannelsObjectsList.append(&channel3object);
    ChannelsObjectsList.append(&channel4object);


    painter.begin(ui->MessagesWidget);
    painter.setRenderHint(QPainter::Antialiasing, false);


    // здесь собственно рисуем квадрат для каждого канала (в последствии можно будет добавить больше квадратов
    foreach (ChannelOptions * Chanel, ChannelsObjectsList) {
        {
            //            qDebug() << Chanel->GetChannelName();

            double channelcurrentvalue =Chanel->GetCurrentChannelValue();
            double channelstate1value = Chanel->GetState1Value();
            double channelstate2value = Chanel->GetState2Value();
            QColor channelcolor = Chanel->GetCurrentColor();

            // рисуем прямоугольник  с заполненным цветом

            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            if (channelcurrentvalue>channelstate1value)
            {
                if  (GetHalfSecFlag())
                    painter.setBrush(QBrush(ChannelColorHighState, Qt::SolidPattern));
                else
                    painter.setBrush(QBrush(channelcolor, Qt::SolidPattern));
            }
            else if (channelcurrentvalue<channelstate2value)
            {
                if  (GetHalfSecFlag())
                    painter.setBrush(QBrush(ChannelColorLowState, Qt::SolidPattern));
                else
                    painter.setBrush(QBrush(channelcolor, Qt::SolidPattern));
            }
            else
            {
                painter.setBrush(QBrush(channelcolor, Qt::SolidPattern));
            }

            painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);

            QString ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);

            if (ChannelValueString == NaNMessage)
                ChannelValueString = ObryvErrorMessage;

            // выводим значения каналов большими цифрами
            painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignVCenter,ChannelValueString);

            // подписываем названия каналов
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignTop,Chanel->GetChannelName());

            //    // подписываем единицы измерения
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom,channel1object.GetUnitsName());

            //    // подписываем math, если канал математически обрабатывается
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
    int bigrectingleheight = (widgheight)/2-2; // высота прямоугольничка в пикселях задается вручную
    int bigrectinglewidth = (widgwidth)/2-2; // ширина прямоугольничка в пикселях высчитывается
    int alerttextsize = bigrectingleheight/2.8;
    int smalltextsize = (bigrectingleheight - alerttextsize ) / 5;


    //    painter.drawRect(borderwidth, borderwidth, bigrectinglewidth, bigrectingleheight);
    //    painter.setBrush(QBrush(GetChannel2Color(), Qt::SolidPattern));
    //    painter.drawRect(borderwidth+bigrectinglewidth, borderwidth, bigrectinglewidth, bigrectingleheight);
    //    painter.setBrush(QBrush(GetChannel3Color(), Qt::SolidPattern));
    //    painter.drawRect(borderwidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight);
    //    painter.setBrush(QBrush(GetChannel4Color(), Qt::SolidPattern));
    //    painter.drawRect(borderwidth+bigrectinglewidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight);

#ifdef Q_OS_WIN32
    alerttextsize/=1.5;
    smalltextsize/=1.2;
#endif

    channel1object.SetCurrentChannelValue( UartDriver::channelinputbuffer[0]);
    channel2object.SetCurrentChannelValue( UartDriver::channelinputbuffer[1]);
    channel3object.SetCurrentChannelValue( UartDriver::channelinputbuffer[2]);
    channel4object.SetCurrentChannelValue( UartDriver::channelinputbuffer[3]);

    // задаем координаты отображения квадратов
    channel1object.xposition = borderwidth;
    channel1object.yposition = borderwidth;
    channel1object.w = bigrectinglewidth;
    channel1object.h = bigrectingleheight;

    channel2object.xposition = borderwidth+bigrectinglewidth;
    channel2object.yposition = borderwidth;
    channel2object.w = bigrectinglewidth;
    channel2object.h = bigrectingleheight;

    channel3object.xposition = borderwidth;
    channel3object.yposition = borderwidth+bigrectingleheight;
    channel3object.w = bigrectinglewidth;
    channel3object.h = bigrectingleheight;

    channel4object.xposition = borderwidth+bigrectinglewidth;
    channel4object.yposition = borderwidth+bigrectingleheight;
    channel4object.w = bigrectinglewidth;
    channel4object.h = bigrectingleheight;

    // создаем лист объектов для его отображения на графике

    QList<ChannelOptions *> ChannelsObjectsList;

    ChannelsObjectsList.append(&channel1object);
    ChannelsObjectsList.append(&channel2object);
    ChannelsObjectsList.append(&channel3object);
    ChannelsObjectsList.append(&channel4object);


    painter.begin(ui->MessagesWidget);
    painter.setRenderHint(QPainter::Antialiasing, false);


    int index = 1;

    // здесь собственно рисуем квадрат для каждого канала (в последствии можно будет добавить больше квадратов
    foreach (ChannelOptions * Chanel, ChannelsObjectsList) {
        {
            //            qDebug() << Chanel->GetChannelName();

            double channelcurrentvalue =Chanel->GetCurrentChannelValue();
            double channelstate1value = Chanel->GetState1Value();
            double channelstate2value = Chanel->GetState2Value();
            QColor channelcolor = Chanel->GetCurrentColor();

            // рисуем прямоугольник  с заполненным цветом

            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            if (channelcurrentvalue>channelstate1value)
            {
                if  (GetHalfSecFlag())
                    painter.setBrush(QBrush(ChannelColorHighState, Qt::SolidPattern));
                else
                    painter.setBrush(QBrush(channelcolor, Qt::SolidPattern));
            }
            else if (channelcurrentvalue<channelstate2value)
            {
                if  (GetHalfSecFlag())
                    painter.setBrush(QBrush(ChannelColorLowState, Qt::SolidPattern));
                else
                    painter.setBrush(QBrush(channelcolor, Qt::SolidPattern));
            }
            else
            {
                painter.setBrush(QBrush(channelcolor, Qt::SolidPattern));
            }

            painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);

            QString ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);

            if (ChannelValueString == NaNMessage)
                ChannelValueString = ObryvErrorMessage;

            // выводим значения каналов большими цифрами
            painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignVCenter,ChannelValueString);

            // подписываем названия каналов
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignTop,Chanel->GetChannelName());

            //    // подписываем единицы измерения
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom,channel1object.GetUnitsName());

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

    channel1object.SetCurrentChannelValue( UartDriver::channelinputbuffer[0]);
    channel2object.SetCurrentChannelValue( UartDriver::channelinputbuffer[1]);
    channel3object.SetCurrentChannelValue( UartDriver::channelinputbuffer[2]);
    channel4object.SetCurrentChannelValue( UartDriver::channelinputbuffer[3]);

    // задаем координаты отображения квадратов
    channel1object.xposition = 0;
    channel1object.yposition = 0;
    channel1object.w = alertwindowwidth;
    channel1object.h = alertwindowheight;

    channel2object.xposition = alertwindowwidth;
    channel2object.yposition = 0;
    channel2object.w = alertwindowwidth;
    channel2object.h = alertwindowheight;

    channel3object.xposition = 0;
    channel3object.yposition = alertwindowheight;
    channel3object.w = alertwindowwidth;
    channel3object.h = alertwindowheight;

    channel4object.xposition = alertwindowwidth;
    channel4object.yposition = alertwindowheight;
    channel4object.w = alertwindowwidth;
    channel4object.h = alertwindowheight;

    // создаем лист объектов для его отображения на графике

    QList<ChannelOptions *> ChannelsObjectsList;

    ChannelsObjectsList.append(&channel1object);
    ChannelsObjectsList.append(&channel2object);
    ChannelsObjectsList.append(&channel3object);
    ChannelsObjectsList.append(&channel4object);

    painter.begin(ui->MessagesWidget);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));

    int confirmwindowwidth = widgwidth/3;
    int confirmwindowheight  = widgheight/3;
    int confirmwindowposx = (widgwidth -  confirmwindowwidth)/2;
    int confirmwindowposy = (widgheight -  confirmwindowheight)/2;
    int index = 0;

    // здесь собственно рисуем квадрат для каждого канала (в последствии можно будет добавить больше квадратов
    foreach (ChannelOptions * Chanel, ChannelsObjectsList) {
        {
            //            qDebug() << Chanel->GetChannelName();

            double channelcurrentvalue =Chanel->GetCurrentChannelValue();
            double channelstate1value = Chanel->GetState1Value();
            double channelstate2value = Chanel->GetState2Value();
            QColor channelcolor = Chanel->GetCurrentColor();

            // рисуем прямоугольник  с заполненным цветом

            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            if (channelcurrentvalue>channelstate1value)
            {
                if  (GetHalfSecFlag())
                    painter.setBrush(QBrush(ChannelColorHighState, Qt::SolidPattern));
                else
                    painter.setBrush(QBrush(channelcolor, Qt::SolidPattern));
            }
            else if (channelcurrentvalue<channelstate2value)
            {
                if  (GetHalfSecFlag())
                    painter.setBrush(QBrush(ChannelColorLowState, Qt::SolidPattern));
                else
                    painter.setBrush(QBrush(channelcolor, Qt::SolidPattern));
            }
            else
            {
                painter.setBrush(QBrush(channelcolor, Qt::SolidPattern));
            }

            painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);
            ////

            // рисуем текстовую инфу
            painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignTop, Chanel->GetChannelName());
            painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));

            if (channelcurrentvalue>channelstate1value)
            {
                painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom, Chanel->GetState1HighMessage());

                // часть квитирования
                if ( (Chanel->GetConfirmationNeed() == true) && (ui->ConfirmBox->isChecked()) )
                {
                    painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
                    painter.drawRect(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight);
                    painter.drawText(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight, Qt::AlignHCenter | Qt::AlignVCenter,Chanel->GetState1HighMessage());
                }
            }
            // уменьшение уставки  Channel 1
            else if (channelcurrentvalue<channelstate2value)
            {
                painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom, Chanel->GetState2LowMessage());
            }
            else
            {
                painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom, OKMessage);
            }

            if  (GetHalfSecFlag())
            {
                painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
                painter.setFont(QFont(Font, alerttextsize*2, QFont::ExtraBold));

                // если сработала какая-то уставка, то начинаем мигать восклицательным флагом
                if ((channelcurrentvalue>channelstate1value) || (channelcurrentvalue<channelstate2value))
                    painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignRight | Qt::AlignVCenter, AlertSign);
            }

            if (channelcurrentvalue<channelstate1value)
                Chanel->SetConfirmationNeed(true);
        }
        index++;
    }

    return;
}

void MainWindow::PaintPolarDiagramm()
{
    QPainter painter;
    double maximumradius;

    maximumradius = 400;

    painter.begin(ui->MessagesWidget);

    int channel1value = X_Coordinates.last();
    int channel2value = X_Coordinates.last();
    int channel3value = X_Coordinates.last();
    int channel4value = X_Coordinates.last();

    int channel1length = UartDriver::channelinputbuffer[0]/200*maximumradius;
    int channel2length = UartDriver::channelinputbuffer[1]/200*maximumradius;
    int channel3length = UartDriver::channelinputbuffer[2]/200*maximumradius;
    int channel4length = UartDriver::channelinputbuffer[3]/200*maximumradius;

    int centerx1,centerx2,centerx3,centerx4;
    int centery1,centery2,centery3,centery4;

    centerx1 = centerx2 =  300;
    centerx3 = centerx4 =  600;

    centery1 = centery3 =  200;
    centery2 = centery4 =  400;

    /* Create the line object: */
    QLineF Channel1Line;
    painter.setPen(QPen(Qt::green, 1)); //, Qt::DashDotLine, Qt::RoundCap));
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
    painter.drawLine(Channel2Line);
    painter.drawLine(Channel3Line);
    painter.drawLine(Channel4Line);

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

    int a = X_Coordinates.last();
    if ( a%360 == 0)
    {
        //X_Coordinates.clear();
        PolarChartPointsChannel1.clear(); //*** после продолжительной работы замедляется (тормзоит ) построение графика - проверить
        PolarChartPointsChannel2.clear();
        PolarChartPointsChannel3.clear();
        PolarChartPointsChannel4.clear();
    }

    painter.setPen(QPen(Channel1Color, 1));
    painter.drawPolyline(PolarChartPointsChannel1);
    painter.setPen(QPen(Channel2Color, 1));
    painter.drawPolyline(PolarChartPointsChannel2);
    painter.setPen(QPen(Channel3Color, 1));
    painter.drawPolyline(PolarChartPointsChannel3);
    painter.setPen(QPen(Channel4Color, 1));
    painter.drawPolyline(PolarChartPointsChannel4);
    painter.setRenderHint(QPainter::Antialiasing, false);

    painter.end();
}

void MainWindow::PaintOnWidget()
{
    switch( Options::DisplayParametr )
    {
    case Options::Cyfra:
        PaintCyfrasFullScreen();
        break;
    case Options::TrendsCyfra:
        PaintStatesAndAlertsAtTop();
        PaintCyfrasRight();
        break;
    case Options::Trends:
        PaintStatesAndAlertsAtTop();
        break;
    case Options::TrendsCyfraBars:
        PaintStatesAndAlertsAtTop();
        PaintCyfrasBottom();
        break;
    case Options::BarsCyfra:
        PaintStatesAndAlertsAtTop();
        PaintCyfrasBottom();
        break;
    case Options::Polar:
        PaintStatesAndAlertsAtTop();
        PaintCyfrasRight();
        PaintPolarDiagramm();
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
    // подкрашиваем квадраты куда коснулись в желтый цвет
    // отступ  сверху и слева в пикселях
    // высчитываются

    int xcenter  = 652;// общая точка четырех квадратов по иксу в пикселях
    int ycenter  = 384; // ширина всей области построения в пикселях

    int xpos = QCursor::pos().x();
    int ypos = QCursor::pos().y();

    QString x = QString::number(xpos);
    QString y = QString::number(ypos);

    SetChannel1Color(ChannelColorNormal);
    SetChannel2Color(Channel2ColorNormal);
    SetChannel3Color(Channel3ColorNormal);
    SetChannel4Color(Channel4ColorNormal);

    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях
    int confirmwindowwidth = widgwidth/4;
    int confirmwindowheight  = widgheight/4;
    int confirmwindowposx = (widgwidth -  confirmwindowwidth)/2;
    int confirmwindowposy = (widgheight -  confirmwindowheight)/2;
    int confirmwindowposx2 = confirmwindowposx  +  confirmwindowwidth;
    int confirmwindowposy2 = confirmwindowposy + confirmwindowheight ;

    if ((xpos < xcenter)&&(ypos < ycenter)) // если лев. верхн. квадрат
    {
        SetChannel1Color(Qt::yellow);
    }

    if ((xpos > xcenter)&& (ypos < ycenter)) // если правый верхний квадрат
    {
        SetChannel2Color(Qt::yellow);
    }

    if ((xpos < xcenter) && (ypos > ycenter)) // если левый нижний квадрат
    {
        SetChannel3Color(Qt::yellow);
    }

    if ((xpos > xcenter)&& (ypos > ycenter)) // если правый нижний квадрат
    {
        SetChannel4Color(Qt::yellow);
    }

    if      ((xpos>confirmwindowposx) &&
             (xpos<confirmwindowposx2) &&
             (ypos>confirmwindowposy) &&
             (ypos<confirmwindowposy2) )
    {
        if (channel4object.GetConfirmationNeed() == true)
        {
            channel4object.SetConfirmationNeed(false);
        }
        if (channel3object.GetConfirmationNeed() == true) {
            channel3object.SetConfirmationNeed(false);
        }
        if (channel2object.GetConfirmationNeed() == true) {
            channel2object.SetConfirmationNeed(false);
        }
        if (channel1object.GetConfirmationNeed() == true) {
            channel1object.SetConfirmationNeed(false);
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
            painter.setRenderHint(QPainter::Antialiasing, false);
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
