#ifndef UDPCONNECTOR_H
#define UDPCONNECTOR_H

#include <QObject>
#include <QUdpSocket>
#include <vector>
#include <iostream>
class UDPConnector : public QObject
{
    Q_OBJECT
public:
    UDPConnector(QObject* parent = nullptr);
    Q_INVOKABLE void sendData(std::vector<unsigned char>& data);
    Q_INVOKABLE void sendData(int data);
    void readData();
    static uint8_t gencrc(unsigned char *data, size_t len);
private:
    QUdpSocket *udpConnector_ = nullptr;
    QHostAddress localHost_ = QHostAddress::Any;
    quint16 localPort_ = 11223;
    const QHostAddress foreignHost_ = QHostAddress::Any;
    quint16 foreignPort_ = 11223;
    std::vector<unsigned char>  m_revData;
    static const size_t QDT_SYNC_LEN = 2;
    static const size_t QDT_KEY_LEN = 2;
    static const size_t QDT_LENGTH_LEN = 1;
    static const size_t QDT_ACK_LEN = 2;
    static const size_t QDT_CDCR_LEN = 1;
    static const size_t QDT_CS_LEN = 1;
    static const size_t QDT_SYNC_KEY_LEN = 4;
    static const size_t QDT_SYNC_KEY_LENGTH_LEN = 5;
    static const size_t QDT_SYNC_KEY_LENGTH_ACK_LEN = 7;
    static const size_t QDT_KEY_LENGTH_ACK_CDCR_LEN = 6;
    static const size_t QDT_HEADER_LEN = 8;
    static const size_t QDT_MINIMUM_LENGTH = 9;
    static const unsigned char SYNC1 = 0xAD;
    static const unsigned char SYNC2 = 0xEE;
    uint16_t ack_ = 0;
};

#endif // UDPCONNECTOR_H
