#include "updategraph.h"
#include "mainwindow.h"
#include "uartdriver.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "metrologicalcalc.h"
#include "mathresolver.h"
#include "options.h"

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

//#define ConnectionErrorMessage "ошибка\nсоединения"
//#define CRCErrorMessage "ошибка конт.\nсуммы"
//#define ObryvErrorMessage "обрыв\nдатчика"

#define ConnectionErrorMessage "ош.соед"
#define CRCErrorMessage "ош.crc"
#define ObryvErrorMessage "обрыв"

#define NaNMessage "nan"

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

    QString Channel1ValueString,Channel2ValueString,Channel3ValueString,Channel4ValueString ;

    painter.begin(ui->MessagesWidget);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 1)); //, Qt::DashDotLine, Qt::RoundCap));

    ModBus mb;

    {
        painter.setBrush(QBrush(GetChannel1Color(), Qt::Dense4Pattern));
        painter.drawRect(2, otstupsverhu, smallrectinglewidth, smallrectingleheight);
        painter.setBrush(QBrush(GetChannel2Color(), Qt::Dense4Pattern));
        painter.drawRect(2+smallrectinglewidth, otstupsverhu, smallrectinglewidth, smallrectingleheight);
        painter.setBrush(QBrush(GetChannel3Color(), Qt::Dense4Pattern));
        painter.drawRect(2+smallrectinglewidth*2, otstupsverhu, smallrectinglewidth, smallrectingleheight);
        painter.setBrush(QBrush(GetChannel4Color(), Qt::Dense4Pattern));
        painter.drawRect(2+smallrectinglewidth*3, otstupsverhu, smallrectinglewidth-4, smallrectingleheight);
    }


        painter.setFont(QFont("Times New Roman", 50, QFont::ExtraBold));


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
    painter.setFont(QFont("Times New Roman", 15, QFont::ExtraBold));
    painter.drawText(2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignTop, channel1object.GetChannelName());
    painter.drawText(2+smallrectinglewidth, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignTop,channel2object.GetChannelName());
    painter.drawText(2+smallrectinglewidth*2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignTop,channel3object.GetChannelName());
    painter.drawText(2+smallrectinglewidth*3, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignTop,channel4object.GetChannelName());

    // подписываем единицы измерения
    painter.setFont(QFont("Times New Roman", 12, QFont::ExtraBold));
    painter.drawText(2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignBottom, channel1object.GetUnitsName());
    painter.drawText(2+smallrectinglewidth, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignBottom,channel2object.GetUnitsName());
    painter.drawText(2+smallrectinglewidth*2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignBottom,channel3object.GetUnitsName());
    painter.drawText(2+smallrectinglewidth*3, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignBottom,channel4object.GetUnitsName());

    // подписываем букву m если канал математически обрабатывается
    painter.setPen(Qt::darkRed);
    painter.setFont(QFont("Times New Roman", 12, QFont::ExtraBold));
    if (channel1object.IsChannelMathematical())
        painter.drawText(2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignRight | Qt::AlignTop, "math ");
    if (channel2object.IsChannelMathematical())
        painter.drawText(2+smallrectinglewidth, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignRight | Qt::AlignTop,"math ");
    if (channel3object.IsChannelMathematical())
        painter.drawText(2+smallrectinglewidth*2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignRight | Qt::AlignTop,"math ");
    if (channel4object.IsChannelMathematical())
        painter.drawText(2+smallrectinglewidth*3, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignRight | Qt::AlignTop,"math ");

    painter.end();

    mb.deleteLater();
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



    //высчитываются

    int widgwidth  = ui->MessagesWidget->width()-borderwidth;// высота всей области построения в пикселях
    int widgheight  = ui->MessagesWidget->height()- borderwidth; // ширина всей области построения в пикселях
    int bigrectingleheight = (widgheight)/2-2; // высота прямоугольничка в пикселях задается вручную
    int bigrectinglewidth = (widgwidth)/2-2; // ширина прямоугольничка в пикселях высчитывается

    painter.begin(ui->MessagesWidget);
    //    painter.setRenderHint(QPainter::NonCosmeticDefaultPen, true);
    painter.setPen(QPen(Qt::black, 4)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.setBrush(QBrush(GetChannel1Color(), Qt::SolidPattern));
    painter.drawRect(borderwidth, borderwidth, bigrectinglewidth, bigrectingleheight);
    painter.setBrush(QBrush(GetChannel2Color(), Qt::SolidPattern));
    painter.drawRect(borderwidth+bigrectinglewidth, borderwidth, bigrectinglewidth, bigrectingleheight);
    painter.setBrush(QBrush(GetChannel3Color(), Qt::SolidPattern));
    painter.drawRect(borderwidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight);
    painter.setBrush(QBrush(GetChannel4Color(), Qt::SolidPattern));
    painter.drawRect(borderwidth+bigrectinglewidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight);

    painter.setPen(QPen(Qt::cyan, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.setFont(QFont("Times New Roman", 110, QFont::ExtraBold));
    painter.drawText(borderwidth, borderwidth, bigrectinglewidth, bigrectingleheight,     Qt::AlignHCenter | Qt::AlignVCenter,Channel1ValueString);
    painter.drawText(borderwidth+bigrectinglewidth, borderwidth, bigrectinglewidth, bigrectingleheight, Qt::AlignHCenter | Qt::AlignVCenter,Channel2ValueString);
    painter.drawText(borderwidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignHCenter | Qt::AlignVCenter,Channel3ValueString);
    painter.drawText(borderwidth+bigrectinglewidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignHCenter | Qt::AlignVCenter,Channel4ValueString);

    painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.setFont(QFont("Times New Roman", 50, QFont::ExtraBold));

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
    painter.setFont(QFont("Times New Roman", 15, QFont::ExtraBold));
    if (channel1object.IsChannelMathematical())
        painter.drawText(borderwidth, borderwidth, bigrectinglewidth, bigrectingleheight, Qt::AlignRight | Qt::AlignTop, "math ");
    if (channel2object.IsChannelMathematical())
        painter.drawText(borderwidth+bigrectinglewidth, borderwidth, bigrectinglewidth, bigrectingleheight, Qt::AlignRight | Qt::AlignTop,"math ");
    if (channel3object.IsChannelMathematical())
        painter.drawText(borderwidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignRight | Qt::AlignTop,"math ");
    if (channel4object.IsChannelMathematical())
        painter.drawText(borderwidth+bigrectinglewidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignRight | Qt::AlignTop,"math ");

    painter.setFont(QFont("Times New Roman", 30, QFont::ExtraBold));
    painter.drawText(borderwidth, borderwidth, bigrectinglewidth, bigrectingleheight, Qt::AlignLeft | Qt::AlignBottom, " 1");
    painter.drawText(borderwidth+bigrectinglewidth, borderwidth, bigrectinglewidth, bigrectingleheight, Qt::AlignLeft | Qt::AlignBottom," 2");
    painter.drawText(borderwidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignLeft | Qt::AlignBottom," 3");
    painter.drawText(borderwidth+bigrectinglewidth, borderwidth+bigrectingleheight, bigrectinglewidth, bigrectingleheight, Qt::AlignLeft | Qt::AlignBottom," 4");

    painter.end();
}

void MainWindow::PaintStatesAndAlertsAtTop() // отрисовывает события на уставки
{
    QPainter painter;

    int smallrectingleheight = 100; // высота прямоугольничка в пикселях задается вручную
    //высчитываются
    int widgwidth  = ui->MessagesWidget->width();// высота всей области построения в пикселях
    int widgheight  = ui->MessagesWidget->height(); // ширина всей области построения в пикселях
    int smallrectinglewidth = widgwidth / 4; // ширина прямоугольничка в пикселях высчитывается
    int otstupsnizu = smallrectingleheight + 24;
    int otstupsverhu = widgheight - otstupsnizu;

    int alertwindowwidth = widgwidth/2-2;
    //    int alertwindowheight = widgheight/2/4-2;
    int alertwindowheight  = 60;
    int alerttextsize = 30;

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
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 4)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.setBrush(QBrush(GetChannel1Color(), Qt::SolidPattern));
    painter.drawRect(2, 2, alertwindowwidth, alertwindowheight);
    painter.setBrush(QBrush(GetChannel2Color(), Qt::SolidPattern));
    painter.drawRect(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight);
    painter.setBrush(QBrush(GetChannel3Color(), Qt::SolidPattern));
    painter.drawRect(2, 2+alertwindowheight, alertwindowwidth, alertwindowheight);
    painter.setBrush(QBrush(GetChannel4Color(), Qt::SolidPattern));
    painter.drawRect(2+alertwindowwidth, 2+alertwindowheight, alertwindowwidth, alertwindowheight);

    painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
    painter.setFont(QFont("Times New Roman", 12, QFont::ExtraBold));
    painter.drawText(2, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignTop, channel1object.GetChannelName());
    painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignTop,channel2object.GetChannelName());
    painter.drawText(2, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignTop,channel3object.GetChannelName());
    painter.drawText(2+alertwindowwidth, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignTop,channel4object.GetChannelName());

    painter.setFont(QFont("Times New Roman", alerttextsize, QFont::ExtraBold));


    int confirmwindowwidth = widgwidth/4;
    int confirmwindowheight  = widgheight/4;
    int confirmwindowposx = (widgwidth -  confirmwindowwidth)/2;
    int confirmwindowposy = (widgheight -  confirmwindowheight)/2;
    //    int confirmwindowposx2 = confirmwindowposx  +  confirmwindowwidth;
    //    int confirmwindowposy2 = confirmwindowposy + confirmwindowheight ;

    // увеличение уставки Channel 1
    if (channel1currentvalue>channel1state1value)
    {
        painter.drawText(2, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel1object.GetState1HighMessage());
        SetChannel1Color(ChannelColorHighState);

        if ( (channel1object.GetConfirmationNeed() == true) && (ui->ConfirmBox->isChecked()) )
        {
            painter.setBrush(QBrush(Qt::blue, Qt::Dense2Pattern));
            painter.drawRect(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight);
            painter.drawText(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight, Qt::AlignHCenter | Qt::AlignVCenter,channel1object.GetState1HighMessage());
        }
    }
    // уменьшение уставки  Channel 1
    else if (channel1currentvalue<channel1state2value)
    {
        painter.drawText(2, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel1object.GetState2LowMessage());
        SetChannel1Color(ChannelColorLowState);
    }
    else
    {
        painter.drawText(2, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, "Ok");
        SetChannel1Color(Channel1ColorNormal);
    }

    // увеличение уставки Channel 2
    if (channel2currentvalue>channel2state1value)
    {
        painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel2object.GetState1HighMessage());
        SetChannel2Color(ChannelColorHighState);

        if ( (channel2object.GetConfirmationNeed() == true) && (ui->ConfirmBox->isChecked()) )
        {
            painter.setBrush(QBrush(Qt::blue, Qt::Dense2Pattern));
            painter.drawRect(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight);
            painter.drawText(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight, Qt::AlignHCenter | Qt::AlignVCenter,channel2object.GetState1HighMessage());
        }

    }
    // уменьшение уставки  Channel 2
    else if (channel2currentvalue<channel2state2value)
    {
        painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel2object.GetState2LowMessage());
        SetChannel2Color(ChannelColorLowState);
    }

    else
    {
        painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, "Ok");
        SetChannel2Color(Channel2ColorNormal);
    }

    // увеличение уставки Channel 3
    if (channel3currentvalue>channel3state1value)
    {
        painter.drawText(2, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel3object.GetState1HighMessage());
        SetChannel3Color(ChannelColorHighState);

        if ( (channel3object.GetConfirmationNeed() == true) && (ui->ConfirmBox->isChecked()) )
        {
            painter.setBrush(QBrush(Qt::blue, Qt::Dense2Pattern));
            painter.drawRect(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight);
            painter.drawText(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight, Qt::AlignHCenter | Qt::AlignVCenter,channel3object.GetState1HighMessage());
        }
    }
    // уменьшение уставки  Channel 3
    else if (channel3currentvalue<channel3state2value)
    {
        painter.drawText(2, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel3object.GetState2LowMessage());
        SetChannel3Color(ChannelColorLowState);
    }
    else
    {
        painter.drawText(2, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, "Ok");
        SetChannel3Color(Channel3ColorNormal);
    }

    // увеличение уставки Channel 4
    if (channel4currentvalue>channel4state1value)
    {
        painter.drawText(2+alertwindowwidth, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel4object.GetState1HighMessage());
        SetChannel4Color(ChannelColorHighState);

        if ( (channel4object.GetConfirmationNeed() == true) && (ui->ConfirmBox->isChecked()) )
        {
            painter.setBrush(QBrush(Qt::blue, Qt::Dense2Pattern));
            painter.drawRect(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight);
            painter.drawText(confirmwindowposx, confirmwindowposy, confirmwindowwidth, confirmwindowheight, Qt::AlignHCenter | Qt::AlignVCenter,channel4object.GetState1HighMessage());
        }
    }
    // уменьшение уставки  Channel 4
    else if (channel4currentvalue<channel4state2value)
    {
        painter.drawText(2+alertwindowwidth, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, channel4object.GetState2LowMessage());
        SetChannel4Color(ChannelColorLowState);
    }
    else
    {
        painter.drawText(2+alertwindowwidth, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignHCenter | Qt::AlignBottom, "Ok");
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

    if  (GetHalfSecFlag() == 1)
    {
        painter.setPen(QPen(Qt::white, 1)); //, Qt::DashDotLine, Qt::RoundCap));
        painter.setFont(QFont("Times New Roman", 40, QFont::ExtraBold));

        // если сработала какая-то уставка, то начинаем мигать восклицательным флагом
        if ((channel1currentvalue>channel1state1value) || (channel1currentvalue<channel1state2value))
            painter.drawText(2, 2, alertwindowwidth, alertwindowheight, Qt::AlignRight | Qt::AlignVCenter, "!");
        if ((channel2currentvalue>channel2state1value) || (channel2currentvalue<channel2state2value))
            painter.drawText(2+alertwindowwidth, 2, alertwindowwidth, alertwindowheight, Qt::AlignRight | Qt::AlignVCenter,"!");
        if ((channel3currentvalue>channel3state1value) || (channel3currentvalue<channel3state2value))
            painter.drawText(2, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignRight | Qt::AlignVCenter,"!");
        if ((channel4currentvalue>channel4state1value) || (channel4currentvalue<channel4state2value))
            painter.drawText(2+alertwindowwidth, 2+alertwindowheight, alertwindowwidth, alertwindowheight, Qt::AlignRight | Qt::AlignVCenter,"!");
    }

    // здесь мы рисуем квитирование если что...
    return;
    // дальше идет проверка связи, пока отключим
    ModBus mb;
    // если связб плохая и прошло пол секунды то нужно мигнуть красным цветом
    if ( (GetHalfSecFlag() == 1)&&(mb.GetConnectFailureStatus() >0) )
    {
        QString Channel1ValueString,Channel2ValueString,Channel3ValueString,Channel4ValueString ;

        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.drawRect(2, otstupsverhu, smallrectinglewidth, smallrectingleheight);
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.drawRect(2+smallrectinglewidth, otstupsverhu, smallrectinglewidth, smallrectingleheight);
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.drawRect(2+smallrectinglewidth*2, otstupsverhu, smallrectinglewidth, smallrectingleheight);
        painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter.drawRect(2+smallrectinglewidth*3, otstupsverhu, smallrectinglewidth-4, smallrectingleheight);

        painter.setFont(QFont("Times New Roman", 25, QFont::ExtraBold));

        if ( mb.GetConnectFailureStatus()  == 1 )
            Channel1ValueString = "Connection Fail";
        if ( mb.GetConnectFailureStatus()  == 2 )
            Channel2ValueString = "Connection Fail";
        if ( mb.GetConnectFailureStatus()  == 3 )
            Channel3ValueString = "Connection Fail";
        if ( mb.GetConnectFailureStatus()  == 4 )
            Channel4ValueString = "Connection Fail";

        // выводим значения каналов большими цифрами
        painter.drawText(2, otstupsverhu, smallrectinglewidth, smallrectingleheight,     Qt::AlignHCenter | Qt::AlignVCenter,Channel1ValueString);
        painter.drawText(2+smallrectinglewidth, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignVCenter,Channel2ValueString);
        painter.drawText(2+smallrectinglewidth*2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignVCenter,Channel3ValueString);
        painter.drawText(2+smallrectinglewidth*3, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignVCenter,Channel4ValueString);

        // подписываем названия каналов
        painter.setFont(QFont("Times New Roman", 15, QFont::ExtraBold));
        painter.drawText(2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignTop, channel1object.GetChannelName());
        painter.drawText(2+smallrectinglewidth, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignTop,channel2object.GetChannelName());
        painter.drawText(2+smallrectinglewidth*2, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignTop,channel3object.GetChannelName());
        painter.drawText(2+smallrectinglewidth*3, otstupsverhu, smallrectinglewidth, smallrectingleheight, Qt::AlignHCenter | Qt::AlignTop,channel4object.GetChannelName());
    }
    painter.end();

    mb.deleteLater();
}

