#include "updategraph.h"
#include "mainwindow.h"
#include "uartdriver.h"
#include "ui_mainwindow.h"
#include "messages.h"
#include "metrologicalcalc.h"
#include "mathresolver.h"
#include "options.h"
#include "stackedoptions.h"
#include "qglobal.h"
#include "defines.h"
#include <stdlib.h>
#include "registersmap.h"
#include "assert.h"
#include "Channels/group_channels.h"



#define PERIOD_MEASURE_STEEL 100    //msec период точек на графике анализа стали

int xoffset=0;

double randVal[24] = {((double)((rand()%101) - 50) / 10),\
                     ((double)((rand()%101) - 50) / 10),\
                     ((double)((rand()%101) - 50) / 10),\
                     ((double)((rand()%101) - 50) / 10)};


QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;
QVector<double> X_Coordinates_archive, Y_coordinates_Chanel_1_archive, Y_coordinates_Chanel_2_archive, Y_coordinates_Chanel_3_archive, Y_coordinates_Chanel_4_archive;
QVector<double> vectorsChannels[24];
QVector<double> vectorsChannelsArch[24];
QVector<QDateTime> X_Date_Coordinates;
QVector<double> X_Coord_Steel, Y_Coord_SteelTemp, Y_Coord_SteelEds;

extern cChannelSlotController csc;
extern QList<ChannelOptions*> listChannels;
extern QList<Ustavka *> listUstavok;
extern QList<cSteel*> listSteel;
extern QList<cRelay*> listRelais;
extern cSteelController ssc;
extern cSystemOptions systemOptions;  //класс хранения состемных опций
extern QList<cGroupChannels*> listGroup;
extern QList<cMathChannel *> listMath;


int MainWindow::GetXOffset(int smallrectinglewidth, QGraphicsTextItem *ChannelValueText)
{
    ChannelValueText->setTextWidth(ChannelValueText->boundingRect().width());
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignCenter);
    QTextCursor cursor = ChannelValueText->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    ChannelValueText->setTextCursor(cursor);
    int GW = smallrectinglewidth;
    int lw =  ChannelValueText->boundingRect().width();
    int xoffset = (GW  - lw) /2;

    return xoffset;
}

void MainWindow::DrawScene()
{

}

void MainWindow::AddValuesToBuffer()
{
    X_Coordinates.append(xoffset); // добавляем смещение по иксу
    X_Coordinates_archive.append(xoffset);
    X_Date_Coordinates.append(QDateTime::currentDateTime());

//    vectorsChannels.resize(listChannels.size());
//    QVector<double> * vectors[] = {&Y_coordinates_Chanel_1,\
//                                &Y_coordinates_Chanel_2,\
//                                &Y_coordinates_Chanel_3,\
//                                &Y_coordinates_Chanel_4};
//    vectorsChannelsArch.resize(listChannels.size());
//    QVector<double> * vectorsArch[] = {&Y_coordinates_Chanel_1_archive,\
//                                &Y_coordinates_Chanel_2_archive,\
//                                &Y_coordinates_Chanel_3_archive,\
//                                &Y_coordinates_Chanel_4_archive};

    int i = 0;
    foreach (ChannelOptions * channel, listChannels) {
//        vectors[i]->append(channel->GetCurrentChannelValue());
//        QVector<double> * vector = &vectorsChannels.value(i);
        vectorsChannels[i].append(channel->GetCurrentChannelValue());
//        vectorsArch[i]->append(channel->GetCurrentChannelValue());
//        QVector<double> * vectorArch = &vectorsChannelsArch.at(i);
        vectorsChannelsArch[i].append(channel->GetCurrentChannelValue());
        while (vectorsChannels[i].length()>150)
        {
            vectorsChannels[i].removeFirst();
        }
        while (vectorsChannelsArch[i].length()>15000)
        {
            vectorsChannelsArch[i].removeFirst();
        }
        i++;
    }

//    Y_coordinates_Chanel_1.append(listChannels.at(0)->GetCurrentChannelValue());
//    Y_coordinates_Chanel_2.append(listChannels.at(1)->GetCurrentChannelValue());
//    Y_coordinates_Chanel_3.append(listChannels.at(2)->GetCurrentChannelValue());
//    Y_coordinates_Chanel_4.append(listChannels.at(3)->GetCurrentChannelValue());

//    Y_coordinates_Chanel_1_archive.append(listChannels.at(0)->GetCurrentChannelValue());
//    Y_coordinates_Chanel_2_archive.append(listChannels.at(1)->GetCurrentChannelValue());
//    Y_coordinates_Chanel_3_archive.append(listChannels.at(2)->GetCurrentChannelValue());
//    Y_coordinates_Chanel_4_archive.append(listChannels.at(3)->GetCurrentChannelValue());

    while (X_Coordinates.length()>150)
    {
        X_Coordinates.removeFirst();
        X_Date_Coordinates.removeFirst();
//        Y_coordinates_Chanel_1.removeFirst();
//        Y_coordinates_Chanel_2.removeFirst();
//        Y_coordinates_Chanel_3.removeFirst();
//        Y_coordinates_Chanel_4.removeFirst();
    }

    //пока нет ограничений на объём хранения
    while (X_Coordinates_archive.length()>15000)
    {
        X_Coordinates_archive.removeFirst();
//        Y_coordinates_Chanel_1_archive.removeFirst();
//        Y_coordinates_Chanel_2_archive.removeFirst();
//        Y_coordinates_Chanel_3_archive.removeFirst();
//        Y_coordinates_Chanel_4_archive.removeFirst();
    }

    int tickstep = GetTickStep();

    // если последняя точка по иксу попала на тайм-лейбел, его нужно корректировать
    if (xoffset%tickstep==0)
    {
        LabelsCorrect();
    }

    xoffset++;
    SetPolarAngle(GetPolarAngle()+1);
}

