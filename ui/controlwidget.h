#ifndef THORQ_CONTROLWIDGET_H
#define THORQ_CONTROLWIDGET_H

#include <QWidget>

namespace ThorQ::UI {

class ControlWidget;

/**
 * @brief The OverlayWidget class
 */
class OverlayWidget : public QWidget
{
	Q_OBJECT
    Q_DISABLE_COPY(OverlayWidget)
public:
    /**
     * @brief OverlayWidget
     * @param parent
     */
	OverlayWidget(QWidget *parent = nullptr);
	~OverlayWidget();
private:
};

}

#endif // THORQ_CONTROLWIDGET_H
