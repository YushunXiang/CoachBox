#ifndef WINDOW_H
#define WINDOW_H
#define PI 3.1415926
#include "custombtn.h"
#include "slidepage.h"
#include "subwindow.h"
#include "rtdb/include/rtdb/rtdb_api.h"
#include "rtdb/include/rtdb/rtdb_user.h"
#include "rtdb/include/world_model/robot.h"
#include "tcpclient.h"
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QDebug>
#include <QComboBox>
#include <QTimer>
#include <QTextBrowser>
#include <QLineEdit>
#include <string.h>
#include <stdlib.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Window; }
QT_END_NAMESPACE

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = nullptr);
    ~Window();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void startComm();
    void timeoutProcess();
    void updateCoachinfo();
    void resetCoachinfo();
    void printLog(QString log);
    void printGameinfo(QString log);
    void ModeChange();
    void ConnectToRefereeBox();
    void DisconnectRefereeBox();
    void RBinfoupdate();
    char parse(char flag);
private:
    QPoint mouseStartPoint;
    QPoint windowTopLeftPoint;
    bool isMovingWindow;
    CustomBtn* close_btn;
    CustomBtn* minimize_btn;
    CustomBtn* info_btn;
    CustomBtn* settings_btn;
    QPushButton* startcomm_btn;
    bool isComm;
    QPushButton* connectrb_btn;
    QPushButton* disconnectrb_btn;
    bool isConnectRB;
    SlidePage* info_page;
    SlidePage* settings_page;
    QWidget* worldmap_shadow;
    QWidget* shadow;
    QGroupBox* robot_panel;
    QGroupBox* coachinfo_panel;
    QGroupBox* gameinfo_panel;
    QGroupBox* log_panel;
    QComboBox* coachinfo_panel_combobox1;
    QComboBox* coachinfo_panel_combobox2;
    QComboBox* coachinfo_panel_combobox3;
    SubWindow* roboinfo1;
    SubWindow* roboinfo2;
    SubWindow* roboinfo3;
    QTextBrowser* gameinfo;
    QTextBrowser* logs;
    QLabel* ball;
    QLabel* agent1_mark;
    QLabel* agent2_mark;
    QLabel* agent3_mark;
    nubot::MessageToCoach robot_info[3];
    nubot::MessageFromCoach coach_info;
    QString NetInterface;
    int Mode;
    QPushButton* DebugMode;
    QPushButton* GameMode;
    QPushButton* TacMode;
    TCPclient* RBClient;
    QString RBip;
    int RBport;
    int Team;

private:
    Ui::Window *ui;
};
#endif // WINDOW_H
