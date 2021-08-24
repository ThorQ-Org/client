#ifndef P2PCLIENT_H
#define P2PCLIENT_H

#include <QObject>

class P2PClient : public QObject
{
    Q_OBJECT
public:
    P2PClient(QObject* parent = nullptr);
};

#endif // P2PCLIENT_H
