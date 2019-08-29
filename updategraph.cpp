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
#include "Channels/freq_channel.h"
#include "qrect.h"


#define PERIOD_MEASURE_STEEL 100    //msec период точек на графике анализа стали

int xoffset=0;

double randVal[24] = {((double)((rand()%101) - 50) / 10),\
                     ((double)((rand()%101) - 50) / 10),\
                     ((double)((rand()%101) - 50) / 10),\
                     ((double)((rand()%101) - 50) / 10)};


QVector<double> X_Coordinates, Y_coordinates_Chanel_1, Y_coordinates_Chanel_2, Y_coordinates_Chanel_3, Y_coordinates_Chanel_4;
//QVector<double> X_Coordinates_archive, Y_coordinates_Chanel_1_archive, Y_coordinates_Chanel_2_archive, Y_coordinates_Chanel_3_archive, Y_coordinates_Chanel_4_archive;
QVector<double> vectorsChannels[24];
QVector<double> vectorsChannelsArch[24];
QVector<double> X_Date_Coordinates;
QVector<double> X_Coord_Steel, Y_Coord_SteelTemp, Y_Coord_SteelEds;

extern QList<ChannelOptions*> listChannels;
extern QList<Ustavka *> listUstavok;
extern QList<cSteel*> listSteel;
extern QList<cRelay*> listRelais;
extern cSystemOptions systemOptions;  //класс хранения состемных опций
extern QList<cGroupChannels*> listGroup;
extern QList<cMathChannel *> listMath;
extern QList<cFreqChannel *> listFreq;
extern QMutex mListUstvok;
extern QMutex mListMath;
extern QMutex mListChannel;
extern QMutex mListFreq;


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
//    X_Coordinates_archive.append(xoffset);
    X_Date_Coordinates.append(QDateTime::currentDateTime().toTime_t());

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
    mListChannel.lock();
    foreach (ChannelOptions * channel, listChannels) {

        vectorsChannels[i].append(channel->GetCurrentChannelValue());

//        vectorsChannelsArch[i].append(channel->GetCurrentChannelValue());
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
    mListChannel.unlock();

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
//        Y_coordinates_Chanel_1.removeFirst();
//        Y_coordinates_Chanel_2.removeFirst();
//        Y_coordinates_Chanel_3.removeFirst();
//        Y_coordinates_Chanel_4.removeFirst();
    }

    while (X_Date_Coordinates.length() > 300)
    {
        X_Date_Coordinates.removeFirst();
    }

    //пока нет ограничений на объём хранения
//    while (X_Coordinates_archive.length()>15000)
//    {
//        X_Coordinates_archive.removeFirst();
//        Y_coordinates_Chanel_1_archive.removeFirst();
//        Y_coordinates_Chanel_2_archive.removeFirst();
//        Y_coordinates_Chanel_3_archive.removeFirst();
//        Y_coordinates_Chanel_4_archive.removeFirst();
//    }

    int tickstep = GetTickStep();

    // если последняя точка по иксу попала на тайм-лейбел, его нужно корректировать
    if (xoffset%tickstep==0)
    {
//        LabelsCorrect();
    }

    xoffset++;
    SetPolarAngle(GetPolarAngle()+1);
}

