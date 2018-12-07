#ifndef CIPCONTROLLER_H
#define CIPCONTROLLER_H

#include <QTextCodec>
#include <QObject>
#include <QTimer>
#include <QProcess>

#define DEF_IP "192.168.0.2"
#define DEF_MASK "255.255.255.0"
#define DEF_GW "192.168.0.1"

class cIpController : public QObject
{
    Q_OBJECT
public:
    explicit cIpController(QString eth = "eth0", QObject *parent = 0);
    void setConfig(QString ip = DEF_IP, QString mask = DEF_MASK, QString gw = DEF_GW);
    void setOff();
    QString getIpAddr() { return ipAddr; }
    QString getNetMask() { return netMask; }
    QString getGateWay() { return gateWay; }
    bool getEnable() { return enable; }
    bool getOnline() { return online; }
    bool getDhcpEn() { return dhcpEn; }
    void updateParamEternet();

signals:
    void signalStatus(bool status, bool up);
    void signalErrIp(void);

public slots:

private slots:
    void updateStatus();

private:
    bool dhcpEn;
    QTimer timerUpdate;
    QString ethName;
    bool online;
    bool enable;
    QString ipAddr;
    QString netMask;
    QString gateWay;
    bool winDebugEnable;
    QTextCodec* textCodec;
};

#endif // CIPCONTROLLER_H
