#ifndef THORQ_PERSISTENTCOOKIEJAR_H
#define THORQ_PERSISTENTCOOKIEJAR_H

#include <QObject>
#include <QNetworkCookieJar>
#include <QMutex>

namespace ThorQ {

class PersistentCookieJar : public QNetworkCookieJar
{
    Q_OBJECT
    Q_DISABLE_COPY(PersistentCookieJar)
public:
    PersistentCookieJar(QObject* parent = nullptr);
    ~PersistentCookieJar();

    QList<QNetworkCookie> cookiesForUrl(const QUrl &url) const override;
    bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url) override;
private:
    void save();
    void load();

    mutable QMutex m_mtx;
};

}

#endif // THORQ_PERSISTENTCOOKIEJAR_H
