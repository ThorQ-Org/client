#include "windowcontroller.h"

#include "mainwindow.h"
#include "loginwindow.h"
#include "constants.h"

#include <QApplication>
#include <QScreen>
#include <QStyle>
#include <QTimer>

ThorQ::UI::WindowController::WindowController(QObject* parent)
    : QObject(parent)
    , m_loginWindow(new ThorQ::UI::LoginWindow())
    , m_mainWindow(new ThorQ::UI::MainWindow())
{
    m_loginWindow->setWindowTitle(THORQ_APPLICATION_NAME " - Login");
    m_loginWindow->setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    m_loginWindow->setFixedSize(350, 450);
    m_loginWindow->setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    m_loginWindow->size(),
                    QApplication::primaryScreen()->availableGeometry()
                )
            );
}
