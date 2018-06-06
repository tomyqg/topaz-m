#-------------------------------------------------
#
# Project created by QtCreator 2012-03-04T23:24:55
# By Andrey Shmelev e-mail: andrshmel@gmail.com +79227112858
#-------------------------------------------------

QT += core gui #script
unix:QT += script
QT += serialport
QT += testlib

VERSION = 1.0.0

Q_OS_LINUX = 1

QT_IM_MODULE=mockup
CONFIG += mockup

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
CONFIG += c++11

TARGET = TopazM_MB
INCLUDEPATH += 3rdparty/libmodbus 3rdparty/qextserialport   \
        Anybus Anybus/abcc_abp Anybus/abcc_adapt Anybus/abcc_drv Anybus/abcc_obj \
        Anybus/abcc_drv/inc Anybus/abcc_drv/src/spi Anybus/abcc_obj/nw_obj \
        Drivers/ Communicator/ Grafika/
TEMPLATE = app

SOURCES += main.cpp \
    qcustomplot.cpp \
    updategraph.cpp \
    keyboard.cpp \
    eventfilter.cpp \
    uartdriver.cpp \
    readoptionsfromfile.cpp \
    messages.cpp \
    writetofile.cpp \
    metrologicalcalc.cpp \
    mathresolver.cpp \
    options_gui.cpp \
    options_logic.cpp \
    mainwindow_gui.cpp \
    mainwindow_logic.cpp \
    painting.cpp \
    worker.cpp \
    3rdparty/libmodbus/src/modbus-data.c \
    3rdparty/libmodbus/src/modbus-rtu.c \
    3rdparty/libmodbus/src/modbus-tcp.c \
    3rdparty/libmodbus/src/modbus.c \
    3rdparty/qextserialport/qextserialport.cpp \
    stackedoptions.cpp \
    device.cpp \
    ustavka.cpp \
    Slots/slot.cpp \
    Channels/channelOptions.cpp \
    Channels/channelslotconnector.cpp \
    Channels/channelslotcontroller.cpp \
    Relais/relayslotcontroller.cpp \
    Relais/relayslotconnector.cpp \
    Slots/slotsconfig.cpp \
    registermap.cpp \
    processwritetofile.cpp \
    Slots/devicechannel.cpp \
    archivator.cpp \
    log.cpp \
    kvitirovanie.cpp \
    Grafika/voluebar.cpp \
    Grafika/buttonstyled.cpp \
    menu.cpp \
    settings.cpp \
    Grafika/vol.cpp \
    Alarms/alarm.cpp \
    filemanager.cpp

unix:SOURCES += 3rdparty/qextserialport/posix_qextserialport.cpp	\
                3rdparty/qextserialport/qextserialenumerator_unix.cpp   \
                Drivers/driveri2c.cpp \
                Drivers/driverspi.c \
                Drivers/pin.c \
                Communicator/communicator.cpp \
                Anybus/abcc_adapt/abcc_sw_port.c \
                Anybus/abcc_drv/src/spi/abcc_crc32.c \
                Anybus/abcc_drv/src/spi/abcc_handler_spi.c \
                Anybus/abcc_drv/src/spi/abcc_spi_drv.c \
                Anybus/abcc_drv/src/abcc_cmd_seq.c \
                Anybus/abcc_drv/src/abcc_debug_err.c \
                Anybus/abcc_drv/src/abcc_handler.c \
                Anybus/abcc_drv/src/abcc_link.c \
                Anybus/abcc_drv/src/abcc_mem.c \
                Anybus/abcc_drv/src/abcc_remap.c \
                Anybus/abcc_drv/src/abcc_setup.c \
                Anybus/abcc_drv/src/abcc_timer.c \
                Anybus/abcc_obj/nw_obj/ccl.c \
                Anybus/abcc_obj/nw_obj/cfn.c \
                Anybus/abcc_obj/nw_obj/cop.c \
                Anybus/abcc_obj/nw_obj/dev.c \
                Anybus/abcc_obj/nw_obj/dpv1.c \
                Anybus/abcc_obj/nw_obj/ect.c \
                Anybus/abcc_obj/nw_obj/eip.c \
                Anybus/abcc_obj/nw_obj/epl.c \
                Anybus/abcc_obj/nw_obj/mod.c \
                Anybus/abcc_obj/nw_obj/prt.c \
                Anybus/abcc_obj/ad_obj.c \
                Anybus/abcc_obj/app_obj.c \
                Anybus/abcc_obj/etn_obj.c \
                Anybus/abcc_obj/opcua_obj.c \
                Anybus/abcc_obj/safe_obj.c \
                Anybus/abcc_obj/sync_obj.c \
                Anybus/abcc_adapt/abcc_sys_adapt.c \
                Anybus/appl_abcc_handler.c \
                Anybus/appl_adi_config.c

