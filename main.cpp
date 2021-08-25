#include "stylesheets.h"
#include "collar/serial.h"
#include "api/client.h"
#include "ui/mainwidget.h"
#include "ui/loginwidget.h"
#include "ui/registerwidget.h"
#include "ui/recoverwidget.h"
#include "vr/openvroverlaycontroller.h"

#include <constants.h>

#include <QApplication>
#include <QCoreApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QStyle>
#include <QLabel>
#include <QIcon>
#include <QSpinBox>
#include <QInputDialog>
#include <QRegExp>
#include <QDebug>
#include <QDir>
#include <QTranslator>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPushButton>

#include <filesystem>

#define TEST_VR     0
#define TEST_COM    1
#define TEST_COLLAR 0
#define TEST_PWHASH 0

int main(int argc, char** argv)
{
    // TODO: customize GUI
    // TODO: finalize support for SteamVR
    // TODO: Fix SteamVR crashing on exit (has something to do with freeing up already free'd memory i think)
    QCoreApplication::addLibraryPath(QDir::currentPath().append("/libs/"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName(THORQ_APPLICATION_NAME);
    QCoreApplication::setApplicationVersion(QVersionNumber(ThorQ::VERSION_MAJOR, ThorQ::VERSION_MINOR, ThorQ::VERSION_PATCH).toString());
    QCoreApplication::setOrganizationName(THORQ_ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(THORQ_ORGANIZATION_DOMAIN);

    QApplication app(argc, argv);

#if TEST_VR
    if (!ThorQ::VR::Initialize()) {
        qDebug() << "Failed to init VR";
        return EXIT_FAILURE;
    }

    if (ThorQ::VR::IsManifestInstalled()) {
        ThorQ::VR::RemoveManifest();
    }

    if (!ThorQ::VR::InstallManifest()) {
        qDebug() << "Failed to install VR manifest files";
        return EXIT_FAILURE;
    }
#endif

    app.setStyleSheet(ThorQ::StyleSheets::tryGetStylesheet("main"));
    app.setDesktopFileName(THORQ_APPLICATION_NAME);
    app.setWindowIcon(QIcon(":/shockGrey.ico"));
    app.setQuitOnLastWindowClosed(true);

#if TEST_COM

    ThorQ::Api::Client* apiClient = new ThorQ::Api::Client(&app);
#if TEST_COLLAR

    CollarSerial* ser = new CollarSerial(&app);

    QObject::connect(ser, &CollarSerial::FoundCollar, [ser](){
        ser->SendBeep(1);
        QThread::sleep(1);
        ser->SendBeep(1);
        QThread::sleep(1);
        ser->SendBeep(1);
        QThread::sleep(1);
        ser->SendBeep(1);
        QThread::sleep(1);
        ser->SendBeep(1);
    });

    ser->FindCollar();
    QObject::connect(apiClient, &ThorQ::ApiClient::gotShock, ser, &CollarSerial::SendShock);
    QObject::connect(apiClient, &ThorQ::ApiClient::gotVibrate, ser, &CollarSerial::SendVibration);
    QObject::connect(apiClient, &ThorQ::ApiClient::gotBeep, ser, &CollarSerial::SendBeep);

#endif

    QWidget mainWindow;
    mainWindow.setWindowTitle(THORQ_APPLICATION_NAME);
    mainWindow.setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    mainWindow.setFixedSize(350, 450);
    mainWindow.setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    mainWindow.size(),
                    QApplication::primaryScreen()->availableGeometry()
                )
            );

    mainWindow.show();
#endif

#if TEST_VR
    QPixmap pix(":/uwu.png");
    QLabel lab;
    lab.setPixmap(pix);

    ThorQ::VR::OpenVROverlayController* ovr = new ThorQ::VR::OpenVROverlayController(&app);

    ovr->setWidget(&lab);
    QObject::connect(ovr, &ThorQ::VR::OpenVROverlayController::vrQuit, ovr, &QObject::deleteLater);
    QObject::connect(ovr, &ThorQ::VR::OpenVROverlayController::vrQuit, &app, &QApplication::quit);
#endif

    int retval = app.exec();

#if TEST_VR
    ThorQ::VR::Shutdown();
#endif

    return retval;
}
