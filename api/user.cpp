#include "user.h"

#include "image.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ThorQ::Api::User::User(QObject* parent)
    : QObject(parent)
    , m_id()
    , m_username()
    , m_profilePicture(new ThorQ::Api::Image(this))
{
}

bool ThorQ::Api::User::UpdateFromJson(QJsonObject& json)
{
    QJsonValueRef jsonIdRef = json["id"];
    QJsonValueRef jsonUsernameRef = json["username"];
    QJsonValueRef jsonProfilePictureIdRef = json["pfp_id"];

    if (!jsonIdRef.isString() || !jsonUsernameRef.isString() || !jsonProfilePictureIdRef.isString()) {
        return false;
    }

    m_id = jsonIdRef.toString();
    m_username = jsonUsernameRef.toString();
    m_profilePicture->setImageId(jsonProfilePictureIdRef.toString());

    return true;
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

void ThorQ::Api::User::setUsername(const QString& username)
{
    if (m_username != username) {
        m_username = username;
        emit usernameChanged(username);
    }
}
