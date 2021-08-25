#include "apiconsumer.h"

#include "client.h"

#include <QObject>

ThorQ::Api::ApiConsumer::ApiConsumer(ThorQ::Api::Client* apiClient)
    : QObject(apiClient)
    , m_apiClient(apiClient)
{
}

ThorQ::Api::ApiConsumer::ApiConsumer(ThorQ::Api::ApiConsumer* apiConsumer)
    : QObject(apiConsumer)
    , m_apiClient(apiConsumer->m_apiClient)
{
}

ThorQ::Api::Config* ThorQ::Api::ApiConsumer::config() const
{
    return m_apiClient->config();
}

ThorQ::Api::Account* ThorQ::Api::ApiConsumer::currentAccount() const
{
    return m_apiClient->currentAccount();
}

QNetworkReply* ThorQ::Api::ApiConsumer::headRequest(const QUrl& endpoint, bool requiresAuth) const
{
    return m_apiClient->headRequest(endpoint, requiresAuth);
}

QNetworkReply* ThorQ::Api::ApiConsumer::getRequest(const QUrl& endpoint, bool requiresAuth) const
{
    return m_apiClient->getRequest(endpoint, requiresAuth);
}

QNetworkReply* ThorQ::Api::ApiConsumer::postRequest(const QUrl& endpoint, const QByteArray& data, bool requiresAuth) const
{
    return m_apiClient->postRequest(endpoint, data, requiresAuth);
}

QNetworkReply* ThorQ::Api::ApiConsumer::putRequest(const QUrl& endpoint, const QByteArray& data, bool requiresAuth) const
{
    return m_apiClient->putRequest(endpoint, data, requiresAuth);
}

QNetworkReply* ThorQ::Api::ApiConsumer::deleteRequest(const QUrl& endpoint, bool requiresAuth) const
{
    return m_apiClient->deleteRequest(endpoint, requiresAuth);
}
