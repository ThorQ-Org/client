#include "account.h"

#include "passwordhash.h"

#include <QJsonObject>
#include <QJsonValue>

ThorQ::Api::Account::Account(ThorQ::Api::User* apiUser)
    : ThorQ::Api::User(apiUser)
    , m_passwordHash(new ThorQ::Api::PasswordHash(this))
{
}
ThorQ::Api::Account::Account(ThorQ::Api::Client* apiClient)
    : ThorQ::Api::User(apiClient)
    , m_passwordHash(new ThorQ::Api::PasswordHash(this))
{
}
ThorQ::Api::Account::Account(ThorQ::Api::ApiConsumer* apiConsumer)
    : ThorQ::Api::User(apiConsumer)
    , m_passwordHash(new ThorQ::Api::PasswordHash(this))
{
}

bool ThorQ::Api::Account::UpdateFromJson(QJsonObject& json)
{
    QJsonValueRef jsonPassword = json["password"];
    QJsonValueRef jsonEmail = json["email"];

    if (!jsonPassword.isObject() || !jsonEmail.isString()) {
        return false;
    }

    m_passwordHash->UpdateFromJson(jsonPassword.toObject());
    setEmail(jsonEmail.toString());

    return true;
}

QStringView ThorQ::Api::Account::email() const
{
    return m_email;
}

void ThorQ::Api::Account::setEmail(const QString& email)
{
    if (m_email != email) {
        m_email = email;
        emit emailChanged(email);
    }
}
