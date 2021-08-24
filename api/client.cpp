#include "client.h"

#include "config.h"

#include "persistentcookiejar.h"
#include "constants.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

QNetworkRequest ThorQ::Api::Client::CreateRequest(const QString& endpoint)
{
    QNetworkRequest req(m_apiUrl.resolved(QUrl(endpoint)));
    req.setRawHeader("Hardware-Id", QSysInfo::machineUniqueId());
    return req;
}


ThorQ::Api::Client::Client(QObject* parent)
    : QObject(parent)
    , m_networkAccessManager(new QNetworkAccessManager(this))
    , m_apiUrl(THORQ_SERVER_HOSTNAME)
    , m_config(new ThorQ::Api::Config(this))
{
    m_networkAccessManager->setCookieJar(new ThorQ::PersistentCookieJar(m_networkAccessManager));
    getHealth();
    getConfig();
}

ThorQ::Api::Config* ThorQ::Api::Client::config() const
{
    return m_config;
}

ThorQ::Api::Account* ThorQ::Api::Client::currentAccount() const
{
    return m_account;
}

void ThorQ::Api::Client::getHealth()
{
    QNetworkReply* reply = m_networkAccessManager->get(CreateRequest("health"));
    QObject::connect(reply, &QNetworkReply::finished, [reply, this](){
        QJsonParseError err;
        auto doc = QJsonDocument::fromJson(reply->readAll(), &err);
        if (err.error == QJsonParseError::NoError) {
            setHealthOk(doc.object()["ok"].toBool(false));
        }
    });
}

void ThorQ::Api::Client::getConfig()
{
    QNetworkReply* reply = m_networkAccessManager->get(CreateRequest("config"));
    QObject::connect(reply, &QNetworkReply::finished, [reply, this](){
        QJsonParseError err;
        auto doc = QJsonDocument::fromJson(reply->readAll(), &err).object();
        if (err.error == QJsonParseError::NoError) {
            m_config->UpdateFromJson(doc);
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
