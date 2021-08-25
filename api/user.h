#ifndef APIUSER_H
#define APIUSER_H

#include "apiconsumer.h"

#include <QObject>
#include <QStringView>
#include <QString>

namespace ThorQ::Api {

class Image;

class User : public ThorQ::Api::ApiConsumer
{
    Q_OBJECT
public:
    User(ThorQ::Api::Client* apiClient);
    User(ThorQ::Api::ApiConsumer* apiConsumer);

    bool UpdateFromJson(QJsonObject& json);

    QStringView id() const;
    QStringView username() const;
    ThorQ::Api::Image* profilePicture() const;
signals:
    void usernameChanged(const QString& username);
public slots:
    void setUsername(const QString& username);
private:
    QString m_id;
    QString m_username;
    ThorQ::Api::Image* m_profilePicture;
};

}

#endif // APIUSER_H
