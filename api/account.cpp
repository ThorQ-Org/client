#include "account.h"

#include "client.h"
#include "passwordhash.h"

#include <QCryptographicHash>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

void ThorQ::Api::Account::login(const QString& username, const QString& password)
{
    ThorQ::Api::Client* client = ThorQ::Api::Client::Singleton();

    QUrlQuery paramsUrlQuery;
    paramsUrlQuery.addQueryItem("username", username);

    QUrl paramsUrl("auth/params");
    paramsUrl.setQuery(paramsUrlQuery);

    QNetworkReply* reply = client->requestGet(client->createApiRequest(paramsUrl));

    QObject::connect(reply, &QNetworkReply::finished, [client, reply, username, password](){
        QJsonParseError err;
        auto json = QJsonDocument::fromJson(reply->readAll(), &err).object();
        if (err.error != QJsonParseError::NoError) {
            qDebug().noquote() << "Failed to parse hashing parameters:" << err.errorString();
            return;
        }
        if (json["ok"].toBool(true) == false) {
            qDebug().noquote() << "Failed to fetch hashing parameters:" << json["msg"].toString();
            return;
        }

        ThorQ::Api::PasswordHash* passwordHash = new ThorQ::Api::PasswordHash(client);
        passwordHash->UpdateFromJson(json);

        passwordHash->generateHash(password);

        QObject::connect(passwordHash, &ThorQ::Api::PasswordHash::hashGenerated, [client, username](const QByteArray& hash){
            QNetworkRequest req = client->createApiRequest(QUrl("auth/login"));

            QJsonObject loginObj;
            loginObj["username"] = username;
            loginObj["password_hash"] = QString(hash.toHex());

            QNetworkReply* reply = client->requestPut(req, QJsonDocument(loginObj).toJson());
            QObject::connect(reply, &QNetworkReply::finished, [client, reply, username](){
                qDebug() << "Login reply:" << reply->readAll();
            });
        });
    });
}

void ThorQ::Api::Account::registerAccount(const QString& username, const QString& password, const QString& email)
{
    ThorQ::Api::Client* client = ThorQ::Api::Client::Singleton();

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(username.toUtf8());

    ThorQ::Api::PasswordHash* passwordHash = new ThorQ::Api::PasswordHash(client);
    passwordHash->setSalt(hash.result());
    passwordHash->setPerformance(ThorQ::Api::PasswordHash::PerformancePresets::Moderate);
    passwordHash->generateHash(password);

    QJsonObject registerObj;
    registerObj["username"] = username;
    registerObj["password"] = passwordHash->ToJsonFull();
    registerObj["email"] = email;

    QNetworkReply* reply = client->requestPost(client->createApiRequest(QUrl("account"), "application/json"), QJsonDocument(registerObj).toJson());

    QObject::connect(passwordHash, &ThorQ::Api::PasswordHash::hashGenerated, [client, reply, username](const QByteArray& hash){
        qDebug().noquote() << "Registration reply:" << reply->readAll();
    });
}

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
    QNetworkReply* reply = apiClient()->requestGet(apiClient()->createApiRequest(QUrl("account")));
    QObject::connect(reply, &QNetworkReply::finished, [this, reply](){
        QJsonParseError err;
        auto json = QJsonDocument::fromJson(reply->readAll(), &err).object();
        if (err.error != QJsonParseError::NoError) {
            qDebug().noquote() << "Failed to parse account:" << err.errorString();
            return;
        }
        if (json["ok"].toBool(true) == false) {
            qDebug().noquote() << "Failed to fetch account:" << json["msg"].toString();
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