void MainWindow::UpdateGraphics()
{
#ifndef RANDOM_CHAN
    // если нет данных от плат, то и строить нечего
    if(!slotSteelOnline && !slotAnalogOnline) return;
#endif
    // если прелоадер не завершился
    if(!plotReady) return;
    // если есть плата STEEL, то ставим режим Steel
    if(slotSteelOnline)
    {
        systemOptions.display = cSystemOptions::Steel;
    }
    needUpdatePainter = true;
    switch(systemOptions.display)
    {
    case cSystemOptions::TrendsCyfra:
        ui->stackedWidget->setCurrentIndex(1);
        GrafsUpdateTrends();
        updateVols();
        break;
    case cSystemOptions::Bars :
        ui->stackedWidget->setCurrentIndex(0);
//        GrafsUpdateBars();
        updateBars();
        break;
    case cSystemOptions::TrendsBars:
        ui->stackedWidget->setCurrentIndex(0);
        GrafsUpdateTrends();
        updateBars();
        break;
    case cSystemOptions::PolarBars:
        ui->stackedWidget->setCurrentIndex(0);
        updateBars();
        GrafsUpdateNone();
        break;
    case cSystemOptions::PolarCyfra:
        ui->stackedWidget->setCurrentIndex(1);
        GrafsUpdateNone();
        updateVols();
        break;
    case cSystemOptions::Cyfra:
        ui->stackedWidget->setCurrentIndex(1);
//        GrafsUpdateNone();
        updateVols();
        break;
    case cSystemOptions::Steel:
        updateSteelWidget();
        break;
    default:
        break;
    }
    selectWidgetDiagram();
}


void MainWindow::GrafsUpdateTrends()
{

    cGroupChannels * group = listGroup.at(curGroupChannel);

    ui->customPlot->xAxis->setRange(xoffset-GetXRange(), xoffset+GetXRange());
    ui->customPlot->clearGraphs();
    graphPen.setWidth(GraphWidthinPixels);

    QList<QColor> colors;
    colors << ColorCh1 << ColorCh2 << ColorCh3 << ColorCh4;

    for(int i = 0; i < MAX_NUM_CHAN_GROUP; i++)
    {
        ui->customPlot->addGraph();
        if((group->typeInput[i] == 1) && (group->channel[i] != -1))
        {
            ChannelOptions * channel = listChannels.at(group->channel[i]);
            ui->customPlot->graph()->setName(channel->GetChannelName());
            ui->customPlot->graph()->setData(channel->GetChannelXBuffer(), channel->GetChannelValuesBuffer());
        }
        else if((group->typeInput[i] == 2) && (group->mathChannel[i] != -1))
        {
            //Vag: вставить сюда буффер матканала
        }
        graphPen.setColor(colors.at(i));
        ui->customPlot->graph()->setPen(graphPen);
    }



    ui->customPlot->xAxis->setAutoTickStep(false); // выключаем автоматические отсчеты
    ui->customPlot->xAxis->setTickStep(GetTickStep()); // 60 secs btw timestamp

    ui->customPlot->xAxis->setAutoTickLabels(false);
    ui->customPlot->xAxis->setTickVectorLabels(Labels);

    // авто масшабирование
    if (systemOptions.autoscale && !waitAutoScale /*ui->autoscalecheckbox->checkState()*/)
    {
        ui->customPlot->yAxis->rescale();
    }

    // add the helper arrow:

    if (systemOptions.arrows)
    {

        for(int index = 0; index < MAX_NUM_CHAN_GROUP; index++)
        {
            if(group->typeInput[index] == 0) continue;
            if((group->typeInput[index] == 1) && (group->channel[index] == -1)) continue;
            if((group->typeInput[index] == 2) && (group->mathChannel[index] == -1)) continue;
            ChannelOptions * channel = listChannels.at(group->channel[index]);

            QCPItemLine *arrow = new QCPItemLine(ui->customPlot);

            int ystart = ui->customPlot->height() / 5;
            int xstart = ui->customPlot->width() - ui->customPlot->width() / 10;

            arrow->start->setPixelPoint(QPointF(xstart, ystart + ui->customPlot->height() * index / 5));
            arrow->end->setCoords(xoffset, channel->GetCurrentChannelValue()); // point to (4, 1.6) in x-y-plot coordinates
            arrow->setHead(QCPLineEnding::esSpikeArrow);
            arrow->setPen(QPen(colors.at(index),1,  Qt::DashLine));
            arrow->setAntialiased(true);
            ui->customPlot->setAntialiasedElements(QCP::aeAll);
//            ui->customPlot->addItem(arrow);

            arrow->setAntialiased(true);
            ui->customPlot->setAntialiasedElements(QCP::aeAll);

            QCPItemText *NameLabel = new QCPItemText(ui->customPlot);
//            ui->customPlot->addItem(NameLabel);
            NameLabel->position->setPixelPoint(QPointF(xstart+20, ystart + ui->customPlot->height() * index / 5));
            NameLabel->setText(channel->GetChannelName());
            NameLabel->setFont(QFont(Font, 14, QFont::Bold));
            NameLabel->setColor(colors.at(index));

            arrow->deleteLater();
            NameLabel->deleteLater();
        }
    }

    ui->customPlot->setAntialiasedElements(QCP::aeAll);

    ui->customPlot->replot();
//    ui->customPlot->clearGraphs();
//    ui->customPlot->clearItems();
}

