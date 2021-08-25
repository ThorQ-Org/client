#ifndef APICLIENT_H
#define APICLIENT_H

#include "apiconsumer.h"

#include <QObject>
#include <QNetworkRequest>

class QNetworkAccessManager;
class QNetworkCookieJar;

namespace ThorQ::Api {

class Config;
class Account;

class Client : public QObject
{
    Q_OBJECT
public:
    Client(QObject* parent = nullptr);

    ThorQ::Api::Config* config() const;
    ThorQ::Api::Account* currentAccount() const;
    QNetworkReply* headRequest(const QUrl& endpoint, bool requiresAuth) const;
    QNetworkReply* getRequest(const QUrl& endpoint, bool requiresAuth) const;
    QNetworkReply* postRequest(const QUrl& endpoint, const QByteArray& data, bool requiresAuth) const;
    QNetworkReply* putRequest(const QUrl& endpoint, const QByteArray& data, bool requiresAuth) const;
    QNetworkReply* deleteRequest(const QUrl& endpoint, bool requiresAuth = false) const;
signals:
    void healthOkChanged(bool ok);
    void configChanged(ThorQ::Api::Config* config);
public slots:
    void getHealth();
    void getConfig();
private slots:
    void setHealthOk(bool ok);
private:
    QNetworkRequest createRequest(const QUrl& endpoint, bool requiresAuth) const;

    QUrl m_apiUrl;
    QUrl m_fileUrl;
    QNetworkAccessManager* m_networkAccessManager;

    ThorQ::Api::Config* m_config;
    ThorQ::Api::Account* m_account;
    bool m_healthOk;
};

}

#endif // APICLIENT_H
