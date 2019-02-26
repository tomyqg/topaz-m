#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "options.h"
#include "messages.h"
#include "keyboard.h"
#include "mathresolver.h"
#include "Channels/channelOptions.h"
#include "uartdriver.h"
#include "filemanager.h"
#include "Channels/group_channels.h"
#include "device_slot.h"

#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QPen>
#include <QVector>
#include <QFile>
#include <QDataStream>
#include <QPainterPath>
#include <QPainter>
#include <QDateTime>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QThread>
#include <QPoint>


QString inputstr = "";
QDateTime start(QDateTime::currentDateTime());
QString MainWindow::startdate = start.toString("dd/MM/yy");
QString MainWindow::starttime = start.toString("hh:mm:ss");
QString MainWindow::endtime = "";
QVector<QDateTime> MainWindow::Dates;

QList<cDevice*> listDevice;
QList<ChannelOptions *> listChannels;
QList<Ustavka *> listUstavok;
QList<cGroupChannels*> listGroup;
QList<cSteel*> listSteel;
QList<cRelay*> listRelais;
typeSteelTech steelTech[NUM_TECHNOLOGIES];
cSystemOptions systemOptions;  //класс хранения состемных опций
cIpController * ethernet;
QList<cMathChannel*> listMath;


extern QColor Channel1Color;
extern QColor Channel2Color;
extern QColor Channel3Color;
extern QColor Channel4Color;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindowInitialization();
}

MainWindow::~MainWindow()
{
//    messwrite.LogAddMessage("Programm Closed");
//    messwrite.WriteAllLogToFile();
    logger->addMess("Programm Closed", cLogger::STATISTIC);
    delete timeUpdUst;
    delete ui;
}


void MainWindow::updateDateLabel()
{
    DateUpdate();

    // между делом обновляем лэйбл и стрелки переключения групп
    if(listGroup.size() > 1) {
        ui->nameGroupChannels->setText(listGroup.at(curGroupChannel)->groupName);
        ui->arrowGroupLeft->setPixmap(QPixmap(pathtoleftarrow));
        ui->arrowGroupRight->setPixmap(QPixmap(pathtorightarrow));
        ui->frameGroup->show();
    }
    else ui->frameGroup->hide();
}

void MainWindow::on_WorkButton_clicked()
{
    dMenu * menu = (dMenu*)dialogMenu;
    connect(menu, SIGNAL(saveButtonSignal()), this, SLOT(updateSystemOptions()));
    //сигнал из меню о создании новой уставки
    connect(menu, SIGNAL(newUstavka(int)), this, SLOT(newUstavkaConnect(int)));
    menu->selectPageWork();
    menu->exec();
//    cLogger * log = new cLogger(pathtomessages, cLogger::UI);
    logger->addMess("Menu > Open ", cLogger::SERVICE, cLogger::UI);
    sendConfigChannelsToSlave();
    setTextBars();
    setStyleBars();
    updateWidgetsVols();
}

void MainWindow::on_ArchiveButton_clicked()
{
//    OpenArchiveWindow();
    if(systemOptions.display == cSystemOptions::Steel)
    {
        dialogSetingsChannel = new dSettings(listChannels, 1, 5, arch);
    }
    else
    {
        dialogSetingsChannel = new dSettings(listChannels, 1, 2, arch);
    }
//    dialogSetingsChannel->addArch(arch);
    dialogSetingsChannel->exec();
    dialogSetingsChannel->deleteLater();
}


//void MainWindow::on_EcoCheckBox_toggled(bool checked)
//{
//    SetEcoMode(checked);

//    switch (checked) {
//    case 0:
//        ui->EcoCheckBox->setStyleSheet(" color: rgb(255, 255, 255);background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0123, 123, 123, 255), stop:1 rgba(0, 0, 0, 255)); ");
//        break;
//    case 1:
//        ui->EcoCheckBox->setStyleSheet("background-color: rgb(0, 108, 217);background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 108, 217, 255), stop:1 rgba(0, 170, 255, 255));color : white;");
//        break;
//    default:
//        break;
//    }

//    ChangePalette(checked);


//}

//void MainWindow::on_timeButton_clicked()
//{
//    ++dateindex;
//    if (dateindex>=datestrings.length())
//        dateindex = 0 ;

