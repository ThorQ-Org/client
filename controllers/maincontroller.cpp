#include "maincontroller.h"

#include "api/client.h"
#include "api/account.h"
#include "ui/mainwindow.h"
#include "ui/loginwindow.h"
#include "ui/loginwidget.h"
#include "ui/registerwidget.h"
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

    QObject::connect(m_loginWindow->loginWidget(), &ThorQ::UI::LoginWidget::loginClicked, [this](){
        ThorQ::Api::Account::login(m_loginWindow->loginWidget()->username(), m_loginWindow->loginWidget()->password());
        m_loginWindow->loginWidget()->clearPassword();
    });
    QObject::connect(m_loginWindow->registerWidget(), &ThorQ::UI::RegisterWidget::registerClicked, [this](){
        ThorQ::Api::Account::registerAccount(m_loginWindow->registerWidget()->username(), m_loginWindow->registerWidget()->password(), m_loginWindow->registerWidget()->email());
        m_loginWindow->registerWidget()->clearPassword();
    });

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
