﻿#ifndef MIDDLE_WIDGET_H
#define MIDDLE_WIDGET_H

#include <QStackedWidget>
#include <QPropertyAnimation>

#include"baseWidget.h"
#include"myPushButton.h"

#include "pageSelectImage.h"
#include "pageShowImage.h"

class MiddleWidget : public baseWidget
{

    Q_OBJECT

    Q_PROPERTY(int m_x READ x WRITE setX)  //要实现属性动画，必须指定属性的setter
public:
    explicit MiddleWidget(QWidget *parent = 0);

    void initWidget();
    void initAnimation();


    int x() const;
    void setX(int x);

private slots:
    void slot_btn();
    void pageChanged(int i);//改变选中页面
    void slot_finished()
    {
        m_isAnima=false;
    }

    void SwitchToShowPage(){m_stackwid.setCurrentIndex(1);}
    void returnToSelect(){m_stackwid.setCurrentIndex(0);}

protected:
    void paintEvent(QPaintEvent *);

public:
    int mHeaderButtonHeight;        //页面按钮高度
    stackButton *m_btn[2];

    QStackedWidget  m_stackwid;
    PageSelectImage pageSelect;
    PageShowImage pageShow;

    QPixmap m_pix;
    int m_index;
    int m_preindex;
    bool m_isDrawVerticalLine;
    bool m_isAnima;
    int m_x;
    int m_preItem;

private:
    QPropertyAnimation m_animation;
    QPropertyAnimation m_Widanimation;
};

#endif // MIDDLE_WIDGET_H
