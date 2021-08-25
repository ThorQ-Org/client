#ifndef APICONSUMER_H
#define APICONSUMER_H

#include <QObject>

class QUrl;
class QJsonObject;
class QNetworkReply;

namespace ThorQ::Api {

class Client;
class Config;
class Account;

class ApiObject : public QObject
{
    Q_OBJECT
public:
    ApiObject(ThorQ::Api::Client* apiClient);
    ApiObject(ThorQ::Api::ApiObject* apiObject);

    virtual void update() = 0;
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
