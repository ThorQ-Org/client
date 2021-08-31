#ifndef REALTIMECLIENT_H
#define REALTIMECLIENT_H

#include <QObject>

class RealtimeClient : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(RealtimeClient)
public:
    RealtimeClient(QObject* parent = nullptr);
};

#endif // REALTIMECLIENT_H
