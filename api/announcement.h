#ifndef THORQ_ANNOUNCEMENT_H
#define THORQ_ANNOUNCEMENT_H

#include "apiobject.h"

#include <QObject>
#include <QDateTime>
#include <QString>

namespace ThorQ::Api {

class Announcement : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Announcement)
public:
    Announcement(QObject* parent = nullptr);

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

#endif // THORQ_ANNOUNCEMENT_H
