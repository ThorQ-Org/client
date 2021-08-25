#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>

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
    QNetworkRequest createApiRequest(const QUrl& endpoint, bool requiresAuth) const;
    QNetworkReply* requestHead(const QNetworkRequest& request) const;
    QNetworkReply* requestGet(const QNetworkRequest& request) const;
    QNetworkReply* requestPost(const QNetworkRequest& request, const QByteArray& dataurl) const;
    QNetworkReply* requestPut(const QNetworkRequest& request, const QByteArray& dataurl) const;
    QNetworkReply* requestDelete(const QNetworkRequest& request) const;
signals:
    void healthOkChanged(bool ok);
public slots:
    void getHealth();
private slots:
    void setHealthOk(bool ok);
private:

    QUrl m_apiUrl;
    QUrl m_fileUrl;
    QNetworkAccessManager* m_networkAccessManager;

    ThorQ::Api::Config* m_config;
    ThorQ::Api::Account* m_account;
    bool m_healthOk;
};

}

#endif // APICLIENT_H
