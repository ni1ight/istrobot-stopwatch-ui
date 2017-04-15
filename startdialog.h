#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>
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
#include <QSettings>

#if defined(_WIN32)
    #include "windows.h"
#elif defined(__APPLE__)
    // apple, todo
#elif defined(__linux)
    // linux, todo
#endif

#include "communicator.h"

#define COMPANYNAME     "Freevision"
#define PROJECTNAME     "StopWatch"
#define SETTINGSNAME    "Serial Settings"
#define COMPORTNAME     "Com Port"
#define BAUDRATENAME    "Baud Rate"
#define MAXSERIALS      30
#define READ_PERIOD_MS  5
#define ANIM_PERIOD_MS  16
#define BG_PATH         ":/bg.jpg"
#define FONT_PATH       ":/fonts/digital-7 (mono).ttf"
#define INIT_STR        "00:00.000"
#define DELAY_STR       "Delay: "
#define TIMETEXT_WIDTH  1900
#define TIMETEXT_YPOS   100
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
    void sendMsg(QString);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_pushButton_connect_clicked();
    void on_pushButton_offline_clicked();
    void onAnimTimer();
    void onStartTimer();
    void onStopTimer();
    void onResetTimer();
    void onSetDelay(int nSecs);
    void onSetNumber(int nTime);
    void onSerialOpen(bool bSuccess);

private:
    QString toTimeStr(int nMilliseconds);
    void renderTime();
    void setMemberVariables();
    void drawScene();
    void createCommunicator();
    void updateScreenMode();
    void handleResize();
    void readSettings();
    void writeSettings();
    void enableScreensaver();
    void disableScreensaver();

private:
    Ui::SerialSettings* m_pUi;
    QGraphicsView* m_pView;
    QGraphicsScene* m_pScene;
    QGraphicsTextItem* m_pTimeText;
    QGraphicsTextItem* m_pDelayText;
    QTimer* m_pAnimTimer;
    QTime* m_pTime;
    QThread* m_pReadThread;
    Communicator* m_pCommunicator;

    bool m_bCommunicatorCreated;
    bool m_bMeasuring;
    bool m_bIsFullScreen;
    bool m_bSizeChanged;

    QString m_qsActTime;
};

#endif // STARTDIALOG_H
