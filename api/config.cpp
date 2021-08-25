#include "config.h"

#include "client.h"
#include "announcement.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ThorQ::Api::Config::Config(ThorQ::Api::Client* apiClient)
    : ThorQ::Api::ApiObject(apiClient)
{
}
ThorQ::Api::Config::Config(ThorQ::Api::ApiObject* apiObject)
    : ThorQ::Api::ApiObject(apiObject)
{
}

void ThorQ::Api::Config::update()
{
    QNetworkReply* reply = apiClient()->requestGet(apiClient()->createApiRequest(QUrl("config"), false));
    QObject::connect(reply, &QNetworkReply::finished, [this, reply](){
        QJsonParseError err;
        auto json = QJsonDocument::fromJson(reply->readAll(), &err).object();
        if (err.error != QJsonParseError::NoError) {
            return;
        }

        QJsonValueRef jsonTosRef = json["tos"];
        QJsonValueRef jsonAnnouncementsRef = json["announcements"];
        QJsonValueRef jsonWebsocketsRef = json["websockets"];
        QJsonValueRef jsonUploadsRef = json["uploads"];

        if (!jsonTosRef.isObject() || !jsonAnnouncementsRef.isArray() || !jsonWebsocketsRef.isObject() || !jsonUploadsRef.isObject()) {
            return;
        }

        QJsonObject jsonTos = jsonTosRef.toObject();
        QJsonArray jsonAnnouncements = jsonAnnouncementsRef.toArray();
        QJsonObject jsonWebsockets = jsonWebsocketsRef.toObject();
        QJsonObject jsonUploads = jsonUploadsRef.toObject();

        setTosVersion(jsonTos["version"].toInt());
        setTosMessage(jsonTos["content"].toString());

        foreach (const QJsonValue& jsonValue, jsonAnnouncements) {
            auto jsonAnnouncement = jsonValue.toObject();

            if (jsonAnnouncement.isEmpty()) {
                return;
            }

            auto announcement = new ThorQ::Api::Announcement(this);

            QJsonValueRef jsonTitleRef = jsonAnnouncement["title"];
            QJsonValueRef jsonContentRef = jsonAnnouncement["content"];
            QJsonValueRef jsonPublishedAtRef = jsonAnnouncement["published_at"];

            if (!jsonTitleRef.isString() || !jsonContentRef.isString() || !jsonPublishedAtRef.isString()) {
                return;
            }

            QDateTime publishedAt = QDateTime::fromString(jsonPublishedAtRef.toString(), Qt::ISODate);

            if (!publishedAt.isValid()) {
                return;
            }

            announcement->setTitle(jsonTitleRef.toString());
            announcement->setContent(jsonContentRef.toString());
            announcement->setPublishedAt(publishedAt);

            announcementAdd(announcement);
        }

        setContactEmail(json["email_contact"].toString());
        setDiscordInvite(json["discord_invite"].toString());
        setPeer2PeerDisabled(json["disable_p2p"].toBool());

        QString releaseVersion = json["latest_release"].toString();
        int suffixIdx = releaseVersion.length();

        setLatestReleaseVersion(QVersionNumber::fromString(releaseVersion, &suffixIdx));
        setLatestReleaseType(releaseVersion.right(releaseVersion.size() - suffixIdx - 1));

        setWebsocketRateMin(jsonWebsockets["rate_min"].toInt());
        setWebsocketRateMax(jsonWebsockets["rate_max"].toInt());

        setUploadSizeMax(jsonUploads["size_max"].toInt());

        int minRes = jsonUploads["res_min"].toInt();
        setUploadResolutionMin(QSize{minRes, minRes});

        int maxRes = jsonUploads["res_max"].toInt();
        setUploadResolutionMax(QSize{maxRes, maxRes});

        setImageUrlPrefix(QUrl(json["img_url_prefix"].toString()));
    });
}

int ThorQ::Api::Config::tosVersion() const
{
    return m_tosVersion;
}

QStringView ThorQ::Api::Config::tosMessage() const
{
    return m_tosMessage;
}

QList<ThorQ::Api::Announcement*> ThorQ::Api::Config::announcements() const
{
    return m_announcements;
}

QStringView ThorQ::Api::Config::contactEmail() const
{
    return m_contactEmail;
}

QStringView ThorQ::Api::Config::discordInvite() const
{
    return m_discordInvite;
}

bool ThorQ::Api::Config::peer2PeerDisabled() const
{
    return m_peer2PeerDisabled;
}

QVersionNumber ThorQ::Api::Config::latestReleaseVersion() const
{
    return m_latestReleaseVersion;
}

QStringView ThorQ::Api::Config::latestReleaseType() const
{
    return m_latestReleaseType;
}

