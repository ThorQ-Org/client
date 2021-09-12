#include "mainwindow.h"

#include "controllers/windowgeometrysaver.h"
#include "constants.h"

#include <QGraphicsScene>
#include <QCloseEvent>
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

void ThorQ::UI::MainWindow::closeEvent(QCloseEvent* event)
{
    if (event->spontaneous()) {
        emit windowClosed();
    }

    QWidget::closeEvent(event);
}
