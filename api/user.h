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
    Q_DISABLE_COPY(User)
public:
    User(QString id, ThorQ::Api::Client* apiClient);
    User(QString id, ThorQ::Api::ApiObject* apiObject);

    QStringView id() const;
    QStringView username() const;
    ThorQ::Api::Image* profilePicture() const;
    int statusOnline() const;
    QString statusText() const;
signals:
    void usernameChanged(const QString& username);
    void statusOnlineChanged(int online);
    void statusTextChanged(const QString& text);
public slots:
    void update() override;
    void setUsername(const QString& username);
    void setStatusOnline(int online);
    void setStatusText(const QString& text);
protected:
    const QString m_id;
    QString m_username;
    ThorQ::Api::Image* m_profilePicture;
    int m_statusOnline;
    QString m_statusText;
};

}

#endif // APIUSER_H
