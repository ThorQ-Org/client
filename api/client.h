#ifndef THORQ_APICLIENT_H
#define THORQ_APICLIENT_H

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
    Q_DISABLE_COPY(Client)
    Client(QObject* parent = nullptr);
public:
    static ThorQ::Api::Client* InitializeSingleton(QObject* parent = nullptr);
    static ThorQ::Api::Client* Singleton();

    ThorQ::Api::Config* config() const;

    bool healthOk() const;
    ThorQ::Api::Account* currentAccount() const;

    QNetworkRequest createApiRequest(const QUrl& endpoint) const;
    QNetworkRequest createApiRequest(const QUrl& endpoint, const QString& contentType) const;
    QNetworkReply* requestHead(const QNetworkRequest& request) const;
    QNetworkReply* requestGet(const QNetworkRequest& request) const;
    QNetworkReply* requestPost(const QNetworkRequest& request, const QByteArray& dataurl) const;
    QNetworkReply* requestPut(const QNetworkRequest& request, const QByteArray& dataurl) const;
    QNetworkReply* requestDelete(const QNetworkRequest& request) const;
signals:
    void healthOkChanged(bool ok);
    void currentAccountChanged(bool ok);
public slots:
    void getHealth();
    void getAccount();
private slots:
    void setHealthOk(bool ok);
private:
    QUrl const m_apiUrl;
    QUrl const m_fileUrl;
    QNetworkAccessManager* const m_networkAccessManager;
    ThorQ::Api::Config* const m_config;

    ThorQ::Api::Account* m_account;
    bool m_healthOk;
};

}

#endif // THORQ_APICLIENT_H
