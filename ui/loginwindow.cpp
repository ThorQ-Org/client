#include "loginwindow.h"

#include "controllers/windowpositionsaver.h"
#include "loginwidget.h"
#include "registerwidget.h"
#include "recoverwidget.h"
#include "stylesheets.h"
#include "constants.h"

#include <QTabWidget>
#include <QTabBar>
#include <QWidget>
#include <QApplication>
#include <QSettings>
#include <QScreen>
#include <QStyle>
#include <QTimer>

ThorQ::UI::LoginWindow::LoginWindow(QWidget* parent)
    : QTabWidget(parent)
    , m_loginWidget(new ThorQ::UI::LoginWidget(this))
    , m_registerWidget(new ThorQ::UI::RegisterWidget(this))
    , m_recoverWidget(new ThorQ::UI::RecoverWidget(this))
    , m_positionSaver(new ThorQ::Controllers::WindowPositionSaver("WindowLoginPosition", QSize(350, 450), this))
{
    // Set window title and geometry
    setWindowTitle(THORQ_APPLICATION_NAME " - Login");
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    m_loginWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_registerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_recoverWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Add widgets to tab
    addTab(m_loginWidget, "Login");
    addTab(m_registerWidget, "Register");
    addTab(m_recoverWidget, "Recover");

    // Configure tab looks
    setTabPosition(QTabWidget::TabPosition::South);
    tabBar()->setDocumentMode(true);
    tabBar()->setExpanding(true);
}

ThorQ::UI::LoginWidget* ThorQ::UI::LoginWindow::loginWidget() const
{
    return m_loginWidget;
}

ThorQ::UI::RegisterWidget* ThorQ::UI::LoginWindow::registerWidget() const
{
    return m_registerWidget;
}

ThorQ::UI::RecoverWidget* ThorQ::UI::LoginWindow::recoverWidget() const
{
    return m_recoverWidget;
}
