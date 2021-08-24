#ifndef APIACCOUNT_H
#define APIACCOUNT_H

#include "user.h"

namespace ThorQ::Api {

class PasswordHash;

class Account : public User
{
    Q_OBJECT
public:
    Account(QObject* parent = nullptr);

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
