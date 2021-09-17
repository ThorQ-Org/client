#include "account.h"

#include "client.h"
#include "passwordhash.h"

#include <QCryptographicHash>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

ThorQ::Api::Account::Account(ThorQ::Api::Client* apiClient)
    : ThorQ::Api::ApiObject(apiClient)
    , m_passwordHash(new ThorQ::Api::PasswordHash(this))
    , m_email()
{
}
ThorQ::Api::Account::Account(ThorQ::Api::ApiObject* apiObject)
    : ThorQ::Api::ApiObject(apiObject)
    , m_passwordHash(new ThorQ::Api::PasswordHash(this))
    , m_email()
{
}

bool ThorQ::Api::Account::updateFromJson(const QJsonObject& json)
{

    QJsonValue jsonPassword = json["password"];
    QJsonValue jsonEmail = json["email"];

    if (!jsonPassword.isObject() || !jsonEmail.isString() ||
        !m_passwordHash->updateFromJson(jsonPassword.toObject())) {
        return false;
    }

    setEmail(jsonEmail.toString());

    return true;
}

void ThorQ::Api::Account::update()
{
    ThorQ::Api::Client* client = apiClient();

    QNetworkReply* reply = client->requestGet(client->createApiRequest(QUrl("account")));
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
        updateFromJson(json);
    });
}

void ThorQ::Api::Account::login(const QString& username, const QString& password)
{
    ThorQ::Api::Client* client = apiClient();

    QUrlQuery paramsUrlQuery;
    paramsUrlQuery.addQueryItem("username", username);

    QUrl paramsUrl("auth/params");
    paramsUrl.setQuery(paramsUrlQuery);

    QNetworkReply* reply = client->requestGet(client->createApiRequest(paramsUrl));

    QObject::connect(reply, &QNetworkReply::finished, [this, client, reply, username, password](){
        QJsonParseError err;
        auto json = QJsonDocument::fromJson(reply->readAll(), &err).object();
        if (err.error != QJsonParseError::NoError) {
            emit loginFailed("Failed to parse response: " + err.errorString());
            return;
        }
        if (json["ok"].toBool(true) == false) {
            emit loginFailed(json["msg"].toString());
            return;
        }

        ThorQ::Api::PasswordHash* passwordHash = new ThorQ::Api::PasswordHash(this);
        if (!passwordHash->updateFromJson(json)) {
            emit loginFailed("Failed to parse response");
            return;
        }

        passwordHash->generateHash(password);

        QObject::connect(passwordHash, &ThorQ::Api::PasswordHash::hashingDone, [this, client, username](const QByteArray& hash){
            if (hash.isEmpty()) {
                emit loginFailed("Password hashing failed!");
                return;
            }
            QNetworkRequest req = client->createApiRequest(QUrl("auth/login"));

            QJsonObject loginObj;
            loginObj["username"] = username;
            loginObj["password_hash"] = QString(hash.toHex());

            QNetworkReply* reply = client->requestPut(req, QJsonDocument(loginObj).toJson());
            QObject::connect(reply, &QNetworkReply::finished, [this, reply, username](){
                QJsonParseError err;
                auto json = QJsonDocument::fromJson(reply->readAll(), &err).object();
                if (err.error != QJsonParseError::NoError) {
                    emit loginFailed("Error: " + err.errorString());
                    return;
                }
                if (json["ok"].toBool(true) == false) {
                    emit loginFailed(json["msg"].toString());
                    return;
                }

                if (updateFromJson(json)) {
                    emit loginSuccess();
                } else {
                    emit loginFailed("Failed to parse response");
                }
            });
        });
        QObject::connect(passwordHash, &ThorQ::Api::PasswordHash::hashingDone, passwordHash, &QObject::deleteLater);
    });
}

void ThorQ::Api::Account::logout()
{
    ThorQ::Api::Client* client = apiClient();

    QNetworkReply* reply = client->requestGet(client->createApiRequest(QUrl("auth/logout")));

    QObject::connect(reply, &QNetworkReply::finished, [this, reply](){
        QJsonParseError err;
        auto json = QJsonDocument::fromJson(reply->readAll(), &err).object();
        if (err.error != QJsonParseError::NoError) {
            emit logoutFailed("Failed to parse response: " + err.errorString());
            return;
        }
        if (json["ok"].toBool(true) == false) {
            emit logoutFailed(json["msg"].toString());
            return;
        }

        ThorQ::Api::PasswordHash* passwordHash = new ThorQ::Api::PasswordHash(this);
        if (!passwordHash->updateFromJson(json)) {
            emit logoutFailed("Failed to parse response");
            return;
        }

        emit logoutSuccess();
    });
}

void ThorQ::Api::Account::registerAccount(const QString& username, const QString& password, const QString& email)
{
    ThorQ::Api::Client* client = apiClient();

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(username.toUtf8());

    ThorQ::Api::PasswordHash* passwordHash = new ThorQ::Api::PasswordHash(this);
    passwordHash->setSalt(hash.result());
    passwordHash->setPerformance(ThorQ::Api::PasswordHash::PerformancePresets::Moderate);
    passwordHash->generateHash(password);

    QObject::connect(passwordHash, &ThorQ::Api::PasswordHash::hashingDone, [this, client, username, passwordHash, email](const QByteArray& hash){
        if (hash.isEmpty()) {
            emit registerAccountFailed("Password hashing failed!");
            return;
        }

        QJsonObject registerObj;
        registerObj["username"] = username;
        registerObj["password"] = passwordHash->toJsonFull();
        registerObj["email"] = email;

        QNetworkReply* reply = client->requestPost(client->createApiRequest(QUrl("account"), "application/json"), QJsonDocument(registerObj).toJson());
        QObject::connect(reply, &QNetworkReply::finished, [this, reply, username](){
            QJsonParseError err;
            auto json = QJsonDocument::fromJson(reply->readAll(), &err).object();
            if (err.error != QJsonParseError::NoError) {
                emit registerAccountFailed("Failed to parse response: " + err.errorString());
                return;
            }
            if (json["ok"].toBool(true) == false) {
                emit registerAccountFailed(json["msg"].toString());
                return;
            }

            if (updateFromJson(json)) {
                emit registerAccountSuccess();
            } else {
                emit registerAccountFailed("Failed to parse response");
            }
        });
    });
    QObject::connect(passwordHash, &ThorQ::Api::PasswordHash::hashingDone, passwordHash, &QObject::deleteLater);
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
