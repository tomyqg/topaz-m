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

extern QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;
extern cSystemOptions systemOptions;  //класс хранения состемных опций

void MainWindow::DrawRectangles(QList<ChannelOptions *> ChannelsObjectsList, int alerttextsize, int smalltextsize)
{
    painter.begin(ui->MessagesWidget);

    // здесь собственно рисуем квадрат для каждого канала (в последствии можно будет добавить больше квадратов
    int ch = 0;
    foreach (ChannelOptions * Chanel, ChannelsObjectsList) {
        {
            double channelcurrentvalue =Chanel->GetCurrentChannelValue();

            // рисуем прямоугольник  с заполненным цветом

            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            if (isChannelInMaxNow(ch))
            {
                painter.setBrush(QBrush(Chanel->GetMaximumColor(), Qt::SolidPattern));
            }
            else if (isChannelInMinNow(ch))
            {
                painter.setBrush(QBrush(Chanel->GetMinimumColor(), Qt::SolidPattern));
            }
            else
            {
                painter.setBrush(QBrush(Chanel->GetNormalColor(), Qt::SolidPattern));
            }

            painter.setBrush(QBrush(Chanel->GetStateDependentColor(), Qt::SolidPattern));
            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);


            QString ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);

            if (ChannelValueString == NaNMessage)
                ChannelValueString = ObryvErrorMessage;
            else
            {
//                if (!ui->percentCheckBox->checkState())
                    ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);
//                else
//                    ChannelValueString = QString::number( Chanel->GetValuePercent(), 'f', 1) + " %";
            }

            if (( isChannelInMaxNow(ch) || isChannelInMinNow(ch)) )
                painter.setPen(QPen(Qt::red, 1)); // делаем чтобы при срабатывании уставки включался только красный цвет
            else
                painter.setPen(QPen(Qt::black, 2)); // иначе черный цвет

            // выводим значения каналов большими цифрами
            painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignVCenter,ChannelValueString);


            // подписываем названия каналов
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignTop,Chanel->GetChannelName());

            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            // подписываем единицы измерения
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom,Chanel->GetUnitsName());

            // подписываем math, если канал математически обрабатывается
            painter.setPen(Qt::white);
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));

            if (Chanel->IsChannelMathematical())
                painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignRight | Qt::AlignTop, MathString);
            ////
            ch++;
        }
    }
    painter.end();
}

void MainWindow::PaintCyfrasBottom() // пока не используем
{
    // задается вручную
    int smallrectingleheight = 80; // высота прямоугольничка в пикселях задается вручную
    //высчитываются
    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях
    int smallrectinglewidth = widgwidth / 4; // ширина прямоугольничка в пикселях высчитывается
    int otstupsnizu = smallrectingleheight + 28+115;
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

    DrawRectangles(ChannelsObjectsList, alerttextsize, smalltextsize);
}


//void MainWindow::PaintCyfrasBottomSeparate()
//{
//    // задается вручную
//    int smallrectingleheight = 80; // высота прямоугольничка в пикселях задается вручную
//    //высчитываются
//    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
//    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях
//    int smallrectinglewidth = widgwidth / 4; // ширина прямоугольничка в пикселях высчитывается
//    int otstupsnizu = smallrectingleheight + 28;
//    int otstupsverhu = widgheight - otstupsnizu;
//    int alerttextsize = smallrectingleheight/2;
//    int smalltextsize = (smallrectingleheight - alerttextsize ) / 4;

//#ifdef Q_OS_WIN32
//    alerttextsize/=1.5;
//    smalltextsize/=1.2;
//#endif


//    // задаем координаты отображения квадратов
//    channel1.xposition = 0;
//    channel1.yposition = otstupsverhu;
//    channel1.w = smallrectinglewidth;
//    channel1.h = smallrectingleheight;

//    channel2.xposition = smallrectinglewidth;
//    channel2.yposition = otstupsverhu;
//    channel2.w = smallrectinglewidth;
//    channel2.h = smallrectingleheight;

//    channel3.xposition = smallrectinglewidth*2;
//    channel3.yposition = otstupsverhu;
//    channel3.w = smallrectinglewidth;
//    channel3.h = smallrectingleheight;