unix:DEFINES += _TTY_POSIX_

win32:SOURCES += 3rdparty/qextserialport/win_qextserialport.cpp \
                        3rdparty/qextserialport/qextserialenumerator_win.cpp
win32:DEFINES += _TTY_WIN_  WINVER=0x0501
win32:LIBS += -lsetupapi -lwsock32
win32:LIBS += -lws2_32

HEADERS  += mainwindow.h \
         qcustomplot.h \
    updategraph.h \
    options.h \
    keyboard.h \
    messages.h \
    uartdriver.h \
    metrologicalcalc.h \
    mathresolver.h \
    worker.h \
    defines.h \
    3rdparty/qextserialport/qextserialenumerator.h \
    3rdparty/qextserialport/qextserialport.h \
    device.h \
    stackedoptions.h \
    registermap.h \
    transaction.h \
    ustavka.h \
    Slots/slot.h \
    Channels/channelOptions.h \
    Channels/channelslotconnector.h \
    Channels/channelslotcontroller.h \
    Relais/relayslotcontroller.h \
    Relais/relayslotconnector.h \
    Slots/slotsconfig.h \
    processwritetofile.h \
    Slots/devicechannel.h \
    archivator.h \
    log.h \
    kvitirovanie.h \
    Grafika/voluebar.h \
    Grafika/buttonstyled.h \
    menu.h \
    settings.h \
    Grafika/vol.h \
    Alarms/alarm.h \
    filemanager.h

