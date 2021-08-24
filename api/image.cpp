#include "image.h"

ThorQ::Api::Image::Image(QObject* parent)
    : QObject(parent)
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
    }
}
