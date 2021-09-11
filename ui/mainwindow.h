#ifndef THORQ_MAINWINDOW_H
#define THORQ_MAINWINDOW_H

#include <QWidget>
#include <QRect>

class QTimer;
class QVBoxLayout;
class QHBoxLayout;
class QGraphicsScene;

namespace ThorQ::UI {

class WindowGeometrySaver;

class MainWindow : public QWidget
{
	Q_OBJECT
    Q_DISABLE_COPY(MainWindow)
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;
private:
    QVBoxLayout* m_vlayout;
    QHBoxLayout* m_hlayout;
    ThorQ::UI::WindowGeometrySaver* m_geometrySaver;

    QGraphicsScene* m_userScene;
};

}

#endif // THORQ_MAINWINDOW_H