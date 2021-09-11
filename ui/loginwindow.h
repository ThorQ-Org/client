#ifndef THORQ_LOGINWINDOW_H
#define THORQ_LOGINWINDOW_H

#include <QWidget>

class QTabWidget;

namespace ThorQ::UI {

class LoginWidget;
class RegisterWidget;
class RecoverWidget;
class WindowPositionSaver;

class LoginWindow : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(LoginWindow)
public:
    LoginWindow(QWidget* parent = nullptr);
private:
    QTabWidget* m_tabWidget;
    ThorQ::UI::LoginWidget* m_loginWidget;
    ThorQ::UI::RegisterWidget* m_registerWidget;
    ThorQ::UI::RecoverWidget* m_recoverWidget;
    ThorQ::UI::WindowPositionSaver* m_positionSaver;
};

}

#endif // THORQ_LOGINWINDOW_H
