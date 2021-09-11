#include "user.h"

#include "client.h"
#include "image.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ThorQ::Api::User::User(QString id, ThorQ::Api::Client* apiClient)
    : ThorQ::Api::ApiObject(apiClient)
    , m_id(id)
    , m_username()
    , m_profilePicture(ThorQ::Api::Image::DefaultImage())
{
}
ThorQ::Api::User::User(QString id, ThorQ::Api::ApiObject* apiObject)
    : ThorQ::Api::ApiObject(apiObject)
    , m_id(id)
    , m_username()
    , m_profilePicture(ThorQ::Api::Image::DefaultImage())
{
}

void ThorQ::Api::User::update()
{
    QNetworkReply* reply = apiClient()->requestGet(apiClient()->createApiRequest(QUrl("user/" + m_id)));
    QObject::connect(reply, &QNetworkReply::finished, [this, reply](){
        QJsonParseError err;
        auto json = QJsonDocument::fromJson(reply->readAll(), &err).object();
        if (err.error != QJsonParseError::NoError) {
            return;
        }

        m_username = json["username"].toString();

        QString profilePictureId = json["pfp_id"].toString();

        if (m_profilePicture->id() != profilePictureId) {
            setProfilePicture(new ThorQ::Api::Image(profilePictureId, this)); // TODO: make a ApiImageManager where apiobjects can get images by id, for less memory usage and faster load times
        }
    });
}

QStringView ThorQ::Api::User::id() const
{
    return m_id;
}

QStringView ThorQ::Api::User::username() const
{
    return m_username;
}

ThorQ::Api::Image* ThorQ::Api::User::profilePicture() const
{
    return m_profilePicture;
}

int ThorQ::Api::User::statusOnline() const
{
    return m_statusOnline;
}

QString ThorQ::Api::User::statusText() const
{
    return m_statusText;
}

void ThorQ::Api::User::setUsername(const QString& username)
{
    if (m_username != username) {
        m_username = username;
        emit usernameChanged(username);
    }
}

void ThorQ::Api::User::setProfilePicture(ThorQ::Api::Image* profilePicture)
{
    if (m_profilePicture != profilePicture) {
        m_profilePicture = profilePicture;
        emit profilePictureChanged(profilePicture);
    }
}

void ThorQ::Api::User::setStatusOnline(int online)
{
    if (m_statusOnline != online) {
        m_statusOnline = online;
        emit statusOnlineChanged(online);
    }
}

void ThorQ::Api::User::setStatusText(const QString& text)
{
    if (m_statusText != text) {
        m_statusText = text;
        emit statusTextChanged(text);
    }
}