void MainWindow::UpdateGraphics()
{
#ifndef RANDOM_CHAN
    // если нет данных от плат, то и строить нечего
//    if(!slotSteelOnline && !slotAnalogOnline) return;
#endif
    // если прелоадер не завершился
    if(!plotReady) return;
    // если есть плата STEEL, то ставим режим Steel
//    if(slotSteelOnline)
    if(systemOptions.typeMultigraph == cSystemOptions::Multigraph_Steel)
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
//        setTextBars();
        break;
    case cSystemOptions::TrendsBars:
        ui->stackedWidget->setCurrentIndex(0);
        GrafsUpdateTrends();
        updateBars();
//        setTextBars();
        break;
    case cSystemOptions::PolarBars:
        ui->stackedWidget->setCurrentIndex(0);
        updateBars();
//        setTextBars();
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

/*
 *      Построение графиков трендов
 * */
void MainWindow::GrafsUpdateTrends()
{

    if(dialogMenu->isVisible()) return;     //Не рисовать графики пока в меню
    if(pauseUpdateGraph)
    {
        ui->customPlot->replot();
//        ui->customPlot->update();
        return;
    }
    if(curGroupChannel >= listGroup.size()) return;

    cGroupChannels * group = listGroup.at(curGroupChannel);
    double xCurTime = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    QList<QColor> colors;
    colors << ColorCh1 << ColorCh2 << ColorCh3 << ColorCh4;
    double leftXTime, rightXTime;

    if (systemOptions.arrows)
    {
        leftXTime = xCurTime - GetXRange()/2;
        rightXTime = xCurTime + GetXRange()/5;
    }
    else
    {
        leftXTime = xCurTime - GetXRange()/2;
        rightXTime = xCurTime + GetXRange()/100;
    }

    ui->customPlot->xAxis->setRange(leftXTime, rightXTime);
    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setTickLabelFont(QFont("Open Sans", 10));

    graphPen.setWidth(GraphWidthinPixels);


    for(int i = 0; i < MAX_NUM_CHAN_GROUP; i++)
    {
        QVector<double> xCoord = QVector<double>(1, leftXTime);
        QVector<double> yCoord = QVector<double>(1, 0);

        if((group->typeInput[i] == cGroupChannels::Input_Analog) && (group->channel[i] != -1))
        {
            mListChannel.lock();
            ChannelOptions * channel = listChannels.at(group->channel[i]);
            xCoord = channel->GetChannelTimeBuffer();
            yCoord = channel->GetChannelValuesBuffer();
            mListChannel.unlock();
        }
        else if((group->typeInput[i] == cGroupChannels::Input_Math) && (group->mathChannel[i] != -1))
        {
            if((group->mathChannel[i] < listMath.size()) && (listMath.size() != 0))
            {
                mListMath.lock();
                cMathChannel * math = listMath.at(group->mathChannel[i]);
                xCoord = math->GetMathTimeBuffer();
                yCoord = math->GetMathValuesBuffer();
                mListMath.unlock();
            }
        }
        else if((group->typeInput[i] == cGroupChannels::Input_Freq) && (group->freqChannel[i] != -1))
        {
            mListFreq.lock();
            cFreqChannel * channel = listFreq.at(group->freqChannel[i]);
            xCoord = channel->GetChannelTimeBuffer();
            yCoord = channel->GetChannelValuesBuffer();
            mListFreq.unlock();
        }

        if(updateGraph)
        {
            ui->customPlot->clearGraphs();
            updateGraph = false;
        }
        //Добавление в конец недостающих точек или добавление линии тренда целиком
        if(ui->customPlot->graphCount() > i)
        {
            for(int j=0; (j<xCoord.size()) && (j<yCoord.size()); j++)
            {
                if(xCoord.at(j) > ui->customPlot->graph(i)->data()->lastKey())
                {
                    ui->customPlot->graph(i)->addData(xCoord.at(j), yCoord.at(j));
                }
            }
        }
        else
        {
            ui->customPlot->addGraph();
            ui->customPlot->graph()->setData(xCoord, yCoord);
            graphPen.setColor(colors.at(i));
            ui->customPlot->graph()->setPen(graphPen);

            //              Чистка точек за пределами графика
            double lastKey = leftXTime;
            foreach (double key, ui->customPlot->graph()->data()->keys())
            {
                if(key > leftXTime)
                {
                    ui->customPlot->graph()->removeDataBefore(lastKey);
                    break;
                }
                lastKey = key;
            }
        }
    }
    ui->customPlot->xAxis->setAutoTickStep(true);

    ui->customPlot->xAxis->setAutoTickLabels(true);

    // авто масштабирование
    if (systemOptions.autoscale && !waitAutoScale /*ui->autoscalecheckbox->checkState()*/)
    {
        ui->customPlot->yAxis->rescale();
        ui->customPlot->yAxis->setScaleRatio(ui->customPlot->yAxis, 1.1);
    }

    // add the helper arrow:

    if (systemOptions.arrows)
    {

        for(int index = 0; index < MAX_NUM_CHAN_GROUP; index++)
        {
            if(group->typeInput[index] == 0) continue;
            if((group->typeInput[index] == 1) && (group->channel[index] == -1)) continue;
            if((group->typeInput[index] == 2) && (group->mathChannel[index] == -1)) continue;

            double value;
            QString name;
            if(group->typeInput[index] == 1)
            {
                mListChannel.lock();
                value = listChannels.at(group->channel[index])->GetCurrentChannelValue();
                name = listChannels.at(group->channel[index])->GetChannelName();
                mListChannel.unlock();
            }
            else if(group->typeInput[index] == 2)
            {
                mListMath.lock();
                value = listMath.at(group->mathChannel[index])->GetCurrentMathValue();
                name = listMath.at(group->mathChannel[index])->getName();
                mListMath.unlock();
            }


            int ystart = ui->customPlot->height() / 5;
            int xstart = ui->customPlot->width() - ui->customPlot->width() / 10;

            QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
            arrow->start->setPixelPoint(QPointF(xstart - 55, ystart + ui->customPlot->height() * index / 5));
//            arrow->end->setCoords(xoffset, channel->GetCurrentChannelValue()); // point to (4, 1.6) in x-y-plot coordinates
//            arrow->end->setCoords(xoffset, channel->ConvertVisualValue(channel->GetCurrentChannelValue()));
            arrow->end->setCoords(xCurTime, value);
            arrow->setHead(QCPLineEnding::esSpikeArrow);
            arrow->setPen(QPen(colors.at(index), 1, Qt::DashLine));
            arrow->setAntialiased(true);
            ui->customPlot->setAntialiasedElements(QCP::aeAll);
//            ui->customPlot->addItem(arrow);

            arrow->setAntialiased(true);
            ui->customPlot->setAntialiasedElements(QCP::aeAll);

            QCPItemText *NameLabel = new QCPItemText(ui->customPlot);
//            ui->customPlot->addItem(NameLabel);
            NameLabel->position->setPixelPoint(QPointF(xstart+35, ystart + ui->customPlot->height() * index / 5 - 10));
            NameLabel->setText(name);
            NameLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignRight);
            NameLabel->setFont(QFont(Font, 12, QFont::Bold));
            NameLabel->setColor(colors.at(index));

            arrow->deleteLater();
            NameLabel->deleteLater();
        }
    }

//    ui->customPlot->setAntialiasedElements(QCP::aeAll);

    ui->customPlot->replot();

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
    if(curGroupChannel >= listGroup.size()) return;

//    QList<QSpacerItem*> listSpacerBar;
//    listSpacerBar.append(ui->spacerBar0);
//    listSpacerBar.append(ui->spacerBar1);
//    listSpacerBar.append(ui->spacerBar2);
//    listSpacerBar.append(ui->spacerBar3);
//    listSpacerBar.append(ui->spacerBar4);
//    ui->spacerBar0->changeSize(0, 0, QSizePolicy::Maximum);
//    ui->spacerBar1->changeSize(0, 0, QSizePolicy::Maximum);

    QList<wVolueBar*> listBars;
    listBars.append(ui->wBar_1);
    listBars.append(ui->wBar_2);
    listBars.append(ui->wBar_3);
    listBars.append(ui->wBar_4);

    cGroupChannels * group = listGroup.at(curGroupChannel);

    //добавляются дополнительные виджеты для следующей группы сигналов
    cGroupChannels * groupDop;
    if(systemOptions.display == cSystemOptions::Bars)
    {
        if(listGroup.size() > 1)
        {
            listBars.append(ui->wBar_5);
            listBars.append(ui->wBar_6);
            listBars.append(ui->wBar_7);
            listBars.append(ui->wBar_8);
            int indexNextGroup = curGroupChannel + 1;
            if(indexNextGroup >= listGroup.size()) indexNextGroup = 0;
            groupDop = listGroup.at(indexNextGroup);
        }
    }
    else
    {
        ui->wBar_5->hide();
        ui->wBar_6->hide();
        ui->wBar_7->hide();
        ui->wBar_8->hide();
        ui->nameGroupChannels2->hide();
    }

//    listSpacerBar.at(0)->changeSize(0, 0, QSizePolicy::Preferred);

    int i = 0;
    int indexGroup = 0;
    foreach (wVolueBar * bar, listBars)
    {
        if(indexGroup >= 4) group = groupDop;
        bar->hide();
        if(group->typeInput[i] == cGroupChannels::Input_Analog)
        {
            if(group->channel[i] != -1)
            {
                mListChannel.lock();
                ChannelOptions * channel = listChannels.at(group->channel[i]);

#ifdef RANDOM_CHAN
                if((channel->getNum() <= NUM_CHAN_IN_4AI) || channel->enable)
#else
                if(channel->enable)
#endif
                {
                    double diapason = channel->GetUserDiapason();
                    double maximum = channel->ConvertVisualValue(channel->getMaxInDiapason());
                    double minimum = channel->ConvertVisualValue(channel->getMinInDiapason());
                    bar->setExtr(channel->ConvertVisualValue(channel->GetMinimumChannelValue()),\
                                 channel->ConvertVisualValue(channel->GetMaximumChannelValue()));
                    bar->setBarDiapazon(maximum, minimum);
                    bar->setValue(channel->ConvertVisualValue(channel->GetCurrentChannelValue()));
                    bar->cleanMarker();
                    mListUstvok.lock();
                    foreach(Ustavka * ust, listUstavok)
                    {
                        if(ust->getChannel() == channel->getNum())
                        {
                            bar->addMarker(ust->getHiStateValue(), ust->getTypeFix());
                        }
                    }
                    mListUstvok.unlock();
                    bar->show();
                    if(indexGroup >= 4)
                    {
                        ui->nameGroupChannels2->show();
                    }
                }
                mListChannel.unlock();
            }
        }
        else if(group->typeInput[i] == cGroupChannels::Input_Math)
        {
            if(group->mathChannel[i] != -1)
            {
                if((group->mathChannel[i] < listMath.size()) && (listMath.size() != 0))
                {
                    mListMath.lock();
                    cMathChannel * math = listMath.at(group->mathChannel[i]);
//                    bar->setExtr(math->GetMinimumChannelValue(), math->GetMaximumChannelValue());
                    bar->setExtr(math->GetMinimumMathValue(),\
                                 math->GetMaximumMathValue());
                    bar->setBarDiapazon(math->GetHigherMeasureLimit(), \
                                            math->GetLowerMeasureLimit());
                    bar->setValue(math->GetCurrentMathValue());
                    mListMath.unlock();
                    bar->cleanMarker();
                    bar->show();
                    if(indexGroup >= 4)
                    {
                        ui->nameGroupChannels2->show();
                    }
                }
            }
        }
        else if(group->typeInput[i] == cGroupChannels::Input_Freq)
        {
            if(group->freqChannel[i] != -1)
            {
                mListFreq.lock();
                cFreqChannel * channel = listFreq.at(group->freqChannel[i]);

                if(channel->enable)
                {
                    //bar->setExtr(channel->GetMinimumChannelValue(),\
                                 channel->GetMaximumChannelValue());
                    bar->setBarDiapazon(channel->GetHigherMeasureLimit(), \
                                            channel->GetLowerMeasureLimit());
//                    if(channel->getVoltageType() == ChannelOptions::Value_Real)
//                    {
                        bar->setValueType(wVolueBar::BarValue_Real);
//                    }
//                    else
//                    {
//                        bar->setValueType(wVolueBar::BarValue_Procent);
////                        val = channel->GetValuePercent();
//                    }
                    bar->setValue(channel->GetCurrentChannelValue());
                    bar->cleanMarker();
//                    foreach(Ustavka * ust, listUstavok)
//                    {
//                        if(ust->getChannel() == channel->getNum())
//                        {
//                            bar->addMarker(ust->getHiStateValue(), ust->getTypeFix());
//                        }
//                    }
                    bar->show();
                    if(indexGroup >= 4)
                    {
                        ui->nameGroupChannels2->show();
                    }
                }
                mListFreq.unlock();
            }
        }
        i++;
        i %= MAX_NUM_CHAN_GROUP;
        indexGroup++;
    }
}

