#include "loginwindow.h"

#include "loginwidget.h"
#include "registerwidget.h"
#include "recoverwidget.h"

ThorQ::UI::LoginWindow::LoginWindow(QWidget* parent)
    : QWidget(parent)
    , m_loginWidget(new ThorQ::UI::LoginWidget(this))
    , m_registerWidget(new ThorQ::UI::RegisterWidget(this))
    , m_recoverWidget(new ThorQ::UI::RecoverWidget(this))
{
}
