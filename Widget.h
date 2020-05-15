/**************************************************************************
**   Created by TemetNosce 2020/5/13
**************************************************************************/
#pragma once

#include "EngineXfader.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:

    Widget(QWidget *parent = nullptr);
    // X-fader values
    double m_xFaderMode= MIXXX_XFADER_ADDITIVE;
    double m_transform = EngineXfader::kTransformDefault ;
    double m_cal =  0.0;
    bool reverse = false;//true反向平滑转换(hamster类型)
    virtual void paintEvent(QPaintEvent *event);
    ~Widget();

private:
    Ui::Widget *ui;
};
