#ifndef THORQ_SETTINGSWIDGET_H
#define THORQ_SETTINGSWIDGET_H

#include <QWidget>

class QSettings;

namespace ThorQ::UI {

class SettingsWidget : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(SettingsWidget)
public:
    SettingsWidget(QWidget* parent = nullptr);
    ~SettingsWidget();

public slots:
    void cleanup();
private:
    QSettings* m_settings;
};

}

#endif // THORQ_SETTINGSWIDGET_H
