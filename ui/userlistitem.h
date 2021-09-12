#ifndef THORQ_USERLISTITEM_H
#define THORQ_USERLISTITEM_H

#include <QGraphicsItem>

class QString;
class QGraphicsTextItem;
class QGraphicsPixmapItem;
class QGraphicsPixmapItem;
class QGraphicsRectItem;

namespace ThorQ::UI {

class UserListItem : public QGraphicsItem
{
    Q_DISABLE_COPY(UserListItem)
public:
	UserListItem(const QString& username, QGraphicsItem* parent = nullptr);
private:
	QGraphicsTextItem* m_usernameText;
	QGraphicsPixmapItem* m_collarIcon;
	QGraphicsPixmapItem* m_vrIcon;
	QGraphicsRectItem* m_requestButton;
	QGraphicsRectItem* m_acceptButton;
	QGraphicsRectItem* m_denyButton;
};

}

#endif // THORQ_USERLISTITEM_H