void MainWindow::GrafsUpdateNone()
{
    ui->MessagesWidget->update();
}

//void MainWindow::updateChannelSlot(int ch)
//{
//    ChannelOptions * channel = listChannels.at(ch);
//    if(!channel->enable) return;
//    if(channel->GetSignalType() == 0) return;
//    if(systemOptions.display == cSystemOptions::Steel) return;
//    int slot = channel->getSlot();
//    int slChannel = channel->getSlotChannel();
////    int period = channel->GetMeasurePeriod()*1000;
//    DataBuffer::writeupdatestatus(0,true);
//    QString nameParam = "chan" + QString::number(slChannel) + "Data";
//    uint16_t offset = cRegistersMap::getOffsetByName(nameParam);
//    Transaction tr(Transaction::R, (uint8_t)slot, offset, 0);
//#ifndef RANDOM_CHAN
//    emit sendTransToWorker(tr);
//#else
//    /* Test */
////    if(ch < 4)
////    {
//        randVal[ch] += ((double)((rand()%101) - 50) / 100);
//        channel->SetCurrentChannelValue(randVal[ch]);
////        wVolueBar * bars[] = {ui->wBar_1 , ui->wBar_2, ui->wBar_3, ui->wBar_4};
////        bars[ch]->setVolue(randVal[ch]);
////        wVol * vol[] = {ui->widgetVol1, ui->widgetVol2, ui->widgetVol3, ui->widgetVol4};
////        vol[ch]->setVol(randVal[ch]);
////    }
//#endif
////    QTimer * timers[] = {channeltimer1, channeltimer2, channeltimer3, channeltimer4};
////    timers[ch]->setInterval(period);
//}




void MainWindow::updateBars(void)
{
    QList<QSpacerItem*> listSpacerBar;
    listSpacerBar.append(ui->spacerBar0);
    listSpacerBar.append(ui->spacerBar1);
    listSpacerBar.append(ui->spacerBar2);
    listSpacerBar.append(ui->spacerBar3);
    listSpacerBar.append(ui->spacerBar4);
//    ui->spacerBar0->changeSize(0, 0, QSizePolicy::Maximum);
//    ui->spacerBar1->changeSize(0, 0, QSizePolicy::Maximum);

    QList<wVolueBar*> listBars;
    listBars.append(ui->wBar_1);
    listBars.append(ui->wBar_2);
    listBars.append(ui->wBar_3);
    listBars.append(ui->wBar_4);

    listSpacerBar.at(0)->changeSize(0, 0, QSizePolicy::Preferred);

    cGroupChannels * group = listGroup.at(curGroupChannel);
    int i = 0;
    foreach (wVolueBar * bar, listBars)
    {
        bar->hide();
        listSpacerBar.at(i+1)->changeSize(0, 0, QSizePolicy::Fixed);
        if(group->typeInput[i] == 1)
        {
            if(group->channel[i] != -1)
            {
                ChannelOptions * channel = listChannels.at(group->channel[i]);

#ifdef RANDOM_CHAN
                if((channel->getNum() <= NUM_CHAN_IN_4AI) || channel->enable)
#else
                if(channel->enable)
#endif
                {
                    bar->setExtr(channel->GetMinimumChannelValue(), channel->GetMaximumChannelValue());
                    bar->setBarDiapazon(max(abs(channel->GetHigherMeasureLimit()), \
                                            abs(channel->GetLowerMeasureLimit())));
                    bar->setVolue(channel->GetCurrentChannelValue());
                    bar->cleanMarker();
                    foreach(Ustavka * ust, listUstavok)
                    {
                        if(ust->getChannel() == channel->getNum())
                        {
                            bar->addMarker(ust->getHiStateValue(), ust->getTypeFix());
                        }
                    }
                    bar->show();
                    listSpacerBar.at(i+1)->changeSize(0, 0, QSizePolicy::Preferred);
                }



            }
        }
        else if(group->typeInput[i] == 2)
        {
            if(group->mathChannel[i] != -1)
            {
                if(group->mathChannel[i] < listMath.size())
                {
                    cMathChannel * math = listMath.at(group->mathChannel[i]);

//                    bar->setExtr(math->GetMinimumChannelValue(), math->GetMaximumChannelValue());
                    bar->setBarDiapazon(max(abs(math->GetHigherMeasureLimit()), \
                                            abs(math->GetLowerMeasureLimit())));
                    bar->setVolue(math->GetCurrentMathValue());
                    bar->cleanMarker();
//                    foreach(Ustavka * ust, listUstavok)
//                    {
//                        if(ust->getChannel() == channel->getNum())
//                        {
//                            bar->addMarker(ust->getHiStateValue(), ust->getTypeFix());
//                        }
//                    }
                    bar->show();
                    listSpacerBar.at(i+1)->changeSize(0, 0, QSizePolicy::Preferred);
                }
            }
        }
        i++;
    }
}

