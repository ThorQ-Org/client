#ifndef THORQ_WINDOWGEOMETRYSAVER_H
#define THORQ_WINDOWGEOMETRYSAVER_H

#include <QRect>
#include <QObject>
#include <QSettings>

class QTimer;
class QWidget;
class QSettings;

namespace ThorQ::Controllers {

class WindowGeometrySaver : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(WindowGeometrySaver)
public:
    WindowGeometrySaver(QString settingName, QWidget* targetWidget);
private slots:
    void saveGeometry();
private:
    QRect m_lastRect;
    QString m_settingName;
    QTimer* m_updateTimer;
    QWidget* m_targetWidget;
    QSettings* m_settings;
};

}

#endif // THORQ_WINDOWGEOMETRYSAVER_H
