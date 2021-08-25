#include "image.h"

#include "client.h"

#include <QPixmapCache>

ThorQ::Api::Image::Image(ThorQ::Api::Client* apiClient)
    : ThorQ::Api::ApiConsumer(apiClient)
{
}
ThorQ::Api::Image::Image(ThorQ::Api::ApiConsumer* apiConsumer)
    : ThorQ::Api::ApiConsumer(apiConsumer)
{
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