//    channel4.xposition = smallrectinglewidth*3;
//    channel4.yposition = otstupsverhu;
//    channel4.w = smallrectinglewidth;
//    channel4.h = smallrectingleheight;

//    // создаем лист объектов для его отображения на графике

//    QList<ChannelOptions *> ChannelsObjectsList;

//    ChannelsObjectsList.append(&channel1);
//    ChannelsObjectsList.append(&channel2);
//    ChannelsObjectsList.append(&channel3);
//    ChannelsObjectsList.append(&channel4);

//    QPainter* newpainter = new QPainter(ui->MessagesWidget);

////    newpainter->begin(ui->MessagesWidget);

//    // здесь собственно рисуем квадрат для каждого канала (в последствии можно будет добавить больше квадратов
//    int ch = 0;
//    foreach (ChannelOptions * Chanel, ChannelsObjectsList) {
//        {
//            double channelcurrentvalue =Chanel->GetCurrentChannelValue();

//            // рисуем прямоугольник  с заполненным цветом

//            newpainter->setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
//            if (isChannelInMaxNow(ch)/*Chanel->MaximumNow()*/)
//            {
//                newpainter->setBrush(QBrush(Chanel->GetMaximumColor(), Qt::SolidPattern));
//            }
//            else if (isChannelInMinNow(ch)/*Chanel->MinimumNow()*/)
//            {
//                newpainter->setBrush(QBrush(Chanel->GetMinimumColor(), Qt::SolidPattern));
//            }
//            else
//            {
//                newpainter->setBrush(QBrush(Chanel->GetNormalColor(), Qt::SolidPattern));
//            }

//            newpainter->setBrush(QBrush(Chanel->GetStateDependentColor(), Qt::SolidPattern));
//            newpainter->setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
//            newpainter->drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);


//            QString ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);

//            if (ChannelValueString == NaNMessage)
//                ChannelValueString = ObryvErrorMessage;
//            else
//            {
//                if (!ui->percentCheckBox->checkState())
//                    ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);
//                else
//                    ChannelValueString = QString::number( Chanel->GetValuePercent(), 'f', 1) + " %";
//            }

//            if (( isChannelInMaxNow(ch) || isChannelInMinNow(ch) /*Chanel->MinimumNow() || Chanel->MaximumNow()*/) )
//                newpainter->setPen(QPen(Qt::red, 1)); // делаем чтобы при срабатывании уставки включался только красный цвет
//            else
//                newpainter->setPen(QPen(Qt::black, 2)); // иначе черный цвет

//            // выводим значения каналов большими цифрами
//            newpainter->setFont(QFont(Font, alerttextsize, QFont::ExtraBold));
//            newpainter->drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignVCenter,ChannelValueString);


//            // подписываем названия каналов
//            newpainter->setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
//            newpainter->drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignTop,Chanel->GetChannelName());

//            newpainter->setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
//            // подписываем единицы измерения
//            newpainter->setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
//            newpainter->drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom,Chanel->GetUnitsName());

//            // подписываем math, если канал математически обрабатывается
//            newpainter->setPen(Qt::white);
//            newpainter->setFont(QFont(Font, smalltextsize, QFont::ExtraBold));

//            if (Chanel->IsChannelMathematical())
//                newpainter->drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignRight | Qt::AlignTop, MathString);
//            ////
//            ch++;
//        }
//    }
//    newpainter->end();
//}

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

    DrawRectangles(ChannelsObjectsList, alerttextsize, smalltextsize);
}