void MainWindow::setStyleBars()
{
    ui->wBar_1->setColor(ColorCh1, ColorCh1Light);
    ui->wBar_2->setColor(ColorCh2, ColorCh2Light);
    ui->wBar_3->setColor(ColorCh3, ColorCh3Light);
    ui->wBar_4->setColor(ColorCh4, ColorCh4Light);

//    ui->wBar_1->setText(channel1.GetChannelName(), channel1.GetUnitsName());
//    ui->wBar_2->setText(channel2.GetChannelName(), channel2.GetUnitsName());
//    ui->wBar_3->setText(channel3.GetChannelName(), channel3.GetUnitsName());
    //    ui->wBar_4->setText(channel4.GetChannelName(), channel4.GetUnitsName());
}

void MainWindow::setTextBars()
{
    QList<wVolueBar*> listBars;
    listBars.append(ui->wBar_1);
    listBars.append(ui->wBar_2);
    listBars.append(ui->wBar_3);
    listBars.append(ui->wBar_4);
    cGroupChannels * group = listGroup.at(curGroupChannel);

    int i = 0;
    foreach (wVolueBar * bar, listBars)
    {
        if(group->typeInput[i] == 1)
        {
            if(group->channel[i] != -1)
            {
                bar->setText(listChannels.at(group->channel[i])->GetChannelName(), \
                             listChannels.at(group->channel[i])->GetUnitsName());
            }

        }
        else if(group->typeInput[i] == 2)
        {
            if(group->mathChannel[i] != -1)
            {
                if(group->mathChannel[i] < listMath.size())
                {
                    bar->setText(listMath.at(group->mathChannel[i])->getName(), "Хз");
                }
                else
                {
                    bar->setText("", "");
                }
            }
        }
        i++;
    }
}



void MainWindow::updateWidgetsVols(void)
{
    QList<wVol*> listVols;
    listVols.append(ui->widgetVol1);
    listVols.append(ui->widgetVol2);
    listVols.append(ui->widgetVol3);
    listVols.append(ui->widgetVol4);
    cGroupChannels * group = listGroup.at(curGroupChannel);

    QColor color[4] = {ColorCh1, ColorCh2, ColorCh3, ColorCh4};

    int i = 0;
    foreach (wVol * vol, listVols)
    {
        if(group->typeInput[i] == 1)
        {
            if(group->channel[i] != -1)
            {
                vol->setText(listChannels.at(group->channel[i])->GetChannelName(), \
                             listChannels.at(group->channel[i])->GetUnitsName());
            }
            vol->setColor(color[i]);
        }
        else if(group->typeInput[i] == 2)
        {
            if(group->mathChannel[i] != -1)
            {
                if(group->mathChannel[i] < listMath.size())
                {
                    vol->setText(listMath.at(group->mathChannel[i])->getName(), "Хз");
                }
                else
                {
                    vol->setText("", "");
                }
            }
            vol->setColor(color[i]);
        }
        i++;
    }
}