void MainWindow::setStyleBars()
{
    ui->wBar_1->setColor(ColorCh1, ColorCh1Light);
    ui->wBar_2->setColor(ColorCh2, ColorCh2Light);
    ui->wBar_3->setColor(ColorCh3, ColorCh3Light);
    ui->wBar_4->setColor(ColorCh4, ColorCh4Light);
    ui->wBar_5->setColor(ColorCh5, ColorCh5Light);
    ui->wBar_6->setColor(ColorCh6, ColorCh6Light);
    ui->wBar_7->setColor(ColorCh7, ColorCh7Light);
    ui->wBar_8->setColor(ColorCh8, ColorCh8Light);

//    ui->wBar_1->setText(channel1.GetChannelName(), channel1.GetUnitsName());
//    ui->wBar_2->setText(channel2.GetChannelName(), channel2.GetUnitsName());
//    ui->wBar_3->setText(channel3.GetChannelName(), channel3.GetUnitsName());
    //    ui->wBar_4->setText(channel4.GetChannelName(), channel4.GetUnitsName());
}

void MainWindow::setTextBars()
{
    if(curGroupChannel >= listGroup.size()) return;

    QList<wVolueBar*> listBars;
    listBars.append(ui->wBar_1);
    listBars.append(ui->wBar_2);
    listBars.append(ui->wBar_3);
    listBars.append(ui->wBar_4);
    cGroupChannels * group = listGroup.at(curGroupChannel);

    //добавляются дополнительные виджеты для следующей группы сигналов
    cGroupChannels * groupDop;
    if(systemOptions.display == cSystemOptions::Bars)
    {
        if(listGroup.size() > 1)
        {
            listBars.append(ui->wBar_5);
            listBars.append(ui->wBar_6);
            listBars.append(ui->wBar_7);
            listBars.append(ui->wBar_8);
            int indexNextGroup = curGroupChannel + 1;
            if(indexNextGroup >= listGroup.size()) indexNextGroup = 0;
            groupDop = listGroup.at(indexNextGroup);
        }
    }
    else
    {
        ui->wBar_5->hide();
        ui->wBar_6->hide();
        ui->wBar_7->hide();
        ui->wBar_8->hide();
        ui->nameGroupChannels2->hide();
    }

    int i = 0;
    int indexGroup = 0;
    foreach (wVolueBar * bar, listBars)
    {
        if(indexGroup >= 4) group = groupDop;
        if(group->typeInput[i] == cGroupChannels::Input_Analog)
        {
            if(group->channel[i] != -1)
            {
                mListChannel.lock();
                bar->setNumChan(listChannels.at(group->channel[i])->getNum());
                bar->setText(listChannels.at(group->channel[i])->GetChannelName(), \
                             listChannels.at(group->channel[i])->GetUnitsName());
                mListChannel.unlock();
            }

        }
        else if(group->typeInput[i] == cGroupChannels::Input_Math)
        {
            if(group->mathChannel[i] != -1)
            {
                if((group->mathChannel[i] < listMath.size()) && (listMath.size() != 0))
                {
                    mListMath.lock();
                    bar->setNumChan(listMath.at(group->mathChannel[i])->getNum());
                    bar->setText(listMath.at(group->mathChannel[i])->getName(), \
                                 listMath.at(group->mathChannel[i])->getUnit());
                    mListMath.unlock();
                }
                else
                {
                    bar->setNumChan(0);
                    bar->setText("", "");
                }
            }
        }
        else if(group->typeInput[i] == cGroupChannels::Input_Freq)
        {
            if(group->freqChannel[i] != -1)
            {
                mListFreq.lock();
                bar->setNumChan(listFreq.at(group->freqChannel[i])->getNum());
                bar->setText(listFreq.at(group->freqChannel[i])->GetChannelName(), \
                             listFreq.at(group->freqChannel[i])->getUnit());
                mListFreq.unlock();
            }
        }

        i++;
        i %= MAX_NUM_CHAN_GROUP;
        indexGroup++;
    }
}



