/**************************************************************************
**   Created by TemetNosce 2020/5/13
**************************************************************************/
#include "Widget.h"

#include "EngineXfader.h"
#include "ui_Widget.h"
#include <QCheckBox>
#include <QPainter>
#include <QPen>
#include <QSlider>

class RescalerUtils
{
public:
    // converts a value on a linear scale to a 1/x scale staring at 1
    static double linearToOneByX(double in, double inMin, double inMax, double outMax)
    {
        double outRange = outMax - 1;
        double inRange = inMax - inMin;
        return outMax / (((inMax - in) / inRange * outRange) + 1);
    }

    // converts value on a 1/x scale starting by 1 to a linear scale
    static double oneByXToLinear(double in, double inMax, double outMin, double outMax)
    {
        double outRange = outMax - outMin;
        double inRange = inMax - 1;
        return outMax - (((inMax / in) - 1) / inRange * outRange);
    }

private:
    RescalerUtils() {}
};
Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{

    ui->setupUi(this);
    // Range Slider 0 .. 100
    double sliderVal = RescalerUtils::oneByXToLinear(m_transform - EngineXfader::kTransformMin + 1,
                                                     EngineXfader::kTransformMax - EngineXfader::kTransformMin + 1,
                                                     ui->slider->minimum(), ui->slider->maximum());
    ui->slider->setValue(static_cast<int>(sliderVal + 0.5));
    m_cal = EngineXfader::getPowerCalibration(m_transform);
    if (m_xFaderMode == MIXXX_XFADER_CONSTPWR) {
        ui->check->setChecked(true);
    } else {
        ui->check->setChecked(false);
    }

    connect(ui->slider, &QSlider::valueChanged, [this](int val) {
        //        m_transform = val / 100.0 + 1;

        // m_transform is in the range of 1 to 1000 while 50 % slider results
        // to ~2, which represents a medium rounded fader curve.
        m_transform = RescalerUtils::linearToOneByX(val, ui->slider->minimum() /*0*/, ui->slider->maximum() /* 100*/,
                                                    EngineXfader::kTransformMax) -
                      1 + EngineXfader::kTransformMin;

        m_cal = EngineXfader::getPowerCalibration(m_transform);

        update();
    });
    connect(ui->check, &QCheckBox::clicked, [this](bool val) {
        if (val) {
            m_xFaderMode = MIXXX_XFADER_CONSTPWR;
        } else {
            m_xFaderMode = MIXXX_XFADER_ADDITIVE;
        }

        update();
    });
}
//#include <QDebug>
void Widget::paintEvent(QPaintEvent *)
{
    // m_transform is in the range of 1 to 1000 while 50 % slider results
    // to ~2, which represents a medium rounded fader curve.

    int offset = ui->slider->height() + 10;
    int sizeX = this->width();
    int sizeY = this->height() - offset;

    QPainter p(this);
    // Initialize QPens
    QPen gridPen(Qt::red);
    gridPen.setWidthF(4);
    p.setPen(gridPen);

    // Draw graph lines
    QPointF point1;
    QPointF point1Prev;
    int pointCount = sizeX - 4;
    // reduced by 2 x 1 for border + 2 x 1 for inner distance to border
    double xfadeStep = 2. / (pointCount - 1);

    for (int i = 0; i < pointCount; i++) {
        double gain1;
        double val = 0;
        if (m_xFaderMode == MIXXX_XFADER_ADDITIVE) {
            val = double(i) / (pointCount - 1);
        } else {
            val = -1. + (xfadeStep * i);
        }

        EngineXfader::getXfadeGains(val, m_transform, m_cal, m_xFaderMode, reverse, &gain1);

        // draw it
        point1 = QPointF(i + 1, (1. - gain1) * (sizeY) + offset /*-3*/);

        if (i > 0) {
            p.drawLine(QLineF(point1, point1Prev));
        }

        // Save old values
        point1Prev = point1;
    }
}

Widget::~Widget() { delete ui; }
