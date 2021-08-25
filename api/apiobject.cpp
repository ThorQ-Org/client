#include "apiobject.h"

#include "client.h"

#include <QObject>

ThorQ::Api::ApiObject::ApiObject(ThorQ::Api::Client* apiClient)
    : QObject(apiClient)
    , m_apiClient(apiClient)
{
}

ThorQ::Api::ApiObject::ApiObject(ThorQ::Api::ApiObject* apiObject)
    : QObject(apiObject)
    , m_apiClient(apiObject->m_apiClient)
{
}

ThorQ::Api::Config* ThorQ::Api::ApiObject::config() const
{
    return m_apiClient->config();
}

ThorQ::Api::Account* ThorQ::Api::ApiObject::currentAccount() const
{
    return m_apiClient->currentAccount();
}

QNetworkReply* ThorQ::Api::ApiObject::headRequest(const QUrl& endpoint, bool requiresAuth) const
{
    return m_apiClient->headRequest(endpoint, requiresAuth);
}

QNetworkReply* ThorQ::Api::ApiObject::getRequest(const QUrl& endpoint, bool requiresAuth) const
{
    return m_apiClient->getRequest(endpoint, requiresAuth);
}

QNetworkReply* ThorQ::Api::ApiObject::postRequest(const QUrl& endpoint, const QByteArray& data, bool requiresAuth) const
{
    return m_apiClient->postRequest(endpoint, data, requiresAuth);
}

QNetworkReply* ThorQ::Api::ApiObject::putRequest(const QUrl& endpoint, const QByteArray& data, bool requiresAuth) const
{
    return m_apiClient->putRequest(endpoint, data, requiresAuth);
}

QNetworkReply* ThorQ::Api::ApiObject::deleteRequest(const QUrl& endpoint, bool requiresAuth) const
{
    return m_apiClient->deleteRequest(endpoint, requiresAuth);
}