void MainWindow::updateWidgetsVols(void)
{
    if(curGroupChannel >= listGroup.size()) return;

    QList<wVol*> listVols;
    listVols.append(ui->widgetVol1);
    listVols.append(ui->widgetVol2);
    listVols.append(ui->widgetVol3);
    listVols.append(ui->widgetVol4);
    cGroupChannels * group = listGroup.at(curGroupChannel);

    //добавляются дополнительные виджеты для следующей группы сигналов
    cGroupChannels * groupDop;
    if(systemOptions.display == cSystemOptions::Cyfra)
    {
        if(listGroup.size() > 1)
        {
            listVols.append(ui->widgetVol5);
            listVols.append(ui->widgetVol6);
            listVols.append(ui->widgetVol7);
            listVols.append(ui->widgetVol8);
            int indexNextGroup = curGroupChannel + 1;
            if(indexNextGroup >= listGroup.size()) indexNextGroup = 0;
            groupDop = listGroup.at(indexNextGroup);
        }
    }
    else
    {
        ui->widgetVol5->hide();
        ui->widgetVol6->hide();
        ui->widgetVol7->hide();
        ui->widgetVol8->hide();
        ui->nameGroupChannels2->hide();
//        ui->horizontalSpacerGroupName1->changeSize(0, 0, QSizePolicy::Maximum, QSizePolicy::Maximum);
//        ui->horizontalSpacerGroupName2->changeSize(0, 0, QSizePolicy::Maximum, QSizePolicy::Maximum);

    }

    QColor color[8] = {ColorCh1, ColorCh2, ColorCh3, ColorCh4, ColorCh5, ColorCh6, ColorCh7, ColorCh8};

    int i = 0;
    int indexGroup = 0;
    foreach (wVol * vol, listVols)
    {
        if(indexGroup >= 4) group = groupDop;
        if(group->typeInput[i] == cGroupChannels::Input_Analog)
        {
            if(group->channel[i] != -1)
            {
                mListChannel.lock();
                vol->setChanNum(listChannels.at(group->channel[i])->getNum());
                vol->setText(listChannels.at(group->channel[i])->GetChannelName(), \
                             listChannels.at(group->channel[i])->GetUnitsName());
                mListChannel.unlock();
            }
            vol->setColor(color[indexGroup]);
        }
        else if(group->typeInput[i] == cGroupChannels::Input_Math)
        {
            if(group->mathChannel[i] != -1)
            {
                if((group->mathChannel[i] < listMath.size()) && (listMath.size() != 0))
                {
                    mListMath.lock();
                    vol->setChanNum(listMath.at(group->mathChannel[i])->getNum());
                    vol->setText(listMath.at(group->mathChannel[i])->getName(), \
                                 listMath.at(group->mathChannel[i])->getUnit());
                    mListMath.unlock();
                }
                else
                {
                    vol->setChanNum(0);
                    vol->setText("", "");
                }
            }
            vol->setColor(color[indexGroup]);
        }
        else if(group->typeInput[i] == cGroupChannels::Input_Freq)
        {
            if(group->freqChannel[i] != -1)
            {
                mListFreq.lock();
                vol->setChanNum(listFreq.at(group->freqChannel[i])->getNum());
                vol->setText(listFreq.at(group->freqChannel[i])->GetChannelName(), \
                             listFreq.at(group->freqChannel[i])->getUnit());
                mListFreq.unlock();
            }
            vol->setColor(color[indexGroup]);
        }
        i++;
        i %= MAX_NUM_CHAN_GROUP;
        indexGroup++;
    }
}