void MainWindow::updateVols()
{

    QList<wVol*> listVols;
    listVols.append(ui->widgetVol1);
    listVols.append(ui->widgetVol2);
    listVols.append(ui->widgetVol3);
    listVols.append(ui->widgetVol4);
    cGroupChannels * group = listGroup.at(curGroupChannel);
    int i = 0;
    foreach (wVol * vol, listVols)
    {
        vol->hide();
        if(group->typeInput[i] == 1)
        {
            if(group->channel[i] != -1)
            {
                ChannelOptions * channel = listChannels.at(group->channel[i]);
#ifdef RANDOM_CHAN
                if((channel->getNum() <= NUM_CHAN_IN_4AI) || channel->enable)
#else
                if(listChannels.at(group->channel[i])->enable)
#endif
                {
                    vol->setVol(listChannels.at(group->channel[i])->GetCurrentChannelValue());
                    vol->show();
                }
            }
        }
        else if(group->typeInput[i] == 2)
        {
            if(group->mathChannel[i] != -1)
            {
                vol->setVol(listMath.at(group->mathChannel[i])->GetCurrentMathValue());
                vol->show();
            }
        }
        i++;
    }
}

void MainWindow::selectWidgetDiagram(void)
{
    if ( (systemOptions.display == cSystemOptions::BarsCyfra) || \
         (systemOptions.display == cSystemOptions::TrendsCyfraBars) || \
         (systemOptions.display == cSystemOptions::TrendsBars) || \
         (systemOptions.display == cSystemOptions::TrendsCyfra) || \
         (systemOptions.display == cSystemOptions::Trends) )
    {
        ui->stackedWidgetDiagram->setCurrentIndex(0);
        ui->right->show();
        ui->left->show();
        ui->frameSteel->hide();
    }
    else if((systemOptions.display == cSystemOptions::Cyfra) || \
            (systemOptions.display == cSystemOptions::Bars))
    {
        ui->left->hide();
        ui->right->show();
        ui->frameSteel->hide();
    }
    else if(systemOptions.display == cSystemOptions::Polar ||\
            systemOptions.display == cSystemOptions::PolarBars ||\
            systemOptions.display == cSystemOptions::PolarCyfra)
    {
        ui->stackedWidgetDiagram->setCurrentIndex(1);
        ui->right->show();
        ui->left->show();
        ui->frameSteel->hide();
    }
    else if(systemOptions.display == cSystemOptions::Steel)
    {
        ui->right->hide();
        ui->left->hide();
        ui->frameSteel->show();
    }
}

void MainWindow::updateSteel()
{
    if(!slotSteelOnline) return;

    /*
     * Управление реле
     */
    for(int i = 0; i<listSteel.size(); i++)
    {
        if(listRelais.size() == 0) return;
        cSteel * steel = listSteel.at(i);
        int countRele = SUM_RELAYS;
        uint8_t relayStates[countRele];
        memset(relayStates, 0, countRele * sizeof(relayStates[0]));
        if(steel->status == StatusCh_SteelWaitData)
        {
            relayStates[1] = 1;
        }
        else if(steel->status == StatusCh_SteelUpdateData)
        {
            relayStates[2] = 1;
        }
        else if((steel->status == StatusCh_SteelNotFoundSquareTemp)\
                || (steel->status == StatusCh_SteelNotFoundSquareEds)\
                || (steel->status == StatusCh_SteelNotFoundSquares)\
                || (steel->status == StatusCh_SteelSquaresOK))
        {
            relayStates[3] = 1;
        }
        else if(steel->status == StatusCh_WaitConf) //обрыв датчика
        {
            relayStates[0] = 1;
        }

        for(int i = 0; i < countRele; i++)
        {
            int8_t relay = steel->relais[i];
            if((relay != -1) && (listRelais.size() > relay))
            {
                if(listRelais.at(relay)->getCurState() != relayStates[i])
                {
                    listRelais.at(relay)->setState(relayStates[i]);
//                    Transaction tr(Transaction::W, (uint8_t)listRelais.at(relay)->mySlot);
//                    uint8_t numDevRelay = listRelais.at(relay)->myPhysicalNum;
//                    if(relay%2)
//                        tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(numDevRelay >> 1) + "ReleyLo");
//                    else
//                        tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(numDevRelay >> 1) + "ReleyHi");
//                    tr.volInt = relayStates[i];
//                    emit sendTransToWorker(tr);
                    steel->countRelayTime = 4000 / UpdateSteelTime;
                }
                if((listRelais.at(relay)->getCurState() == true) && (i == 3))
                {
                    if(steel->countRelayTime > 0)
                        steel->countRelayTime--;
                    else
                    {
//                        Transaction tr(Transaction::W, (uint8_t)listRelais.at(relay)->mySlot);
//                        uint8_t numDevRelay = listRelais.at(relay)->myPhysicalNum;
//                        if(relay%2)
//                            tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(numDevRelay >> 1) + "ReleyLo");
//                        else
//                            tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(numDevRelay >> 1) + "ReleyHi");
//                        tr.volInt = 0;
//                        emit sendTransToWorker(tr);
                    }
                }

            }

        }
    }

    //Запрос статусов входных групп внезависимости от текущего состояния виджетов
//    Transaction tr(Transaction::R, (uint8_t)ssc.getSlotBySteel(steelReadyNum));
//    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(ssc.getDevSteel(0)) + "Status");
//    emit sendTransToWorker(tr);
//    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(ssc.getDevSteel(1)) + "Status");
//    emit sendTransToWorker(tr);
//    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(ssc.getDevSteel(2)) + "Status");
//    emit sendTransToWorker(tr);
//    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(ssc.getDevSteel(3)) + "Status");
//    emit sendTransToWorker(tr);
    Transaction tr(Transaction::R);
    foreach (cSteel * st, listSteel) {
        if(st->enable)
        {
            tr.slave = st->slot;
            tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(st->slotIndex) + "Status");
            emit sendTransToWorker(tr);
        }
    }

    if(stateWidgetSteel == STEEL_READY)
    {   //в плате STEEL имеются данные для получения
//        if((listSteel.at(steelReadyNum)->vectorEdsReceived) && \
//                (listSteel.at(steelReadyNum)->vectorTempReceived))
//        {
//            if(!listSteel.at(steelReadyNum)->allVectorsReceived)
//            {
//                //запись в журнал
//                logginSteel(steelReadyNum);
//                writeArchiveSteel(steelReadyNum);
//            }
//            listSteel.at(steelReadyNum)->allVectorsReceived = true;
//        }
//        else
//        {
//            listSteel.at(steelReadyNum)->allVectorsReceived = false;
//        }

        if(listSteel.at(steelReadyNum)->allVectorsReceived)
        {
            //запись в журнал
            logginSteel(steelReadyNum);
            writeArchiveSteel(steelReadyNum);
            listSteel.at(steelReadyNum)->allVectorsReceived = false;
        }

        if((listSteel.at(steelReadyNum)->status == StatusCh_SteelUpdateData)\
                || (listSteel.at(steelReadyNum)->status == StatusCh_SteelErrorTC)\
                || (listSteel.at(steelReadyNum)->status == StatusCh_SteelErrorEds))
        {
//            steelReady = false;
//            steelMeasure = false;
            stateWidgetSteel = STEEL_WAIT;
            devicesPause(false);
        }

//        return;
    }
    else if(stateWidgetSteel == STEEL_MEASURE)
    {   //началось измерение температуры и поиск площадки
        Transaction tr(Transaction::R, listSteel.at(steelReadyNum)->slot);
        tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(listSteel.at(steelReadyNum)->slotIndex) + "Data");
        emit sendTransToWorker(tr);
