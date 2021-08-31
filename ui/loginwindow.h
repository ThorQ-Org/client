#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace ThorQ::UI {

class LoginWidget;
class RegisterWidget;
class RecoverWidget;

class LoginWindow : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(LoginWindow)
public:
    LoginWindow(QWidget* parent = nullptr);
private:
    ThorQ::UI::LoginWidget* m_loginWidget;
    ThorQ::UI::RegisterWidget* m_registerWidget;
    ThorQ::UI::RecoverWidget* m_recoverWidget;
};

}

#endif // LOGINWINDOW_H
