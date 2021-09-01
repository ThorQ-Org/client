#include "image.h"

#include "client.h"
#include "constants.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmapCache>
#include <QMimeDatabase>
#include <QMovie>
#include <QBuffer>

ThorQ::Api::Image::Image(ThorQ::Api::Client* apiClient)
    : ThorQ::Api::ApiObject(apiClient)
    , m_id("default")
    , m_type(ThorQ::Api::Image::ImageType::Png)
    , m_image()
    , m_animation(new QMovie(this))
{
    QObject::connect(this, &ThorQ::Api::Image::idChanged, this, &ThorQ::Api::Image::update);
    QObject::connect(this, &ThorQ::Api::Image::typeChanged, this, &ThorQ::Api::Image::loadData);
    loadData();
}
ThorQ::Api::Image::Image(ThorQ::Api::ApiObject* apiObject)
    : ThorQ::Api::ApiObject(apiObject)
    , m_id("default")
    , m_type(ThorQ::Api::Image::ImageType::Png)
    , m_image()
    , m_animation(new QMovie(this))
{
    QObject::connect(this, &ThorQ::Api::Image::idChanged, this, &ThorQ::Api::Image::update);
    QObject::connect(this, &ThorQ::Api::Image::typeChanged, this, &ThorQ::Api::Image::loadData);
    loadData();
}

QStringView ThorQ::Api::Image::id() const
{
    return m_id;
}

ThorQ::Api::Image::ImageType ThorQ::Api::Image::type() const
{
    return m_type;
}

QPixmap ThorQ::Api::Image::image() const
{
    return m_image;
}

QMovie* ThorQ::Api::Image::animation() const
{
    return m_animation;
}

void ThorQ::Api::Image::update()
{
    if (QPixmapCache::find(m_id, &m_image)) {
        emit imageLoaded(m_image);
        return;
    }

    QNetworkReply* reply = apiClient()->requestGet(apiClient()->createApiRequest(QUrl("img/" + m_id)));
    QObject::connect(reply, &QNetworkReply::finished, [this, reply](){

        // Parse json string
        QJsonParseError err;
        auto json = QJsonDocument::fromJson(reply->readAll(), &err).object();
        if (err.error != QJsonParseError::NoError) {
            qDebug().noquote() << "Failed to parse image:" << err.errorString();
            setNotFound(); // TODO: errorImage
            return;
        }

        // If we got a error response, display it
        if (json["ok"].toBool(true) == false) {
            qDebug().noquote() << "Failed to fetch image:" << json["msg"].toString();
            setNotFound();
            return;
        }

        // Get json values
        QString mime = json["mime"].toString();

        // Parse mime type
        QMimeType imgMime = QMimeDatabase().mimeTypeForName(mime);
        if (mime.isEmpty() || !imgMime.isValid()) {
            qDebug().noquote() << "Failed to parse mime:" << mime;
            setNotFound(); // TODO: errorImage
            return;
        }

        // Get image type
        QString ext = imgMime.preferredSuffix();
        if (ext == "png") {
            setType(ThorQ::Api::Image::ImageType::Png, true);
        } else if (ext == "jpg") {
            setType(ThorQ::Api::Image::ImageType::Jpg, true);
        } else if (ext == "gif") {
            setType(ThorQ::Api::Image::ImageType::Gif, true);
        } else {
            qDebug().noquote() << "Invalid image extension:" << ext;
            setNotFound(); // TODO: errorImage
            return;
        }
    });
}

void ThorQ::Api::Image::setId(const QString& id)
{
    if (m_id != id) {
        m_id = id;
        emit idChanged(id);
    }
}

void ThorQ::Api::Image::setType(ThorQ::Api::Image::ImageType type, bool forceUpdate)
{
    if (m_type != type || forceUpdate) {
        m_type = type;
        emit typeChanged(type);
    }
}

void ThorQ::Api::Image::loadData()
{
    const char* ext;
    switch (m_type) {
    case ThorQ::Api::Image::ImageType::Png:
        ext = ".png";
        break;
    case ThorQ::Api::Image::ImageType::Jpg:
        ext = ".jpg";
        break;
    case ThorQ::Api::Image::ImageType::Gif:
        ext = ".gif";
        break;
    default:
        setDefault();
        return;
    }

    QNetworkReply* reply = apiClient()->requestGet(QNetworkRequest(QUrl(THORQ_SERVER_FILE_ENDPOINT + m_id + ext)));
    QObject::connect(reply, &QNetworkReply::finished, [this, reply](){
        QByteArray data = reply->readAll();

        // Load actual image data
        if (m_type == ThorQ::Api::Image::ImageType::Gif) {
            // Animation
            QBuffer buffer(&data); // TODO: THIS IS BAD, "data" WILL GO OUT OF SCOPE, AND "buffer" WILL BE MEMORY LEAKED
            buffer.open(QIODevice::ReadOnly);
            m_animation->setFormat("GIF");
            m_animation->setDevice(&buffer);
            m_animation->start();
        } else {
            // Image
            if (m_image.loadFromData(data)) {
                emit imageLoaded(m_image);
            } else {
                setNotFound(); // TODO: errorImage
            }
        }


    });
}

void ThorQ::Api::Image::setDefault()
{
    m_id = "default";
    setType(ThorQ::Api::Image::ImageType::Png, true);
}

void ThorQ::Api::Image::setNotFound()
{
    m_id = "not_found";
    setType(ThorQ::Api::Image::ImageType::Png, true);
}
