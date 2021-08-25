#include "announcement.h"

#include <QJsonObject>
#include <QJsonValueRef>

ThorQ::Api::Announcement::Announcement(ThorQ::Api::Client* apiClient)
    : ThorQ::Api::ApiConsumer(apiClient)
{
}
ThorQ::Api::Announcement::Announcement(ThorQ::Api::ApiConsumer* apiConsumer)
    : ThorQ::Api::ApiConsumer(apiConsumer)
{
}

bool ThorQ::Api::Announcement::UpdateFromJson(QJsonObject& json)
{
    QJsonValueRef jsonTitleRef = json["title"];
    QJsonValueRef jsonContentRef = json["content"];
    QJsonValueRef jsonPublishedAtRef = json["published_at"];

    if (!jsonTitleRef.isString() || !jsonContentRef.isString() || !jsonPublishedAtRef.isString()) {
        return false;
    }

    QDateTime publishedAt = QDateTime::fromString(jsonPublishedAtRef.toString(), Qt::ISODate);

    if (!publishedAt.isValid()) {
        return false;
    }

    setTitle(jsonTitleRef.toString());
    setContent(jsonContentRef.toString());
    setPublishedAt(publishedAt);

    return true;
}

QStringView ThorQ::Api::Announcement::title() const
{
    return m_title;
}

QStringView ThorQ::Api::Announcement::content() const
{
    return m_content;
}

QDateTime ThorQ::Api::Announcement::publishedAt() const
{
    return m_publishedAt;
}

void ThorQ::Api::Announcement::setTitle(const QString& title)
{
    if (m_title != title) {
        m_title = title;
        emit titleChanged(title);
    }
}

void ThorQ::Api::Announcement::setContent(const QString& content)
{
    if (m_content != content) {
        m_content = content;
        emit contentChanged(content);
    }
}

void ThorQ::Api::Announcement::setPublishedAt(const QDateTime& publishedAt)
{
    if (m_publishedAt != publishedAt) {
        m_publishedAt = publishedAt;
        emit publishedAtChanged(publishedAt);
    }
}
