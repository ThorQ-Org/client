#include "mainwindow.h"

#include "controllers/windowgeometrysaver.h"
#include "constants.h"

#include <QApplication>
#include <QSettings>
#include <QScreen>
#include <QStyle>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

ThorQ::UI::MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_vlayout(new QVBoxLayout(this))
    , m_hlayout(new QHBoxLayout())
    , m_geometrySaver(new ThorQ::Controllers::WindowGeometrySaver("WindowMainGeometry", this))
    , m_userScene(new QGraphicsScene(this))
{
    // Set window title and geometry
    setWindowTitle(THORQ_APPLICATION_NAME);

    // Set up layouts
    m_vlayout->addLayout(m_hlayout);
}