void MainWindow::PaintCyfrasNew()
{
    //задается вручную
    //высчитываются

    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях
    int smallrectingleheight = widgheight/4-9; // высота прямоугольничка в пикселях задается вручную
    int smallrectinglewidth = 350; // ширина прямоугольничка в пикселях задаётся вручную
    int otstupsnizu = smallrectingleheight + 37;
    int otstupsverhu = widgheight - otstupsnizu; //  widgheight/12*2-otstupsnizu;
    int otstupsverhu1 = otstupsverhu-smallrectingleheight;
    int otstupsverhu2 = otstupsverhu-smallrectingleheight*2;
    int otstupsverhu3 = otstupsverhu-smallrectingleheight*3;
    //    int alerttextsize = smallrectingleheight/2;
    int alerttextsize = 46;
    int smalltextsize = 12;

    //#ifdef Q_OS_WIN32
    alerttextsize/=1.5;
    smalltextsize/=1.2;
    //#endif

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
    int ch = 0;
    foreach (ChannelOptions * Chanel, ChannelsObjectsList) {
        {
            double channelcurrentvalue =Chanel->GetCurrentChannelValue();

            // рисуем прямоугольник  с заполненным цветом

            painter.setPen(QPen(Qt::white, 2)); //, Qt::DashDotLine, Qt::RoundCap));

            if (isChannelInMaxNow(ch)/*Chanel->MaximumNow()*/)
            {
                painter.setBrush(QBrush(Chanel->GetMaximumColor(), Qt::SolidPattern));
            }
            else if (isChannelInMinNow(ch)/*Chanel->MinimumNow()*/)
            {
                painter.setBrush(QBrush(Chanel->GetMinimumColor(), Qt::SolidPattern));
            }
            else
            {
                painter.setBrush(QBrush(Chanel->GetNormalColor(), Qt::SolidPattern));
            }
            painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);


            QString ChannelValueString ;

//            if (!ui->percentCheckBox->checkState())
                ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);
//            else
//                ChannelValueString = QString::number( Chanel->GetValuePercent(), 'f', 1) + " %";

            if (ChannelValueString == NaNMessage)
                ChannelValueString = ObryvErrorMessage;
            else
            {
//                if (!ui->percentCheckBox->checkState())
                    ChannelValueString = QString::number( channelcurrentvalue, 'f', 2);
//                else
//                    ChannelValueString = QString::number( Chanel->GetValuePercent(), 'f', 1) + " %";
            }

            painter.setPen(QPen(Qt::black, 3)); //, Qt::DashDotLine, Qt::RoundCap));

            painter.setBrush(QBrush( Chanel->GetNormalColor(), Qt::NoBrush));
            painter.drawRect(Chanel->xposition, Chanel->yposition, smallrectinglewidth, 35); // верхний квадрат
            painter.drawRect(Chanel->xposition, Chanel->yposition+35, smallrectinglewidth/2, smallrectingleheight-35); // рисуем левый нижний квадратик
            painter.drawRect(Chanel->xposition + smallrectinglewidth/2, Chanel->yposition+35, smallrectinglewidth/2, smallrectingleheight-35);// рисуем правый нижний квадратик

            painter.setPen(QPen(Qt::white, 3)); //, Qt::DashDotLine, Qt::RoundCap));

            // выводим значения каналов большими цифрами
            if (ChannelValueString.length()>6)
                painter.setFont(QFont(Font, alerttextsize/1.2, QFont::ExtraBold));
            else  if (ChannelValueString.length()<6)
                painter.setFont(QFont(Font, alerttextsize*1.2, QFont::ExtraBold));
            else
                painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));

            if (( isChannelInMaxNow(ch) || isChannelInMinNow(ch)/*Chanel->MinimumNow() || Chanel->MaximumNow()*/) )
            {
                painter.setPen(QPen(Qt::red, 1)); //, Qt::DashDotLine, Qt::RoundCap));
            }

            painter.drawText(Chanel->xposition, Chanel->yposition+35, smallrectinglewidth/2, smallrectingleheight-35, Qt::AlignHCenter | Qt::AlignVCenter,ChannelValueString);
            painter.setPen(QPen(Qt::white, 2)); //, Qt::DashDotLine, Qt::RoundCap));


            // подписываем названия каналов
            painter.setFont(QFont(Font, 12, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition, smallrectinglewidth, 35, Qt::AlignHCenter | Qt::AlignVCenter,Chanel->GetChannelName());

            //    // подписываем единицы измерения
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
            painter.drawText(Chanel->xposition, Chanel->yposition+35, smallrectinglewidth/2, smallrectingleheight-35, Qt::AlignHCenter | Qt::AlignBottom,Chanel->GetUnitsName());

            //    // подписываем math, если канал математически обрабатывается
            painter.setPen(Qt::white);
            painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));

            //

            QString statemessage;
            statemessage = "" ;


            if (( isChannelInMaxNow(ch) || isChannelInMinNow(ch)/*Chanel->MinimumNow() || Chanel->MaximumNow()*/) )
            {
                painter.setPen(QPen(Qt::red, 1)); //, Qt::DashDotLine, Qt::RoundCap));
                if (isChannelInMaxNow(ch)/*Chanel->MaximumNow()*/)
                {
                    statemessage = Chanel->GetState1HighMessage();
                }
                // уменьшение уставки  Channel 1
                else if (isChannelInMinNow(ch)/*Chanel->MinimumNow()*/)
                {
                    statemessage = Chanel->GetState2LowMessage();
                }
            }
            else
            {
                painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
                statemessage = OKMessage;
            }

            //            painter.setFont(QFont(Font, smallrectinglewidth/2/( statemessage.length() + 1 ) , QFont::ExtraBold));

            // выводим сработки уставок

            if (statemessage.length()>8)
            {
                if (statemessage.length()>12)
                    statemessage.insert(12, "\n");
                if (statemessage.length()>24)
                    statemessage.insert(24, "\n");
                if (statemessage.length()>36)
                    statemessage.insert(36, "\n");
                painter.setFont(QFont(Font, 14, QFont::ExtraBold));
            }
            else  if (statemessage.length()<6)
                painter.setFont(QFont(Font, 30, QFont::ExtraBold));
            else
                painter.setFont(QFont(Font, 20, QFont::ExtraBold));

            painter.drawText(Chanel->xposition + smallrectinglewidth/2, Chanel->yposition+35, smallrectinglewidth/2, smallrectingleheight-35, Qt::AlignCenter, statemessage);
            ch++;
        }
    }
    painter.end();
}

