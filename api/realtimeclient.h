#ifndef THORQ_REALTIMECLIENT_H
#define THORQ_REALTIMECLIENT_H

#include <QObject>

class RealtimeClient : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(RealtimeClient)
public:
    RealtimeClient(QObject* parent = nullptr);
};

#endif // THORQ_REALTIMECLIENT_H