//    QDateTime local(QDateTime::currentDateTime());

//    ui->timeButton->setStyleSheet("background-color: rgb(0, 108, 217);background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(0, 108, 217, 255), stop:1 rgba(0, 170, 255, 255));color : white;");
//    ui->time_label->setText(local.date().toString(datestrings.at(dateindex) ) + local.time().toString());
//}

void MainWindow::on_MenuButton_clicked()
{
    dMenu * menu = (dMenu*)dialogMenu;
    connect(menu, SIGNAL(saveButtonSignal()), this, SLOT(updateSystemOptions()));
    menu->selectPageMain();
    menu->exec();
    disconnect(menu, SIGNAL(saveButtonSignal()), this, SLOT(updateSystemOptions()));
//    cLogger * log = new cLogger(pathtomessages, cLogger::UI);
    logger->addMess("Menu > Open ", cLogger::SERVICE, cLogger::UI);
    sendConfigChannelsToSlave();
    setTextBars();
    setStyleBars();
    updateWidgetsVols();
}

void MainWindow::updateSystemOptions()
{
    cFileManager::readSystemOptionsFromFile(pathtosystemoptions, &systemOptions);
    ClearPolarCoords();
}

void MainWindow::on_MessButton_clicked()
{
//    OpenMessagesWindow();
    dialogSetingsChannel = new dSettings(listChannels, 1, 1);
    dialogSetingsChannel->exec();
    dialogSetingsChannel->deleteLater();
}


