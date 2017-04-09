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
#include <QTimer>
#include <QKeyEvent>
#include <QDir>

#define MAXSERIALS      30
#define ANIM_PERIOD_MS  30
#define BG_PATH         "images/bg.jpg"
#define INIT_STR        "00:00.00"
#define START_KEY       32
#define RESET_KEY       82

#if defined(Q_OS_OSX)
    #define FONT_SCALE  28.0f
#else
    #define FONT_SCALE  36.0f
#endif

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

    void startTimer();
    void stopTimer();
    void resetTimer();
    void setTime(int nMilliseconds);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_pushButton_connect_clicked();
    void on_anim_timer();
    void on_serial_received();

private:
    void setMemberVariables();
    void drawScene();

private:
    Ui::SerialSettings* m_pUi;
    QSerialPort* m_pSerial;
    QGraphicsView* m_pView;
    QGraphicsScene* m_pScene;
    QTimer* m_pAnimTimer;
    QGraphicsTextItem* m_pTimeText;
    QTime* m_pTime;

    bool m_bMeasuring;
    bool m_bSerialOpen;
};

#endif // STARTDIALOG_H
