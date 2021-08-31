#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QGraphicsScene;
class QVBoxLayout;
class QHBoxLayout;

namespace ThorQ::UI {

class MainWindow : public QWidget
{
	Q_OBJECT
    Q_DISABLE_COPY(MainWindow)
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;
private:
    QGraphicsScene* m_userScene;

	QVBoxLayout* m_vlayout;
	QHBoxLayout* m_hlayout;
};

}

#endif // MAINWINDOW_H
