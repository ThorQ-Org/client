#ifndef THORQ_LOGINWINDOWCONTROLLER_H
#define THORQ_LOGINWINDOWCONTROLLER_H

#include <QObject>

namespace ThorQ::Controllers {

class LoginWindowController : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(LoginWindowController)
public:
    LoginWindowController(QObject* parent = nullptr);
};

}

#endif // THORQ_LOGINWINDOWCONTROLLER_H
