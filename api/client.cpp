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
}

ThorQ::Api::Config* ThorQ::Api::Client::config() const
{
    return m_config;
}

ThorQ::Api::Account* ThorQ::Api::Client::currentAccount() const
{
    return m_account;
}

QNetworkRequest ThorQ::Api::Client::createApiRequest(const QUrl& endpoint, bool requiresAuth) const
{
    QNetworkRequest req(m_apiUrl.resolved(endpoint));
    req.setRawHeader("Hardware-Id", QSysInfo::machineUniqueId());
    if (requiresAuth) {
        // TODO: implement me
    }
    return req;
}

QNetworkReply* ThorQ::Api::Client::requestHead(const QNetworkRequest& request) const
{
    return m_networkAccessManager->head(request);
}

QNetworkReply* ThorQ::Api::Client::requestGet(const QNetworkRequest& request) const
{
    return m_networkAccessManager->get(request);
}

QNetworkReply* ThorQ::Api::Client::requestPost(const QNetworkRequest& request, const QByteArray& data) const
{
    return m_networkAccessManager->post(request, data);
}

QNetworkReply* ThorQ::Api::Client::requestPut(const QNetworkRequest& request, const QByteArray& data) const
{
    return m_networkAccessManager->put(request, data);
}

QNetworkReply* ThorQ::Api::Client::requestDelete(const QNetworkRequest& request) const
{
    return m_networkAccessManager->deleteResource(request);
}


void ThorQ::Api::Client::getHealth()
{
    QNetworkReply* reply = requestGet(createApiRequest(QUrl("health"), false));
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
