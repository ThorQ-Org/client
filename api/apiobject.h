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
    Q_DISABLE_COPY(ApiObject)
public:
    ApiObject(ThorQ::Api::Client* apiClient);
    ApiObject(ThorQ::Api::ApiObject* apiObject);
public slots:
    virtual void update() = 0;
protected:
    ThorQ::Api::Client* apiClient() const;
private:
    ThorQ::Api::Client* m_apiClient;
};

}

#endif // APICONSUMER_H
