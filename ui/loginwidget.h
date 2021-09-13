#ifndef THORQ_LOGINWIDGET_H
#define THORQ_LOGINWIDGET_H

#include <QWidget>

class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;

namespace ThorQ::UI {

class NamedLineEdit;

class LoginWidget : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(LoginWidget)
public:
    LoginWidget(QWidget* parent = nullptr);
    ~LoginWidget();

    QString username() const;
    QString password() const;
signals:
    void usernameChanged(const QString& username);
    void passwordChanged(const QString& password);
    void loginClicked();
public slots:
    void setOnlineStatus(const QString& status, QColor color);
    void invalidateUsername(const QString& reason);
    void invalidatePassword(const QString& reason);
    void clearForms();
    void clearPassword();
private slots:
    void handleUsernameInputChanged();
    void handlePasswordInputChanged();
    void handleLoginClicked();
private:
    QLabel* m_title;
    QLabel* m_onlineStatus;
    NamedLineEdit* m_usernameInput;
    NamedLineEdit* m_passwordInput;
    QPushButton* m_loginButton;

    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_headerLayout;
};

}

#endif // THORQ_LOGINWIDGET_H
