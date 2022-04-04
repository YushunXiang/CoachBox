#ifndef SLIDEPAGE_H
#define SLIDEPAGE_H

#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QMouseEvent>

class SlidePage : public QLabel
{
    Q_OBJECT
public:
    explicit SlidePage(QWidget *parent = nullptr);
    void slidein();
    void slideout();
    void putTitle(QString title);
    void puttext(QString text);
    bool isShow;
    int item_num;
private:
    QPropertyAnimation* movein;
    QPropertyAnimation* moveout;
    QParallelAnimationGroup* group;

signals:
    void exit();
    void show();
};

#endif // SLIDEPAGE_H
