#ifndef THORQ_SERIAL_H
#define THORQ_SERIAL_H

#include <QObject>

class QSerialPort;

class CollarSerial : public QObject
{
	Q_OBJECT
    Q_DISABLE_COPY(CollarSerial)
public:
    CollarSerial(QObject* parent = nullptr);

signals:
    void FoundCollar();
public slots:
    void FindCollar();
    void SendShock(unsigned int strength);
    void SendVibration(unsigned int strength);
    void SendBeep(unsigned int beeps);
    void SetAuto(unsigned int shock, unsigned int vibration, unsigned int beeps);
private slots:
    /**
     * @brief readData
     */
    void readData();
private:
    QSerialPort* m_serial;
};

#endif // THORQ_SERIAL_H
