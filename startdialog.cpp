#include "startdialog.h"
#include "ui_startdialog.h"

#include <QDebug>

StartDialog::StartDialog(QWidget *parent) : QDialog(parent), m_pUi(new Ui::SerialSettings)
{
    m_pUi->setupUi(this);
    setMemberVariables();
}

StartDialog::~StartDialog()
{
    m_pSerial->close();

    delete m_pTime;
    delete m_pScene;
    delete m_pView;
    delete m_pUi;
}

void StartDialog::setMemberVariables()
{
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts())
    {
        m_pUi->comboBox_comport->addItem(port.portName());
    }

    m_pSerial = new QSerialPort(this);
    m_pScene = new QGraphicsScene();
    m_pView = new QGraphicsView(m_pScene);
    m_pAnimTimer = new QTimer(this);
    m_pSerial = new QSerialPort(this);
    m_pTime = new QTime();

    m_pSerial->setDataBits(QSerialPort::Data8);
    m_pSerial->setParity(QSerialPort::NoParity);
    m_pSerial->setStopBits(QSerialPort::OneStop);
    m_pSerial->setFlowControl(QSerialPort::NoFlowControl);

    connect(m_pSerial, SIGNAL(readyRead()), this, SLOT(on_serial_received()));
    connect(m_pAnimTimer, SIGNAL(timeout()), this, SLOT(on_anim_timer()));

    m_pView->installEventFilter(this);

    m_bMeasuring = false;
    m_bSerialOpen = false;
    m_pFinalElapsed = 0;

    //delete for release
    drawScene();
    m_pAnimTimer->start(ANIM_PERIOD_MS);
}

void StartDialog::startTimer()
{
    m_bMeasuring = true;
    m_pTime->start();
}

void StartDialog::stopTimer()
{
    m_pFinalElapsed = m_pTime->elapsed();
    m_pTime = new QTime();
    m_bMeasuring = false;
}

void StartDialog::resetTimer(bool reportReset)
{
    stopTimer();
    renderTime(0);

    if (reportReset && m_bSerialOpen)
        m_pSerial->write("R\n");
}

void StartDialog::renderTime(int nMilliseconds)
{
    int nMin = nMilliseconds / 60000;
    int nSec = (nMilliseconds % 60000) / 1000;
    int nMil = ((nMilliseconds % 60000) % 1000);

    QString time;
    time.sprintf("%02d:%02d.%03d", nMin, nSec, nMil);

    m_pTimeText->setPlainText(time);
}

bool StartDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int nKey = keyEvent->key();

        if (nKey == START_KEY)
        {
            if (m_bMeasuring)
                stopTimer();
            else {
                m_pFinalElapsed = 0;
                startTimer();
            }
        }
        else if (nKey == RESET_KEY)
        {
            resetTimer(true);
            renderTime(0);
        }
    }

    return QObject::eventFilter(obj, event);
}

void StartDialog::on_pushButton_connect_clicked()
{
    m_pSerial->setPortName(m_pUi->comboBox_comport->currentText());
    m_pSerial->setBaudRate(m_pUi->spinBox_baud->value());

    if (!m_pSerial->open(QIODevice::ReadWrite))
    {
        QMessageBox::information(this, "Serial", "Error opening serial");
        return;
    }
    else
    {
        m_bSerialOpen = true;
        drawScene();
        m_pAnimTimer->start(ANIM_PERIOD_MS);
    }
}

void StartDialog::on_anim_timer()
{
    if (m_pFinalElapsed != 0)
      renderTime(m_pFinalElapsed);
    else
      renderTime(m_pTime->elapsed());
}

void StartDialog::on_serial_received()
{
    while (m_pSerial->canReadLine()) {
        QString qsMessage = m_pSerial->readLine();
        qsMessage = qsMessage.trimmed();

        qDebug() << qsMessage;

        if (qsMessage.compare("S") == 0)
        {
            startTimer();
        }
        else if (qsMessage.compare("F") == 0)
        {
            stopTimer();
        }
        else if (qsMessage.compare("R") == 0)
        {
            resetTimer(false);
        }
        else if (qsMessage.startsWith("F"))
        {
            qsMessage = qsMessage.mid(2);
            stopTimer();
            m_pFinalElapsed = qsMessage.toInt();
        }
    }
}

void StartDialog::drawScene()
{
    m_pView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pView->setRenderHint(QPainter::Antialiasing);

    QImage image_bg(BG_PATH);

    if (image_bg.isNull())
    {
        QMessageBox::information(this, "Image Viewer", "Error opening background image");
        return;
    }

    QGraphicsPixmapItem* bg = m_pScene->addPixmap(QPixmap::fromImage(image_bg));
    m_pView->showFullScreen();
    m_pView->fitInView(QRect(1, 1, 1918, 1078));

    m_pTimeText = m_pScene->addText(INIT_STR);
    m_pTimeText->setDefaultTextColor(Qt::white);
    m_pTimeText->setPos(22, 52);
    m_pTimeText->setScale(FONT_SCALE);
}