void  MainWindow::destroyedslot(QObject *)
{
    //    ui->textEdit_2->setText(text);
    //    ui->textEdit_2->setText("Destroyed");
    //    qDebug() << "destroyed slot";
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{

    if (watched == ui->MessagesWidget && event->type() == QEvent::Paint)
    {
        if (needUpdatePainter == true)
        {
            PaintOnWidget();
            needUpdatePainter = false;
        }
    }

    if (watched == ui->MessagesWidget && event->type() == QEvent::Resize)
    {
        ClearPolarCoords();
    }

    if (watched == ui->customPlot && event->type() != QEvent::Paint \
            && event->type() != QEvent::Resize) {
        if (watched == ui->customPlot && event->type() == QEvent::MouseButtonPress) {
            reactOnMousePress();
        }

        if (watched == ui->customPlot && event->type() == QEvent::MouseButtonRelease) {
            reactOnMouseRelease();
        }

        if (watched == ui->customPlot && event->type() == QEvent::MouseMove) {
            ReactOnMouseSlide();
        }
        //Vag: Крайне не рекомендуется передавать управление событием в QCustomPlot,
        //      так как в текущей сборке Linux модуль работает некорректно,
        //      поэтому возвращаем true и завершаем обработку события
        return true;
    }




    if ((event->type() == QEvent::MouseButtonPress)\
         && (QString::fromLatin1(watched->metaObject()->className()) == "QPushButton")\
         && ((watched == ui->MessButton)\
             || (watched == ui->ArchiveButton)\
             || (watched == ui->WorkButton)\
             || (watched == ui->MenuButton)))
    {

        QList<QPushButton *> widgets = findChildren<QPushButton *>(); // ищем в объекте все виджеты и делаем их ресайз

        foreach(QPushButton * widget, widgets)
        {
            // ищем нажатую кнопку и подсвечиваем ее, т.е. назначаем стайлшит

            if (widget->objectName() == watched->property("objectName"))
            {
                widget->setStyleSheet("color:#FFFFFF;background-color:rgb(" \
                                      + QString::number(ColorButtonNormal.red() - 20) + "," \
                                      + QString::number(ColorButtonNormal.green() - 20) + "," \
                                      + QString::number(ColorButtonNormal.blue() - 20) + ");" \
                                      + "border: 0px;");
            }
        }
    }


    if ((event->type() == QEvent::MouseButtonRelease)\
         && ( QString::fromLatin1(watched->metaObject()->className()) == "QPushButton" )\
         && ((watched == ui->MessButton)\
             || (watched == ui->ArchiveButton)\
             || (watched == ui->WorkButton)\
             || (watched == ui->MenuButton)))
    {

        QList<QPushButton *> widgets = findChildren<QPushButton *>(); // ищем в объекте все виджеты и делаем их ресайз

        foreach(QPushButton * widget, widgets)
        {
            // ищем нажатую кнопку и подсвечиваем ее, т.е. назначаем стайлшит

            if (widget->objectName() == watched->property("objectName"))
            {
                widget->setStyleSheet("color:#FFFFFF;background-color:rgb(" \
                                      + QString::number(ColorButtonNormal.red()) + "," \
                                      + QString::number(ColorButtonNormal.green()) + "," \
                                      + QString::number(ColorButtonNormal.blue()) + ");" \
                                      + "border: 0px;");
            }
        }
    }

    if ((event->type() == QEvent::MouseButtonPress)\
         && (QString::fromLatin1(watched->metaObject()->className()) == "QPushButton")\
         && ((watched->objectName().contains("PlavkaButtonUp"))\
             || (watched->objectName().contains("PlavkaButtonDown"))))
    {
        QList<QPushButton *> widgets = findChildren<QPushButton *>(); // ищем в объекте все виджеты и делаем их ресайз
        foreach(QPushButton * widget, widgets)
        {
            // ищем нажатую кнопку и подсвечиваем ее, т.е. назначаем стайлшит
            if (widget->objectName() == watched->property("objectName"))
            {
                widget->setStyleSheet("color:#000000;background-color:rgb(" \
                                      + QString::number(COLOR_GRAY.red() - 20) + "," \
                                      + QString::number(COLOR_GRAY.green() - 20) + "," \
                                      + QString::number(COLOR_GRAY.blue() - 20) + ");" \
                                      + "border: 0px;");
            }
        }
    }

    if ((event->type() == QEvent::MouseButtonRelease)\
         && (QString::fromLatin1(watched->metaObject()->className()) == "QPushButton")\
         && ((watched->objectName().contains("PlavkaButtonUp"))\
             || (watched->objectName().contains("PlavkaButtonDown"))))
    {
        QList<QPushButton *> widgets = findChildren<QPushButton *>(); // ищем в объекте все виджеты и делаем их ресайз
        foreach(QPushButton * widget, widgets)
        {
            // ищем нажатую кнопку и подсвечиваем ее, т.е. назначаем стайлшит
            if (widget->objectName() == watched->property("objectName"))
            {
                widget->setStyleSheet("color:#000000;background-color:rgb(" \
                                      + QString::number(COLOR_GRAY.red()) + "," \
                                      + QString::number(COLOR_GRAY.green()) + "," \
                                      + QString::number(COLOR_GRAY.blue()) + ");" \
                                      + "border: 0px;");
            }
        }
    }

    if ((event->type() == QEvent::MouseButtonRelease)\
            && ((watched->objectName().contains("arrowGroup"))))
    {
        int newCurGroupChannel = curGroupChannel;
        if(watched->objectName().contains("Left"))
        {
            do {    //выполнять поиск следующей группы, пока не встретится активная
                    // или не встретиться текущая
                //сменить группу на группу с меньшим номером
                if(newCurGroupChannel == 0)
                {
                    newCurGroupChannel = listGroup.size() - 1;
                }
                else
                {
                    newCurGroupChannel--;
                }
            } while(!listGroup.at(newCurGroupChannel)->enabled &&\
                  (newCurGroupChannel != curGroupChannel));
        }
        if(watched->objectName().contains("Right"))
        {
            do {
                //сменить группу на группу с большим номером
                if(newCurGroupChannel == listGroup.size() - 1)
                {
                    newCurGroupChannel = 0;
                }
                else
                {
                    newCurGroupChannel++;
                }
            } while(!listGroup.at(newCurGroupChannel)->enabled &&\
                    (newCurGroupChannel != curGroupChannel));
        }
        curGroupChannel = newCurGroupChannel;
        ui->nameGroupChannels->setText(listGroup.at(curGroupChannel)->groupName);
//        ui->arrowGroupLeft->setPixmap(QPixmap(pathtoleftarrow));
//        ui->arrowGroupRight->setPixmap(QPixmap(pathtorightarrow));
        updateWidgetsVols();
        setTextBars();
    }
    return QWidget::eventFilter(watched, event);
}

void MainWindow::ShowMessageBox (QString title,QString message)
{
    QMessageBox::information(this, title, message);
}

void MainWindow::resizeWindow(QWidget & qw, qreal mratio)
{
    QLayout * ql = qw.layout();

    if (ql == NULL)
        return;

    QWidget * pw = ql->parentWidget();

    if (pw == NULL)
        return;

    QList<QLayout *> layouts;

    foreach(QWidget *w, pw->findChildren<QWidget*>())
    {
        QRect g = w->geometry();
        w->setMinimumSize(w->minimumWidth() * mratio, w->minimumHeight() * mratio);
        w->setMaximumSize(w->maximumWidth() * mratio, w->maximumHeight() * mratio);
        w->resize(w->width() * mratio, w->height() * mratio);
        w->move(QPoint(g.x() * mratio, g.y() * mratio));
    }

    foreach(QLayout *l, pw->findChildren<QLayout*>())
    {
        if(l != NULL && !(l->objectName().isEmpty()))
            layouts.append(l);
    }

    foreach(QLayout *l, layouts) {
        QMargins m = l->contentsMargins();

        m.setBottom(m.bottom() * mratio);
        m.setTop(m.top() * mratio);
        m.setLeft(m.left() * mratio);
        m.setRight(m.right() * mratio);
        l->setContentsMargins(m);
        l->setSpacing(l->spacing() * mratio);

        if (l->inherits("QGridLayout")) {
            QGridLayout* gl = ((QGridLayout*)l);

            gl->setHorizontalSpacing(gl->horizontalSpacing() * mratio);
            gl->setVerticalSpacing(gl->verticalSpacing() * mratio);
        }
    }

    QMargins m = qw.contentsMargins();

    m.setBottom(m.bottom() * mratio);
    m.setTop(m.top() * mratio);
    m.setLeft(m.left() * mratio);
    m.setRight(m.right() * mratio);

    // resize accordingly main window
    qw.resize(qw.width() * mratio, qw.height() * mratio);
    qw.setContentsMargins(m);
    qw.adjustSize();
}

void MainWindow::resizeWindow(QObject & qobj, qreal xresolution, qreal yresolution)
{
    qreal xratio = xresolution / 1024;
    qreal yratio = yresolution / 768;

    QList<QWidget *> widgets = qobj.findChildren<QWidget *>(); // ищем в объекте все виджеты и делаем их ресайз

    foreach(QWidget * widget, widgets)
    {
        QRect g = widget->geometry();
        widget->setMinimumSize(widget->minimumWidth() * xratio, widget->minimumHeight() * xratio);
        widget->setMaximumSize(widget->maximumWidth() * yratio, widget->maximumHeight() * yratio);
        widget->resize(widget->width() * xratio, widget->height() * yratio);
        widget->move(QPoint(g.x() * xratio, g.y() * yratio));
    }
    return;
}

void MainWindow::resizeSelf(qreal xresolution, qreal yresolution)
{
    qreal xratio = xresolution / GetWindowWidthPixels();
    qreal yratio = yresolution / GetWindowHeightPixels();

    SetWindowHeightPixels(yresolution);
    SetWindowWidthPixels(xresolution);

    QList<QWidget *> widgets = findChildren<QWidget *>(); // ищем в объекте все виджеты и делаем их ресайз

    foreach(QWidget * widget, widgets)
    {
        QRect g = widget->geometry();
        widget->setMinimumSize(widget->minimumWidth() * xratio, widget->minimumHeight() * xratio);
        widget->setMaximumSize(widget->maximumWidth() * yratio, widget->maximumHeight() * yratio);
        widget->resize(widget->width() * xratio, widget->height() * yratio);
        widget->move(QPoint(g.x() * xratio, g.y() * yratio));
    }
    return;
}

int MainWindow::GetWindowHeightPixels()
{
    return windowheight;
}

int MainWindow::GetWindowWidthPixels()
{
    return windowwidth;
}

//int MainWindow::GetMonitorWidthPixels()
//{
//    QScreen *screen = QGuiApplication::primaryScreen();
//    QRect  screenGeometry = screen->geometry();
//    int height = screenGeometry.height();
//    int width = screenGeometry.width();

//    Options::MonitorResolution = QString::number(width) + "x"+QString::number(height) ;
//    return width;
//}

int MainWindow::GetMonitorHeightPixels()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    return height;
}

