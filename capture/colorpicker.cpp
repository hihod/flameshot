﻿#include "colorpicker.h"
#include <QPainter>
#include <QMouseEvent>
#include <QSettings>
#include <QDebug>

ColorPicker::ColorPicker(QWidget *parent) : QWidget(parent),
        m_colorAreaSize(18) {
    setMouseTracking(true);

    QSettings settings;
    m_uiColor = settings.value("uiColor").value<QColor>();
    m_drawColor = settings.value("drawColor").value<QColor>();
    const int extraSize = 6;
    double radius = (colorList.size()*m_colorAreaSize/1.3)/(3.141592);
    resize(radius*2 + m_colorAreaSize + extraSize,
           radius*2 + m_colorAreaSize+ extraSize);
    double degree = 360 / (colorList.size());
    double degreeAcum = degree;

    QLineF baseLine = QLineF(QPoint(radius+extraSize/2, radius+extraSize/2),
                             QPoint(radius*2, radius));

    for (int i = 0; i<colorList.size(); ++i) {
        m_colorAreaList.append(QRect(baseLine.x2(), baseLine.y2(),
                                 m_colorAreaSize, m_colorAreaSize));
        baseLine.setAngle(degreeAcum);
        degreeAcum += degree;
    }
}

ColorPicker::~ColorPicker() {
    QSettings().setValue("drawColor", m_drawColor);
}

QColor ColorPicker::getDrawColor() {
    return m_drawColor;
}

void ColorPicker::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QVector<QRect> rects = handleMask();
    painter.setPen(QColor(Qt::black));
    for(int i = 0; i < rects.size(); ++i) {
        if (m_drawColor == QColor(colorList.at(i))) {
            QColor c = QColor(m_uiColor);
            c.setAlpha(155);
            painter.setBrush(c);
            c.setAlpha(100);
            painter.setPen(c);
            QRect highlight = rects.at(i);
            highlight.moveTo(highlight.x() - 3, highlight.y() - 3);
            highlight.setHeight(highlight.height() + 6);
            highlight.setWidth(highlight.width() + 6);
            painter.drawRoundRect(highlight, 100, 100);
            painter.setPen(QColor(Qt::black));
        }
        painter.setBrush(QColor(colorList.at(i)));
        painter.drawRoundRect(rects.at(i), 100, 100);
    }
}

void ColorPicker::mouseMoveEvent(QMouseEvent *e) {
  for(int i = 0; i < colorList.size(); ++i) {
    if (m_colorAreaList.at(i).contains(e->pos())) {
        m_drawColor = colorList.at(i);
        update();
        break;
    }
  }
}

QVector<QRect> ColorPicker::handleMask() const {
    QVector<QRect> areas;
    for(QRect rect: m_colorAreaList) {
        areas.append(rect);
    }

    return areas;
}

QVector<Qt::GlobalColor> ColorPicker::colorList = {
    Qt::darkRed,
    Qt::red,
    Qt::yellow,
    Qt::green,
    Qt::darkGreen,
    Qt::cyan,
    Qt::blue,
    Qt::magenta,
    Qt::darkMagenta
};
