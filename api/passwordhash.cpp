#include "passwordhash.h"

#include <QThread>
#include <QJsonObject>

#define SODIUM_STATIC
#include <sodium.h>

static_assert (ThorQ::Api::PasswordHash::SaltLength == crypto_pwhash_SALTBYTES, "PasswordHashing salt length changed");
static_assert (ThorQ::Api::PasswordHash::HashLength == crypto_box_SEEDBYTES, "PasswordHashing hash length changed");

ThorQ::Api::PasswordHash::PasswordHash(QObject* parent)
    : QObject(parent)
    , m_salt()
    , m_hash()
    , m_opsLimit()
    , m_memLimit()
    , m_algorithm()
{
    m_salt.resize(ThorQ::Api::PasswordHash::SaltLength);
    m_hash.resize(ThorQ::Api::PasswordHash::HashLength);
}

ThorQ::Api::PasswordHash::~PasswordHash()
{
}

bool ThorQ::Api::PasswordHash::updateFromJson(const QJsonObject& json)
{
    QJsonValue saltValue = json["salt"];
    QJsonValue opsLimit = json["cpu_limit"];
    QJsonValue memLimit = json["ram_limit"];
    QJsonValue algorithm = json["algorithm"];

    if (!saltValue.isString() || !opsLimit.isDouble() || !memLimit.isDouble() || !algorithm.isDouble()) {
        return false;
    }

    setSalt(saltValue.toString().toUtf8());
    setOpsLimit(opsLimit.toInt()); // TODO: possible bad, do manual conversion to string from uint64
    setMemLimit(memLimit.toInt()); // TODO: possible bad, do manual conversion to string from uint64
    setAlgorithm(algorithm.toInt());

    return true;
}

QJsonObject ThorQ::Api::PasswordHash::toJsonParams() const
{
    QJsonObject json;

    json["salt"] = QString(m_salt.toHex());
    json["cpu_limit"] = (int)m_opsLimit; // TODO: possible bad, do manual conversion to string from uint64
    json["ram_limit"] = (int)m_memLimit; // TODO: possible bad, do manual conversion to string from uint64
    json["algorithm"] = (int)m_algorithm;

    return json;
}

QJsonObject ThorQ::Api::PasswordHash::toJsonFull() const
{
    QJsonObject json = toJsonParams();

    json["hash"] = QString(m_hash.toHex());

    return json;
}

QByteArray ThorQ::Api::PasswordHash::salt() const
{
    return m_salt;
}

QByteArray ThorQ::Api::PasswordHash::hash() const
{
    return m_hash;
}

quint64 ThorQ::Api::PasswordHash::opsLimit() const
{
    return m_opsLimit;
}

quint64 ThorQ::Api::PasswordHash::memLimit() const
{
    return m_memLimit;
}

qint32 ThorQ::Api::PasswordHash::algorithm() const
{
    return m_algorithm;
}

void ThorQ::Api::PasswordHash::generateSalt()
{
    randombytes_buf(m_salt.data(), m_salt.size());
    emit saltChanged(m_salt);
}

void ThorQ::Api::PasswordHash::setSalt(const QByteArray& salt)
{
    if (m_salt != salt) {
        m_salt = salt;
        emit saltChanged(salt);
    }
}

void ThorQ::Api::PasswordHash::generateHash(const QString& password)
{
    ThorQ::Api::Internal::HashingThread* worker = new ThorQ::Api::Internal::HashingThread(password.toUtf8(), m_salt, m_opsLimit, m_memLimit, m_algorithm, this);
    connect(worker, &ThorQ::Api::Internal::HashingThread::hashDone, this, &ThorQ::Api::PasswordHash::handleWorkerDone);
    connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    worker->start();
}

void ThorQ::Api::PasswordHash::setHash(const QByteArray& hash)
{
    if (m_hash != hash) {
        m_hash = hash;
        emit hashChanged(hash);
    }
}

void ThorQ::Api::PasswordHash::setOpsLimit(quint64 opsLimit)
{
    if (m_opsLimit != opsLimit) {
        m_opsLimit = opsLimit;
        emit opsLimitChanged(opsLimit);
    }
}

void ThorQ::Api::PasswordHash::setMemLimit(quint64 memLimit)
{
    if (m_memLimit != memLimit) {
        m_memLimit = memLimit;
        emit memLimitChanged(memLimit);
    }
}

void ThorQ::Api::PasswordHash::setAlgorithm(qint32 algorithm)
{
    if (m_algorithm != algorithm) {
        m_algorithm = algorithm;
        emit algorithmChanged(algorithm);
    }
}

void ThorQ::Api::PasswordHash::setPerformance(ThorQ::Api::PasswordHash::PerformancePresets performance)
{
    switch (performance) {
    case PerformancePresets::Interactive:
        m_opsLimit = crypto_pwhash_OPSLIMIT_INTERACTIVE;
        m_memLimit = crypto_pwhash_MEMLIMIT_INTERACTIVE;
        m_algorithm = crypto_pwhash_ALG_DEFAULT;
        break;
    case PerformancePresets::Moderate:
        m_opsLimit = crypto_pwhash_OPSLIMIT_MODERATE;
        m_memLimit = crypto_pwhash_MEMLIMIT_MODERATE;
        m_algorithm = crypto_pwhash_ALG_DEFAULT;
        break;
    case PerformancePresets::Sensitive:
        m_opsLimit = crypto_pwhash_OPSLIMIT_SENSITIVE;
        m_memLimit = crypto_pwhash_MEMLIMIT_SENSITIVE;
        m_algorithm = crypto_pwhash_ALG_DEFAULT;
        break;
    }
}

void ThorQ::Api::PasswordHash::handleWorkerDone(const QByteArray& hash)
{
    setHash(hash);
    emit hashingDone(hash);
}

ThorQ::Api::Internal::HashingThread::HashingThread(const QByteArray& passwordUtf8, const QByteArray& salt, std::uint64_t opsLimit, std::uint64_t memLimit, std::int32_t algorithm, QObject* parent)
    : QThread(parent)
    , m_passwordUtf8(passwordUtf8)
    , m_salt(salt)
    , m_opsLimit(opsLimit)
    , m_memLimit(memLimit)
    , m_algorithm(algorithm)
{
}

void ThorQ::Api::Internal::HashingThread::run()
{
    QByteArray hash;
    hash.resize(ThorQ::Api::PasswordHash::HashLength);

    if (crypto_pwhash(
                (std::uint8_t*)hash.data(), ThorQ::Api::PasswordHash::HashLength,
                m_passwordUtf8.data(), m_passwordUtf8.length(),
                (std::uint8_t*)m_salt.data(),
                m_opsLimit,
                m_memLimit,
                m_algorithm
                ) != 0) {
        hash.resize(0);
    }

    m_passwordUtf8.fill(' ');
    m_passwordUtf8.resize(0);

    emit hashDone(hash);
}
