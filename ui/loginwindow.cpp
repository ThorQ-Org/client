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
    : QWidget(parent)
    , m_tabWidget(new QTabWidget(this))
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

    m_tabWidget->addTab(m_loginWidget, "Login");
    m_tabWidget->addTab(m_registerWidget, "Register");
    m_tabWidget->addTab(m_recoverWidget, "Recover");
    m_tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Configure tab looks
    m_tabWidget->setTabPosition(QTabWidget::TabPosition::South);
    m_tabWidget->tabBar()->setDocumentMode(true);
    m_tabWidget->tabBar()->setExpanding(true);
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
