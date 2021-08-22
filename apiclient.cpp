#include "apiclient.h"

#include <QNetworkRequest>
#include <QNetworkReply>

#include "constants.h"

QNetworkRequest ApiClient::CreateRequest(const QString& endpoint)
{
    QNetworkRequest req(m_apiUrl.resolved(QUrl(endpoint)));
    req.setRawHeader("Hardware-Id", QSysInfo::machineUniqueId());
    return req;
}


ApiClient::ApiClient(QObject* parent)
    : QObject(parent)
    , m_networkAccessManager(new QNetworkAccessManager(this))
    , m_apiUrl(THORQ_SERVER_HOSTNAME)
{
    QNetworkReply* reply = m_networkAccessManager->get(CreateRequest("config"));
    connect(reply, &QNetworkReply::finished, [reply](){
        qDebug() << reply->readAll();
    });
}
