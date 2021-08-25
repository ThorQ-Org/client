#include "account.h"

#include "passwordhash.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

ThorQ::Api::Account::Account(ThorQ::Api::Client* apiClient)
    : ThorQ::Api::ApiObject(apiClient)
    , m_passwordHash(new ThorQ::Api::PasswordHash(this))
{
}
ThorQ::Api::Account::Account(ThorQ::Api::ApiObject* apiObject)
    : ThorQ::Api::ApiObject(apiObject)
    , m_passwordHash(new ThorQ::Api::PasswordHash(this))
{
}

void ThorQ::Api::Account::update()
{
    QNetworkReply* reply = getRequest(QUrl("account"), false);
    QObject::connect(reply, &QNetworkReply::finished, [this, reply](){
        QJsonParseError err;
        auto json = QJsonDocument::fromJson(reply->readAll(), &err).object();
        if (err.error != QJsonParseError::NoError) {
            return;
        }

        QJsonValueRef jsonPassword = json["password"];
        QJsonValueRef jsonEmail = json["email"];

        if (!jsonPassword.isObject() || !jsonEmail.isString()) {
            return;
        }

        m_passwordHash->UpdateFromJson(jsonPassword.toObject());
        setEmail(jsonEmail.toString());
    });
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
