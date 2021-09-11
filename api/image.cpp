#include "image.h"

#include "client.h"
#include "pathutils.h"
#include "constants.h"

#include <mutex>

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmapCache>
#include <QMimeDatabase>
#include <QMovie>
#include <QBuffer>
#include <QStandardPaths>
#include <QFile>
#include <QDir>

std::mutex l_defaultImage;
ThorQ::Api::Image* g_defaultImage = nullptr;
ThorQ::Api::Image* ThorQ::Api::Image::DefaultImage()
{
    std::scoped_lock l(l_defaultImage);

    if (g_defaultImage == nullptr) {
        g_defaultImage = new ThorQ::Api::Image("default", ThorQ::Api::Client::Singleton());
        g_defaultImage->update();
    }

    return g_defaultImage;
}

ThorQ::Api::Image::Image(const QString& id, ThorQ::Api::Client* apiClient)
    : ThorQ::Api::ApiObject(apiClient)
    , m_id(id)
    , m_resolution()
    , m_sizeBytes()
    , m_fileType()
    , m_uploadedAt()
{
}
ThorQ::Api::Image::Image(const QString& id, ThorQ::Api::ApiObject* apiObject)
    : ThorQ::Api::ApiObject(apiObject)
    , m_id(id)
    , m_resolution()
    , m_sizeBytes()
    , m_fileType()
    , m_uploadedAt()
{
}

QString ThorQ::Api::Image::id() const
{
    return m_id;
}

QSize ThorQ::Api::Image::resolution() const
{
    return m_resolution;
}

std::size_t ThorQ::Api::Image::sizeBytes() const
{
    return m_sizeBytes;
}

ThorQ::FileType ThorQ::Api::Image::fileType() const
{
    return m_fileType;
}

QDateTime ThorQ::Api::Image::uploadedAt() const
{
    return m_uploadedAt;
}

QString ThorQ::Api::Image::cacheLocation() const
{
    return m_cacheLocation;
}

void ThorQ::Api::Image::update()
{
    QNetworkReply* reply = apiClient()->requestGet(apiClient()->createApiRequest(QUrl("img/" + m_id)));
    QObject::connect(reply, &QNetworkReply::finished, [this, reply](){

        // Parse json string
        QJsonParseError err;
        auto json = QJsonDocument::fromJson(reply->readAll(), &err).object();
        if (err.error != QJsonParseError::NoError) {
            qDebug().noquote() << "Failed to parse image:" << err.errorString();
            return;
        }

        // If we got a error response, display it
        if (json["ok"].toBool(true) == false) {
            qDebug().noquote() << "Failed to fetch image:" << json["msg"].toString();
            return;
        }

        // Get json values
        QSize resolution(json["width"].toInt(), json["height"].toInt());
        int sizeBytes = json["bsize"].toInt();
        QString mime = json["mime"].toString();
        QDateTime uploadedAt = QDateTime::fromString(json["uplaoded_at"].toString(), Qt::ISODate);

        // Parse mime type
        QMimeType imgMime = QMimeDatabase().mimeTypeForName(mime);
        if (mime.isEmpty() || !imgMime.isValid()) {
            qDebug().noquote() << "Failed to parse mime:" << mime;
            return;
        }

        // Get image type
        QString ext = imgMime.preferredSuffix();
        ThorQ::FileType fileType;
        if (ext == "png") {
            fileType = ThorQ::FileType::Png;
        } else if (ext == "jpg") {
            fileType = ThorQ::FileType::Jpg;
        } else if (ext == "gif") {
            fileType = ThorQ::FileType::Gif;
        } else {
            qDebug().noquote() << "Invalid image file extension:" << ext << "from mime:" << mime;
            return;
        }

        setResolution(resolution);
        setSizeBytes(sizeBytes);
        setFileType(fileType);
        setUploadedAt(uploadedAt);
        setCacheLocation(ThorQ::PathUtils::GetConfigDirectory("images").filePath(m_id));
    });
}

void ThorQ::Api::Image::setResolution(QSize resolution)
{
    if (m_resolution != resolution) {
        m_resolution = resolution;
        emit resolutionChanged(resolution);
    }
}

void ThorQ::Api::Image::setSizeBytes(std::size_t sizeBytes)
{
    if (m_sizeBytes != sizeBytes) {
        m_sizeBytes = sizeBytes;
        emit sizeBytesChanged(sizeBytes);
    }
}

void ThorQ::Api::Image::setFileType(ThorQ::FileType fileType)
{
    if (m_fileType != fileType) {
        m_fileType = fileType;
        emit fileTypeChanged(fileType);
    }
}

void ThorQ::Api::Image::setUploadedAt(const QDateTime& uploadedAt)
{
    if (m_uploadedAt != uploadedAt) {
        m_uploadedAt = uploadedAt;
        emit uploadedAtChanged(uploadedAt);
    }
}

void ThorQ::Api::Image::setCacheLocation(const QString& cacheLocation)
{
    if (m_cacheLocation != cacheLocation) {
        m_cacheLocation = cacheLocation;
        emit cacheLocationChanged(cacheLocation);
    }
}
