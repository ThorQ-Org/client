#include "windowpositionsaver.h"

#include <QStyle>
#include <QTimer>
#include <QWidget>
#include <QScreen>
#include <QSettings>
#include <QApplication>

ThorQ::UI::WindowPositionSaver::WindowPositionSaver(QString settingName, QSize fixedSize, QWidget* targetWidget)
    : QObject(targetWidget)
    , m_lastPos()
    , m_settingName(settingName)
    , m_updateTimer(new QTimer(this))
    , m_targetWidget(targetWidget)
    , m_settings(new QSettings(this))
{
    // Get settings
    QSettings settings;
    QVariant positionValue = settings.value(settingName);

    // Set screen and position if the settings are valid
    QPoint windowPos;
    QScreen* windowScreen = nullptr;
    if (positionValue.type() == QVariant::Point) {
        windowPos = positionValue.toPoint();
        windowScreen = QApplication::screenAt(windowPos);
    }

    // Check if position is offscreen, and if so reset it
    if (windowScreen == nullptr) {
        // Get primary screen
        windowScreen = QApplication::primaryScreen();

        // Default position
        windowPos = QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                fixedSize,
                windowScreen->availableGeometry()
            ).topLeft();

        // Save updated position to settings
        settings.setValue(settingName, windowPos);
    }

    // Set initial position
    m_lastPos = windowPos;

    m_targetWidget->setFixedSize(fixedSize);
    m_targetWidget->move(windowPos);

    m_updateTimer->setSingleShot(false);
    m_updateTimer->setInterval(1000);

    QObject::connect(m_updateTimer, &QTimer::timeout, this, &ThorQ::UI::WindowPositionSaver::savePosition);

    m_updateTimer->start();
}

void ThorQ::UI::WindowPositionSaver::savePosition()
{
    if (m_targetWidget->isVisible()) {
        QPoint currentPos = m_targetWidget->pos();

        if (m_lastPos != currentPos) {
            m_lastPos = currentPos;
            m_settings->setValue(m_settingName, currentPos);
        }
    }
}
