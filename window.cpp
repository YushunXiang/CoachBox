#include "window.h"
#include "ui_window.h"

Window::Window(QWidget *parent)
    : QWidget(parent), ui(new Ui::Window)
{
    ui->setupUi(this);

    // initial variables and rtdb
    mouseStartPoint = QPoint(-1, -1);
    windowTopLeftPoint = this->frameGeometry().topLeft();
    isMovingWindow = false;
    NetInterface = "ens33";
    RBip = "255.255.255.255";
    RBport = 0;
    RBClient = new TCPclient;
    Team = 0;
    memset(robot_info, 0, sizeof(nubot::MessageToCoach) * 3);
    memset(&coach_info, 0, sizeof(nubot::MessageFromCoach));
    if (DB_init() != 0)
        qDebug() << "init rtdb error!";

    // create timer(30ms)
    QTimer *timer = new QTimer(this);
    timer->start(30);
    connect(timer, &QTimer::timeout, this, &Window::timeoutProcess);

    // set window property
    setFixedSize(1280, 720);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("QWidget{background-color:white}");

    // create frame button
    close_btn = new CustomBtn(this);
    minimize_btn = new CustomBtn(this);
    close_btn->setBtnSize(50, 30);
    minimize_btn->setBtnSize(50, 30);
    close_btn->setImage(":/icons/", "close");
    minimize_btn->setImage(":/icons/", "minimize");
    close_btn->setColor("white", "rgba(255,0,0,0.6)", "rgba(255,0,0,0.6)"); //"rgb(240,240,240)","rgb(200,200,200)"
    minimize_btn->setColor("white", "rgb(240,240,240)", "rgb(200,200,200)");
    close_btn->updateStyle();
    minimize_btn->updateStyle();
    close_btn->move(1280 - close_btn->getWidth(), 0);
    minimize_btn->move(1280 - close_btn->getWidth() - minimize_btn->getWidth(), 0);
    connect(close_btn, &QPushButton::clicked, this, &QWidget::close);
    connect(minimize_btn, &QPushButton::clicked, this, &QWidget::showMinimized);

    // create title and icon
    QGroupBox *mainTitle = new QGroupBox(this);
    mainTitle->setStyleSheet("QGroupBox{border-style:none;background-color:white}");
    mainTitle->resize(250, 70);
    mainTitle->move(40, 30);
    QLabel *title = new QLabel(mainTitle);
    QLabel *subTitle = new QLabel(mainTitle);
    title->setText("HawkingCar");
    title->setStyleSheet("QLabel{background-color:white;font-size:40px;}");
    subTitle->setText("控制平台");
    subTitle->setStyleSheet("QLabel{background-color:white;font-size:20px;};");
    subTitle->move(40, 40);

    // create worldmap
    QLabel *worldmap = new QLabel(this);
    worldmap->setStyleSheet("QLabel{border-style:solid;border-width:2px;border-color:rgb(200,200,200);border-radius:10px}");
    worldmap->resize(710, 510);
    worldmap->move(40, 130);
    QLabel *map = new QLabel(worldmap);
    map->setStyleSheet("QLabel{border-style:none;image:url(:/icons/field_remake.png);}");
    map->resize(700, 500);
    map->move(5, 5);
    ball = new QLabel(map);
    ball->setFixedSize(25, 25);
    ball->setStyleSheet("QLabel{border-style:none;background-color:none;image:url(:/icons/ball.png);}");
    ball->move(0, 0);
    QMatrix direction;
    direction.rotate(0);
    agent1_mark = new QLabel(worldmap);
    agent2_mark = new QLabel(worldmap);
    agent3_mark = new QLabel(worldmap);
    agent1_mark->setFixedSize(34, 34);
    agent2_mark->setFixedSize(34, 34);
    agent3_mark->setFixedSize(34, 34);
    agent1_mark->setStyleSheet("QLabel{border-style:none;background-color:none;}");
    agent2_mark->setStyleSheet("QLabel{border-style:none;background-color:none;}");
    agent3_mark->setStyleSheet("QLabel{border-style:none;background-color:none;}");
    agent1_mark->setPixmap(QPixmap(":/icons/robot1mark.png").transformed(direction, Qt::SmoothTransformation));
    agent2_mark->setPixmap(QPixmap(":/icons/robot2mark.png").transformed(direction, Qt::SmoothTransformation));
    agent3_mark->setPixmap(QPixmap(":/icons/robot3mark.png").transformed(direction, Qt::SmoothTransformation));
    agent1_mark->move(0, 0);
    agent2_mark->move(0, 0);
    agent3_mark->move(0, 0);

    // create worldmap mask
    worldmap_shadow = new QWidget(this);
    worldmap_shadow->resize(worldmap->size());
    worldmap_shadow->setStyleSheet("QWidget{background-color:rgba(0,0,0,0.5);border-radius:10px}");
    worldmap_shadow->setWindowFlag(Qt::FramelessWindowHint);
    worldmap_shadow->move(worldmap->pos());
    QLabel *warn = new QLabel(worldmap_shadow);
    warn->setStyleSheet("QLabel{background-color:none;border-style:none;image-position:top;image:url(:/icons/warning.png);padding:15px 15px 5px 15px;font-size:15px;font-size:20px;color:white}");
    warn->setText("                   连接断开");
    warn->resize(300, 400);
    warn->move(205, 155);
    worldmap_shadow->show();

    // create mode switcher
    DebugMode = new QPushButton(this);
    GameMode = new QPushButton(this);
    TacMode = new QPushButton(this);
    DebugMode->setText("调试模式");
    GameMode->setText("比赛模式");
    TacMode->setText("技术挑战");
    DebugMode->setFixedSize(120, 40);
    DebugMode->move(765, 130);
    GameMode->setFixedSize(120, 40);
    GameMode->move(885, 130);
    TacMode->setFixedSize(120, 40);
    TacMode->move(1005, 130);
    QString switcherStyle = "QPushButton{font-size:20px;border-style:none;background-color:rgb(250,250,250)}"
                            "QPushButton:hover{background-color:rgb(240,240,240)}"
                            "QPushButton:pressed{background-color:rgb(240,240,240)}";
    QString selectedStyle = "QPushButton{font-size:20px;background-color:rgb(240,240,240);border-style:none;border-bottom-style:solid;border-bottom-width:3px;border-bottom-color:black}";
    DebugMode->setStyleSheet(switcherStyle);
    GameMode->setStyleSheet(switcherStyle);
    TacMode->setStyleSheet(switcherStyle);
    DebugMode->setStyleSheet(selectedStyle);
    connect(DebugMode, &QPushButton::clicked, this, [=]()
            {
        Mode=1;
        DebugMode->setStyleSheet(selectedStyle);
        GameMode->setStyleSheet(switcherStyle);
        TacMode->setStyleSheet(switcherStyle);
        ModeChange();
        printLog("切换至调试模式"); });
    connect(GameMode, &QPushButton::clicked, this, [=]()
            {
        Mode=2;
        GameMode->setStyleSheet(selectedStyle);
        DebugMode->setStyleSheet(switcherStyle);
        TacMode->setStyleSheet(switcherStyle);
        ModeChange();
        printLog("切换至比赛模式"); });
    connect(TacMode, &QPushButton::clicked, this, [=]()
            {
        Mode=3;
        TacMode->setStyleSheet(selectedStyle);
        GameMode->setStyleSheet(switcherStyle);
        DebugMode->setStyleSheet(switcherStyle);
        ModeChange();
        printLog("切换至技术挑战"); });

    // create robot info panel
    robot_panel = new QGroupBox(this);
    robot_panel->setStyleSheet("QGroupBox{border-style:none;background-color:white}");
    robot_panel->resize(470, 150);
    robot_panel->move(765, 190);
    QLabel *robot_panel_title = new QLabel(robot_panel);
    robot_panel_title->setText("机器人信息");
    robot_panel_title->setStyleSheet("QLabel{background-color:white;border-style:none;font-size:15px}");
    QLabel *robot_panel_margin = new QLabel(robot_panel);
    robot_panel_margin->setStyleSheet("QLabel{image:url(:/icons/margin)}");
    robot_panel_margin->resize(30, 10);
    robot_panel_margin->move(0, 25);
    QLabel *robot_panel_content = new QLabel(robot_panel);
    robot_panel_content->setStyleSheet("QLabel{background-color:white;border-style:solid;border-width:2px;border-radius:13px;border-color:rgb(200,200,200)}");
    robot_panel_content->resize(470, 90);
    robot_panel_content->move(0, 40);
    QPushButton *robot_panel_button1 = new QPushButton(robot_panel_content);
    QPushButton *robot_panel_button2 = new QPushButton(robot_panel_content);
    QPushButton *robot_panel_button3 = new QPushButton(robot_panel_content);
    robot_panel_button1->setStyleSheet("QPushButton{font-size:24px;border-style:none;border-radius:10px;background-color:rgb(245,245,245)}"
                                       "QPushButton:hover{background-color:rgb(250,250,250)}"
                                       "QPushButton:pressed{background-color:rgb(240,240,240)}");
    robot_panel_button1->setText("Agent 1");
    robot_panel_button1->resize(140, 70);
    robot_panel_button1->move(10, 10);
    robot_panel_button2->setStyleSheet("QPushButton{font-size:24px;border-style:none;border-radius:10px;background-color:rgb(245,245,245)}"
                                       "QPushButton:hover{background-color:rgb(250,250,250)}"
                                       "QPushButton:pressed{background-color:rgb(240,240,240)}");
    robot_panel_button2->setText("Agent 2");
    robot_panel_button2->resize(140, 70);
    robot_panel_button2->move(165, 10);
    robot_panel_button3->setStyleSheet("QPushButton{font-size:24px;border-style:none;border-radius:10px;background-color:rgb(245,245,245)}"
                                       "QPushButton:hover{background-color:rgb(250,250,250)}"
                                       "QPushButton:pressed{background-color:rgb(240,240,240)}");
    robot_panel_button3->setText("Agent 3");
    robot_panel_button3->resize(140, 70);
    robot_panel_button3->move(320, 10);

    // create roboinfo page
    roboinfo1 = new SubWindow(1);
    roboinfo2 = new SubWindow(2);
    roboinfo3 = new SubWindow(3);
    roboinfo1->setPointer(&robot_info[0]);
    roboinfo2->setPointer(&robot_info[1]);
    roboinfo3->setPointer(&robot_info[2]);
    roboinfo1->hide();
    roboinfo2->hide();
    roboinfo3->hide();
    connect(robot_panel_button1, &QPushButton::clicked, roboinfo1, &QWidget::show);
    connect(robot_panel_button2, &QPushButton::clicked, roboinfo2, &QWidget::show);
    connect(robot_panel_button3, &QPushButton::clicked, roboinfo3, &QWidget::show);
    connect(close_btn, &QPushButton::clicked, roboinfo1, &QWidget::close);
    connect(close_btn, &QPushButton::clicked, roboinfo2, &QWidget::close);
    connect(close_btn, &QPushButton::clicked, roboinfo3, &QWidget::close);

    // create coach info panel
    coachinfo_panel = new QGroupBox(this);
    coachinfo_panel->setStyleSheet("QGroupBox{border-style:none;background-color:white}");
    coachinfo_panel->resize(470, 150);
    coachinfo_panel->move(765, 335);
    QLabel *coachinfo_panel_title = new QLabel(coachinfo_panel);
    coachinfo_panel_title->setText("设置调试信息");
    coachinfo_panel_title->setStyleSheet("QLabel{background-color:white;border-style:none;font-size:15px}");
    QLabel *coachinfo_panel_margin = new QLabel(coachinfo_panel);
    coachinfo_panel_margin->setStyleSheet("QLabel{image:url(:/icons/margin)}");
    coachinfo_panel_margin->resize(30, 10);
    coachinfo_panel_margin->move(0, 25);
    QLabel *coachinfo_panel_content = new QLabel(coachinfo_panel);
    coachinfo_panel_content->setStyleSheet("QLabel{background-color:white;border-style:solid;border-width:2px;border-radius:13px;border-color:rgb(200,200,200)}");
    coachinfo_panel_content->resize(470, 110);
    coachinfo_panel_content->move(0, 40);
    QLabel *coachinfo_panel_cbtext1 = new QLabel(coachinfo_panel_content);
    coachinfo_panel_cbtext1->setStyleSheet("QLabel{background-color:white;border-style:none;border-radius:0px;text-align:center;}");
    coachinfo_panel_cbtext1->setText("MatchType");
    coachinfo_panel_cbtext1->resize(235, 32);
    coachinfo_panel_cbtext1->move(15, 15);
    QLabel *coachinfo_panel_cbtext2 = new QLabel(coachinfo_panel_content);
    coachinfo_panel_cbtext2->setStyleSheet("QLabel{background-color:white;border-style:none;border-radius:0px;text-align:center;}");
    coachinfo_panel_cbtext2->setText("MatchMode");
    coachinfo_panel_cbtext2->resize(235, 32);
    coachinfo_panel_cbtext2->move(15, 60);
    QLabel *coachinfo_panel_cbtext3 = new QLabel(coachinfo_panel_content);
    coachinfo_panel_cbtext3->setStyleSheet("QLabel{background-color:white;border-style:none;border-radius:0px;text-align:center;}");
    coachinfo_panel_cbtext3->setText("Head");
    coachinfo_panel_cbtext3->resize(200, 32);
    coachinfo_panel_cbtext3->move(265, 15);
    QString ComboboxSheetstyle = "QComboBox{"
                                 "combobox-popup: 0;"
                                 "border:1px solid rgb(245,245,245);"
                                 "border-radius: 10px;"
                                 "background-color: rgb(245,245,245);"
                                 "color: black;"
                                 "width: 130px;"
                                 "height: 30px;"
                                 "font: 15px;"
                                 "text-align:center;}"
                                 "QComboBox::down-arrow{"
                                 "image:url(:/icons/more.png);"
                                 "width:10px;"
                                 "height:10px;"
                                 "right:2px;}"
                                 "QComboBox::drop-down{"
                                 "subcontrol-origin:padding;"
                                 "subcontrol-position:top right;"
                                 "width:15px;"
                                 "border-left-width:0px;"
                                 "border-left-style:solid;"
                                 "border-top-right-radius:10px;"
                                 "border-bottom-right-radius:10px;}";
    coachinfo_panel_combobox1 = new QComboBox(coachinfo_panel_cbtext1);
    coachinfo_panel_combobox1->setStyleSheet(ComboboxSheetstyle);
    coachinfo_panel_combobox1->addItem("OUR_KICKOFF");
    coachinfo_panel_combobox1->addItem("OPP_KICKOFF");
    coachinfo_panel_combobox1->addItem("OUR_THROWIN");
    coachinfo_panel_combobox1->addItem("OPP_THROWIN");
    coachinfo_panel_combobox1->addItem("OUR_PENALTY");
    coachinfo_panel_combobox1->addItem("OPP_PENALTY");
    coachinfo_panel_combobox1->addItem("OUR_GOALKICK");
    coachinfo_panel_combobox1->addItem("OPP_GOALKICK");
    coachinfo_panel_combobox1->addItem("OUR_CORNERKICK");
    coachinfo_panel_combobox1->addItem("OPP_CORNERKICK");
    coachinfo_panel_combobox1->addItem("OUR_FREEKICK");
    coachinfo_panel_combobox1->addItem("OPP_FREEKICK");
    coachinfo_panel_combobox1->addItem("DROPBALL");
    coachinfo_panel_combobox1->addItem("LOCATION");
    coachinfo_panel_combobox1->addItem("AVOIDANCE");
    coachinfo_panel_combobox1->addItem("PASSING");
    coachinfo_panel_combobox1->addItem("CHECKGIGE");
    coachinfo_panel_combobox1->move(85, 0);
    coachinfo_panel_combobox2 = new QComboBox(coachinfo_panel_cbtext2);
    coachinfo_panel_combobox2->setStyleSheet(ComboboxSheetstyle);
    coachinfo_panel_combobox2->addItem("STOPROBOT");
    coachinfo_panel_combobox2->addItem("STARTROBOT");
    coachinfo_panel_combobox2->addItem("PARKINGROBOT");
    coachinfo_panel_combobox2->move(85, 0);
    coachinfo_panel_combobox3 = new QComboBox(coachinfo_panel_cbtext3);
    coachinfo_panel_combobox3->setStyleSheet(ComboboxSheetstyle);
    coachinfo_panel_combobox3->addItem("No Item");
    coachinfo_panel_combobox3->move(40, 0);
    QPushButton *updatecoachinfo_btn = new QPushButton(coachinfo_panel_content);
    QPushButton *resetcoachinfo_btn = new QPushButton(coachinfo_panel_content);
    updatecoachinfo_btn->setStyleSheet("QPushButton{font-size:15px;border-style:none;border-radius:10px;background-color:rgb(245,245,245)}"
                                       "QPushButton:hover{background-color:rgb(250,250,250)}"
                                       "QPushButton:pressed{background-color:rgb(240,240,240)}");
    resetcoachinfo_btn->setStyleSheet("QPushButton{font-size:15px;border-style:none;border-radius:10px;background-color:rgb(245,245,245)}"
                                      "QPushButton:hover{background-color:rgb(250,250,250)}"
                                      "QPushButton:pressed{background-color:rgb(240,240,240)}");
    updatecoachinfo_btn->setText("更新指令");
    resetcoachinfo_btn->setText("重置指令");
    updatecoachinfo_btn->resize(90, 40);
    resetcoachinfo_btn->resize(90, 40);
    updatecoachinfo_btn->move(260, 55);
    resetcoachinfo_btn->move(360, 55);
    connect(updatecoachinfo_btn, &QPushButton::clicked, this, &Window::updateCoachinfo);
    connect(resetcoachinfo_btn, &QPushButton::clicked, this, &Window::resetCoachinfo);

    // create gameinfo panel
    gameinfo_panel = new QGroupBox(this);
    gameinfo_panel->hide();
    gameinfo_panel->resize(345, 160);
    gameinfo_panel->setStyleSheet("QGroupBox{border-style:none;background-color:white}");
    gameinfo_panel->move(765, 335);
    QLabel *gameinfo_panel_title = new QLabel(gameinfo_panel);
    gameinfo_panel_title->setText("比赛状态");
    gameinfo_panel_title->setStyleSheet("QLabel{background-color:white;border-style:none;font-size:15px}");
    QLabel *gameinfo_panel_margin = new QLabel(gameinfo_panel);
    gameinfo_panel_margin->setStyleSheet("QLabel{image:url(:/icons/margin)}");
    gameinfo_panel_margin->resize(30, 10);
    gameinfo_panel_margin->move(0, 25);
    QLabel *gameinfo_panel_content = new QLabel(gameinfo_panel);
    gameinfo_panel_content->setStyleSheet("QLabel{background-color:white;border-style:solid;border-width:2px;border-radius:13px;border-color:rgb(200,200,200)}");
    gameinfo_panel_content->resize(345, 110);
    gameinfo_panel_content->move(0, 40);
    gameinfo = new QTextBrowser(gameinfo_panel_content);
    gameinfo->setStyleSheet("QTextBrowser{border-style:none;font-size:15px;padding-left:10px;padding-right:10px}");
    gameinfo->resize(335, 100);
    gameinfo->move(5, 5);
    gameinfo->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // create connect rb button
    connectrb_btn = new QPushButton(this);
    connectrb_btn->setText("连接裁判盒");
    connectrb_btn->setStyleSheet("QPushButton{font-size:15px;border-style:none;border-radius:10px;background-color:rgb(245,245,245);}"
                                 "QPushButton:hover{background-color:rgb(250,250,250)}"
                                 "QPushButton:pressed{background-color:rgb(240,240,240)}");
    connectrb_btn->resize(110, 50);
    disconnectrb_btn = new QPushButton(this);
    disconnectrb_btn->setText("断开连接");
    disconnectrb_btn->setStyleSheet("QPushButton{font-size:15px;border-style:none;border-radius:10px;background-color:rgb(245,245,245);}"
                                    "QPushButton:hover{background-color:rgb(250,250,250)}"
                                    "QPushButton:pressed{background-color:rgb(240,240,240)}");
    disconnectrb_btn->resize(110, 50);
    connectrb_btn->move(1125, 375);
    disconnectrb_btn->move(1125, 435);
    connectrb_btn->hide();
    disconnectrb_btn->hide();
    connect(connectrb_btn, &QPushButton::clicked, this, &Window::ConnectToRefereeBox);
    connect(disconnectrb_btn, &QPushButton::clicked, this, &Window::DisconnectRefereeBox);
    connect(RBClient, &TCPclient::RBinfoupdated, this, &Window::RBinfoupdate);
    printGameinfo("请正确设置网卡、ip、端口、队伍后再进行操作");

    // create log panel
    log_panel = new QGroupBox(this);
    log_panel->resize(345, 140);
    log_panel->setStyleSheet("QGroupBox{border-style:none;background-color:white}");
    log_panel->move(765, 500);
    QLabel *log_panel_title = new QLabel(log_panel);
    log_panel_title->setText("日志信息");
    log_panel_title->setStyleSheet("QLabel{background-color:white;border-style:none;font-size:15px}");
    QLabel *log_panel_margin = new QLabel(log_panel);
    log_panel_margin->setStyleSheet("QLabel{image:url(:/icons/margin)}");
    log_panel_margin->resize(30, 10);
    log_panel_margin->move(0, 25);
    QLabel *log_panel_content = new QLabel(log_panel);
    log_panel_content->setStyleSheet("QLabel{background-color:white;border-style:solid;border-width:2px;border-radius:13px;border-color:rgb(200,200,200)}");
    log_panel_content->resize(345, 100);
    log_panel_content->move(0, 40);
    logs = new QTextBrowser(log_panel_content);
    logs->setStyleSheet("QTextBrowser{border-style:none;font-size:15px;padding-left:10px;padding-right:10px}");
    logs->resize(335, 90);
    logs->move(5, 5);
    logs->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // create start comm btn
    isComm = false;
    startcomm_btn = new QPushButton(this);
    startcomm_btn->setText("开始通信");
    startcomm_btn->setStyleSheet("QPushButton{text-align:bottom;padding:15px 15px 5px 15px;font-size:15px;border-style:none;border-radius:10px;background-color:rgb(245,245,245);image-position:TOP;image:url(:/icons/startcomm_normal.png)}"
                                 "QPushButton:hover{background-color:rgb(250,250,250)}"
                                 "QPushButton:pressed{background-color:rgb(240,240,240)}");
    startcomm_btn->resize(110, 140);
    startcomm_btn->move(1125, 500);
    connect(startcomm_btn, &QPushButton::clicked, this, &Window::startComm);

    // create info and settings button
    settings_btn = new CustomBtn(this);
    info_btn = new CustomBtn(this);
    settings_btn->setBtnSize(30, 30);
    info_btn->setBtnSize(30, 30);
    settings_btn->setImage(":/icons/", "settings");
    info_btn->setImage(":/icons/", "info");
    settings_btn->updateStyle();
    info_btn->updateStyle();
    settings_btn->move(270, 50);
    info_btn->move(310, 50);

    // create shadow mask
    shadow = new QWidget(this);
    shadow->resize(this->size());
    shadow->setStyleSheet("QWidget{background-color:rgba(0,0,0,0.5)}");
    shadow->setWindowFlag(Qt::FramelessWindowHint);
    shadow->move(0, 0);
    shadow->hide();

    // create info page
    info_page = new SlidePage(this);
    connect(info_btn, &QPushButton::clicked, info_page, &SlidePage::slidein);
    connect(info_btn, &QPushButton::clicked, info_page, &SlidePage::show);
    connect(info_page, &SlidePage::exit, info_page, &SlidePage::slideout);
    connect(info_page, &SlidePage::show, shadow, &QWidget::show);
    connect(info_page, &SlidePage::exit, shadow, &QWidget::hide);
    info_page->putTitle("关于");
    info_page->puttext("西北工业大学@足球机器人基地轮式组");
    info_page->puttext("机器人控制平台/教练机");
    info_page->puttext("版本: 2.0");
    info_page->puttext("最后更新: 2022/2/12");
    info_page->puttext("作者：黎琳皓(315993803@qq.com)");

    // create settings page
    settings_page = new SlidePage(this);
    connect(settings_btn, &QPushButton::clicked, settings_page, &SlidePage::slidein);
    connect(settings_btn, &QPushButton::clicked, settings_page, &SlidePage::show);
    connect(settings_page, &SlidePage::exit, settings_page, &SlidePage::slideout);
    connect(settings_page, &SlidePage::show, shadow, &QWidget::show);
    connect(settings_page, &SlidePage::exit, shadow, &QWidget::hide);
    settings_page->putTitle("设置");
    settings_page->puttext("通信网卡");
    QLineEdit *inputbox1 = new QLineEdit(settings_page);
    inputbox1->setText(NetInterface);
    inputbox1->resize(150, 25);
    inputbox1->move(130, 120);
    settings_page->puttext("裁判盒地址");
    QLineEdit *inputbox2 = new QLineEdit(settings_page);
    inputbox2->setText(RBip);
    inputbox2->resize(140, 25);
    inputbox2->move(140, 170);
    settings_page->puttext("裁判盒端口");
    QLineEdit *inputbox3 = new QLineEdit(settings_page);
    inputbox3->setText(QString().number(RBport));
    inputbox3->resize(140, 25);
    inputbox3->move(140, 220);
    settings_page->puttext("队伍编号");
    QPushButton *CyanBtn = new QPushButton(settings_page);
    QPushButton *MagentaBtn = new QPushButton(settings_page);
    QString CBtnss = "QPushButton{font-size:15px;border-style:none;background-color:rgb(245,245,245)}"
                     "QPushButton:hover{background-color:rgba(0,255,255,0.5)}"
                     "QPushButton:pressed{background-color:rgb(0,255,255)}";
    QString MBtnss = "QPushButton{font-size:15px;border-style:none;background-color:rgb(245,245,245)}"
                     "QPushButton:hover{background-color:rgba(228,0,127,0.5)}"
                     "QPushButton:pressed{background-color:rgb(228,0,127)}";
    CyanBtn->setText("Cyan");
    MagentaBtn->setText("Magenta");
    CyanBtn->setStyleSheet(CBtnss);
    MagentaBtn->setStyleSheet(MBtnss);
    CyanBtn->setFixedSize(75, 25);
    MagentaBtn->setFixedSize(75, 25);
    CyanBtn->move(130, 270);
    MagentaBtn->move(205, 270);
    connect(CyanBtn, &QPushButton::clicked, this, [=]()
            {
        Team=0;
        CyanBtn->setStyleSheet("QPushButton{font-size:15px;border-style:none;background-color:rgb(0,255,255)}");
        MagentaBtn->setStyleSheet(MBtnss); });
    connect(MagentaBtn, &QPushButton::clicked, this, [=]()
            {
        Team=1;
        CyanBtn->setStyleSheet(CBtnss);
        MagentaBtn->setStyleSheet("QPushButton{font-size:15px;border-style:none;background-color:rgb(228,0,127)}"); });
    QPushButton *settings_confirm = new QPushButton(settings_page);
    QPushButton *settings_reset = new QPushButton(settings_page);
    settings_confirm->setStyleSheet("QPushButton{font-size:15px;border-style:none;border-radius:10px;background-color:rgb(245,245,245)}"
                                    "QPushButton:hover{background-color:rgb(250,250,250)}"
                                    "QPushButton:pressed{background-color:rgb(245,245,245)}");
    settings_reset->setStyleSheet("QPushButton{font-size:15px;border-style:none;border-radius:10px;background-color:rgb(245,245,245)}"
                                  "QPushButton:hover{background-color:rgb(250,250,250)}"
                                  "QPushButton:pressed{background-color:rgb(245,245,245)}");
    settings_confirm->setText("保存设置");
    settings_reset->setText("恢复默认值");
    settings_confirm->resize(100, 30);
    settings_reset->resize(100, 30);
    settings_confirm->move(50, 310);
    settings_reset->move(170, 310);
    connect(settings_confirm, &QPushButton::clicked, this, [=]()
            {
        NetInterface=inputbox1->text();
        RBip=inputbox2->text();
        RBport=inputbox3->text().toInt();
        printLog("设置已生效"); });
    connect(settings_reset, &QPushButton::clicked, this, [=]()
            {
        NetInterface=QString("ens33");
        RBip="255.255.255.255";
        RBport=0;
        inputbox1->setText(NetInterface);
        inputbox2->setText(RBip);
        inputbox3->setText(QString().number(RBport));
        emit CyanBtn->clicked();
        printLog("设置已重置"); });
    settings_page->item_num = settings_page->item_num + 1;
    settings_page->puttext("务必确保网卡信息正确，通信时修改需要\n重启通信后生效");

    printLog("教练机界面初始化完成!");
}