void MainWindow::SetWindowWidthPixels(int neww)
{
    windowwidth = neww;
}

void MainWindow::SetWindowHeightPixels(int newh)
{
    windowheight = newh;
}

//void MainWindow::on_bWriteTypeSignal_clicked()
//{
//    uint32_t tmp = ui->setTypeSignal->text().toInt();
////    typeSign1.dir = DIR_W;
////    typeSign1.param = device.channel0.SignalType;
////    typeSign1.id = 0;
////    typeSign1.volInt = tmp;
//    Transaction trans(Transaction::W, 1, 32781, tmp);
//    emit sendTransToWorker(trans);
//}

//void MainWindow::on_sendI2C_clicked()
//{

//}

//void MainWindow::on_readI2C_clicked()
//{
//    cDriverI2C i2c_1;
//    i2c_1.init(1);
//    i2c_1.detect();
//    cDriverI2C i2c_2;
//    i2c_2.init(2);
//    i2c_2.detect();
//}


//void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
//{
////        randVal[0] = arg1;
//        channel1.SetCurrentChannelValue(arg1);
//        ui->wBar_1->setVolue(arg1);
//}

void MainWindow::openSettingsChannel(int num)
{

    //проверка на наличие такого номера канала
    cGroupChannels * group = listGroup.at(curGroupChannel);
    if(group->typeInput[num-1] != 1) return;
    if(group->channel[num-1] > listChannels.size()) return;

//    ChannelOptions * channel = group->channel[num-1];
    ChannelOptions * channel = listChannels.at(group->channel[num-1]);
    int index = channel->getNum();

    dialogSetingsChannel = new dSettings(listChannels, index);
    dialogSetingsChannel->exec();
    dialogSetingsChannel->deleteLater();
    sendConfigChannelsToSlave();
    setTextBars();
    setStyleBars();
    updateWidgetsVols();
}


