#include "loginwindow.h"

#include "loginwidget.h"
#include "registerwidget.h"
#include "recoverwidget.h"
#include "windowpositionsaver.h"
#include "constants.h"

#include <QTabWidget>
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
    , m_positionSaver(new ThorQ::UI::WindowPositionSaver("WindowLoginPosition", QSize(350, 450), this))
{
    // Set window title and geometry
    setWindowTitle(THORQ_APPLICATION_NAME " - Login");
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    m_tabWidget->addTab(m_loginWidget, "Login");
    m_tabWidget->addTab(m_recoverWidget, "Recover");
    m_tabWidget->addTab(m_registerWidget, "Register");
}
