#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include "custombtn.h"
#include "rtdb/include/world_model/robot.h"
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QDebug>
#include <QPainter>

class SubWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SubWindow(int agent, QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent*);
    void putItem(QString item);
    void setPointer(nubot::MessageToCoach* info);
private:
    QPoint mouseStartPoint;
    QPoint windowTopLeftPoint;
    bool isMovingWindow;
    CustomBtn* close_btn;
    CustomBtn* minimize_btn;
    int item_num;
    nubot::MessageToCoach* roboinfo;

signals:

};

#endif // SUBWINDOW_H
