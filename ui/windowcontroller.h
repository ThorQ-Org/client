#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H

#include <QObject>

namespace ThorQ::UI {

class LoginWindow;
class MainWindow;

class WindowController : public QObject
{
public:
    WindowController(QObject* parent = nullptr);
private:
    ThorQ::UI::LoginWindow* m_loginWindow;
    ThorQ::UI::MainWindow* m_mainWindow;
};

}

#endif // WINDOWCONTROLLER_H
