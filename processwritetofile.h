#ifndef CPROCESSWRITETOFILE_H
#define CPROCESSWRITETOFILE_H

#include <QObject>

class cProcessWriteToFile : public QObject
{
    Q_OBJECT
public:
    explicit cProcessWriteToFile(QObject *parent = 0);
    void writeToFile(QString namefile, QString &content);

signals:
    void finished();

public slots:
    void process();

private:
    QString pathToFile;
    QString textToFile;
};

#endif // CPROCESSWRITETOFILE_H
