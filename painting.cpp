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
    smalltextsize/=1.5;
#endif

    QString Channel1ValueString,Channel2ValueString,Channel3ValueString,Channel4ValueString ;
    painter.begin(ui->MessagesWidget);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));

    {
        painter.setBrush(QBrush(GetChannel1Color(), Qt::SolidPattern));
        painter.drawRect(2, otstupsverhu, smallrectinglewidth, smallrectingleheight);
        painter.setBrush(QBrush(GetChannel2Color(), Qt::SolidPattern));
        painter.drawRect(2+smallrectinglewidth, otstupsverhu, smallrectinglewidth, smallrectingleheight);
        painter.setBrush(QBrush(GetChannel3Color(), Qt::SolidPattern));
        painter.drawRect(2+smallrectinglewidth*2, otstupsverhu, smallrectinglewidth, smallrectingleheight);
        painter.setBrush(QBrush(GetChannel4Color(), Qt::SolidPattern));
        painter.drawRect(2+smallrectinglewidth*3, otstupsverhu, smallrectinglewidth-4, smallrectingleheight);
    }

    painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));

    double Channel1ValueNumber =  UartDriver::channelinputbuffer[0];
    double Channel2ValueNumber =  UartDriver::channelinputbuffer[1];
    double Channel3ValueNumber =  UartDriver::channelinputbuffer[2];
    double Channel4ValueNumber =  UartDriver::channelinputbuffer[3];

    Channel1ValueString = QString::number( Channel1ValueNumber, 'f', 2);
    Channel2ValueString = QString::number( Channel2ValueNumber, 'f', 2);
    Channel3ValueString = QString::number( Channel3ValueNumber, 'f', 2);
    Channel4ValueString = QString::number( Channel4ValueNumber, 'f', 2);

    if (Channel1ValueString== NaNMessage)
        Channel1ValueString = ObryvErrorMessage;
    if (Channel2ValueString== NaNMessage)
        Channel2ValueString = ObryvErrorMessage;
    if (Channel3ValueString== NaNMessage)
        Channel3ValueString = ObryvErrorMessage;
    if (Channel4ValueString== NaNMessage)
        Channel4ValueString = ObryvErrorMessage;

    if (Channel1ValueNumber== ModBus::ConnectionError)
        Channel1ValueString = ConnectionErrorMessage;
    if (Channel2ValueNumber== ModBus::ConnectionError)
        Channel2ValueString =  ConnectionErrorMessage;
    if (Channel3ValueNumber== ModBus::ConnectionError)
        Channel3ValueString =  ConnectionErrorMessage;
    if (Channel4ValueNumber== ModBus::ConnectionError)
        Channel4ValueString = ConnectionErrorMessage;


    // выводим значения каналов большими цифрами
    painter.drawText(2, otstupsverhu, smallrectinglewidth, smallrectingleheight,     Qt::AlignHCenter | Qt::AlignVCenter,Channel1ValueString);
    painter.drawText(2+smallrectinglewidth, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignVCenter,Channel2ValueString);
    painter.drawText(2+smallrectinglewidth*2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignVCenter,Channel3ValueString);
    painter.drawText(2+smallrectinglewidth*3, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignVCenter,Channel4ValueString);

    // подписываем названия каналов
    painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
    painter.drawText(2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignTop, channel1object.GetChannelName());
    painter.drawText(2+smallrectinglewidth, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignTop,channel2object.GetChannelName());
    painter.drawText(2+smallrectinglewidth*2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignTop,channel3object.GetChannelName());
    painter.drawText(2+smallrectinglewidth*3, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignTop,channel4object.GetChannelName());

    // подписываем единицы измерения
    painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
    painter.drawText(2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignBottom, channel1object.GetUnitsName());
    painter.drawText(2+smallrectinglewidth, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignBottom,channel2object.GetUnitsName());
    painter.drawText(2+smallrectinglewidth*2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignBottom,channel3object.GetUnitsName());
    painter.drawText(2+smallrectinglewidth*3, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignBottom,channel4object.GetUnitsName());

    // подписываем букву m если канал математически обрабатывается
    painter.setPen(Qt::darkRed);
    painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
    if (channel1object.IsChannelMathematical())
        painter.drawText(2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignRight | Qt::AlignTop, MathString);
    if (channel2object.IsChannelMathematical())
        painter.drawText(2+smallrectinglewidth, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignRight | Qt::AlignTop,MathString);
    if (channel3object.IsChannelMathematical())
        painter.drawText(2+smallrectinglewidth*2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignRight | Qt::AlignTop,MathString);
    if (channel4object.IsChannelMathematical())
        painter.drawText(2+smallrectinglewidth*3, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignRight | Qt::AlignTop,MathString);

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
    smalltextsize/=1.5;
