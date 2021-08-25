#include "apiobject.h"

#include "client.h"

#include <QObject>

ThorQ::Api::ApiObject::ApiObject(ThorQ::Api::Client* apiClient)
    : QObject(apiClient)
    , m_apiClient(apiClient)
{
}

ThorQ::Api::ApiObject::ApiObject(ThorQ::Api::ApiObject* apiObject)
    : QObject(apiObject)
    , m_apiClient(apiObject->m_apiClient)
{
}

ThorQ::Api::Client* ThorQ::Api::ApiObject::apiClient() const
{
    return m_apiClient;
}
