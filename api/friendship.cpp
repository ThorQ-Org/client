#include "friendship.h"

ThorQ::Api::Friendship::Friendship(QString accountId, ThorQ::Api::Client* apiClient)
    : ThorQ::Api::ApiObject(apiClient)
    , m_accountId(accountId)
    , m_nickname()
    , m_autoAccept(false)
    , m_notifyOnline(false)
{
}

ThorQ::Api::Friendship::Friendship(QString accountId, ThorQ::Api::ApiObject* apiObject)
    : ThorQ::Api::ApiObject(apiObject)
    , m_accountId(accountId)
    , m_nickname()
    , m_autoAccept(false)
    , m_notifyOnline(false)
{
}

void ThorQ::Api::Friendship::update()
{

}
