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
    , m_loginWindow(new ThorQ::UI::LoginWindow())
    , m_mainWindow(new ThorQ::UI::MainWindow())
    , m_apiClient(ThorQ::Api::Client::InitializeSingleton(this))
{
    m_loginWindow->show();
    //m_mainWindow->show();

    // Windows
    QObject::connect(m_loginWindow, &ThorQ::UI::LoginWindow::windowClosed, this, &ThorQ::Controllers::MainController::handleWindowClose);
    QObject::connect(m_mainWindow, &ThorQ::UI::MainWindow::windowClosed, this, &ThorQ::Controllers::MainController::handleWindowClose);
    QObject::connect(m_loginWindow->loginWidget(), &ThorQ::UI::LoginWidget::loginClicked, [this](){
        m_apiClient->currentAccount()->login(m_loginWindow->loginWidget()->username(), m_loginWindow->loginWidget()->password());
        m_loginWindow->loginWidget()->clearPassword();
        m_loginWindow->setEnabled(false);
    });
    QObject::connect(m_loginWindow->registerWidget(), &ThorQ::UI::RegisterWidget::registerClicked, [this](){
         m_apiClient->currentAccount()->registerAccount(m_loginWindow->registerWidget()->username(), m_loginWindow->registerWidget()->password(), m_loginWindow->registerWidget()->email());
        m_loginWindow->registerWidget()->clearPassword();
        m_loginWindow->setEnabled(false);
    });

    // Api stuff
    // TODO: Implement API feedback with description if the reason for the fail was specific to one input, or a general error like "invalid username/password" or "account already exists"
    QObject::connect(m_apiClient->currentAccount(), &ThorQ::Api::Account::loginFailed, [this](const QString& error){
        m_loginWindow->setEnabled(true);
        m_loginWindow->loginWidget()->invalidatePassword(error);
    });
    QObject::connect(m_apiClient->currentAccount(), &ThorQ::Api::Account::registerAccountFailed, [this](const QString& error){
        m_loginWindow->setEnabled(true);
        m_loginWindow->registerWidget()->invalidatePassword(error);
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
