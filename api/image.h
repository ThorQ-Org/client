#ifndef IMAGE_H
#define IMAGE_H

#include "apiobject.h"

#include <QObject>
#include <QPixmap>

class QMvoie;

namespace ThorQ::Api {

class Image : public ThorQ::Api::ApiObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Image)
public:
    enum class ImageType {
        Png,
        Jpg,
        Gif
    };
    Q_ENUMS(ImageType)

    Image(ThorQ::Api::Client* apiClient);
    Image(ThorQ::Api::ApiObject* apiObject);

    QStringView id() const;
    ImageType type() const;
    QPixmap image() const;
    QMovie* animation() const;
signals:
    void idChanged(const QString& id);
    void typeChanged(ImageType type);
    void imageLoaded(const QPixmap& image);
    void animationLoaded(QMovie* animation);
public slots:
    void update() override;
    void setId(const QString& id);
private slots:
    void setType(ImageType type, bool forceUpdate = false);
    void loadData();
    void setDefault();
    void setNotFound();
private:
    QString m_id;
    ImageType m_type;

    QPixmap m_image;
    QMovie* m_animation;
};

}

#endif // IMAGE_H
