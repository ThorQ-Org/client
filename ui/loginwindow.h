#ifndef THORQ_LOGINWINDOW_H
#define THORQ_LOGINWINDOW_H

#include <QWidget>

class QTabWidget;

namespace ThorQ {

namespace Controllers {

class WindowPositionSaver;

}

namespace UI {

class LoginWidget;
class RegisterWidget;
class RecoverWidget;

class LoginWindow : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(LoginWindow)
public:
    LoginWindow(QWidget* parent = nullptr);

    ThorQ::UI::LoginWidget* loginWidget() const;
    ThorQ::UI::RegisterWidget* registerWidget() const;
    ThorQ::UI::RecoverWidget* recoverWidget() const;
private:
    QTabWidget* m_tabWidget;
    ThorQ::UI::LoginWidget* m_loginWidget;
    ThorQ::UI::RegisterWidget* m_registerWidget;
    ThorQ::UI::RecoverWidget* m_recoverWidget;
    ThorQ::Controllers::WindowPositionSaver* m_positionSaver;
};

}
}

#endif // THORQ_LOGINWINDOW_H