int ThorQ::Api::Config::websocketRateMin() const
{
    return m_websocketRateMin;
}

int ThorQ::Api::Config::websocketRateMax() const
{
    return m_websocketRateMax;
}

int ThorQ::Api::Config::uploadSizeMax() const
{
    return m_uploadSizeMax;
}

QSize ThorQ::Api::Config::uploadResolutionMin() const
{
    return m_uploadResolutionMin;
}

QSize ThorQ::Api::Config::uploadResolutionMax() const
{
    return m_uploadResolutionMax;
}

QUrl ThorQ::Api::Config::imageUrlPrefix() const
{
    return m_imageUrlPrefix;
}

void ThorQ::Api::Config::setTosVersion(int version)
{
    if (m_tosVersion != version) {
        m_tosVersion = version;
        emit tosVersionChanged(version);
    }
}

void ThorQ::Api::Config::setTosMessage(const QString& message)
{
    if (m_tosMessage != message) {
        m_tosMessage = message;
        emit tosMessageChanged(message);
    }
}

void ThorQ::Api::Config::announcementAdd(ThorQ::Api::Announcement* announcement)
{
    for (int i = 0; i < m_announcements.length(); i++) {
        if (m_announcements[i] == announcement) {
            return;
        }

        if (m_announcements[i]->publishedAt() >= announcement->publishedAt()) {
            m_announcements.insert(i, announcement);
            emit announcementAdded(i, announcement);
        }
    }
}

void ThorQ::Api::Config::announcementRemove(ThorQ::Api::Announcement* announcement)
{
    int i = m_announcements.indexOf(announcement);
    if (i != -1) {
        m_announcements.removeAt(i);
        emit announcementRemoved(i, announcement);
    }
}

void ThorQ::Api::Config::nnouncementsClear()
{
    for (int i = 0; i < m_announcements.length(); i++) {
        ThorQ::Api::Announcement* announcement = m_announcements[i];
        m_announcements.removeAt(i);
        emit announcementRemoved(i, announcement);
    }
}

void ThorQ::Api::Config::setContactEmail(const QString& email)
{
    if (m_contactEmail != email) {
        m_contactEmail = email;
        emit contactEmailChanged(email);
    }
}

void ThorQ::Api::Config::setDiscordInvite(const QString& invite)
{
    if (m_discordInvite != invite) {
        m_discordInvite = invite;
        emit discordInviteChanged(invite);
    }
}

void ThorQ::Api::Config::setPeer2PeerDisabled(bool disabled)
{
    if (m_peer2PeerDisabled != disabled) {
        m_peer2PeerDisabled = disabled;
        emit peer2PeerDisabledChanged(disabled);
    }
}

void ThorQ::Api::Config::setLatestReleaseVersion(const QVersionNumber& version)
{
    if (m_latestReleaseVersion != version) {
        m_latestReleaseVersion = version;
        emit latestReleaseVersionChanged(version);
    }
}

void ThorQ::Api::Config::setLatestReleaseType(const QString& type)
{
    if (m_latestReleaseType != type) {
        m_latestReleaseType = type;
        emit latestReleaseTypeChanged(type);
    }
}

void ThorQ::Api::Config::setWebsocketRateMin(int messagesPerMinute)
{
    if (m_websocketRateMin != messagesPerMinute) {
        m_websocketRateMin = messagesPerMinute;
        emit websocketRateMinChanged(messagesPerMinute);
    }
}

void ThorQ::Api::Config::setWebsocketRateMax(int messagesPerMinute)
{
    if (m_websocketRateMax != messagesPerMinute) {
        m_websocketRateMax = messagesPerMinute;
        emit websocketRateMaxChanged(messagesPerMinute);
    }
}

void ThorQ::Api::Config::setUploadSizeMax(int sizeInBytes)
{
    if (m_uploadSizeMax != sizeInBytes) {
        m_uploadSizeMax = sizeInBytes;
        emit uploadSizeMaxChanged(sizeInBytes);
    }
}

void ThorQ::Api::Config::setUploadResolutionMin(QSize resolution)
{
    if (m_uploadResolutionMin != resolution) {
        m_uploadResolutionMin = resolution;
        emit uploadResolutionMinChanged(resolution);
    }
}

void ThorQ::Api::Config::setUploadResolutionMax(QSize resolution)
{
    if (m_uploadResolutionMax != resolution) {
        m_uploadResolutionMax = resolution;
        emit uploadResolutionMaxChanged(resolution);
    }
}

void ThorQ::Api::Config::setImageUrlPrefix(const QUrl& imageUrlPrefix)
{
    if (m_imageUrlPrefix != imageUrlPrefix) {
        m_imageUrlPrefix = imageUrlPrefix;
        emit imageUrlPrefixChanged(imageUrlPrefix);
    }
}