unix:HEADERS += Drivers/driveri2c.h \
            Drivers/driverspi.h \
            Drivers/pin.h \
            Communicator/communicator.h \
            Anybus/abcc_abp/abp.h \
            Anybus/abcc_abp/abp_add.h \
            Anybus/abcc_abp/abp_asm.h \
            Anybus/abcc_abp/abp_bac.h \
            Anybus/abcc_abp/abp_ccl.h \
            Anybus/abcc_abp/abp_cfn.h \
            Anybus/abcc_abp/abp_cipid.h \
            Anybus/abcc_abp/abp_cnt.h \
            Anybus/abcc_abp/abp_cop.h \
            Anybus/abcc_abp/abp_cpc.h \
            Anybus/abcc_abp/abp_cpn.h \
            Anybus/abcc_abp/abp_dev.h \
            Anybus/abcc_abp/abp_dpv0di.h \
            Anybus/abcc_abp/abp_dpv1.h \
            Anybus/abcc_abp/abp_eco.h \
            Anybus/abcc_abp/abp_ect.h \
            Anybus/abcc_abp/abp_eip.h \
            Anybus/abcc_abp/abp_eme.h \
            Anybus/abcc_abp/abp_epl.h \
            Anybus/abcc_abp/abp_er.h \
            Anybus/abcc_abp/abp_etn.h \
            Anybus/abcc_abp/abp_fsi.h \
            Anybus/abcc_abp/abp_fusm.h \
            Anybus/abcc_abp/abp_mdd.h \
            Anybus/abcc_abp/abp_mod.h \
            Anybus/abcc_abp/abp_nwccl.h \
            Anybus/abcc_abp/abp_nwcfn.h \
            Anybus/abcc_abp/abp_nwdpv1.h \
            Anybus/abcc_abp/abp_nwetn.h \
            Anybus/abcc_abp/abp_nwpnio.h \
            Anybus/abcc_abp/abp_opcua.h \
            Anybus/abcc_abp/abp_pnam.h \
            Anybus/abcc_abp/abp_pnio.h \
            Anybus/abcc_abp/abp_safe.h \
            Anybus/abcc_abp/abp_smtp.h \
            Anybus/abcc_abp/abp_soc.h \
            Anybus/abcc_abp/abp_src3.h \
            Anybus/abcc_abp/abp_sync.h \
            Anybus/abcc_adapt/abcc_drv_cfg.h \
            Anybus/abcc_adapt/abcc_identification.h \
            Anybus/abcc_adapt/abcc_obj_cfg.h \
            Anybus/abcc_adapt/abcc_platform_cfg.h \
            Anybus/abcc_adapt/abcc_sw_port.h \
            Anybus/abcc_adapt/abcc_td.h \
            Anybus/abcc_drv/inc/abcc.h \
            Anybus/abcc_drv/inc/abcc_ad_if.h \
            Anybus/abcc_drv/inc/abcc_cfg.h \
            Anybus/abcc_drv/inc/abcc_cmd_seq_if.h \
            Anybus/abcc_drv/inc/abcc_port.h \
            Anybus/abcc_drv/inc/abcc_sys_adapt.h \
            Anybus/abcc_drv/inc/abcc_sys_adapt_spi.h \
            Anybus/abcc_drv/src/spi/abcc_crc32.h \
            Anybus/abcc_drv/src/spi/abcc_drv_spi_if.h \
            Anybus/abcc_drv/src/abcc_cmd_seq.h \
            Anybus/abcc_drv/src/abcc_debug_err.h \
            Anybus/abcc_drv/src/abcc_drv_if.h \
            Anybus/abcc_drv/src/abcc_handler.h \
            Anybus/abcc_drv/src/abcc_link.h \
            Anybus/abcc_drv/src/abcc_mem.h \
            Anybus/abcc_drv/src/abcc_setup.h \
            Anybus/abcc_drv/src/abcc_timer.h \
            Anybus/abcc_obj/nw_obj/ccl.h \
            Anybus/abcc_obj/nw_obj/cfn.h \
            Anybus/abcc_obj/nw_obj/cop.h \
            Anybus/abcc_obj/nw_obj/dev.h \
            Anybus/abcc_obj/nw_obj/dpv1.h \
            Anybus/abcc_obj/nw_obj/ect.h \
            Anybus/abcc_obj/nw_obj/eip.h \
            Anybus/abcc_obj/nw_obj/epl.h \
            Anybus/abcc_obj/nw_obj/mod.h \
            Anybus/abcc_obj/nw_obj/prt.h \
            Anybus/abcc_obj/ad_obj.h \
            Anybus/abcc_obj/app_obj.h \
            Anybus/abcc_obj/etn_obj.h \
            Anybus/abcc_obj/opcua_obj.h \
            Anybus/abcc_obj/safe_obj.h \
            Anybus/abcc_obj/sync_obj.h \
            Anybus/abcc_versions.h \
            Anybus/appl_abcc_handler.h \
            Anybus/appl_adi_config.h

FORMS    += mainwindow.ui \
    options.ui \
    keyboard.ui \
    messages.ui \
    stackedoptions.ui \
    kvitirovanie.ui \
    Grafika/voluebar.ui \
    Grafika/buttonstyled.ui \
    menu.ui \
    settings.ui \
    Grafika/vol.ui

target.path  = /opt
INSTALLS    += target

DISTFILES += \
    ../build-untitled2-MYD-Release/Makefile \
    translator_en.ts \
    untitled_2_de.ts \
    untitled_2_en.ts \
    untitled_2_ru.ts \
    untitled2_de.ts \
    untitled2_en.ts \
    untitled2_ru.ts \
    ../build-TopazM_MB-MYD1-Debug/Makefile \
    ../build-TopazM_MB-MYD1-Release/Makefile \
    ../build-TopazM_MB-MYD1-Debug/Makefile


TRANSLATIONS +=  untitled2_en.ts \
                 untitled2_ru.ts \
                 untitled2_de.ts

RESOURCES +=
