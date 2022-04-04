#ifndef CUSTOMBTN_H
#define CUSTOMBTN_H

#include <QPushButton>
#include <QMouseEvent>

class CustomBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit CustomBtn(QWidget *parent = nullptr);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void setBtnSize(int w, int h);
    void setImage(QString loc, QString image_name);
    void setColor(QString normal,QString enter,QString press);
    void updateStyle();
    int getWidth();
    int getHeight();
private:
    enum btnstatus{NORMAL,ENTER,PRESS};
    btnstatus status;
    int btn_width;
    int btn_height;
    QString image_url[3];
    QString background_color[3];
signals:
};

#endif // CUSTOMBTN_H
