#include "windowgeometrysaver.h"

#include <QStyle>
#include <QTimer>
#include <QWidget>
#include <QScreen>
#include <QSettings>
#include <QApplication>

ThorQ::Controllers::WindowGeometrySaver::WindowGeometrySaver(QString settingName, QWidget* targetWidget)
    : QObject(targetWidget)
    , m_lastRect()
    , m_settingName(settingName)
    , m_updateTimer(new QTimer(this))
    , m_targetWidget(targetWidget)
    , m_settings(new QSettings(this))
{
    // Get settings
    QSettings settings;
    QVariant positionValue = settings.value(settingName);

    // Set screen and position if the settings are valid
    QRect windowRect;
    QScreen* windowScreen = nullptr;
    if (positionValue.type() == QVariant::Rect) {
        windowRect = positionValue.toRect();
        windowScreen = QApplication::screenAt(windowRect.topLeft());
    }

    // Check if position is offscreen, and if so reset it
    if (windowScreen == nullptr) {
        // Get primary screen
        windowScreen = QApplication::primaryScreen();

        // Default position
        windowRect = QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                windowScreen->size() / 2,
                windowScreen->availableGeometry()
            );

        // Save updated position to settings
        settings.setValue(settingName, windowRect);
    }

    // Set initial position
    m_lastRect = windowRect;

    m_targetWidget->setGeometry(windowRect);

    m_updateTimer->setSingleShot(false);
    m_updateTimer->setInterval(1000);

    QObject::connect(m_updateTimer, &QTimer::timeout, this, &ThorQ::Controllers::WindowGeometrySaver::saveGeometry);

    m_updateTimer->start();
}

void ThorQ::Controllers::WindowGeometrySaver::saveGeometry()
{
    if (m_targetWidget->isVisible()) {
        QRect currentRect = m_targetWidget->rect();

        if (m_lastRect != currentRect) {
            m_lastRect = currentRect;
            m_settings->setValue(m_settingName, currentRect);
        }
    }
}
