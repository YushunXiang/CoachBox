#include "subwindow.h"

SubWindow::SubWindow(int agent, QWidget *parent) : QWidget(parent)
{
    item_num=0;
    mouseStartPoint=QPoint(-1,-1);
    windowTopLeftPoint=this->frameGeometry().topLeft();
    isMovingWindow=false;
    setFixedSize(400,720);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("QWidget{background-color:white}");
    close_btn = new CustomBtn(this);
    minimize_btn = new CustomBtn(this);
    close_btn->setBtnSize(50,30);
    minimize_btn->setBtnSize(50,30);
    close_btn->setImage(":/icons/","close");
    minimize_btn->setImage(":/icons/","minimize");
    close_btn->setColor("white","rgb(240,240,240)","rgb(200,200,200)");//color:255 138 81
    minimize_btn->setColor("white","rgb(240,240,240)","rgb(200,200,200)");
    close_btn->updateStyle();
    minimize_btn->updateStyle();
    close_btn->move(400-close_btn->getWidth(),0);
    minimize_btn->move(400-close_btn->getWidth()-minimize_btn->getWidth(),0);
    connect(close_btn,&QPushButton::clicked,this,&QWidget::close);
    connect(minimize_btn,&QPushButton::clicked,this,&QWidget::showMinimized);
    QLabel* title = new QLabel(this);
    title->setText("Agent"+QString().setNum(agent));
    title->setStyleSheet("QLabel{border-style:none none solid none;border-width:2px;border-color:rgb(210,210,210);font-size:30px;}");
    title->move(50,50);
    putItem("当前信息有效:");
    putItem("位置:");
    putItem("朝向:");
    putItem("球的位置:");
    putItem("球速:");
    putItem("持球:");
    putItem("当前角色:");
    putItem("当前行为:");
}
void SubWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton && event->pos().y()<=close_btn->getHeight()*2)
    {
        isMovingWindow=true;
        mouseStartPoint = event->globalPos();
        windowTopLeftPoint=this->frameGeometry().topLeft();
    }
}
void SubWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(isMovingWindow)
    {
        QPoint move_vector=event->globalPos()-mouseStartPoint;
        this->move(windowTopLeftPoint+move_vector);
    }
}
void SubWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        isMovingWindow=false;
    }
}
void SubWindow::putItem(QString info)
{
    QLabel* t = new QLabel(this);
    t->setText(info);
    t->setStyleSheet("QLabel{border-style:none;font-size:17px}");
    t->move(50,120+item_num*50);
    item_num++;
}
void SubWindow::setPointer(nubot::MessageToCoach *info)
{
    roboinfo=info;
}
void SubWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QFont font;
    font.setPixelSize(17);
    painter.setFont(font);
    QString data="";
    if(roboinfo->is_valid)painter.drawText(180,140,"有效");
    else painter.drawText(180,140,"无效");
    data=data+"("+QString().setNum(roboinfo->robot_global_loc.x_)+","+QString().setNum(roboinfo->robot_global_loc.y_)+")";
    painter.drawText(110,190,data);
    data.clear();
    data=data+QString().setNum(roboinfo->robot_head);
    painter.drawText(110,240,data);
    data.clear();
    data=data+"("+QString().setNum(roboinfo->ball_global_loc.x_)+","+QString().setNum(roboinfo->ball_global_loc.y_)+")";
    painter.drawText(145,290,data);
    data.clear();
    data=data+"("+QString().setNum(roboinfo->ball_global_vec.x_)+","+QString().setNum(roboinfo->ball_global_vec.y_)+")";
    painter.drawText(110,340,data);
    data.clear();
    if(roboinfo->is_dribble)painter.drawText(110,390,"正在持球");
    else painter.drawText(110,390,"没有持球");
    switch (roboinfo->role) {
    case GOALIE:
        painter.drawText(145,440,"GOALIE");
        break;
    case ACTIVE:
        painter.drawText(145,440,"ACTIVE");
        break;
    case PASSIVE:
        painter.drawText(145,440,"PASSIVE");
        break;
    case MIDFIELD:
        painter.drawText(145,440,"MIDFIELD");
        break;
    case ASSISTANT:
        painter.drawText(145,440,"ASSISTANT");
        break;
    case ACIDPASSIVE:
        painter.drawText(145,440,"ACIDPASSIVE");
        break;
    case GAZER:
        painter.drawText(145,440,"GAZER");
        break;
    case BLOCK:
        painter.drawText(145,440,"BLOCK");
        break;
    case NOROLE:
        painter.drawText(145,440,"NOROLE");
        break;
    case CATCHOFPASS:
        painter.drawText(145,440,"CATCHOFPASS");
        break;
    case PASSOFPASS:
        painter.drawText(145,440,"PASSOFPASS");
        break;
    default:
        break;
    }
    switch (roboinfo->action) {
    case Stucked:
        painter.drawText(145,490,"Stucked");
        break;
    case Penalty:
        painter.drawText(145,490,"Penalty");
        break;
    case CanNotSeeBall:
        painter.drawText(145,490,"CanNotSeeBall");
        break;
    case SeeNotDribbleBall:
        painter.drawText(145,490,"SeeNotDribbleBall");
        break;
    case TurnForShoot:
        painter.drawText(145,490,"TurnForShoot");
        break;
    case AtShootSituation:
        painter.drawText(145,490,"AtShootSituation");
        break;
    case TurnToPass:
        painter.drawText(145,490,"TurnToPass");
        break;
    case StaticPass:
        painter.drawText(145,490,"StaticPass");
        break;
    case AvoidObs:
        painter.drawText(145,490,"AvoidObs");
        break;
    case Catch_Positioned:
        painter.drawText(145,490,"Catch_Positioned");
        break;
    case Positioned:
        painter.drawText(145,490,"Positioned");
        break;
    case Positioned_Static:
        painter.drawText(145,490,"Positioned_Static");
        break;
    case No_Action:
        painter.drawText(145,490,"No_Action");
        break;
    case Eight_Attack:
        painter.drawText(145,490,"Eight_Attack");
        break;
    case NULLaction:
        painter.drawText(145,490,"NULLaction");
        break;
    default:
        break;
    }

}
