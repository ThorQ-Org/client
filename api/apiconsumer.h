#ifndef APICONSUMER_H
#define APICONSUMER_H

#include <QObject>
#include <QUrl>

class QNetworkReply;

namespace ThorQ::Api {

class Client;
class Config;
class Account;

class ApiConsumer : public QObject
{
    Q_OBJECT
public:
    ApiConsumer(ThorQ::Api::Client* apiClient);
    ApiConsumer(ThorQ::Api::ApiConsumer* apiConsumer);
protected:
    ThorQ::Api::Config* config() const;
    ThorQ::Api::Account* currentAccount() const;
    QNetworkReply* headRequest(const QUrl& endpoint, bool requiresAuth) const;
    QNetworkReply* getRequest(const QUrl& endpoint, bool requiresAuth) const;
    QNetworkReply* postRequest(const QUrl& endpoint, const QByteArray& data, bool requiresAuth) const;
    QNetworkReply* putRequest(const QUrl& endpoint, const QByteArray& data, bool requiresAuth) const;
    QNetworkReply* deleteRequest(const QUrl& endpoint, bool requiresAuth = false) const;
private:
    ThorQ::Api::Client* m_apiClient;
};

}

#endif // APICONSUMER_H
