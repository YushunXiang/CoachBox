#include "custombtn.h"
#include <QDebug>

CustomBtn::CustomBtn(QWidget *parent) : QPushButton(parent)
{
    status=NORMAL;
    btn_width=0;
    btn_height=0;
    for(int i=0;i<3;i++)
    {
        image_url[i]="";
        background_color[i]="white";
    }
}
void CustomBtn::enterEvent(QEvent*)
{
    status=ENTER;
    updateStyle();
}
void CustomBtn::leaveEvent(QEvent*)
{
    status=NORMAL;
    updateStyle();
}
void CustomBtn::mousePressEvent(QMouseEvent* event)
{
    if(event->button()==Qt::LeftButton)
    {
        status=PRESS;
        updateStyle();
    }
}
void CustomBtn::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(event->pos().x()>=0 && event->pos().x()<=btn_width && event->pos().y()>=0 && event->pos().y()<=btn_height)
        {
            emit clicked();
        }
        status=ENTER;
        updateStyle();
    }
}
void CustomBtn::setBtnSize(int w, int h)
{
    btn_width=w;
    btn_height=h;
    this->resize(btn_width,btn_height);
}
void CustomBtn::updateStyle()
{
    switch (status) {
    case NORMAL:
        this->setStyleSheet(QString("QPushButton{background-color:"+background_color[0]+";border-style:none;image:url(")+image_url[0]+QString(");}"));
        break;
    case ENTER:
        this->setStyleSheet(QString("QPushButton{background-color:"+background_color[1]+";border-style:none;image:url(")+image_url[1]+QString(");}"));
        break;
    case PRESS:
        this->setStyleSheet(QString("QPushButton{background-color:"+background_color[2]+";border-style:none;image:url(")+image_url[2]+QString(");}"));
        break;
    default:
        break;
    }
}
void CustomBtn::setImage(QString loc, QString image_name)
{
    image_url[0]=loc+image_name+QString("_normal.png");
    image_url[1]=loc+image_name+QString("_hover.png");
    image_url[2]=loc+image_name+QString("_press.png");
}
void CustomBtn::setColor(QString normal,QString enter,QString press)
{
    background_color[0]=normal;
    background_color[1]=enter;
    background_color[2]=press;
}
int CustomBtn::getWidth()
{
    return btn_width;
}
int CustomBtn::getHeight()
{
    return  btn_height;
}
