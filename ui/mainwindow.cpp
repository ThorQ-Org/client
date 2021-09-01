#include "mainwindow.h"

#include "constants.h"

#include <QApplication>
#include <QSettings>
#include <QScreen>
#include <QStyle>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

ThorQ::UI::MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_settingsTimer(new QTimer(this))
    , m_vlayout(new QVBoxLayout(this))
    , m_hlayout(new QHBoxLayout())
    , m_userScene(new QGraphicsScene(this))
{
    // Get settings
    QSettings settings;
    QVariant geometryValue = settings.value("WindowMainGeometry");

    // Set screen and geometry if the settings are valid
    QRect windowGeometry;
    QScreen* windowScreen = nullptr;
    if (geometryValue.type() == QVariant::Rect) {
        windowGeometry = geometryValue.toRect();
        windowScreen = QApplication::screenAt(windowGeometry.topLeft());
    }

    // Check if geometry is offscreen, and if so reset it
    if (windowScreen == nullptr) {
        // Get primary screen
        windowScreen = QApplication::primaryScreen();

        // Default geometry
        windowGeometry = QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                windowScreen->size() / 2,
                windowScreen->availableGeometry()
            );

        // Save updated geometry to settings
        settings.setValue("WindowMainGeometry", windowGeometry);
    }

    // Set initial geometry
    m_lastGeometry = windowGeometry;

    // Set window title and geometry
    setWindowTitle(THORQ_APPLICATION_NAME);
    setGeometry(windowGeometry);

    // Set up layouts
    m_vlayout->addLayout(m_hlayout);

    // Initialize timer to autosave window geometry
    m_settingsTimer->setSingleShot(false);
    m_settingsTimer->setInterval(1000);
    QObject::connect(m_settingsTimer, &QTimer::timeout, this, &ThorQ::UI::MainWindow::saveWindowState);
    m_settingsTimer->start();
}

void ThorQ::UI::MainWindow::saveWindowState()
{
    if (!isVisible()) {
        return;
    }

    QRect currentGeometry = geometry();

    if (m_lastGeometry != currentGeometry) {
        m_lastGeometry = currentGeometry;
        QSettings().setValue("WindowMainGeometry", currentGeometry);
    }
}
