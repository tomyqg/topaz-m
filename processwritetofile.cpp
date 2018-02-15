#include <QFile>
#include <QTextStream>
#include "processwritetofile.h"
#include "defines.h"


cProcessWriteToFile::cProcessWriteToFile(QObject *parent) : QObject(parent)
{

}

void cProcessWriteToFile::writeToFile(QString namefile, QString &content)
{
    pathToFile = namefile;
    textToFile = content;
}

void cProcessWriteToFile::process()
{
    QFile file(pathToFile);
    while(file.isOpen());   //подождать пока файл не закроется другим потоком
    file.open(QIODevice::ReadWrite);
    file.resize(0); // clear file
    QTextStream out(&file);
    out << textToFile;
    file.close();
    emit finished();
    return;
}
