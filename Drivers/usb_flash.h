#ifndef CUSBFLASH_H
#define CUSBFLASH_H

#include <QObject>
#include <QTimer>

class cUsbFlash : public QObject
{
    Q_OBJECT
public:
    explicit cUsbFlash(QObject *parent = 0);
    QString getNameDisk(uint8_t num);
    int getNumDisk(void);

signals:
    void newFlash(int size);

public slots:
    void scanUsbFlash();

private:
    QTimer timerScanFlash;
    QStringList listDiscs;
    int  sizeListFlash;
    bool firstScan;
};





#endif // CUSBFLASH_H