//void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
//{
//    listChannels.at(0)->SetCurrentChannelValue(arg1);
//    ui->wBar_1->setVolue(arg1);
//    ui->widgetVol1->setVol(arg1);
//}

void MainWindow::plotPress(QMouseEvent * pe)
{
    if(pe->pos().x() > (ui->customPlot->width() / 10))
    {
        mouseOnMove = true;
    }
    xPos = QCursor::pos().x();
    yPos = QCursor::pos().y();
    sizePlot = ui->customPlot->yAxis->range().size();
    posPlot = ui->customPlot->yAxis->range().center();
    mouseOnScalede = true;
    waitAutoScale = true;
    qDebug() << "plotPress";
}

void MainWindow::plotReleas(QMouseEvent * pe)
{
    mouseOnScalede = false;
    mouseOnMove = false;
    timerScale.start(3000);
    qDebug() << "plotReleas";
}

void MainWindow::plotMove(QMouseEvent * pe)
{
    if(mouseOnScalede && !mouseOnMove)
    {
        int y = QCursor::pos().y();
        double scale = 1 + (((double)y - (double)yPos) / (double)desktopHeight);
        double pos = ui->customPlot->yAxis->range().center();
        double size = sizePlot * scale * scale ;
        ui->customPlot->yAxis->setRange(pos, size, Qt::AlignCenter);
//        ui->customPlot->replot();
    }
    else if(mouseOnMove)
    {
        int y = QCursor::pos().y();
        double move = ((double)y - (double)yPos) / (double)ui->customPlot->height();
        double pos = posPlot + move * sizePlot;
        ui->customPlot->yAxis->setRange(pos, sizePlot, Qt::AlignCenter);
//        ui->customPlot->replot();
    }
    timerScale.start(3000);
    qDebug() << "plotMove";
}

void MainWindow::reactOnMousePress()
{
    xPos = ui->customPlot->mapFromGlobal(QCursor::pos()).x();
    yPos = ui->customPlot->mapFromGlobal(QCursor::pos()).y();
    if(xPos > (ui->customPlot->width() / 10))
    {
        mouseOnMove = true;
    }
    sizePlot = ui->customPlot->yAxis->range().size();
    posPlot = ui->customPlot->yAxis->range().center();
    mouseOnScalede = true;
    waitAutoScale = true;
    qDebug() << "plotPress";
}

