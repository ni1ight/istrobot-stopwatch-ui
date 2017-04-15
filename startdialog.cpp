#include "startdialog.h"
#include "ui_startdialog.h"

StartDialog::StartDialog(QWidget *parent) : QDialog(parent), m_pUi(new Ui::SerialSettings)
{
    m_pUi->setupUi(this);
    setMemberVariables();
    readSettings();
}

StartDialog::~StartDialog()
{
    if (m_bCommunicatorCreated)
    {
        m_pReadThread->terminate();
        m_pReadThread->deleteLater();
        writeSettings();
    }

    m_pAnimTimer->stop();

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
    m_pScene = new QGraphicsScene();
    m_pView = new QGraphicsView(m_pScene);
    m_pAnimTimer = new QTimer(this);
    m_pTime = new QTime();

    connect(m_pAnimTimer, SIGNAL(timeout()), this, SLOT(onAnimTimer()));

    m_pView->installEventFilter(this);
    m_pScene->setItemIndexMethod(QGraphicsScene::NoIndex);

    m_bCommunicatorCreated = false;
    m_bMeasuring = false;
    m_bIsFullScreen = true;
    m_bSizeChanged = true;
    m_qsActTime = INIT_STR;
}

QString StartDialog::toTimeStr(int nMilliseconds)
{
    int nMin = nMilliseconds / 60000;
    int nSec = (nMilliseconds % 60000) / 1000;
    int nMil = (nMilliseconds % 60000) % 1000;

    QString time;
    time.sprintf("%02d:%02d.%03d", nMin, nSec, nMil);

    return time;
}

void StartDialog::renderTime()
{    
    m_pTimeText->setPlainText(m_qsActTime);
}

bool StartDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int nKey = keyEvent->key();

        if (nKey == Qt::Key_Space)
        {
            if (m_bMeasuring)
            {
                onStopTimer();
            }
            else
            {
                onStartTimer();
            }
        }
        else if (nKey == Qt::Key_R)
        {
            emit sendMsg("R\n");
            onResetTimer();
        }
        else if (nKey == Qt::Key_F)
        {
            m_bIsFullScreen = !m_bIsFullScreen;
            updateScreenMode();
        }
        else if (nKey == Qt::Key_Escape)
        {
            m_pView->close();
            this->close();
        }
        else if (nKey == Qt::Key_Control)
        {
            bool bVisible = m_pDelayText->isVisible();
            m_pDelayText->setVisible(!bVisible);
        }
        else if (nKey >= Qt::Key_0 && nKey <= Qt::Key_9)
        {
            QString qsMessage;
            qsMessage.sprintf("0 %d\n", nKey - Qt::Key_0);
            sendMsg(qsMessage);
        }
    }
    else if (event->type() == QEvent::Resize)
    {
        m_bSizeChanged = true;
    }
    else if (event->type() == QEvent::Close)
    {
        this->close();
    }

    return QObject::eventFilter(obj, event);
}

void StartDialog::drawScene()
{
    m_pView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pView->setRenderHint(QPainter::Antialiasing);
    m_pView->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

    QImage image_bg(BG_PATH);

    if (image_bg.isNull())
    {
        QMessageBox::information(this, "Image Viewer", "Error opening background image");
        return;
    }

    int id = QFontDatabase::addApplicationFont(FONT_PATH);
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont digiFont(family);

    m_pScene->addPixmap(QPixmap::fromImage(image_bg));
    updateScreenMode();

    m_pTimeText = m_pScene->addText(INIT_STR);
    m_pTimeText->setFont(digiFont);
    m_pTimeText->setDefaultTextColor(Qt::white);

    QRectF box = m_pTimeText->boundingRect();

    m_pTimeText->setPos((SCENE_WIDTH - TIMETEXT_WIDTH) / 2, TIMETEXT_YPOS);
    m_pTimeText->setScale(TIMETEXT_WIDTH / box.width());

    m_pDelayText = m_pScene->addText(DELAY_STR);
    m_pDelayText->setFont(digiFont);
    m_pDelayText->setDefaultTextColor(Qt::white);
    m_pDelayText->setPos(0.2f, 0.2f);
    m_pDelayText->setScale(3.0f);
    m_pDelayText->setVisible(false);
}

