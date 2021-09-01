#include "loginwindow.h"

#include "loginwidget.h"
#include "registerwidget.h"
#include "recoverwidget.h"
#include "constants.h"

#include <QApplication>
#include <QSettings>
#include <QScreen>
#include <QStyle>
#include <QTimer>

const QSize FixedSize(350, 450);

ThorQ::UI::LoginWindow::LoginWindow(QWidget* parent)
    : QWidget(parent)
    , m_settingsTimer(new QTimer(this))
    , m_loginWidget(new ThorQ::UI::LoginWidget(this))
    , m_registerWidget(new ThorQ::UI::RegisterWidget(this))
    , m_recoverWidget(new ThorQ::UI::RecoverWidget(this))
{
    // Get settings
    QSettings settings;
    QVariant positionValue = settings.value("WindowLoginPosition");

    // Set screen and position if the settings are valid
    QPoint windowPosition;
    QScreen* windowScreen = nullptr;
    if (positionValue.type() == QVariant::Point) {
        windowPosition = positionValue.toPoint();
        windowScreen = QApplication::screenAt(windowPosition);
    }

    // Check if position is offscreen, and if so reset it
    if (windowScreen == nullptr) {
        // Get primary screen
        windowScreen = QApplication::primaryScreen();

        // Default position
        windowPosition = QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                FixedSize,
                windowScreen->availableGeometry()
            ).topLeft();

        // Save updated position to settings
        settings.setValue("WindowLoginPosition", windowPosition);
    }

    // Set initial position
    m_lastPosition = windowPosition;

    // Set window title and geometry
    setWindowTitle(THORQ_APPLICATION_NAME " - Login");
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    setFixedSize(FixedSize);
    move(windowPosition);

    // Initialize timer to autosave window position
    m_settingsTimer->setSingleShot(false);
    m_settingsTimer->setInterval(1000);
    QObject::connect(m_settingsTimer, &QTimer::timeout, this, &ThorQ::UI::LoginWindow::saveWindowState);
    m_settingsTimer->start();
}

void ThorQ::UI::LoginWindow::saveWindowState()
{
    if (!isVisible()) {
        return;
    }

    QPoint currentPosition = pos();

    if (m_lastPosition != currentPosition) {
        m_lastPosition = currentPosition;
        QSettings().setValue("WindowLoginPosition", currentPosition);
    }
}
