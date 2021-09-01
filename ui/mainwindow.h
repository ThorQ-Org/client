#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QRect>

class QTimer;
class QVBoxLayout;
class QHBoxLayout;
class QGraphicsScene;

namespace ThorQ::UI {

class MainWindow : public QWidget
{
	Q_OBJECT
    Q_DISABLE_COPY(MainWindow)
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;
private slots:
    void saveWindowState();
private:
    QTimer* m_settingsTimer;
    QVBoxLayout* m_vlayout;
    QHBoxLayout* m_hlayout;

    QGraphicsScene* m_userScene;
    QRect m_lastGeometry;
};

}

#endif // MAINWINDOW_H