void MainWindow::PaintPolarDiagramm()
{
    QPainter painter;
    int widgheight  = ui->MessagesWidget->height();
    double maximumradius;

    maximumradius = widgheight/4 - 10;

    painter.begin(ui->MessagesWidget);

    int channel1value = X_Coordinates.last();
    int channel2value = X_Coordinates.last();
    int channel3value = X_Coordinates.last();
    int channel4value = X_Coordinates.last();

    int channel1length = UartDriver::channelinputbuffer[0]/200*maximumradius;
    int channel2length = UartDriver::channelinputbuffer[1]/200*maximumradius;
    int channel3length = UartDriver::channelinputbuffer[2]/200*maximumradius;
    int channel4length = UartDriver::channelinputbuffer[3]/200*maximumradius;

    /* Create the line object: */
    QLineF Channel1Line;
    painter.setPen(QPen(Qt::green, 4)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel1Line.setP1(QPointF(widgheight/2-2, maximumradius + 10));
    Channel1Line.setAngle(channel1value);
    Channel1Line.setLength(channel1length);

    int x1 = Channel1Line.x2(); // мы берем координаты `1 точки
    int y1 = Channel1Line.y2(); // мы берем координаты второй точки

    /* Create the line object: */
    QLineF Channel2Line;
    painter.setPen(QPen(Qt::green, 4)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel2Line.setP1(QPointF(widgheight-2, maximumradius + 10));
    Channel2Line.setAngle(channel2value);
    Channel2Line.setLength(channel2length);
    int x2 = Channel2Line.x2(); // мы берем координаты `1 точки
    int y2 = Channel2Line.y2(); // мы берем координаты второй точки

    /* Create the line object: */
    QLineF Channel3Line;
    painter.setPen(QPen(Qt::green, 4)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel3Line.setP1(QPointF(widgheight/2-2, widgheight - (maximumradius + 10)));
    Channel3Line.setAngle(channel3value);
    Channel3Line.setLength(channel3length);
    int x3 = Channel3Line.x2(); // мы берем координаты `1 точки
    int y3 = Channel3Line.y2(); // мы берем координаты второй точки

    /* Create the line object: */
    QLineF Channel4Line;
    painter.setPen(QPen(Qt::green, 4)); //, Qt::DashDotLine, Qt::RoundCap));
    /* Set the origin: */
    Channel4Line.setP1(QPointF(widgheight-2,  widgheight - (maximumradius + 10)) );
    Channel4Line.setAngle(channel4value);
    Channel4Line.setLength(channel4length);

    int x4 = Channel4Line.x2(); // мы берем координаты `1 точки
    int y4 = Channel4Line.y2(); // мы берем координаты второй точки

    //qDebug() << Channel1Line.y2(); // мы берем координаты второй точки

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

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(QPen(Channel1Color, 4));
    painter.drawPolyline(PolarChartPointsChannel1);
    painter.setPen(QPen(Channel2Color, 4));
    painter.drawPolyline(PolarChartPointsChannel2);
    painter.setPen(QPen(Channel3Color, 4));
    painter.drawPolyline(PolarChartPointsChannel3);
    painter.setPen(QPen(Channel4Color, 4));
    painter.drawPolyline(PolarChartPointsChannel4);
    painter.setRenderHint(QPainter::Antialiasing, false);

    painter.end();
}

void MainWindow::PaintOnWidget()
{
    switch( Options::DisplayParametr )
    {
    case Options::Cyfra:
        PaintCyfrasFullScreen();break;
    case Options::TrendsCyfra:
        PaintStatesAndAlertsAtTop();PaintCyfrasBottom();break;
    case Options::Trends:
        PaintStatesAndAlertsAtTop();PaintStatesAndAlertsAtTop();break;
    case Options::TrendsCyfraBars:
        PaintStatesAndAlertsAtTop();PaintCyfrasBottom();break;
    case Options::BarsCyfra:
        PaintStatesAndAlertsAtTop();PaintCyfrasBottom();break;
    case Options::Polar:
        PaintStatesAndAlertsAtTop();PaintPolarDiagramm();break;
    default:
        break;
    }
}

void MainWindow::ReactOnMouseSlide()
{
    int  y   =  QCursor::pos().y() ;
    //    int  x   =  QCursor::pos().x() ;
    int ky = 20 + y/3;

    ui->customPlot->yAxis->setRange(-ky, ky);
}

void MainWindow::ReactOnTouch()
{
    // подкрашиваем квадраты куда коснулись в желтый цвет
    // отступ  сверху и слева в пикселях
    //высчитываются
    //    int xcenter  = ui->MessagesWidget->width()-borderwidth/2;// высота всей области построения в пикселях
    //    int ycenter  = ui->MessagesWidget->height()- borderwidth/2; // ширина всей области построения в пикселях

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

    channel1object.SetChannelName("x : " + x + "; Y : " + y);

    // проверка если ткнули пальцем в окно квитирования

    if ((xpos>confirmwindowposx) &&
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
}