//        for(int i = 1; i <= 10; i ++)
//        {
        if((numArraySteel < 5) && askNewArray)
        {
            //запрос текущего участка графика ЭДС
            tr.offset = cRegistersMap::getOffsetByName("DataArray" + QString::number(numArraySteel+1));
            emit sendTransToWorker(tr);
            //запрос текущего участка графика температуры
            tr.offset = cRegistersMap::getOffsetByName("DataArray" + QString::number(numArraySteel+6));
            emit sendTransToWorker(tr);
            askNewArray = false;
            listSteel.at(steelReadyNum)->lastItemEds = false;
            listSteel.at(steelReadyNum)->lastItemTemp = false;
        }
//        }
        if((listSteel.at(steelReadyNum)->status == StatusCh_SteelWaitData)\
                || (listSteel.at(steelReadyNum)->status == StatusCh_WaitConf))
        {
            stateWidgetSteel = STEEL_WAIT;
            devicesPause(false);
        }
    }

    for(int i = 0; i<listSteel.size(); i++)
    {   // поочерёдный опрос статуса всех входных групп
        cSteel * steel = listSteel.at(i);
        int slot = steel->slot;
        int devCh = steel->slotIndex;
        Transaction tr(Transaction::R, (uint8_t)slot, 0, 0);

        if((steel->status == StatusCh_SteelSquaresOK) ||\
                (steel->status == StatusCh_SteelNotFoundSquareTemp) || \
                (steel->status == StatusCh_SteelNotFoundSquareEds) || \
                (steel->status == StatusCh_SteelNotFoundSquares))
        {   // i-тая входная группа нашла площадку и имеются готовые данные
            if(stateWidgetSteel != STEEL_READY)
            {
                QList<QString> str;
                str << "Data" << "OxActivity" << "MassAl" << "MassAl" << "PrimaryActivity" << "Carbon";
                if(listSteel.at(steelReadyNum)->technology->COH != 0)
                {
                    foreach (QString s, str) {
                        tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(devCh) + s);
                        emit sendTransToWorker(tr);
                    }
                }
                else
                {
                    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(devCh) + "Data");
                    emit sendTransToWorker(tr);
                    tr.offset = cRegistersMap::getOffsetByName("chan" + QString::number(devCh) + "Carbon");
                    emit sendTransToWorker(tr);
                }
            }
