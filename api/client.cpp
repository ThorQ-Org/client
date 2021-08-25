#include "client.h"

#include "config.h"
#include "account.h"

#include "persistentcookiejar.h"
#include "constants.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>


ThorQ::Api::Client::Client(QObject* parent)
    : QObject(parent)
    , m_apiUrl(THORQ_SERVER_API_ENDPOINT)
    , m_networkAccessManager(new QNetworkAccessManager(this))
    , m_config(new ThorQ::Api::Config(this))
    , m_account(new ThorQ::Api::Account(this))
{
    m_networkAccessManager->setCookieJar(new ThorQ::PersistentCookieJar(m_networkAccessManager));
    getHealth();
    m_config->update();
}

ThorQ::Api::Config* ThorQ::Api::Client::config() const
{
    return m_config;
}

ThorQ::Api::Account* ThorQ::Api::Client::currentAccount() const
{
    return m_account;
}

QNetworkReply* ThorQ::Api::Client::headRequest(const QUrl& endpoint, bool requiresAuth) const
{
    return m_networkAccessManager->head(createRequest(endpoint, requiresAuth));
}

QNetworkReply* ThorQ::Api::Client::getRequest(const QUrl& endpoint, bool requiresAuth) const
{
    return m_networkAccessManager->get(createRequest(endpoint, requiresAuth));
}

QNetworkReply* ThorQ::Api::Client::postRequest(const QUrl& endpoint, const QByteArray& data, bool requiresAuth) const
{
    return m_networkAccessManager->post(createRequest(endpoint, requiresAuth), data);
}

QNetworkReply* ThorQ::Api::Client::putRequest(const QUrl& endpoint, const QByteArray& data, bool requiresAuth) const
{
    return m_networkAccessManager->put(createRequest(endpoint, requiresAuth), data);
}

QNetworkReply* ThorQ::Api::Client::deleteRequest(const QUrl& endpoint, bool requiresAuth) const
{
    return m_networkAccessManager->deleteResource(createRequest(endpoint, requiresAuth));
}


void ThorQ::Api::Client::getHealth()
{

    QNetworkReply* reply = getRequest(QUrl("health"), false);
    QObject::connect(reply, &QNetworkReply::finished, [reply, this](){
        QJsonParseError err;
        auto doc = QJsonDocument::fromJson(reply->readAll(), &err);
        if (err.error == QJsonParseError::NoError) {
            setHealthOk(doc.object()["ok"].toBool(false));
        }
    });
}

void ThorQ::Api::Client::setHealthOk(bool ok)
{
    if (m_healthOk != ok) {
        m_healthOk = ok;
        emit healthOkChanged(ok);
    }
}

QNetworkRequest ThorQ::Api::Client::createRequest(const QUrl& endpoint, bool requiresAuth) const
{
    QNetworkRequest req(m_apiUrl.resolved(endpoint));
    req.setRawHeader("Hardware-Id", QSysInfo::machineUniqueId());
    if (requiresAuth) {
        // TODO: implement me
    }
    return req;
}
