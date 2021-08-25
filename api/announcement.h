#ifndef ANNOUNCEMENT_H
#define ANNOUNCEMENT_H

#include "apiconsumer.h"

#include <QObject>
#include <QDateTime>
#include <QString>

namespace ThorQ::Api {

class Announcement : public ThorQ::Api::ApiConsumer
{
    Q_OBJECT
public:
    Announcement(ThorQ::Api::Client* apiClient);
    Announcement(ThorQ::Api::ApiConsumer* apiConsumer);

    bool UpdateFromJson(QJsonObject& json);

    QStringView title() const;
    QStringView content() const;
    QDateTime publishedAt() const;
signals:
    void titleChanged(const QString& title);
    void contentChanged(const QString& content);
    void publishedAtChanged(const QDateTime& publishedAt);
public slots:
    void setTitle(const QString& title);
    void setContent(const QString& content);
    void setPublishedAt(const QDateTime& publishedAt);
private:
    QDateTime m_publishedAt;
    QString m_title;
    QString m_content;
};

}

#endif // ANNOUNCEMENT_H