//            for(int i = 1; i <= 10; i ++)
//            {
//                tr.offset = cRegistersMap::getOffsetByName("DataArray" + QString::number(i));
//                emit sendTransToWorker(tr);
//            }
            //очистка векторов для новых данных
//            QVector<double> vec = QVector(SIZE_ARRAY, NAN);
//            steel->vectorTemp = QVector<double>(SIZE_ARRAY, NAN);
//            steel->vectorEds = QVector<double>(SIZE_ARRAY, NAN);
            //очистка признаков завершения получения векторов (только всё начинается)
//            steel->allVectorsReceived = false;
            steel->vectorTempReceived = false;
            steel->vectorEdsReceived = false;
            steel->allVectorsReceived = false;
            steel->alg = 0x7FFF;
            steel->ao = 0x7FFF;
            steel->temp = NAN;
            steel->eds = NAN;
            steel->cl = NAN;
            //данные на стороне платы STEEL готовы - запомним
//            steelReady = true;  //данные готовы
            stateWidgetSteel = STEEL_READY;
            devicesPause(false);
            steelReadyNum = i;  //запоминаем номер входной группы
//            steelSelectFrame = false;   //разрешаем показывать график
            steel->timeUpdateData = QDateTime::currentDateTime();
               break;  //выход из цикла, на первой же входной группе с площадкой
        }
        else if(steel->status == StatusCh_SteelUpdateData)
        {
            if(stateWidgetSteel != STEEL_MEASURE)
            {
                stateWidgetSteel = STEEL_MEASURE;
                devicesPause(true);
                steelSelectFrame = false;
                steelReadyNum = i;
                numArraySteel = 0;
                askNewArray = true;
                steel->vectorEds = QVector<double>(SIZE_ARRAY, NAN);
                steel->vectorTemp = QVector<double>(SIZE_ARRAY, NAN);
            }
        }
    }
}

