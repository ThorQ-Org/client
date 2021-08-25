#include "image.h"

#include "client.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmapCache>

ThorQ::Api::Image::Image(ThorQ::Api::Client* apiClient)
    : ThorQ::Api::ApiObject(apiClient)
    , m_id("000000000000000000000000")
    , m_data()
{
}
ThorQ::Api::Image::Image(ThorQ::Api::ApiObject* apiObject)
    : ThorQ::Api::ApiObject(apiObject)
    , m_id("000000000000000000000000")
    , m_data()
{
}

void ThorQ::Api::Image::update()
{
    QNetworkReply* reply = getRequest(QUrl("img/" + m_id), false);
    QObject::connect(reply, &QNetworkReply::finished, [this, reply](){
        QJsonParseError err;
        auto json = QJsonDocument::fromJson(reply->readAll(), &err).object();
        if (err.error != QJsonParseError::NoError) {
            return;
        }

        // TODO: implement me?
    });
}

QStringView ThorQ::Api::Image::id() const
{
    return m_id;
}

void ThorQ::Api::Image::setImageId(const QString& id)
{
    if (m_id != id) {
        m_id = id;
        emit idChanged(id);

        if (QPixmapCache::find(id, &m_data)) {
            emit imageLoaded();
        } else {
            config();
        }
    }
}
