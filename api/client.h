#ifndef APICLIENT_H
#define APICLIENT_H

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
signals:
    void healthOkChanged(bool ok);
    void configChanged(ThorQ::Api::Config* config);
public slots:
    void getHealth();
    void getConfig();
private slots:
    void setHealthOk(bool ok);
private:
    QNetworkRequest CreateRequest(const QString& endpoint);

    QNetworkAccessManager* m_networkAccessManager;
    QUrl m_apiUrl;

    bool m_healthOk;
    ThorQ::Api::Config* m_config;
    ThorQ::Api::Account* m_account;
};

}

#endif // APICLIENT_H