void MainWindow::PaintCyfrasFullScreen()
{
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

    DrawRectangles(ChannelsObjectsList, alerttextsize, smalltextsize);
}

void MainWindow::DrawAlertsAndStatesRectangles(QList<ChannelOptions *> ChannelsObjectsList, int smalltextsize, int alerttextsize)
{
    painter.begin(ui->MessagesWidget);

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));

    // здесь собственно рисуем квадрат для каждого канала (в последствии можно будет добавить больше квадратов
    int ch = 0;
    foreach (ChannelOptions * Chanel, ChannelsObjectsList) {
        {

            // рисуем прямоугольник  с заполненным цветом

            painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
            if (isChannelInMaxNow(ch)/*Chanel->MaximumNow()*/)
            {
                painter.setBrush(QBrush(Chanel->GetMaximumColor(), Qt::SolidPattern));
                painter.drawRect(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h);
            }
            else if (isChannelInMinNow(ch)/*Chanel->MinimumNow()*/)
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

            if ( ( isChannelInMaxNow(ch) || isChannelInMinNow(ch)/*Chanel->MinimumNow() || Chanel->MaximumNow()*/) )
            {
                painter.setPen(QPen(Qt::red, 1)); //, Qt::DashDotLine, Qt::RoundCap));
            }

            if (isChannelInMaxNow(ch)/*Chanel->MaximumNow()*/)
            {
                painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom, Chanel->GetState1HighMessage());
            }
            // уменьшение уставки  Channel 1
            else if (isChannelInMinNow(ch)/*Chanel->MinimumNow()*/)
            {
                painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom, Chanel->GetState2LowMessage());
            }
            else
            {
                painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
                painter.drawText(Chanel->xposition, Chanel->yposition, Chanel->w, Chanel->h, Qt::AlignHCenter | Qt::AlignBottom, tr(OKMessage));
            }
            ch++;
       }
    }
    painter.end();
}

void MainWindow::PaintStatesAndAlertsAtTop() // отрисовывает события на уставки
{
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
    
    DrawAlertsAndStatesRectangles(ChannelsObjectsList, smalltextsize, alerttextsize);

    return;
}