#endif

    QString Channel1ValueString,Channel2ValueString,Channel3ValueString,Channel4ValueString ;

    painter.begin(ui->MessagesWidget);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));

    painter.setBrush(QBrush(GetChannel1Color(), Qt::SolidPattern));
    painter.drawRect(rect1coords);
    painter.setBrush(QBrush(GetChannel2Color(), Qt::SolidPattern));
    painter.drawRect(rect2coords);
    painter.setBrush(QBrush(GetChannel3Color(), Qt::SolidPattern));
    painter.drawRect(rect3coords);
    painter.setBrush(QBrush(GetChannel4Color(), Qt::SolidPattern));
    painter.drawRect(rect4coords);

    painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));

    double Channel1ValueNumber =  UartDriver::channelinputbuffer[0];
    double Channel2ValueNumber =  UartDriver::channelinputbuffer[1];
    double Channel3ValueNumber =  UartDriver::channelinputbuffer[2];
    double Channel4ValueNumber =  UartDriver::channelinputbuffer[3];

    Channel1ValueString = QString::number( Channel1ValueNumber, 'f', 2);
    Channel2ValueString = QString::number( Channel2ValueNumber, 'f', 2);
    Channel3ValueString = QString::number( Channel3ValueNumber, 'f', 2);
    Channel4ValueString = QString::number( Channel4ValueNumber, 'f', 2);

    if (Channel1ValueString == NaNMessage)
        Channel1ValueString = ObryvErrorMessage;
    if (Channel2ValueString == NaNMessage)
        Channel2ValueString = ObryvErrorMessage;
    if (Channel3ValueString == NaNMessage)
        Channel3ValueString = ObryvErrorMessage;
    if (Channel4ValueString == NaNMessage)
        Channel4ValueString = ObryvErrorMessage;

    if (Channel1ValueNumber == ModBus::ConnectionError)
        Channel1ValueString = ConnectionErrorMessage;
    if (Channel2ValueNumber == ModBus::ConnectionError)
        Channel2ValueString =  ConnectionErrorMessage;
    if (Channel3ValueNumber == ModBus::ConnectionError)
        Channel3ValueString =  ConnectionErrorMessage;
    if (Channel4ValueNumber == ModBus::ConnectionError)
        Channel4ValueString = ConnectionErrorMessage;

    // выводим значения каналов большими цифрами
    painter.drawText(rect1coords, Qt::AlignHCenter | Qt::AlignVCenter,Channel1ValueString);
    painter.drawText(rect2coords, Qt::AlignHCenter | Qt::AlignVCenter,Channel2ValueString);
    painter.drawText(rect3coords, Qt::AlignHCenter | Qt::AlignVCenter,Channel3ValueString);
    painter.drawText(rect4coords, Qt::AlignHCenter | Qt::AlignVCenter,Channel4ValueString);

    // подписываем названия каналов
    painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
    painter.drawText(rect1coords, Qt::AlignHCenter | Qt::AlignTop,channel1object.GetChannelName());
    painter.drawText(rect2coords, Qt::AlignHCenter | Qt::AlignTop,channel2object.GetChannelName());
    painter.drawText(rect3coords, Qt::AlignHCenter | Qt::AlignTop,channel3object.GetChannelName());
    painter.drawText(rect4coords, Qt::AlignHCenter | Qt::AlignTop,channel4object.GetChannelName());

    // подписываем единицы измерения
    painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
    painter.drawText(rect1coords, Qt::AlignHCenter | Qt::AlignBottom,channel1object.GetUnitsName());
    painter.drawText(rect2coords, Qt::AlignHCenter | Qt::AlignBottom,channel2object.GetUnitsName());
    painter.drawText(rect3coords, Qt::AlignHCenter | Qt::AlignBottom,channel3object.GetUnitsName());
    painter.drawText(rect4coords, Qt::AlignHCenter | Qt::AlignBottom,channel4object.GetUnitsName());

    // подписываем букву m если канал математически обрабатывается
    painter.setPen(Qt::white);
    painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
    if (channel1object.IsChannelMathematical())
        painter.drawText(rect1coords, Qt::AlignRight | Qt::AlignTop, MathString);
    if (channel2object.IsChannelMathematical())
        painter.drawText(rect2coords, Qt::AlignRight | Qt::AlignTop,MathString);
    if (channel3object.IsChannelMathematical())
        painter.drawText(rect3coords, Qt::AlignRight | Qt::AlignTop,MathString);
    if (channel4object.IsChannelMathematical())
        painter.drawText(rect4coords, Qt::AlignRight | Qt::AlignTop,MathString);

    painter.end();
}

