#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QPixmap>

namespace ThorQ::Api {

class Image : public QObject
{
    Q_OBJECT
public:
    Image(QObject* parent = nullptr);

    QStringView id() const;
signals:
    void idChanged(const QString& id);
    void imageLoaded();
public slots:
    void setImageId(const QString& id);
private:
    QString m_id;
    QPixmap m_data;
};

}

#endif // IMAGE_H
