#ifndef APIUSER_H
#define APIUSER_H

#include "apiobject.h"

#include <QObject>
#include <QStringView>
#include <QString>

namespace ThorQ::Api {

class Image;

class User : public ThorQ::Api::ApiObject
{
    Q_OBJECT
public:
    User(QString id, ThorQ::Api::Client* apiClient);
    User(QString id, ThorQ::Api::ApiObject* apiObject);

    void update() override;

    QStringView id() const;
    QStringView username() const;
    ThorQ::Api::Image* profilePicture() const;
signals:
    void usernameChanged(const QString& username);
public slots:
    void setUsername(const QString& username);
private:
    const QString m_id;
    QString m_username;
    ThorQ::Api::Image* m_profilePicture;
};

}

#endif // APIUSER_H
