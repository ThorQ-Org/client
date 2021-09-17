#ifndef THORQ_PASSWORDHASH_H
#define THORQ_PASSWORDHASH_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QThread>

#include <cstdint>

namespace ThorQ::Api {

class PasswordHash : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(PasswordHash)
public:
    static const std::size_t SaltLength = 16;
    static const std::size_t HashLength = 32;

    enum class PerformancePresets {
        Interactive,
        Moderate,
        Sensitive
    };
    Q_ENUMS(PerformancePresets)

    PasswordHash(QObject* parent = nullptr);
    ~PasswordHash();

    bool updateFromJson(const QJsonObject& json);
    QJsonObject toJsonParams() const;
    QJsonObject toJsonFull() const;

    QByteArray salt() const;
    QByteArray hash() const;
    quint64 opsLimit() const;
    quint64 memLimit() const;
    qint32 algorithm() const;
signals:
    void saltChanged(const QByteArray& salt);
    void hashChanged(const QByteArray& hash);
    void opsLimitChanged(quint64 opsLimit);
    void memLimitChanged(quint64 memLimit);
    void algorithmChanged(qint32 algorithm);

    void hashingDone(const QByteArray& hash);
public slots:
    void generateSalt();
    void setSalt(const QByteArray& salt);

    void generateHash(const QString& password);
    void setHash(const QByteArray& hash);

    void setOpsLimit(quint64 opsLimit);
    void setMemLimit(quint64 memLimit);
    void setAlgorithm(qint32 algorithm);
    void setPerformance(PerformancePresets performance);
private slots:
    void handleWorkerDone(const QByteArray& hash);
private:
    QByteArray m_salt;
    QByteArray m_hash;
    std::uint64_t m_opsLimit;
    std::uint64_t m_memLimit;
    std::int32_t  m_algorithm;
};

namespace Internal {

class HashingThread : public QThread {
    Q_OBJECT
public:
    HashingThread(const QByteArray& passwordUtf8, const QByteArray& salt, std::uint64_t opsLimit, std::uint64_t memLimit, std::int32_t algorithm, QObject* parent = nullptr);
signals:
    void hashDone(const QByteArray& hash);
private:
    void run() override;

    QByteArray m_passwordUtf8;
    QByteArray m_salt;
    std::uint64_t m_opsLimit;
    std::uint64_t m_memLimit;
    std::int32_t  m_algorithm;
};

}

}

#endif // THORQ_PASSWORDHASH_H