void MainWindow::reactOnMouseRelease()
{
    mouseOnScalede = false;
    mouseOnMove = false;
    timerScale.start(3000);
    qDebug() << "plotReleas";
}

void MainWindow::ReactOnMouseSlide()
{
    if(mouseOnScalede && !mouseOnMove)
    {
        int y = ui->customPlot->mapFromGlobal(QCursor::pos()).y();
        double scale = 1 + (((double)y - (double)yPos) / (double)desktopHeight);
        double pos = ui->customPlot->yAxis->range().center();
        double size = sizePlot * scale * scale ;
        ui->customPlot->yAxis->setRange(pos, size, Qt::AlignCenter);
//        ui->customPlot->replot();
    }
    else if(mouseOnMove)
    {
        int y = ui->customPlot->mapFromGlobal(QCursor::pos()).y();
        double move = ((double)y - (double)yPos) / (double)ui->customPlot->height();
        double pos = posPlot + move * sizePlot;
        ui->customPlot->yAxis->setRange(pos, sizePlot, Qt::AlignCenter);
//        ui->customPlot->replot();
    }
    timerScale.start(3000);
    qDebug() << "plotMove";
}

void MainWindow::updateAutoScale()
{
    waitAutoScale = false;
    timerScale.stop();
}

void MainWindow::tickLoadWidget()
{
    if(countLoader <= 100)
    {
        ui->progressBar->setValue(countLoader);
        countLoader++;
        if(allDeviceStable && (countLoader < 98))
        {
            countLoader++;
        }
    }
    else
    {
        ui->splash->hide();

        /* для аналоговых сигналов */
        ui->left->setMaximumWidth(16777215);
        ui->right->setMaximumWidth(16777215);
        ui->left->show();
        ui->right->show();
        plotReady = true;

        /* для анализа стали */
        ui->frameSteel->setMaximumWidth(16777215);
        ui->frameSteel->hide();

        ui->header->show();
        ui->footer->show();
        timerLoad.stop();

    }
}

void MainWindow::initSteel()
{
    for(int i = 0; i < NUM_TECHNOLOGIES; i++)
    {
        steelTech[i] = defTech[i];
    }

    for(int i = 0; i < NUM_STEEL; i++)
    {
        cSteel * steel = new cSteel(this);
        steel->enable = false;
        if(i < 2)
            steel->slot = CONST_SLAVE_STEEL;
        else
            steel->slot = CONST_SLAVE_STEEL_2;
        steel->slotIndex = i % 2;
        steel->technology = &steelTech[i%NUM_TECHNOLOGIES];
        listSteel.append(steel);
    }
}

//void MainWindow::on_pushButton_clicked()
//{
////    simulatorSteel();
//}


void MainWindow::on_buttonInputsGraphs_clicked()
{
    steelSelectFrame = !steelSelectFrame;
}


//void MainWindow::on_buttonInputsGraphs_pressed()
//{
//    ui->buttonInputsGraphs->setStyleSheet("");
//}

//void MainWindow::on_buttonInputsGraphs_released()
//{

//}

void MainWindow::on_PlavkaButtonUp_clicked()
{
    listSteel.at(0)->numSmelt++;
}

void MainWindow::on_PlavkaButtonDown_clicked()
{
    if(listSteel.at(0)->numSmelt != 1)
        listSteel.at(0)->numSmelt--;
}
void MainWindow::on_PlavkaButtonUp_2_clicked()
{
    listSteel.at(1)->numSmelt++;
}
void MainWindow::on_PlavkaButtonDown_2_clicked()
{
    if(listSteel.at(1)->numSmelt != 1)
        listSteel.at(1)->numSmelt--;
}
void MainWindow::on_PlavkaButtonUp_3_clicked()
{
    listSteel.at(2)->numSmelt++;
}
void MainWindow::on_PlavkaButtonDown_3_clicked()
{
    if(listSteel.at(2)->numSmelt != 1)
        listSteel.at(2)->numSmelt--;
}
void MainWindow::on_PlavkaButtonUp_4_clicked()
{
    listSteel.at(3)->numSmelt++;
}
void MainWindow::on_PlavkaButtonDown_4_clicked()
{
    if(listSteel.at(3)->numSmelt != 1)
        listSteel.at(3)->numSmelt--;
}
