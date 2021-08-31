#ifndef IMAGE_H
#define IMAGE_H

#include "apiobject.h"

#include <QObject>
#include <QPixmap>

namespace ThorQ::Api {

class Image : public ThorQ::Api::ApiObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Image)
public:
    Image(ThorQ::Api::Client* apiClient);
    Image(ThorQ::Api::ApiObject* apiObject);

    QStringView id() const;
signals:
    void idChanged(const QString& id);
    void imageLoaded();
public slots:
    void update() override;
    void setImageId(const QString& id);
private:
    QString m_id;
    QPixmap m_data;
};

}

#endif // IMAGE_H
