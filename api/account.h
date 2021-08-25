#ifndef APIACCOUNT_H
#define APIACCOUNT_H

#include "user.h"

namespace ThorQ::Api {

class PasswordHash;

class Account : public User
{
    Q_OBJECT
public:
    Account(ThorQ::Api::User* apiUser);
    Account(ThorQ::Api::Client* apiClient);
    Account(ThorQ::Api::ApiConsumer* apiConsumer);

    bool UpdateFromJson(QJsonObject& json);

    QStringView email() const;
signals:
    void emailChanged(const QString& email);
public slots:
    void setEmail(const QString& email);
private:
    ThorQ::Api::PasswordHash* m_passwordHash;
    QString m_email;
};

}

#endif // APIACCOUNT_H
