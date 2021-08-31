#ifndef APIACCOUNT_H
#define APIACCOUNT_H

#include "apiobject.h"

namespace ThorQ::Api {

class PasswordHash;

class Account : public ThorQ::Api::ApiObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Account)
public:
    Account(ThorQ::Api::Client* apiClient);
    Account(ThorQ::Api::ApiObject* apiObject);

    QStringView email() const;
signals:
    void emailChanged(const QString& email);
public slots:
    void update() override;
    void setEmail(const QString& email);
private:
    ThorQ::Api::PasswordHash* m_passwordHash;
    QString m_email;
};

}

#endif // APIACCOUNT_H