void MainWindow::updateSteelWidget(void)
{
//    simulatorSteel();

    QList<QString> strings;
    strings << "ОТКЛЮЧЕНО" << " " << "ОБРЫВ" \
            << " " << " " << "ГОТОВ" \
            << "ИЗМЕРЕНИЕ" << " " << "ОШИБКА ТС" \
            << "ОШИБКА ЭДС" << "ВРЕМЯ" << "ВРЕМЯ" \
            << "ВРЕМЯ" << "ВРЕМЯ";
    QList<QString> colorStyles;
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_GRAY.red()) + ", " + QString::number(COLOR_GRAY.green()) + ", " + QString::number(COLOR_GRAY.blue()) + ");color: rgb(0, 0, 0);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_GRAY.red()) + ", " + QString::number(COLOR_GRAY.green()) + ", " + QString::number(COLOR_GRAY.blue()) + ");color: rgb(0, 0, 0);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_3.red()) + ", " + QString::number(COLOR_3.green()) + ", " + QString::number(COLOR_3.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_GRAY.red()) + ", " + QString::number(COLOR_GRAY.green()) + ", " + QString::number(COLOR_GRAY.blue()) + ");color: rgb(0, 0, 0);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_GRAY.red()) + ", " + QString::number(COLOR_GRAY.green()) + ", " + QString::number(COLOR_GRAY.blue()) + ");color: rgb(0, 0, 0);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_4.red()) + ", " + QString::number(COLOR_4.green()) + ", " + QString::number(COLOR_4.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_2.red()) + ", " + QString::number(COLOR_2.green()) + ", " + QString::number(COLOR_2.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_GRAY.red()) + ", " + QString::number(COLOR_GRAY.green()) + ", " + QString::number(COLOR_GRAY.blue()) + ");color: rgb(0, 0, 0);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_3.red()) + ", " + QString::number(COLOR_3.green()) + ", " + QString::number(COLOR_3.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_3.red()) + ", " + QString::number(COLOR_3.green()) + ", " + QString::number(COLOR_3.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_1.red()) + ", " + QString::number(COLOR_1.green()) + ", " + QString::number(COLOR_1.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_1.red()) + ", " + QString::number(COLOR_1.green()) + ", " + QString::number(COLOR_1.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_1.red()) + ", " + QString::number(COLOR_1.green()) + ", " + QString::number(COLOR_1.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_1.red()) + ", " + QString::number(COLOR_1.green()) + ", " + QString::number(COLOR_1.blue()) + ");color: rgb(255, 255, 255);");
    ui->steelStatus1->setText(strings.at(listSteel.at(0)->status));
    ui->steelStatus2->setText(strings.at(listSteel.at(1)->status));
    ui->steelStatus3->setText(strings.at(listSteel.at(2)->status));
    ui->steelStatus4->setText(strings.at(listSteel.at(3)->status));

    ui->steelStatus1->setStyleSheet(colorStyles.at(listSteel.at(0)->status));
    ui->steelStatus2->setStyleSheet(colorStyles.at(listSteel.at(1)->status));
    ui->steelStatus3->setStyleSheet(colorStyles.at(listSteel.at(2)->status));
    ui->steelStatus4->setStyleSheet(colorStyles.at(listSteel.at(3)->status));

    ui->PlavkaNum->setText(QString::number(listSteel.at(0)->numSmelt));
    ui->PlavkaNum_2->setText(QString::number(listSteel.at(1)->numSmelt));
    ui->PlavkaNum_3->setText(QString::number(listSteel.at(2)->numSmelt));
    ui->PlavkaNum_4->setText(QString::number(listSteel.at(3)->numSmelt));

    if((stateWidgetSteel == STEEL_READY) || (stateWidgetSteel == STEEL_MEASURE))
    {   //найдена площадка
        //steelReadyNum
        cSteel * steel = listSteel.at(steelReadyNum);
        ui->nameSteelTech->setText(steel->technology->name);

        if(stateWidgetSteel == STEEL_READY)
        {
            ui->labelTimeSteel->setText(steel->timeUpdateData.toString("dd.MM.yy hh:mm:ss"));
            if(!std::isnan(steel->temp))
                ui->steelTemp->setText(QString::number(steel->temp));
            if(!std::isnan(steel->eds))
                ui->steelEmf->setText(QString::number(steel->eds));
            if(steel->ao != 0x7FFF)
                ui->steelAO->setText(QString::number(steel->ao));
            if(steel->alg != 0x7FFF)
                ui->steelAl->setText(QString::number(steel->alg));
            if(!std::isnan(steel->cl))
                ui->steelC->setText(QString::number(steel->cl));
        }

        if(!steelSelectFrame)
        {
            ui->framePlotSteel->show();
            ui->frameSteelStatus->hide();
            ui->frameTemperature->hide();
        }
        else
        {
            ui->framePlotSteel->hide();
            ui->frameSteelStatus->show();
            ui->frameTemperature->hide();
        }

//        if(listSteel.at(steelReadyNum)->allVectorsReceived)
//        {
            Y_Coord_SteelTemp = listSteel.at(steelReadyNum)->vectorTemp;
//            if(listSteel.at(steelReadyNum)->technology->COH != 0)
//            {
                Y_Coord_SteelEds = listSteel.at(steelReadyNum)->vectorEds;
//            }
//            else
//            {
//                Y_Coord_SteelEds = QVector<double>(Y_Coord_SteelTemp.size(), NAN);
//            }
            X_Coord_Steel.resize(0);
            for(int i = 0; i < Y_Coord_SteelTemp.size(); i++)
            {
                X_Coord_Steel.append((i * PERIOD_MEASURE_STEEL) / 1000.0);
            }
            ui->plotSteel->clearGraphs();
            ui->plotSteel->addGraph();
            ui->plotSteel->graph()->setData(X_Coord_Steel, Y_Coord_SteelTemp);
            ui->plotSteel->xAxis->setLabel("t,sec");
            ui->plotSteel->yAxis2->setLabel("Emf, mV");
            ui->plotSteel->yAxis->setLabel("Temp, °C");
            ui->plotSteel->graph()->setPen(QPen(QBrush(ColorCh3), 2));

            ui->plotSteel->addGraph(ui->plotSteel->xAxis, ui->plotSteel->yAxis2);
            ui->plotSteel->graph()->setData(X_Coord_Steel, Y_Coord_SteelEds);
            ui->plotSteel->graph()->setPen(QPen(QBrush(ColorCh4), 2));
//            ui->plotSteel->rescaleAxes();
            double size = steel->technology->tPt;
            double position = size / 2;
            ui->plotSteel->xAxis->setRange(position, size, Qt::AlignCenter);
            size = (steel->technology->LPth - steel->technology->LPtl) * 1.1;
            position = (steel->technology->LPth + steel->technology->LPtl) / 2;
            ui->plotSteel->yAxis->setRange(position, size, Qt::AlignCenter);
            ui->plotSteel->yAxis2->setRange(100, 800, Qt::AlignCenter);
            ui->plotSteel->replot();
            ui->plotSteel->clearItems();
//        }
    }
    else
    {
        QString str = "Нет данных";
        ui->nameSteelTech->setText(str);
        ui->labelTimeSteel->setText(" ");
        ui->steelTemp->setText(str);
        ui->steelEmf->setText(str);
        ui->steelAO->setText(str);
        ui->steelAl->setText(str);
        ui->steelC->setText(str);
        ui->plotSteel->clearGraphs();

        ui->framePlotSteel->hide();
        ui->frameSteelStatus->show();
        ui->frameTemperature->hide();

        ui->steelTech1->setText(listSteel.at(0)->technology->name);
        ui->steelTech2->setText(listSteel.at(1)->technology->name);
        ui->steelTech3->setText(listSteel.at(2)->technology->name);
        ui->steelTech4->setText(listSteel.at(3)->technology->name);
    }
}

