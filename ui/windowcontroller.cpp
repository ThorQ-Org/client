#include "windowcontroller.h"

#include "mainwindow.h"
#include "loginwindow.h"
#include "constants.h"

#include <QApplication>
#include <QScreen>
#include <QStyle>

ThorQ::UI::WindowController::WindowController(QObject* parent)
    : QObject(parent)
    , m_loginWindow(new ThorQ::UI::LoginWindow())
    , m_mainWindow(new ThorQ::UI::MainWindow())
{
    m_loginWindow->show();
    //m_mainWindow->show();
}
