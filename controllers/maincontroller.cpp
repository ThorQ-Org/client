#include "maincontroller.h"

#include "api/client.h"
#include "ui/mainwindow.h"
#include "ui/loginwindow.h"
#include "constants.h"

#include <QApplication>
#include <QScreen>
#include <QStyle>

ThorQ::Controllers::MainController::MainController(QObject* parent)
    : QObject(parent)
    , m_apiClient(ThorQ::Api::Client::InitializeSingleton(this))
    , m_loginWindow(new ThorQ::UI::LoginWindow())
    , m_mainWindow(new ThorQ::UI::MainWindow())
{
    m_loginWindow->show();
    //m_mainWindow->show();

    QObject::connect(m_loginWindow, &ThorQ::UI::LoginWindow::windowClosed, this, &ThorQ::Controllers::MainController::handleWindowClose);
    QObject::connect(m_mainWindow, &ThorQ::UI::MainWindow::windowClosed, this, &ThorQ::Controllers::MainController::handleWindowClose);

    m_apiClient->getHealth();
}

ThorQ::Controllers::MainController::~MainController()
{
    if (m_loginWindow != nullptr) {
        m_loginWindow->deleteLater();
    }
    if (m_mainWindow != nullptr) {
        m_mainWindow->deleteLater();
    }
}

void ThorQ::Controllers::MainController::handleWindowClose()
{
    QApplication::quit();
}