Window::~Window()
{
    delete ui;
}

void Window::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && event->pos().y() <= close_btn->getHeight() * 2)
    {
        isMovingWindow = true;
        mouseStartPoint = event->globalPos();
        windowTopLeftPoint = this->frameGeometry().topLeft();
    }
}
void Window::mouseMoveEvent(QMouseEvent *event)
{
    if (isMovingWindow)
    {
        QPoint move_vector = event->globalPos() - mouseStartPoint;
        this->move(windowTopLeftPoint + move_vector);
    }
}
void Window::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isMovingWindow = false;
    }
    if (event->button() == Qt::LeftButton && event->pos().x() > info_page->width())
    {
        if (info_page->isShow)
            emit info_page->exit();
        else if (settings_page->isShow)
            emit settings_page->exit();
    }
}
void Window::startComm()
{
    isComm = !isComm;
    if (isComm)
    {
        QString command;
        command = QString("./rtdb/comm ") + NetInterface + " &";
        system(command.toUtf8().data());
        startcomm_btn->setStyleSheet("QPushButton{text-align:bottom;padding:15px 15px 5px 15px;font-size:15px;border-style:none;border-radius:10px;background-color:rgb(240,240,240);image-position:TOP;image:url(:/icons/endcomm_normal.png)}"
                                     "QPushButton:hover{background-color:rgb(250,250,250)}"
                                     "QPushButton:pressed{background-color:rgb(240,240,240)}");
        startcomm_btn->setText("结束通信");
        worldmap_shadow->hide();
        printLog("教练机通信已启动!");
        printLog("若终端rtdb报错则重启通信至正常");
    }
    else
    {
        QString command;
        FILE *stream;
        char *line = NULL;
        size_t len = 0;
        stream = popen("pidof ./rtdb/comm ens33", "r");
        getline(&line, &len, stream);
        pclose(stream);
        command = QString("kill ") + line;
        system(command.toUtf8().data());
        startcomm_btn->setStyleSheet("QPushButton{text-align:bottom;padding:15px 15px 5px 15px;font-size:15px;border-style:none;border-radius:10px;background-color:rgb(240,240,240);image-position:TOP;image:url(:/icons/startcomm_normal.png)}"
                                     "QPushButton:hover{background-color:rgb(250,250,250)}"
                                     "QPushButton:pressed{background-color:rgb(240,240,240)}");
        startcomm_btn->setText("开始通信");
        worldmap_shadow->show();
        printLog("教练机通信已结束!");
    }
}
void Window::timeoutProcess()
{
    if (isComm)
    {
        for (int i = 0; i < 3; i++)
        {
            DB_get(i + 1, MESSAGETOCOACHINFO, &robot_info[i]);
        }
        QMatrix direction;
        roboinfo1->repaint(0, 0, 400, 720);
        roboinfo2->repaint(0, 0, 400, 720);
        roboinfo3->repaint(0, 0, 400, 720);
        ball->move(robot_info[0].ball_global_loc.x_ + 337, 235 - robot_info[0].ball_global_loc.y_);
        direction.rotate(robot_info[0].robot_head / PI * 180);
        agent1_mark->setPixmap(QPixmap(":/icons/robot1mark.png").transformed(direction, Qt::SmoothTransformation));
        direction.rotate(robot_info[1].robot_head / PI * 180);
        agent2_mark->setPixmap(QPixmap(":/icons/robot2mark.png").transformed(direction, Qt::SmoothTransformation));
        direction.rotate(robot_info[2].robot_head / PI * 180);
        agent3_mark->setPixmap(QPixmap(":/icons/robot3mark.png").transformed(direction, Qt::SmoothTransformation));
        agent1_mark->move(robot_info[0].robot_global_loc.x_ + 340, 235 - robot_info[0].robot_global_loc.y_);
        agent2_mark->move(robot_info[1].robot_global_loc.x_ + 340, 235 - robot_info[1].robot_global_loc.y_);
        agent3_mark->move(robot_info[2].robot_global_loc.x_ + 340, 235 - robot_info[2].robot_global_loc.y_);
        if (DB_put(MESSAGEFROMCOACHINFO, &coach_info) == -1)
            qDebug() << "DB_put error!";
    }
}
void Window::updateCoachinfo()
{
    coach_info.Head = 0;
    coach_info.MatchType = 0;

    switch (coachinfo_panel_combobox1->currentIndex())
    {
    case 0:
        coach_info.MatchType = OUR_KICKOFF;
        break;
    case 1:
        coach_info.MatchType = OPP_KICKOFF;
        break;
    case 2:
        coach_info.MatchType = OUR_THROWIN;
        break;
    case 3:
        coach_info.MatchType = OPP_THROWIN;
        break;
    case 4:
        coach_info.MatchType = OUR_PENALTY;
        break;
    case 5:
        coach_info.MatchType = OPP_PENALTY;
        break;
    case 6:
        coach_info.MatchType = OUR_GOALKICK;
        break;
    case 7:
        coach_info.MatchType = OPP_GOALKICK;
        break;
    case 8:
        coach_info.MatchType = OUR_CORNERKICK;
        break;
    case 9:
        coach_info.MatchType = OPP_CORNERKICK;
        break;
    case 10:
        coach_info.MatchType = OUR_FREEKICK;
        break;
    case 11:
        coach_info.MatchType = OPP_FREEKICK;
        break;
    case 12:
        coach_info.MatchType = DROPBALL;
        break;
    /// Challenge
    case 13:
        coach_info.MatchType = LOCATION;
        break;
    case 14:
        coach_info.MatchType = AVOIDANCE;
        break;
    case 15:
        coach_info.MatchType = PASSING;
        break;
    case 16:
        coach_info.MatchType = CHECKGIGE;
        break;
    default:
        break;
    }

    switch (coachinfo_panel_combobox2->currentIndex())
    {
    case 0:
        coach_info.MatchMode = STOPROBOT;
        break;
    case 1:
        coach_info.MatchMode = STARTROBOT;
        break;
    case 2:
        coach_info.MatchMode = PARKINGROBOT;
        break;
    default:
        break;
    }
    printLog("调试指令更新:Head=0 MatchMode=" + QString().setNum(coach_info.MatchMode) + "MatchType=" + QString().setNum(coach_info.MatchType));
}
void Window::resetCoachinfo()
{
    memset(&coach_info, 0, sizeof(nubot::MessageFromCoach));
    coachinfo_panel_combobox1->setCurrentIndex(0);
    coachinfo_panel_combobox2->setCurrentIndex(0);
    coachinfo_panel_combobox3->setCurrentIndex(0);
    updateCoachinfo();
}
void Window::printLog(QString log)
{
    logs->insertPlainText(log);
    logs->moveCursor(QTextCursor::End);
    logs->append(QString(""));
}
void Window::printGameinfo(QString log)
{
    gameinfo->insertPlainText(log);
    gameinfo->moveCursor(QTextCursor::End);
    gameinfo->append(QString(""));
}
void Window::ModeChange()
{
    switch (Mode)
    {
    case 1:
        coachinfo_panel->show();
        gameinfo_panel->hide();
        disconnectrb_btn->hide();
        connectrb_btn->hide();
        robot_panel->move(765, 190);
        coachinfo_panel->move(765, 335);
        log_panel->move(765, 500);
        break;
    case 2:
        coachinfo_panel->hide();
        gameinfo_panel->show();
        disconnectrb_btn->show();
        connectrb_btn->show();
        break;
    case 3:
        break;
    default:
        break;
    }
}
void Window::ConnectToRefereeBox()
{
    printLog("开始连接裁判盒");
    RBClient->Config(RBip, RBport);
    isConnectRB = RBClient->Connect();
    if (isConnectRB)
    {
        printGameinfo(QString("成功连接至") + RBClient->getip() + QString(":") + QString().number(RBClient->getport()));
        printLog("裁判盒连接成功");
        if (RBClient->isReadable())
            printGameinfo("开始接受裁判指令");
        else
            printGameinfo("连接不可用");
    }
}
void Window::DisconnectRefereeBox()
{
    isConnectRB = !RBClient->Disconnect();
    printLog("裁判盒连接断开");
}
char Window::parse(char flag)
{
    int result = -1;
    switch (flag)
    {
    case 'S':
        return STOPROBOT;
        break;
    case 's':
        return STARTROBOT;
        break;
    case 'H':
        break;
    case ' ':
        break;
    case 'L':
        return PARKINGROBOT;
        break;
    case '1':
        break;
    case 'h':
        break;
    case '2':
        break;
    case 'e':
        break;
    case 'x':
        break;
    case 'a':
        break;
    case 'A':
        break;
    case 'd':
        break;
    case 'D':
        break;
    case 'n':
        break;
    case 'k':
        break;
    case 'K':
        break;
    case 'f':
        break;
    case 'F':
        break;
    case 'g':
        break;
    case 'G':
        break;
    case 't':
        break;
    case 'T':
        break;
    case 'c':
        break;
    case 'C':
        break;
    case 'p':
        break;
    case 'P':
        break;
    case 'N':
        break;
    case '*':
        break;
    default:
        break;
    }
    return result;
}
void Window::RBinfoupdate()
{
    if (isConnectRB)
    {
        //        coach_info.MatchMode = parse(*RBClient->getBuffer().data());
    }
}
