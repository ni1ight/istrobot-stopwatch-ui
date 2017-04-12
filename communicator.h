#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QDebug>

class Communicator : public QObject
{
    Q_OBJECT

public:
    explicit Communicator(QObject *parent = 0);
    ~Communicator();

signals:
    void timerStart();
    void timerFinish();
    void timerReset();
    void setNumber(int);
    void reportOpen(bool);

public slots:
    void onInit(QString qsPort, int nBaud, int nPeriod);
    void onSendReset();
    void onReadTimer();

private:
    QSerialPort* m_pSerial;
    QTimer* m_pReadTimer;
    QString m_qsComPort;
    int m_nBaud;
};

#endif // COMMUNICATOR_H
