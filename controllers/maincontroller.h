#ifndef THORQ_MAINCONTROLLER_H
#define THORQ_MAINCONTROLLER_H

#include <QObject>

namespace ThorQ {

namespace Api {
class Client;
}

namespace UI {
class LoginWindow;
class MainWindow;
}

namespace Controllers {

class MainController : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(MainController)
public:
    MainController(QObject* parent = nullptr);
    ~MainController();
private slots:
    void handleWindowClose();
private:
    ThorQ::Api::Client* m_apiClient;
    ThorQ::UI::LoginWindow* m_loginWindow;
    ThorQ::UI::MainWindow* m_mainWindow;
};

}
}

#endif // THORQ_MAINCONTROLLER_H
