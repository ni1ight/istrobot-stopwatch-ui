#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QTime>
#include <QKeyEvent>
#include <QThread>
#include <QFont>
#include <QFontDatabase>

#include "communicator.h"


#define MAXSERIALS      30
#define READ_PERIOD_MS  5
#define ANIM_PERIOD_MS  16
#define BG_PATH         ":/bg.jpg"
#define FONT_PATH       ":/fonts/digital-7 (mono).ttf"
#define INIT_STR        "00:00.000"
#define START_KEY       32  // SPACE
#define RESET_KEY       82  // R
#define FULLSCREEN_KEY  70  // F
#define TIMETEXT_WIDTH  1900
#define TIMETEXT_YPOS   80
#define SCENE_WIDTH     1920
#define SCENE_HEIGHT    1080

namespace Ui
{
    class SerialSettings;
}

class StartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = 0);
    ~StartDialog();

signals:
    void sendInit(QString, int , int);
    void sendReset();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_pushButton_connect_clicked();
    void onAnimTimer();
    void onStartTimer();
    void onStopTimer();
    void onResetTimer();
    void onSetNumber(int nTime);
    void onSerialOpen(bool bSuccess);

private:
    QString toTimeStr(int nMilliseconds);
    void renderTime();
    void setMemberVariables();
    void drawScene();
    void createCommunicator();

private:
    Ui::SerialSettings* m_pUi;
    QSerialPort* m_pSerial;
    QGraphicsView* m_pView;
    QGraphicsScene* m_pScene;
    QGraphicsTextItem* m_pTimeText;
    QTimer* m_pAnimTimer;
    QTime* m_pTime;
    QThread* m_pReadThread;
    Communicator* m_pCommunicator;

    bool m_bCommunicatorCreated;
    bool m_bMeasuring;
    bool m_bIsFullScreen;

    QString m_qsActTime;
};

#endif // STARTDIALOG_H
