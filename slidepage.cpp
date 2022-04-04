#include "slidepage.h"

SlidePage::SlidePage(QWidget *parent) : QLabel(parent)
{
    isShow=false;
    setFixedSize(400,720);
    setStyleSheet("QLabel{background-color:white;border-width:2px;border-style:none solid none none;border-color:rgb(210,210,210);}");
    move(-400,0);
    movein = new QPropertyAnimation(this,"geometry");
    moveout = new QPropertyAnimation(this,"geometry");
    item_num=0;
}
void SlidePage::slidein()
{
    movein->setDuration(500);
    movein->setEasingCurve(QEasingCurve::InOutExpo);
    movein->setStartValue(QRect(-400,0,this->width(),this->height()));
    movein->setEndValue(QRect(0,0,this->width(),this->height()));
    movein->start();
    isShow=true;
}
void SlidePage::slideout()
{
    moveout->setDuration(500);
    movein->setEasingCurve(QEasingCurve::InOutExpo);
    moveout->setStartValue(QRect(0,0,this->width(),this->height()));
    moveout->setEndValue(QRect(-400,0,this->width(),this->height()));
    moveout->start();
    isShow=false;
}
void SlidePage::putTitle(QString title)
{
    QLabel* t = new QLabel(this);
    t->setText(title);
    t->setStyleSheet("QLabel{border-style:none none solid none;font-size:30px}");
    t->move(50,50);
}
void SlidePage::puttext(QString text)
{
    QLabel* t = new QLabel(this);
    t->setText(text);
    t->setStyleSheet("QLabel{border-style:none;font-size:17px}");
    t->move(50,120+item_num*50);
    item_num++;
}
