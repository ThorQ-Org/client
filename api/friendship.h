#ifndef FRIENDSHIP_H
#define FRIENDSHIP_H

#include "apiobject.h"

namespace ThorQ::Api {

class Friendship : public ThorQ::Api::ApiObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Friendship)
public:
    Friendship(QString accountId, ThorQ::Api::Client* apiClient);
    Friendship(QString accountId, ThorQ::Api::ApiObject* apiObject);
public slots:
    void update() override;
private:
    const QString m_accountId;
    QString m_nickname;
    bool m_autoAccept;
    bool m_notifyOnline;
};

}

#endif // FRIENDSHIP_H
