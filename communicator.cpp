#include "communicator.h"

Communicator::Communicator(QObject *parent) : QObject(parent)
{

}

Communicator::~Communicator()
{
    m_pReadTimer->stop();
    m_pSerial->close();
}

void Communicator::onInit(QString qsPort, int nBaud, int nPeriod)
{
    if (qsPort.compare(NONE) != 0) {
    	m_pSerial = new QSerialPort(this);
    
        m_pReadTimer = new QTimer(this);

        connect(m_pReadTimer, SIGNAL(timeout()), this, SLOT(onReadTimer()), Qt::DirectConnection);
        m_pSerial->setPortName(qsPort);
        m_pSerial->setBaudRate(nBaud);
        m_pSerial->setDataBits(QSerialPort::Data8);
        m_pSerial->setParity(QSerialPort::NoParity);
        m_pSerial->setStopBits(QSerialPort::OneStop);
        m_pSerial->setFlowControl(QSerialPort::NoFlowControl);

        if (!m_pSerial->open(QIODevice::ReadWrite))
        {
            qDebug() << "Error opening serial.";
            emit reportOpen(false);
        }
        else
        {
            qDebug() << "Serial opened.";
            emit reportOpen(true);
            m_pReadTimer->start(nPeriod);
        }
    }else {
	qDebug() << "No serial selected.";
        emit reportOpen(true);
    }
}

void Communicator::onSendReset()
{
    if (m_pSerial != 0) {
        m_pSerial->write("R\n");
    }
}

void Communicator::onReadTimer()
{
    while (m_pSerial->canReadLine())
    {
        QString qsMessage = m_pSerial->readLine();
        qsMessage = qsMessage.trimmed();

        if (qsMessage.compare("S") == 0)
        {
            emit timerStart();
        }
        else if (qsMessage.compare("F") == 0)
        {
            emit timerFinish();
        }
        else if (qsMessage.compare("R") == 0)
        {
            emit timerReset();
        }
        else if (qsMessage.startsWith("F"))
        {
            qsMessage = qsMessage.mid(2);
            emit timerFinish();
            emit setNumber(qsMessage.toInt());
        }
    }
}
