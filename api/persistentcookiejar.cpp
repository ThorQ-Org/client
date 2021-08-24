#include "persistentcookiejar.h"

#include <QNetworkCookie>
#include <QSettings>

ThorQ::PersistentCookieJar::PersistentCookieJar(QObject* parent)
    : QNetworkCookieJar(parent)
{
    load();
}

ThorQ::PersistentCookieJar::~PersistentCookieJar()
{
    save();
}

QList<QNetworkCookie> ThorQ::PersistentCookieJar::cookiesForUrl(const QUrl& url) const
{
    QMutexLocker lock(&m_mtx);
    return QNetworkCookieJar::cookiesForUrl(url);
}

bool ThorQ::PersistentCookieJar::setCookiesFromUrl(const QList<QNetworkCookie>& cookieList, const QUrl& url)
{
    QMutexLocker lock(&m_mtx);

    return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}

void ThorQ::PersistentCookieJar::save()
{
    QMutexLocker lock(&m_mtx);

    QList<QNetworkCookie> cookies = allCookies();

    QByteArray cookieData;
    foreach (QNetworkCookie cookie, cookies) {
        if (!cookie.isSessionCookie()) {
            cookieData.append(cookie.toRawForm());
            cookieData.append("\n");
        }
    }

    QSettings settings;
    settings.setValue("cookies", cookieData);
}

void ThorQ::PersistentCookieJar::load()
{
    QMutexLocker lock(&m_mtx);

    QSettings settings;
    QByteArray cookieData = settings.value("Cookies").toByteArray();
    setAllCookies(QNetworkCookie::parseCookies(cookieData));
}
