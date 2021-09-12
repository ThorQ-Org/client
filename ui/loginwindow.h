#ifndef THORQ_LOGINWINDOW_H
#define THORQ_LOGINWINDOW_H

#include <QTabWidget>
#include <QWidget>

namespace ThorQ {

namespace Controllers {

class WindowPositionSaver;

}

namespace UI {

class LoginWidget;
class RegisterWidget;
class RecoverWidget;

class LoginWindow : public QTabWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(LoginWindow)
public:
    LoginWindow(QWidget* parent = nullptr);

    ThorQ::UI::LoginWidget* loginWidget() const;
    ThorQ::UI::RegisterWidget* registerWidget() const;
    ThorQ::UI::RecoverWidget* recoverWidget() const;
signals:
    void windowClosed();
protected:
    void closeEvent(QCloseEvent* event) override;
private:
    ThorQ::UI::LoginWidget* m_loginWidget;
    ThorQ::UI::RegisterWidget* m_registerWidget;
    ThorQ::UI::RecoverWidget* m_recoverWidget;
    ThorQ::Controllers::WindowPositionSaver* m_positionSaver;
};

}
}

#endif // THORQ_LOGINWINDOW_H