// полярные координаты
void MainWindow::PaintPolarDiagramm()
{
//    QPainter painter;
    
    painter.begin(ui->MessagesWidget);
    int widgetHeight = ui->MessagesWidget->height();
    int widgetWidth = ui->MessagesWidget->width();
    int widgetSize = min(widgetHeight, widgetWidth);
    int diagramSize = widgetSize - 50;

    painter.setRenderHint(QPainter::Antialiasing, true);

    int channel1value = GetPolarAngle();
    int channel2value = GetPolarAngle();
    int channel3value = GetPolarAngle();
    int channel4value = GetPolarAngle();

    //получение текущих значений в процентах
    float channel1length = channel1.GetValuePercent();
    float channel2length = channel2.GetValuePercent();
    float channel3length = channel3.GetValuePercent();
    float channel4length = channel4.GetValuePercent();

    //корректировка данных для диаграммы
    if(channel1length < 0) channel1length = 0;
    if(channel2length < 0) channel2length = 0;
    if(channel3length < 0) channel3length = 0;
    if(channel4length < 0) channel4length = 0;
    if(channel1length > 100) channel1length = 100;
    if(channel2length > 100) channel2length = 100;
    if(channel3length > 100) channel3length = 100;
    if(channel4length > 100) channel4length = 100;

    //перевод процентов в координатную размерность
    channel1length = channel1length * diagramSize / 100;
    channel2length = channel2length * diagramSize / 100;
    channel3length = channel3length * diagramSize / 100;
    channel4length = channel4length * diagramSize / 100;

    QColor color1,color2,color3,color4;

    color1 = channel1.GetStateDependentColor();
    color2 = channel2.GetStateDependentColor();
    color3 = channel3.GetStateDependentColor();
    color4 = channel4.GetStateDependentColor();
    
#define SHIFT_DIAGRAM_X 5
#define SHIFT_DIAGRAM_Y (-5)

    int centerx1,centerx2,centerx3,centerx4;
    int centery1,centery2,centery3,centery4;
    int newxcenter = SHIFT_DIAGRAM_X, newycenter = widgetHeight + SHIFT_DIAGRAM_Y;
    
    centerx1 = centerx2 = centerx3 = centerx4 = 1;
    centery1 = centery2 = centery3 = centery4 = 1;
    
    painter.translate(newxcenter, newycenter);
    /* Create the line object: */
//    painter.setPen(QPen(Qt::black, 1));

    if (GetEcoMode()){
        painter.setBrush(QBrush(EcoColor, Qt::SolidPattern));
        painter.setPen(QPen(NotEcoColor,1,  Qt::DotLine));
    }
    else
    {
        painter.setBrush(QBrush(NotEcoColor, Qt::SolidPattern));
        painter.setPen(QPen(EcoColor,1,  Qt::DotLine));
    }
//    painter.drawRect(0, -newycenter,ui->MessagesWidget->width()-1,ui->MessagesWidget->height());
    painter.drawLine(0, -diagramSize - 20, 0, 0);
    painter.drawLine(0, 0, diagramSize + 40, 0);
    
    // Рисуем пять дуг разметочки
    painter.drawArc(-diagramSize, -diagramSize, 2*diagramSize, 2*diagramSize, 0, 90 * 16); // 100%
    painter.drawArc(-diagramSize*4/5, -diagramSize*4/5, 2*diagramSize*4/5, 2*diagramSize*4/5, 0, 90 * 16);// 80%
    painter.drawArc(-diagramSize*3/5, -diagramSize*3/5, 2*diagramSize*3/5, 2*diagramSize*3/5, 0, 90 * 16);// 60%
    painter.drawArc(-diagramSize*2/5, -diagramSize*2/5, 2*diagramSize*2/5, 2*diagramSize*2/5, 0, 90 * 16);// 40%
    painter.drawArc(-diagramSize/5, -diagramSize/5, 2*diagramSize/5, 2*diagramSize/5, 0, 90 * 16);// 20%

    //ещё 5 дуг дополнительно
    painter.drawArc(-diagramSize*9/10, -diagramSize*9/10, \
                    2*diagramSize*9/10, 2*diagramSize*9/10, 0, 90 * 16); // 100%
    painter.drawArc(-diagramSize*7/10, -diagramSize*7/10, \
                    2*diagramSize*7/10, 2*diagramSize*7/10, 0, 90 * 16);// 80%
    painter.drawArc(-diagramSize*5/10, -diagramSize*5/10, \
                    2*diagramSize*5/10, 2*diagramSize*5/10, 0, 90 * 16);// 60%
    painter.drawArc(-diagramSize*3/10, -diagramSize*3/10, \
                    2*diagramSize*3/10, 2*diagramSize*3/10, 0, 90 * 16);// 40%
    painter.drawArc(-diagramSize*1/10, -diagramSize*1/10, \
                    2*diagramSize*1/10, 2*diagramSize*1/10, 0, 90 * 16);// 20%

    QLineF Channel1Line;
    
    //очистка хвостов
    if (GetPolarAngle()>=90) // если больше 90 градусов то поворачиваем диск
    {
        painter.rotate(GetPolarAngle()-90);
        if (GetPolarAngle()>=179)   // оставляем только 180 точек (на 180 градусов)
        {
            PolarChartPointsChannel1.removeFirst();
            PolarChartPointsChannel2.removeFirst();
            PolarChartPointsChannel3.removeFirst();
            PolarChartPointsChannel4.removeFirst();
            //SetPolarAngle(180);
        }
    }
    

    // радиальные линии
    for(int i=0; i < 36; i++)
    {
        QLineF Line;
        Line.setP1(QPointF(centerx1, centery1));
        Line.setAngle(10*(i));
        Line.setLength(diagramSize);
        painter.drawLine(Line);
    }
    
    // отсюда начинаем рисовать значения на круговой диаграмме
//    painter.setPen(QPen(Qt::green, 1));
    /* Set the origin: */
    Channel1Line.setP1(QPointF(centerx1, centery1));
    Channel1Line.setAngle(channel1value);
    Channel1Line.setLength(channel1length);
    
    int x1 = Channel1Line.x2(); // мы берем координаты `1 точки
    int y1 = Channel1Line.y2(); // мы берем координаты второй точки
    
    /* Create the line object: */
    QLineF Channel2Line;
//    painter.setPen(QPen(Qt::green, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel2Line.setP1(QPointF(centerx2, centery2));
    Channel2Line.setAngle(channel2value);
    Channel2Line.setLength(channel2length);
    
    int x2 = Channel2Line.x2(); // мы берем координаты `1 точки
    int y2 = Channel2Line.y2(); // мы берем координаты второй точки
    
    /* Create the line object: */
    QLineF Channel3Line;
//    painter.setPen(QPen(Qt::green, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel3Line.setP1(QPointF(centerx3, centery3));
    Channel3Line.setAngle(channel3value);
    Channel3Line.setLength(channel3length);
    int x3 = Channel3Line.x2(); // мы берем координаты `1 точки
    int y3 = Channel3Line.y2(); // мы берем координаты второй точки
    
    /* Create the line object: */
    QLineF Channel4Line;
//    painter.setPen(QPen(Qt::green, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel4Line.setP1(QPointF(centerx4, centery4) );
    Channel4Line.setAngle(channel4value);
    Channel4Line.setLength(channel4length);
    
    int x4 = Channel4Line.x2(); // мы берем координаты `1 точки
    int y4 = Channel4Line.y2(); // мы берем координаты второй точки
    

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
    if(PolarChartPointsChannel1.size() > 1)
        painter.drawPolyline(PolarChartPointsChannel1);
    painter.setPen(QPen(color2, 2));
    if(PolarChartPointsChannel2.size() > 1)
        painter.drawPolyline(PolarChartPointsChannel2);
    painter.setPen(QPen(color3, 2));
    if(PolarChartPointsChannel3.size() > 1)
        painter.drawPolyline(PolarChartPointsChannel3);
    painter.setPen(QPen(color4, 2));
    if(PolarChartPointsChannel4.size() > 1)
        painter.drawPolyline(PolarChartPointsChannel4);

    painter.resetTransform(); // все что дальше - не поворачивается динамически

    painter.setFont(QFont(Font, 10, QFont::Bold));
    painter.translate(newxcenter, newycenter);

    painter.drawText(0,               -30, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "0%");
    painter.drawText(diagramSize/5,   -30, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "20%");
    painter.drawText(diagramSize*2/5, -30, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "40%");
    painter.drawText(diagramSize*3/5, -30, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "60%");
    painter.drawText(diagramSize*4/5, -30, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "80%");
    painter.drawText(diagramSize,     -30, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "100%");

    painter.drawText(0, -30-diagramSize/5,   40, 40, Qt::AlignHCenter | Qt::AlignVCenter, "20%");
    painter.drawText(0, -30-diagramSize*2/5, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "40%");
    painter.drawText(0, -30-diagramSize*3/5, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "60%");
    painter.drawText(0, -30-diagramSize*4/5, 40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "80%");
    painter.drawText(0, -30-diagramSize,     40, 40, Qt::AlignHCenter | Qt::AlignVCenter,  "100%");

    //стилизация круговой диаграммы (короче понты всякие)
    //вертикальные понты
    painter.setPen(ColorBlue);
    painter.setBrush(QBrush(ColorBlue, Qt::SolidPattern));
    painter.drawRect(0, 0, -SHIFT_DIAGRAM_X, -diagramSize/5);
    painter.setPen(ColorCh1);
    painter.setBrush(QBrush(ColorCh1, Qt::SolidPattern));
    painter.drawRect(0, -diagramSize/5, -SHIFT_DIAGRAM_X, -diagramSize/5);
    painter.setPen(ColorCh1Light);
    painter.setBrush(QBrush(ColorCh1Light, Qt::SolidPattern));
    painter.drawRect(0, -diagramSize*2/5, -SHIFT_DIAGRAM_X, -diagramSize/5);
    painter.setPen(ColorCh3);
    painter.setBrush(QBrush(ColorCh3, Qt::SolidPattern));
    painter.drawRect(0, -diagramSize*3/5, -SHIFT_DIAGRAM_X, -diagramSize/5);
    painter.setPen(ColorCh3Light);
    painter.setBrush(QBrush(ColorCh3Light, Qt::SolidPattern));
    painter.drawRect(0, -diagramSize*4/5, -SHIFT_DIAGRAM_X, -diagramSize/5);
    //горизонтальные понты
    painter.setPen(ColorBlue);
    painter.setBrush(QBrush(ColorBlue, Qt::SolidPattern));
    painter.drawRect(0, 0, diagramSize/5, -SHIFT_DIAGRAM_Y);
    painter.setPen(ColorCh1);
    painter.setBrush(QBrush(ColorCh1, Qt::SolidPattern));
    painter.drawRect(diagramSize/5, 0, diagramSize/5, -SHIFT_DIAGRAM_Y);
    painter.setPen(ColorCh1Light);
    painter.setBrush(QBrush(ColorCh1Light, Qt::SolidPattern));
    painter.drawRect(diagramSize*2/5, 0, diagramSize/5, -SHIFT_DIAGRAM_Y);
    painter.setPen(ColorCh3);
    painter.setBrush(QBrush(ColorCh3, Qt::SolidPattern));
    painter.drawRect(diagramSize*3/5, 0, diagramSize/5, -SHIFT_DIAGRAM_Y);
    painter.setPen(ColorCh3Light);
    painter.setBrush(QBrush(ColorCh3Light, Qt::SolidPattern));
    painter.drawRect(diagramSize*4/5, 0, diagramSize/5, -SHIFT_DIAGRAM_Y);
    //специальный белый квадратик в начале осей координат
    painter.setPen(Qt::white);
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(0, 0, -SHIFT_DIAGRAM_X, -SHIFT_DIAGRAM_Y);

    painter.end();
}

void MainWindow::PaintOnWidget()
{
    switch(systemOptions.display)
    {
    case cSystemOptions::Cyfra:
        PaintCyfrasFullScreen();
        break;
    case cSystemOptions::TrendsCyfra:
        PaintCyfrasRight();
        break;
    case cSystemOptions::Trends:
        PaintStatesAndAlertsAtTop();
        break;
    case cSystemOptions::TrendsCyfraBars:
        PaintStatesAndAlertsAtTop();
        break;
    case cSystemOptions::BarsCyfra:
        PaintStatesAndAlertsAtTop();
        break;
    case cSystemOptions::PolarBars:
    case cSystemOptions::PolarCyfra:
//        PaintStatesAndAlertsAtTop();
        PaintPolarDiagramm();
        break;
    default:
        break;
    }
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