void MainWindow::PaintCyfrasFullScreen()
{
    QPainter painter;
    // задаётся параметры вручную

    // отступ  сверху и слева в пикселях
    int borderwidth = 0 ;

    QString Channel1ValueString,Channel2ValueString,Channel3ValueString,Channel4ValueString ;

    // берем текущее значение каждого канала и округляем 2 знака после запятой
    double Channel1ValueNumber =  UartDriver::channelinputbuffer[0];
    double Channel2ValueNumber =  UartDriver::channelinputbuffer[1];
    double Channel3ValueNumber =  UartDriver::channelinputbuffer[2];
    double Channel4ValueNumber =  UartDriver::channelinputbuffer[3];

    Channel1ValueString = QString::number( Channel1ValueNumber, 'f', 2);
    Channel2ValueString = QString::number( Channel2ValueNumber, 'f', 2);
    Channel3ValueString = QString::number( Channel3ValueNumber, 'f', 2);
    Channel4ValueString = QString::number( Channel4ValueNumber, 'f', 2);

    if (Channel1ValueString == NaNMessage)
        Channel1ValueString = ObryvErrorMessage;
    if (Channel2ValueString == NaNMessage)
        Channel2ValueString = ObryvErrorMessage;
    if (Channel3ValueString == NaNMessage)
        Channel3ValueString = ObryvErrorMessage;
    if (Channel4ValueString == NaNMessage)
        Channel4ValueString = ObryvErrorMessage;

    if (Channel1ValueNumber == ModBus::ConnectionError)
        Channel1ValueString = ConnectionErrorMessage;
    if (Channel2ValueNumber == ModBus::ConnectionError)
        Channel2ValueString =  ConnectionErrorMessage;
    if (Channel3ValueNumber == ModBus::ConnectionError)
        Channel3ValueString =  ConnectionErrorMessage;
    if (Channel4ValueNumber == ModBus::ConnectionError)
        Channel4ValueString = ConnectionErrorMessage;

    //высчитываются
    int widgwidth = ui->MessagesWidget->width()-borderwidth;// высота всей области построения в пикселях
    int widgheight = ui->MessagesWidget->height()- borderwidth; // ширина всей области построения в пикселях
    int bigrectingleheight = (widgheight)/2-2; // высота прямоугольничка в пикселях задается вручную
    int bigrectinglewidth = (widgwidth)/2-2; // ширина прямоугольничка в пикселях высчитывается


    int alerttextsize = bigrectingleheight/2.8;
    int smalltextsize = (bigrectingleheight - alerttextsize ) / 5;

#ifdef Q_OS_WIN32
    alerttextsize/=1.5;
    smalltextsize/=1.5;
#endif

    painter.begin(ui->MessagesWidget);
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(QBrush(GetChannel1Color(), Qt::SolidPattern));
    painter.drawRect(borderwidth, borderwidth, bigrectinglewidth, bigrectingleheight);
    painter.setBrush(QBrush(GetChannel2Color(), Qt::SolidPattern));
    painter.drawRect(borderwidth+bigrectinglewidth, borderwidth, bigrectinglewidth, bigrectingleheight);
    painter.setBrush(QBrush(GetChannel3Color(), Qt::SolidPattern));
    painter.drawRect(borderwidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight);
    painter.setBrush(QBrush(GetChannel4Color(), Qt::SolidPattern));
    painter.drawRect(borderwidth+bigrectinglewidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight);

    painter.setPen(QPen(Qt::cyan, 1));
    painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));
    painter.drawText(borderwidth, borderwidth, bigrectinglewidth, bigrectingleheight,     Qt::AlignHCenter | Qt::AlignVCenter,Channel1ValueString);
    painter.drawText(borderwidth+bigrectinglewidth, borderwidth, bigrectinglewidth, bigrectingleheight, Qt::AlignHCenter | Qt::AlignVCenter,Channel2ValueString);
    painter.drawText(borderwidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignHCenter | Qt::AlignVCenter,Channel3ValueString);
    painter.drawText(borderwidth+bigrectinglewidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignHCenter | Qt::AlignVCenter,Channel4ValueString);

    painter.setPen(QPen(Qt::white, 1));
    painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));

    painter.drawText(borderwidth, borderwidth, bigrectinglewidth, bigrectingleheight, Qt::AlignHCenter | Qt::AlignTop, channel1object.GetChannelName());
    painter.drawText(borderwidth+bigrectinglewidth, borderwidth, bigrectinglewidth, bigrectingleheight, Qt::AlignHCenter | Qt::AlignTop,channel2object.GetChannelName());
    painter.drawText(borderwidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignHCenter | Qt::AlignTop,channel3object.GetChannelName());
    painter.drawText(borderwidth+bigrectinglewidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignHCenter | Qt::AlignTop,channel4object.GetChannelName());

    painter.drawText(borderwidth, borderwidth, bigrectinglewidth, bigrectingleheight, Qt::AlignHCenter | Qt::AlignBottom, channel1object.GetUnitsName());
    painter.drawText(borderwidth+bigrectinglewidth, borderwidth, bigrectinglewidth, bigrectingleheight, Qt::AlignHCenter | Qt::AlignBottom,channel2object.GetUnitsName());
    painter.drawText(borderwidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignHCenter | Qt::AlignBottom,channel3object.GetUnitsName());
    painter.drawText(borderwidth+bigrectinglewidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignHCenter | Qt::AlignBottom,channel4object.GetUnitsName());

    // подписываем букву m если канал математически обрабатывается
    painter.setPen(Qt::red);
    painter.setFont(QFont(Font, smalltextsize/2, QFont::ExtraBold));
    if (channel1object.IsChannelMathematical())
        painter.drawText(borderwidth, borderwidth, bigrectinglewidth, bigrectingleheight, Qt::AlignRight | Qt::AlignTop, MathString);
    if (channel2object.IsChannelMathematical())
        painter.drawText(borderwidth+bigrectinglewidth, borderwidth, bigrectinglewidth, bigrectingleheight, Qt::AlignRight | Qt::AlignTop,MathString);
    if (channel3object.IsChannelMathematical())
        painter.drawText(borderwidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignRight | Qt::AlignTop,MathString);
    if (channel4object.IsChannelMathematical())
        painter.drawText(borderwidth+bigrectinglewidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignRight | Qt::AlignTop,MathString);

    painter.setFont(QFont(Font, smalltextsize/2, QFont::ExtraBold));
    painter.drawText(borderwidth, borderwidth, bigrectinglewidth, bigrectingleheight, Qt::AlignLeft | Qt::AlignBottom, " 1");
    painter.drawText(borderwidth+bigrectinglewidth, borderwidth, bigrectinglewidth, bigrectingleheight, Qt::AlignLeft | Qt::AlignBottom," 2");
    painter.drawText(borderwidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignLeft | Qt::AlignBottom," 3");
    painter.drawText(borderwidth+bigrectinglewidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignLeft | Qt::AlignBottom," 4");

    painter.end();
}

