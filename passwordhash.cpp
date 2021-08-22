#include "passwordhash.h"

#include <QThread>

#define SODIUM_STATIC
#include <sodium.h>

static_assert (ThorQ::PasswordHash::SaltLength == crypto_pwhash_SALTBYTES, "PasswordHashing salt length changed");
static_assert (ThorQ::PasswordHash::HashLength == crypto_box_SEEDBYTES, "PasswordHashing hash length changed");

ThorQ::PasswordHash::PasswordHash(QObject* parent)
    : QObject(parent)
    , m_salt()
    , m_hash()
    , m_opsLimit()
    , m_memLimit()
    , m_algorithm()
{
    m_salt.resize(ThorQ::PasswordHash::SaltLength);
    m_hash.resize(ThorQ::PasswordHash::HashLength);
}

ThorQ::PasswordHash::~PasswordHash()
{
}

QByteArray ThorQ::PasswordHash::salt() const
{
    return m_salt;
}

QByteArray ThorQ::PasswordHash::hash() const
{
    return m_hash;
}

quint64 ThorQ::PasswordHash::opsLimit() const
{
    return m_opsLimit;
}

quint64 ThorQ::PasswordHash::memLimit() const
{
    return m_memLimit;
}

qint32 ThorQ::PasswordHash::algorithm() const
{
    return m_algorithm;
}

void ThorQ::PasswordHash::generateSalt()
{
    randombytes_buf(m_salt.data(), m_salt.size());
    emit saltChanged();
}

void ThorQ::PasswordHash::setSalt(const QByteArray& salt)
{
    if (m_salt != salt) {
        m_salt = salt;
        emit saltChanged();
    }
}

void ThorQ::PasswordHash::setOpsLimit(quint64 opsLimit)
{
    if (m_opsLimit != opsLimit) {
        m_opsLimit = opsLimit;
        emit opsLimitChanged();
    }
}

void ThorQ::PasswordHash::setMemLimit(quint64 memLimit)
{
    if (m_memLimit != memLimit) {
        m_memLimit = memLimit;
        emit memLimitChanged();
    }
}

void ThorQ::PasswordHash::setAlgorithm(qint32 algorithm)
{
    if (m_algorithm != algorithm) {
        m_algorithm = algorithm;
        emit algorithmChanged();
    }
}

void ThorQ::PasswordHash::setPerformance(ThorQ::PasswordHash::PerformancePresets performance)
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

void ThorQ::PasswordHash::generateHash(const QString& password)
{
    ThorQ::Internal::HashingThread* worker = new ThorQ::Internal::HashingThread(password.toUtf8(), m_salt, m_opsLimit, m_memLimit, m_algorithm, this);
    connect(worker, &ThorQ::Internal::HashingThread::hashReady, this, &ThorQ::PasswordHash::handleWorkerDone);
    connect(worker, &ThorQ::Internal::HashingThread::hashFailed, this, &ThorQ::PasswordHash::handleWorkerFailed);
    connect(worker, &QThread::finished, worker, &QObject::deleteLater);
    worker->start();
}

void ThorQ::PasswordHash::handleWorkerDone(const QByteArray& hash)
{
    if (m_hash != hash) {
        m_hash = hash;
        emit hashGenerated();
    }
}

void ThorQ::PasswordHash::handleWorkerFailed()
{
    emit hashFailed();
}

ThorQ::Internal::HashingThread::HashingThread(const QByteArray& passwordUtf8, const QByteArray& salt, std::uint64_t opsLimit, std::uint64_t memLimit, std::int32_t algorithm, QObject* parent)
    : QThread(parent)
    , m_passwordUtf8(passwordUtf8)
    , m_salt(salt)
    , m_opsLimit(opsLimit)
    , m_memLimit(memLimit)
    , m_algorithm(algorithm)
{
}

void ThorQ::Internal::HashingThread::run()
{
    QByteArray hash;
    hash.resize(ThorQ::PasswordHash::HashLength);

    if (crypto_pwhash(
                (std::uint8_t*)hash.data(), ThorQ::PasswordHash::HashLength,
                m_passwordUtf8.data(), m_passwordUtf8.length(),
                (std::uint8_t*)m_salt.data(),
                m_opsLimit,
                m_memLimit,
                m_algorithm
                ) == 0) {
        emit hashReady(hash);
    } else {
        emit hashFailed();
    }
}
