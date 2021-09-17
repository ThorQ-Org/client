#ifndef THORQ_APIACCOUNT_H
#define THORQ_APIACCOUNT_H

#include "apiobject.h"

namespace ThorQ::Api {

class User;
class PasswordHash;

class Account : public ThorQ::Api::ApiObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Account)
public:
    Account(ThorQ::Api::Client* apiClient);
    Account(ThorQ::Api::ApiObject* apiObject);

    bool updateFromJson(const QJsonObject& json);

    QStringView email() const;
signals:
    void loginSuccess();
    void loginFailed(const QString& error);
    void logoutSuccess();
    void logoutFailed(const QString& error);
    void registerAccountSuccess();
    void registerAccountFailed(const QString& error);

    void emailChanged(const QString& email);
public slots:
    void update() override;

    void login(const QString& username, const QString& password);
    void logout();
    void registerAccount(const QString& username, const QString& password, const QString& email);

    void setEmail(const QString& email);
private:
    ThorQ::Api::User* m_accountUser;
    ThorQ::Api::PasswordHash* m_passwordHash;
    QString m_email;
};

}

#endif // THORQ_APIACCOUNT_H
