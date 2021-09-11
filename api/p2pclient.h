#ifndef THORQ_P2PCLIENT_H
#define THORQ_P2PCLIENT_H

#include <QObject>

class P2PClient : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(P2PClient)
public:
    P2PClient(QObject* parent = nullptr);
};

#endif // THORQ_P2PCLIENT_H
