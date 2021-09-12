#include "stylesheets.h"

#include "api/client.h"
#include "api/config.h"

#include "ui/windowcontroller.h"
#include "constants.h"

#include <QApplication>
#include <QCoreApplication>
#include <QIcon>
#include <QDir>

#include <QLabel>
#include "api/image.h"

int main(int argc, char** argv)
{
    QCoreApplication::addLibraryPath(QDir::currentPath().append("/libs/"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName(THORQ_APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(QVersionNumber(ThorQ::VERSION_MAJOR, ThorQ::VERSION_MINOR, ThorQ::VERSION_PATCH).toString());
    QCoreApplication::setOrganizationName(THORQ_ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(THORQ_ORGANIZATION_DOMAIN);

    QApplication app(argc, argv);
    app.setStyleSheet(ThorQ::StyleSheets::TryGetStylesheet("main"));
    app.setDesktopFileName(THORQ_APPLICATION_NAME);
    app.setWindowIcon(QIcon(":/shockGrey.ico"));
    app.setQuitOnLastWindowClosed(false);

    ThorQ::Api::Client::InitializeSingleton(&app);
    ThorQ::Api::Client* apiClient = ThorQ::Api::Client::Singleton();

    ThorQ::UI::WindowController* windowController = new ThorQ::UI::WindowController(apiClient);

    apiClient->getHealth();

    return app.exec();
}
