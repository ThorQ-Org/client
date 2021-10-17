#ifndef THORQ_REGISTERWIDGET_H
#define THORQ_REGISTERWIDGET_H

#include <QWidget>

class QPushButton;
class QVBoxLayout;

namespace ThorQ::UI {

class NamedLineEdit;

class RegisterWidget : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(RegisterWidget)
public:
    RegisterWidget(QWidget *parent = nullptr);
    ~RegisterWidget();

    QString username() const;
    QString email() const;
    QString password() const;
signals:
    void usernameChanged(const QString& email);
    void emailChanged(const QString& email);
    void passwordChanged(const QString& email);
    void registerClicked();
public slots:
    void invalidateUsername(const QString& reason);
    void invalidateEmail(const QString& reason);
    void invalidatePassword(const QString& reason);
    void clearForms();
    void clearPassword();
private slots:
    void handleUsernameInputChanged();
    void handleEmailInputChanged();
    void handlePasswordInputChanged();
    void handleInputValidChanged();
    void handleRegisterClicked();
private:
    NamedLineEdit* m_usernameInput;
    NamedLineEdit* m_emailInput;
    NamedLineEdit* m_passwordInput;
    NamedLineEdit* m_confirmPasswordInput;
    QPushButton* m_registerButton;

    QVBoxLayout* m_layout;
};

}

#endif // THORQ_REGISTERWIDGET_H
