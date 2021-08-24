#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QVersionNumber>
#include <QSize>
#include <QUrl>

namespace ThorQ::Api {

class Announcement;

class Config : public QObject
{
    Q_OBJECT
public:
    Config(QObject* parent = nullptr);

    bool UpdateFromJson(QJsonObject& json);

    int tosVersion() const;
    QStringView tosMessage() const;
    QList<ThorQ::Api::Announcement*> announcements() const;
    QStringView contactEmail() const;
    QStringView discordInvite() const;
    bool peer2PeerDisabled() const;
    QVersionNumber latestReleaseVersion() const;
    QStringView latestReleaseType() const;
    int websocketRateMin() const;
    int websocketRateMax() const;
    int uploadSizeMax() const;
    QSize uploadResolutionMin() const;
    QSize uploadResolutionMax() const;
    QUrl imageUrlPrefix() const;
signals:
    void tosVersionChanged(int version);
    void tosMessageChanged(const QString& message);
    void announcementsChanged(const QList<ThorQ::Api::Announcement*>& announcements);
    void contactEmailChanged(const QString& email);
    void discordInviteChanged(const QString& invite);
    void peer2PeerDisabledChanged(bool disabled);
    void latestReleaseVersionChanged(const QVersionNumber& version);
    void latestReleaseTypeChanged(const QString& type);
    void websocketRateMinChanged(int messagesPerMinute);
    void websocketRateMaxChanged(int messagesPerMinute);
    void uploadSizeMaxChanged(int sizeInBytes);
    void uploadResolutionMinChanged(QSize resolution);
    void uploadResolutionMaxChanged(QSize resolution);
    void imageUrlPrefixChanged(const QUrl& imageUrlPrefix);
public slots:
    void setTosVersion(int version);
    void setTosMessage(const QString& message);
    void setAnnouncements(const QList<ThorQ::Api::Announcement*>& announcements);
    void setContactEmail(const QString& email);
    void setDiscordInvite(const QString& invite);
    void setPeer2PeerDisabled(bool disabled);
    void setLatestReleaseVersion(const QVersionNumber& version);
    void setLatestReleaseType(const QString& type);
    void setWebsocketRateMin(int messagesPerMinute);
    void setWebsocketRateMax(int messagesPerMinute);
    void setUploadSizeMax(int sizeInBytes);
    void setUploadResolutionMin(QSize resolution);
    void setUploadResolutionMax(QSize resolution);
    void setImageUrlPrefix(const QUrl& imageUrlPrefix);
private:
    int m_tosVersion;
    QString m_tosMessage;
    QList<ThorQ::Api::Announcement*> m_announcements;
    QString m_contactEmail;
    QString m_discordInvite;
    bool m_peer2PeerDisabled;
    QVersionNumber m_latestReleaseVersion;
    QString m_latestReleaseType;
    int m_websocketRateMin;
    int m_websocketRateMax;
    int m_uploadSizeMax;
    QSize m_uploadResolutionMin;
    QSize m_uploadResolutionMax;
    QUrl m_imageUrlPrefix;
};

}

#endif // CONFIG_H