void MainWindow::PaintStatesAndAlertsAtTop() // отрисовывает события на уставки
{
    QPainter painter;

    //высчитываются
    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях

    int alertwindowwidth = widgwidth/2-2;
    int alertwindowheight  = widgheight/12;
    int alerttextsize = alertwindowheight/2;
    int smalltextsize = (alertwindowheight - alerttextsize)/2.5;

#ifdef Q_OS_WIN32
    alerttextsize/=1.5;
    smalltextsize/=1.5;
#endif

    double channel1currentvalue = UartDriver::channelinputbuffer[0];
    double channel2currentvalue = UartDriver::channelinputbuffer[1];
    double channel3currentvalue = UartDriver::channelinputbuffer[2];
    double channel4currentvalue = UartDriver::channelinputbuffer[3];

    double channel1state1value = channel1object.GetState1Value();
    double channel2state1value = channel2object.GetState1Value();
    double channel3state1value = channel3object.GetState1Value();
    double channel4state1value = channel4object.GetState1Value();

    double channel1state2value = channel1object.GetState2Value();
    double channel2state2value = channel2object.GetState2Value();
    double channel3state2value = channel3object.GetState2Value();
    double channel4state2value = channel4object.GetState2Value();

    painter.begin(ui->MessagesWidget);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(QPen(Qt::black, 2)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.setBrush(QBrush(GetChannel1Color(), Qt::SolidPattern));
    painter.drawRect(2, 2, alertwindowwidth, alertwindowheight);
    painter.setBrush(QBrush(GetChannel2Color(), Qt::SolidPattern));
    painter.drawRect(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight);
    painter.setBrush(QBrush(GetChannel3Color(), Qt::SolidPattern));
    painter.drawRect(2, 2+alertwindowheight, alertwindowwidth, alertwindowheight);
    painter.setBrush(QBrush(GetChannel4Color(), Qt::SolidPattern));
    painter.drawRect(2+alertwindowwidth, 2+alertwindowheight, alertwindowwidth, alertwindowheight);

    painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.setFont(QFont(Font, smalltextsize, QFont::ExtraBold));
    painter.drawText(2, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignTop, channel1object.GetChannelName());
    painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignTop,channel2object.GetChannelName());
    painter.drawText(2, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignTop,channel3object.GetChannelName());
    painter.drawText(2+alertwindowwidth, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignTop,channel4object.GetChannelName());

    painter.setFont(QFont(Font, alerttextsize, QFont::ExtraBold));

    int confirmwindowwidth = widgwidth/3;
    int confirmwindowheight  = widgheight/3;
    int confirmwindowposx = (widgwidth -  confirmwindowwidth)/2;
    int confirmwindowposy = (widgheight -  confirmwindowheight)/2;
    //    int confirmwindowposx2 = confirmwindowposx  +  confirmwindowwidth;
    //    int confirmwindowposy2 = confirmwindowposy + confirmwindowheight ;

    // увеличение уставки Channel 1
    if (channel1currentvalue>channel1state1value)
    {
        painter.drawText(2, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel1object.GetState1HighMessage());
        if  (GetHalfSecFlag())
            SetChannel1Color(ChannelColorHighState);
        else
            SetChannel1Color(Channel1ColorNormal);

        if ( (channel1object.GetConfirmationNeed() == true) && (ui->ConfirmBox->isChecked()) )
        {
            painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
            painter.drawRect(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight);
            painter.drawText(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight, Qt::AlignHCenter | Qt::AlignVCenter,channel1object.GetState1HighMessage());
        }
    }
    // уменьшение уставки  Channel 1
    else if (channel1currentvalue<channel1state2value)
    {
        painter.drawText(2, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel1object.GetState2LowMessage());
        if  (GetHalfSecFlag())
            SetChannel1Color(ChannelColorLowState);
        else
            SetChannel1Color(Channel1ColorNormal);

    }
    else
    {
        painter.drawText(2, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, OKMessage);
        SetChannel1Color(Channel1ColorNormal);
    }

    // увеличение уставки Channel 2
    if (channel2currentvalue>channel2state1value)
    {
        painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel2object.GetState1HighMessage());
        if  (GetHalfSecFlag())
            SetChannel2Color(ChannelColorHighState);
        else
            SetChannel2Color(Channel2ColorNormal);

        if ( (channel2object.GetConfirmationNeed() == true) && (ui->ConfirmBox->isChecked()) )
        {
            painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
            painter.drawRect(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight);
            painter.drawText(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight, Qt::AlignHCenter | Qt::AlignVCenter,channel2object.GetState1HighMessage());
        }
    }
    // уменьшение уставки  Channel 2
    else if (channel2currentvalue<channel2state2value)
    {
        painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel2object.GetState2LowMessage());
        if  (GetHalfSecFlag())
            SetChannel2Color(ChannelColorLowState);
        else
            SetChannel2Color(Channel1ColorNormal);
    }

    else
    {
        painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, OKMessage);
        SetChannel2Color(Channel2ColorNormal);
    }

    // увеличение уставки Channel 3
    if (channel3currentvalue>channel3state1value)
    {
        painter.drawText(2, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel3object.GetState1HighMessage());
        if  (GetHalfSecFlag())
            SetChannel3Color(ChannelColorHighState);
        else
            SetChannel3Color(Channel3ColorNormal);

        if ( (channel3object.GetConfirmationNeed() == true) && (ui->ConfirmBox->isChecked()) )
        {
            painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
            painter.drawRect(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight);
            painter.drawText(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight, Qt::AlignHCenter | Qt::AlignVCenter,channel3object.GetState1HighMessage());
        }
    }
    // уменьшение уставки  Channel 3
    else if (channel3currentvalue<channel3state2value)
    {
        painter.drawText(2, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel3object.GetState2LowMessage());
        if  (GetHalfSecFlag())
            SetChannel3Color(ChannelColorLowState);
        else
            SetChannel3Color(Channel3ColorNormal);
    }
    else
    {
        painter.drawText(2, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, OKMessage);
        SetChannel3Color(Channel3ColorNormal);
    }

    // увеличение уставки Channel 4
    if (channel4currentvalue>channel4state1value)
    {
        painter.drawText(2+alertwindowwidth, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel4object.GetState1HighMessage());
        if  (GetHalfSecFlag())
            SetChannel4Color(ChannelColorHighState);
        else
            SetChannel4Color(Channel4ColorNormal);

        if ( (channel4object.GetConfirmationNeed() == true) && (ui->ConfirmBox->isChecked()) )
        {
            painter.setBrush(QBrush(Qt::blue, Qt::SolidPattern));
            painter.drawRect(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight);
            painter.drawText(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight, Qt::AlignHCenter | Qt::AlignVCenter,channel4object.GetState1HighMessage());
        }
    }
    // уменьшение уставки  Channel 4
    else if (channel4currentvalue<channel4state2value)
    {
        painter.drawText(2+alertwindowwidth, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel4object.GetState2LowMessage());
        if  (GetHalfSecFlag())
            SetChannel4Color(ChannelColorLowState);
        else
            SetChannel4Color(Channel4ColorNormal);
    }
    else
    {
        painter.drawText(2+alertwindowwidth, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, OKMessage);
        SetChannel4Color(Channel4ColorNormal);
    }
    //здесь мы возвращаем окна квитирования

    if (channel1currentvalue<channel1state1value)
        channel1object.SetConfirmationNeed(true);

    if (channel2currentvalue<channel2state1value)
        channel2object.SetConfirmationNeed(true);

    if (channel3currentvalue<channel3state1value)
        channel3object.SetConfirmationNeed(true);

    if (channel4currentvalue<channel4state1value)
        channel4object.SetConfirmationNeed(true);

    if  (GetHalfSecFlag())
    {
        painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
        painter.setFont(QFont(Font, alerttextsize*2, QFont::ExtraBold));

        // если сработала какая-то уставка, то начинаем мигать восклицательным флагом
        if ((channel1currentvalue>channel1state1value) || (channel1currentvalue<channel1state2value))
            painter.drawText(2, 2, alertwindowwidth, alertwindowheight, Qt::AlignRight | Qt::AlignVCenter, AlertSign);
        if ((channel2currentvalue>channel2state1value) || (channel2currentvalue<channel2state2value))
            painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight, Qt::AlignRight | Qt::AlignVCenter,AlertSign);
        if ((channel3currentvalue>channel3state1value) || (channel3currentvalue<channel3state2value))
            painter.drawText(2, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignRight | Qt::AlignVCenter,AlertSign);
        if ((channel4currentvalue>channel4state1value) || (channel4currentvalue<channel4state2value))
            painter.drawText(2+alertwindowwidth, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignRight | Qt::AlignVCenter,AlertSign);
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

    SetChannel1Color(Channel1ColorNormal);
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