void MainWindow::updateVols()
{
    if(curGroupChannel >= listGroup.size()) return;

    QList<wVol*> listVols;
    listVols.append(ui->widgetVol1);
    listVols.append(ui->widgetVol2);
    listVols.append(ui->widgetVol3);
    listVols.append(ui->widgetVol4);
    cGroupChannels * group = listGroup.at(curGroupChannel);

    cGroupChannels * groupDop;
    if(systemOptions.display == cSystemOptions::Cyfra)
    {
        if(listGroup.size() > 1)
        {
            listVols.append(ui->widgetVol5);
            listVols.append(ui->widgetVol6);
            listVols.append(ui->widgetVol7);
            listVols.append(ui->widgetVol8);
            int indexNextGroup = curGroupChannel + 1;
            if(indexNextGroup >= listGroup.size()) indexNextGroup = 0;
            groupDop = listGroup.at(indexNextGroup);
        }
    }
    else
    {
        ui->widgetVol5->hide();
        ui->widgetVol6->hide();
        ui->widgetVol7->hide();
        ui->widgetVol8->hide();
    }

    int i = 0;
    int indexGroup = 0;
    foreach (wVol * vol, listVols)
    {
        if(indexGroup >= 4) group = groupDop;
        vol->hide();
        if(group->typeInput[i] == cGroupChannels::Input_Analog)
        {
            if(group->channel[i] != -1)
            {
                mListChannel.lock();
                ChannelOptions * channel = listChannels.at(group->channel[i]);
#ifdef RANDOM_CHAN
                if((channel->getNum() <= NUM_CHAN_IN_4AI) || channel->enable)
#else
                if(listChannels.at(group->channel[i])->enable)
#endif
                {
//                    double value = channel->GetCurrentChannelValue();
//                    if(channel->getVoltageType() == ChannelOptions::Value_Real)
//                    {
//                        vol->setVol(channel->GetCurrentChannelValue(), channel->getPrecision());
//                    }
//                    else
//                    {
                    vol->setVol(channel->ConvertVisualValue(channel->GetCurrentChannelValue(), \
                                                            channel->GetUserDiapason()), \
                                channel->getPrecision());
//                    }
                    vol->show();
                    if(indexGroup >= 4)
                    {
                        ui->nameGroupChannels2->show();
                    }

                }
                mListChannel.unlock();
            }
        }
        else if(group->typeInput[i] == cGroupChannels::Input_Math)
        {
            if(group->mathChannel[i] != -1)
            {
                if((group->mathChannel[i] < listMath.size()) && (listMath.size() != 0))
                {
                    mListMath.lock();
                    vol->setVol(listMath.at(group->mathChannel[i])->GetCurrentMathValue());
                    mListMath.unlock();
                    vol->show();
                    if(indexGroup >= 4)
                    {
                        ui->nameGroupChannels2->show();
                    }
                }
            }
        }
        else if(group->typeInput[i] == cGroupChannels::Input_Freq)
        {
            if(group->freqChannel[i] != -1)
            {
                mListFreq.lock();
                cFreqChannel * channel = listFreq.at(group->freqChannel[i]);
                if(channel->enable)
                {
//                    double value = channel->GetCurrentChannelValue();
//                    if(channel->getVoltageType() == ChannelOptions::Value_Real)
//                    {
                        vol->setVol(channel->GetCurrentChannelValue()/*, channel->optimalPrecision()*/);
//                    }
//                    else
//                    {
//                        vol->setVol(channel->GetValuePercent(), channel->optimalPrecision());
//                    }
                    vol->show();
                    if(indexGroup >= 4)
                    {
                        ui->nameGroupChannels2->show();
                    }

                }
                mListFreq.unlock();
            }
        }
        i++;
        i %= MAX_NUM_CHAN_GROUP;
        indexGroup++;
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
    if(systemOptions.typeMultigraph != cSystemOptions::Multigraph_Steel) return;

    /*
     * Управление реле
     */
    for(int i = 0; i<listSteel.size(); i++)
    {
        if(listRelais.size() == 0) continue;
        cSteel * steel = listSteel.at(i);
        uint8_t relayStates[SUM_RELAYS];
        memset(relayStates, 0, SUM_RELAYS);
        if(steel->status == ESCS_ON)
        {
            relayStates[1] = 1;
        }
        else if(steel->status == ESCS_MEASURE)
        {
            relayStates[2] = 1;
        }
        else if((steel->status == ESCS_NOFIND_TEMP)\
                || (steel->status == ESCS_NOFIND_EMF)\
                || (steel->status == ESCS_NOFIND)\
                || (steel->status == ESCS_ERROR_TC)\
                || (steel->status == ESCS_ERROR_EMF)\
                || (steel->status == ESCS_FIND))
        {
            relayStates[3] = 1;
        }
        else if(/*(steel->status == StatusCh_WaitConf) || */(steel->status == ESCS_BREAK)) //обрыв датчика
        {
            relayStates[0] = 1;
        }

        for(int i = 0; i < SUM_RELAYS; i++)
        {
            int8_t relay = steel->relais[i];
            if((relay != -1) && (listRelais.size() > relay))
            {
                if(listRelais.at(relay)->getCurState() != relayStates[i])
                {
                    listRelais.at(relay)->setState(relayStates[i]);
                    steel->countRelayTime = 4000 / UpdateSteelTime;
                }
                if((listRelais.at(relay)->getCurState() == true) && (i == 3))
                {
                    if(steel->countRelayTime > 0)
                    {
                        steel->countRelayTime--;
                    }
                    else
                    {
                        listRelais.at(relay)->setState(false);
                    }
                }
            }
        }
    }
}

void MainWindow::slotSteelFrame(bool steelFrame)
{
    steelSelectFrame = steelFrame;
}

void MainWindow::updateSteelWidget(void)
{
    QList<QString> strings;
    strings << "ОТКЛЮЧЕНО" << "ГОТОВ" << "ИЗМЕРЕНИЕ" \
            << "ОШИБКА TC" << "ОШИБКА ЭДС" << "НЕТ ПЛОЩАДКИ TC" \
            << "НЕТ ПЛОЩАДКИ ЭДС" << "НЕТ ПЛОЩАДОК" << "КОНЕЦ ИЗМЕРЕНИЯ" \
            << "ОБРЫВ TC" << "ОШИБКА КАНАЛА";
    QList<QString> colorStyles;
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_GRAY.red()) + ", " + QString::number(COLOR_GRAY.green()) + ", " + QString::number(COLOR_GRAY.blue()) + ");color: rgb(0, 0, 0);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_1.red()) + ", " + QString::number(COLOR_1.green()) + ", " + QString::number(COLOR_1.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_2.red()) + ", " + QString::number(COLOR_2.green()) + ", " + QString::number(COLOR_2.blue()) + ");color: rgb(255, 255, 255);");

    colorStyles.append("background-color: rgb(" + QString::number(COLOR_3.red()) + ", " + QString::number(COLOR_3.green()) + ", " + QString::number(COLOR_3.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_3.red()) + ", " + QString::number(COLOR_3.green()) + ", " + QString::number(COLOR_3.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_GRAY.red()) + ", " + QString::number(COLOR_GRAY.green()) + ", " + QString::number(COLOR_GRAY.blue()) + ");color: rgb(0, 0, 0);");

    colorStyles.append("background-color: rgb(" + QString::number(COLOR_GRAY.red()) + ", " + QString::number(COLOR_GRAY.green()) + ", " + QString::number(COLOR_GRAY.blue()) + ");color: rgb(0, 0, 0);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_GRAY.red()) + ", " + QString::number(COLOR_GRAY.green()) + ", " + QString::number(COLOR_GRAY.blue()) + ");color: rgb(0, 0, 0);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_1.red()) + ", " + QString::number(COLOR_1.green()) + ", " + QString::number(COLOR_1.blue()) + ");color: rgb(255, 255, 255);");

    colorStyles.append("background-color: rgb(" + QString::number(COLOR_4.red()) + ", " + QString::number(COLOR_4.green()) + ", " + QString::number(COLOR_4.blue()) + ");color: rgb(255, 255, 255);");
    colorStyles.append("background-color: rgb(" + QString::number(COLOR_3.red()) + ", " + QString::number(COLOR_3.green()) + ", " + QString::number(COLOR_3.blue()) + ");color: rgb(255, 255, 255);");

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

        Y_Coord_SteelTemp = steel->vectorTemp;
        Y_Coord_SteelEds = steel->vectorEds;
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


void MainWindow::slotReadySteel(int n)
{
    steelReadyNum = n;
    stateWidgetSteel = STEEL_READY;
}

void MainWindow::slotMeasureSteel(int n)
{
    steelReadyNum = n;
    stateWidgetSteel = STEEL_MEASURE;
}

void MainWindow::slotWaitSteel(int n)
{
    steelReadyNum = n;
    stateWidgetSteel = STEEL_WAIT;
}

void MainWindow::slotSteelArchivate(int n)
{
    logginSteel(n);
    writeArchiveSteel(n);
}
