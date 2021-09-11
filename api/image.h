#ifndef THORQ_IMAGE_H
#define THORQ_IMAGE_H

#include "apiobject.h"
#include "filetype.h"

#include <QObject>
#include <QSize>
#include <QDateTime>

namespace ThorQ::Api {

class Image : public ThorQ::Api::ApiObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Image)
public:
    static ThorQ::Api::Image* DefaultImage();

    Image(const QString& id, ThorQ::Api::Client* apiClient);
    Image(const QString& id, ThorQ::Api::ApiObject* apiObject);

    QString id() const;
    QSize resolution() const;
    std::size_t sizeBytes() const;
    ThorQ::FileType fileType() const;
    QDateTime uploadedAt() const;
    QString cacheLocation() const;
signals:
    void resolutionChanged(QSize resolution);
    void sizeBytesChanged(std::size_t sizeBytes);
    void fileTypeChanged(ThorQ::FileType fileType);
    void uploadedAtChanged(const QDateTime& uploadedAt);
    void cacheLocationChanged(const QString& cacheLocation);
public slots:
    void update() override;
    void setResolution(QSize resolution);
    void setSizeBytes(std::size_t sizeBytes);
    void setFileType(ThorQ::FileType fileType);
    void setUploadedAt(const QDateTime& uploadedAt);
    void setCacheLocation(const QString& cacheLocation);
private:
    const QString m_id;
    QSize m_resolution;
    std::size_t m_sizeBytes;
    ThorQ::FileType m_fileType;
    QDateTime m_uploadedAt;
    QString m_cacheLocation;
};

}

#endif // THORQ_IMAGE_H