void StartDialog::createCommunicator()
{
    m_pReadThread = new QThread(this);
    m_pCommunicator = new Communicator();

    connect(m_pCommunicator, SIGNAL(timerStart()), this, SLOT(onStartTimer()));
    connect(m_pCommunicator, SIGNAL(timerFinish()), this, SLOT(onStopTimer()));
    connect(m_pCommunicator, SIGNAL(timerReset()), this, SLOT(onResetTimer()));
    connect(m_pCommunicator, SIGNAL(setNumber(int)), this, SLOT(onSetNumber(int)));
    connect(m_pCommunicator, SIGNAL(reportOpen(bool)), this, SLOT(onSerialOpen(bool)));
    connect(this, SIGNAL(sendInit(QString, int, int)), m_pCommunicator, SLOT(onInit(QString, int, int)));
    connect(this, SIGNAL(sendMsg(QString)), m_pCommunicator, SLOT(onSendMsg(QString)));

    m_pCommunicator->moveToThread(m_pReadThread);

    m_pReadThread->start();
    m_pReadThread->setPriority(QThread::HighPriority);

    m_bCommunicatorCreated = true;
}

void StartDialog::updateScreenMode()
{
    if (m_bIsFullScreen)
    {
        m_pView->showFullScreen();
    }
    else
    {
        m_pView->showNormal();
    }

    m_bSizeChanged = true;
}

void StartDialog::handleResize()
{
    m_pView->fitInView(QRect(1, 1, SCENE_WIDTH - 2, SCENE_HEIGHT - 2), Qt::KeepAspectRatio);
    m_bSizeChanged = false;
}

void StartDialog::readSettings()
{
    QSettings settings(COMPANYNAME, PROJECTNAME);
    QString qsComPort;
    int nBaudRate;

    settings.beginGroup(SETTINGSNAME);
    qsComPort = settings.value(COMPORTNAME, QString("")).toString();
    nBaudRate = settings.value(BAUDRATENAME, int(0)).toInt();
    settings.endGroup();

    if (nBaudRate != 0)
    {
        m_pUi->spinBox_baud->setValue(nBaudRate);
    }

    if (qsComPort.size() > 0 && m_pUi->comboBox_comport->findText(qsComPort))
    {
        m_pUi->comboBox_comport->setCurrentText(qsComPort);
    }
}

void StartDialog::writeSettings()
{
    QSettings settings(COMPANYNAME, PROJECTNAME);

    settings.beginGroup(SETTINGSNAME);
    settings.setValue(COMPORTNAME, m_pUi->comboBox_comport->currentText());
    settings.setValue(BAUDRATENAME, m_pUi->spinBox_baud->value());
    settings.endGroup();
}

void StartDialog::on_pushButton_connect_clicked()
{
    createCommunicator();
    emit sendInit(m_pUi->comboBox_comport->currentText(),
                  m_pUi->spinBox_baud->value(), READ_PERIOD_MS);

    m_pUi->pushButton_connect->setDisabled(true);
    m_pUi->pushButton_offline->setDisabled(true);
}

void StartDialog::on_pushButton_offline_clicked()
{
     onSerialOpen(true);
     m_pUi->pushButton_connect->setDisabled(true);
     m_pUi->pushButton_offline->setDisabled(true);
}

void StartDialog::onAnimTimer()
{
    if (m_bMeasuring)
    {
        m_qsActTime = toTimeStr(m_pTime->elapsed());
    }

    if (m_bSizeChanged)
    {
        handleResize();
    }

    renderTime();
}

void StartDialog::onStartTimer()
{
    m_bMeasuring = true;
    m_pTime->restart();
}

void StartDialog::onStopTimer()
{
    m_bMeasuring = false;
}

void StartDialog::onResetTimer()
{
    m_bMeasuring = false;
    m_qsActTime = INIT_STR;
}

void StartDialog::onSetDelay(int nSecs)
{
    QString qsDelay(DELAY_STR);
    qsDelay.append(QString::number(nSecs));
    qsDelay.append(" sec");
    m_pDelayText->setPlainText(qsDelay);
}

void StartDialog::onSetNumber(int nTime)
{
    m_qsActTime = toTimeStr(nTime);
}

void StartDialog::onSerialOpen(bool bSuccess)
{
    if (!bSuccess)
    {
        QMessageBox::information(this, "Serial", "Error opening serial");
        return;
    }
    else
    {
        drawScene();
        m_pAnimTimer->start(ANIM_PERIOD_MS);
    }
}
