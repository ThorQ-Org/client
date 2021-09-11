#ifndef THORQ_USERLISTWIDGET_H
#define THORQ_USERLISTWIDGET_H

#include <QWidget>

class QScrollArea;

namespace ThorQ::UI {

class UserListWidget : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(UserListWidget)
public:
    UserListWidget(QWidget* parent = nullptr);
private:
	QScrollArea* m_scrollArea;
};

}

#endif // THORQ_USERLISTWIDGET_H
