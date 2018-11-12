#ifndef CLOG_H
#define CLOG_H

#include <QObject>
#include <QQueue>
#include <QJsonArray>

#define MAX_SIZE_FILE_LOG (1024*1024) //максимальная размер лог файла (1 МБайт)

class cLogger : public QObject
{
    Q_OBJECT
public:
    explicit cLogger(QString file, int src = 4, QObject *parent = 0);
    void addMess(QString mess, int cls = 1, int src = 0);
    QJsonArray MessRead();
    enum eventClass {
        ERR = 0,          // Ошибки программы, прибора, измерений
        WARNING = 1,      // Предупреждения
        LIMIT = 2,        // Предельные значения
        USER = 3,         // События, настраиваемые пользователем
        SERVICE = 4,      // Доступные только в сервисном режиме
        STATISTIC = 5     // Записи для статистики
    };
    enum eventSource {
        NONE = 0,        // Источник не задан
        USTAVKA = 1,     // Сработки уставок
        UI = 2,          // Интерфейс (кнопки, окна, настройки)
        CHANNEL = 3,     // Сообщения от входных каналов
        DEVICE = 4,      // Устройство, платы
        EXT_NET = 5,     // Внешняя сеть
        ARCHIVE = 6,     // Архив
        STEEL = 7,       // Анализ стали
        CONFIG = 8,      // Конфигуратор
        MODBUS = 9       // Modbus
    };

signals:

public slots:

private:
    QString logFile;
    QQueue <QString> qLog;
    int source;     //источник события
};

#endif // CLOG_H
