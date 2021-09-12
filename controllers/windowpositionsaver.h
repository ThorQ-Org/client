#ifndef THORQ_WINDOWPOSITIONSAVER_H
#define THORQ_WINDOWPOSITIONSAVER_H

#include <QPoint>
#include <QObject>
#include <QSettings>

class QTimer;
class QWidget;
class QSettings;

namespace ThorQ::Controllers {

class WindowPositionSaver : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(WindowPositionSaver)
public:
    WindowPositionSaver(QString settingName, QSize fixedSize, QWidget* targetWidget);
private slots:
    void savePosition();
private:
    QPoint m_lastPos;
    QString m_settingName;
    QTimer* m_updateTimer;
    QWidget* m_targetWidget;
    QSettings* m_settings;
};

}

#endif // THORQ_WINDOWPOSITIONSAVER_H
