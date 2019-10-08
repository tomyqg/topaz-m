#-------------------------------------------------
#
#-------------------------------------------------

QT += core gui qml #script
unix:QT += script
QT += serialport
#QT += testlib
QT += network

VERSION = 1.80
QMAKE_TARGET_COMPANY = Teplopribor-Sensor
QMAKE_TARGET_PRODUCT = Multigraph
QMAKE_TARGET_DESCRIPTION = Рaperless video recorder
QMAKE_TARGET_COPYRIGHT = Vagiz Abdulin

Q_OS_LINUX = 1

QT_IM_MODULE=mockup
CONFIG += mockup

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
CONFIG += c++11
unix:CONFIG += serialport

TARGET = TopazM_MB
INCLUDEPATH += 3rdparty/libmodbus 3rdparty/qextserialport   \
        Anybus Anybus/abcc_abp Anybus/abcc_adapt Anybus/abcc_drv Anybus/abcc_obj \
        Anybus/abcc_drv/inc Anybus/abcc_drv/src/spi Anybus/abcc_obj/nw_obj \
        Drivers Communicator Grafika LookupTable Steel Relais Server Digital CRC
TEMPLATE = app

RC_ICONS = icon.ico

SOURCES += main.cpp \
    qcustomplot.cpp \
    updategraph.cpp \
    keyboard.cpp \
#    eventfilter.cpp \
    uartdriver.cpp \
    readoptionsfromfile.cpp \
    messages.cpp \
    writetofile.cpp \
    metrologicalcalc.cpp \
    mathresolver.cpp \
#    options_gui.cpp \
#    options_logic.cpp \
    mainwindow_gui.cpp \
    mainwindow_logic.cpp \
    painting.cpp \
    worker.cpp \
    3rdparty/libmodbus/src/modbus-data.c \
    3rdparty/libmodbus/src/modbus-rtu.c \
    3rdparty/libmodbus/src/modbus.c \
    3rdparty/qextserialport/qextserialport.cpp \
#    stackedoptions.cpp \
    ustavka.cpp \
    Slots/slot.cpp \
    Channels/channelOptions.cpp \
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
    filemanager.cpp \
    systemoptions.cpp \
    LookupTable/lookup_tables.c \
    LookupTable/registersmap.cpp \
    archworker.cpp \
    Steel/steel.cpp \
    Relais/relay.cpp \
    Server/serverhttp.cpp \
    Server/socket_thread.cpp \
    Drivers/usb_flash.cpp \
    Digital/digital_input.cpp \
    smartconnect.cpp \
    Channels/math_channel.cpp \
    Channels/group_channels.cpp \
    customperiod.cpp \
    device_slot.cpp \
    expert_access.cpp \
    ip_controller.cpp \
    qtcsv-master/sources/contentiterator.cpp \
    qtcsv-master/sources/reader.cpp \
    qtcsv-master/sources/stringdata.cpp \
    qtcsv-master/sources/variantdata.cpp \
    qtcsv-master/sources/writer.cpp \
    ExtModbus/ext_modbus.cpp \
    ExtModbus/modbus_lookup_tables.c \
    3rdparty/libmodbus/src/modbus-tcp.c \
    mainwindow_ext_interface.cpp \
    Channels/freq_channel.cpp \
    CRC/mbcrc.c

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

DEFINES += QTCSV_STATIC_LIB

HEADERS  += mainwindow.h \
         qcustomplot.h \
    updategraph.h \
#    options.h \
    keyboard.h \
    messages.h \
    uartdriver.h \
    metrologicalcalc.h \
    mathresolver.h \
    worker.h \
    defines.h \
    3rdparty/qextserialport/qextserialenumerator.h \
    3rdparty/qextserialport/qextserialport.h \
#    stackedoptions.h \
    transaction.h \
    ustavka.h \
    Slots/slot.h \
    Channels/channelOptions.h \
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
    filemanager.h \
    systemoptions.h \
    LookupTable/registersmap.h \
    LookupTable/lookup_tables.h \
    LookupTable/deviceparams.h \
    archworker.h \
    archticks.h \
    Steel/steel.h \
    Steel/steel_technology.h \
    Relais/relay.h \
    Server/serverhttp.h \
    Server/socket_thread.h \
    Drivers/usb_flash.h \
    Digital/digital_input.h \
    smartconnect.h \
    Channels/math_channel.h \
    Channels/group_channels.h \
    customperiod.h \
    device_slot.h \
    expert_access.h \
    ip_controller.h \
    qtcsv-master/include/qtcsv/abstractdata.h \
    qtcsv-master/include/qtcsv/qtcsv_global.h \
    qtcsv-master/include/qtcsv/reader.h \
    qtcsv-master/include/qtcsv/stringdata.h \
    qtcsv-master/include/qtcsv/variantdata.h \
    qtcsv-master/include/qtcsv/writer.h \
    qtcsv-master/sources/contentiterator.h \
    qtcsv-master/sources/filechecker.h \
    qtcsv-master/sources/symbols.h \
    ExtModbus/ext_modbus.h \
    ExtModbus/device_params.h \
    ExtModbus/modbus_lookup_tables.h \
    Channels/freq_channel.h \
    CRC/mbcrc.h
 
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
    keyboard.ui \
    messages.ui \
#    stackedoptions.ui \
    kvitirovanie.ui \
    Grafika/voluebar.ui \
    Grafika/buttonstyled.ui \
    menu.ui \
    settings.ui \
    Grafika/vol.ui \
    custom_period.ui
#    _options.ui

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
    ../build-TopazM_MB-MYD1-Debug/Makefile \
    ../build-TopazM_MB-MYD3-Release/Makefile


TRANSLATIONS +=  untitled2_en.ts \
                 untitled2_ru.ts \
                 untitled2_de.ts

###### WevServer #############################
HEADERS += \
           Server/src/requestmapper.h \
           Server/src/controller/dumpcontroller.h \
           Server/src/controller/templatecontroller.h \
           Server/src/controller/formcontroller.h \
           Server/src/controller/fileuploadcontroller.h \
           Server/src/controller/sessioncontroller.h

SOURCES += \
           Server/src/requestmapper.cpp \
           Server/src/controller/dumpcontroller.cpp \
           Server/src/controller/templatecontroller.cpp \
           Server/src/controller/formcontroller.cpp \
           Server/src/controller/fileuploadcontroller.cpp \
           Server/src/controller/sessioncontroller.cpp

OTHER_FILES += Server/etc/* Server/etc/docroot/*
OTHER_FILES += Server/etc/templates/* Server/etc/ssl/*
OTHER_FILES += Server/logs/*

include(Server/QtWebApp/logging/logging.pri)
include(Server/QtWebApp/httpserver/httpserver.pri)
include(Server/QtWebApp/templateengine/templateengine.pri)
###### /WevServer #############################

