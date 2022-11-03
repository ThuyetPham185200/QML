#include "UDPConnector.h"
#include <Utils.h>
#include <QDTCodec.h>
UDPConnector::UDPConnector(QObject *parent)
    : QObject(parent)
{
    udpConnector_ = new QUdpSocket();
//    udpConnector_->bind(localHost_, localPort_);
    connect(udpConnector_, &QUdpSocket::readyRead,
            this, &UDPConnector::readData);
}

void UDPConnector::sendData(std::vector<unsigned char> &data)
{
    udpConnector_->writeDatagram(reinterpret_cast<char*>(data.data()), data.size(), foreignHost_, foreignPort_);
}

void UDPConnector::sendData(int data)
{
    SeekerGM::QDTCodec packet;
    packet.key = 0x0006;
    packet.ack_type = ++ack_;
    std::cout << "ack = " << packet.ack_type << "\n";
    packet.cdcr = SeekerGM::CDCRTYPE::REQ_RESPONSE;
    std::vector<unsigned char> da = Utils::toByte<int>(data);
    packet.data = da;
    std::vector<unsigned char> encodedData = packet.encode();
    std::cout << "data send:\n";
    for(int i = 0; i < encodedData.size(); i++)
    {
        printf("%02x ", encodedData[i]);
    }
    std::cout << "\n";
    this->sendData(encodedData);
}

void UDPConnector::readData()
{
    while (udpConnector_->hasPendingDatagrams())
    {
        char receivedData[1024];
        qint64 received = udpConnector_->readDatagram(receivedData, sizeof (receivedData));
        if (received > 0) {
            std::vector<unsigned char> tmpData(receivedData, receivedData + received);
            m_revData.insert(m_revData.begin(), tmpData.begin(), tmpData.end());
            if (m_revData.size() < QDT_MINIMUM_LENGTH) {
                continue;
            }
            //-- Debug
//            printf("Received %d bytes: ", static_cast<int>(m_revData.size()));
//            for(int i=0; i < m_revData.size(); i++){
//                printf(" %02X",static_cast<unsigned char>(m_revData.at(i)));
//            }
//            printf("\r\n");

            while (m_revData.size() >= QDT_MINIMUM_LENGTH) {
              if (m_revData[0] != SYNC1 || m_revData[1] != SYNC2) {
                m_revData.erase(m_revData.begin(), m_revData.begin() + 1);
                continue;
              }
              for(int i = 0; i < m_revData.size(); i++)
              {
                  printf("%02x ", m_revData[i]);
              }
              std::cout << "\n";
              short key = Utils::toValue<short>(m_revData, QDT_SYNC_LEN);
              uint8_t dataLength = Utils::toValue<uint8_t>(m_revData, QDT_SYNC_KEY_LEN);
              uint16_t packetAck = Utils::toValue<uint16_t>(m_revData, QDT_SYNC_KEY_LENGTH_LEN);
              uint8_t cdcr = Utils::toValue<SeekerGM::CDCRTYPE>(m_revData, QDT_SYNC_KEY_LENGTH_ACK_LEN);
              std::cout << "key = " << key << ", ack = " << packetAck << ", cdcr = " << (int)cdcr << "\n";

              // packet size is NOT enough to parse, do nothing and wait for the next time reception
              if (m_revData.size() < static_cast<unsigned int>(dataLength + QDT_MINIMUM_LENGTH)) {
                m_revData.clear();
                break;
              }
              //calculate and check the checksum byte (from key to payload)
              unsigned char checksum = gencrc(m_revData.data() + QDT_SYNC_LEN, dataLength + QDT_KEY_LENGTH_ACK_CDCR_LEN);

              if (checksum == m_revData.at(QDT_HEADER_LEN + dataLength)) {
                    std::vector<unsigned char> data(m_revData.begin() + QDT_HEADER_LEN,
                                      m_revData.begin() + QDT_HEADER_LEN + dataLength);
                    m_revData.erase(m_revData.begin(),
                                    m_revData.begin() + dataLength + QDT_MINIMUM_LENGTH);
              } else {
                    m_revData.erase(m_revData.begin(), m_revData.begin() + 1);
              }
            }
        }
    }
}

uint8_t UDPConnector::gencrc(unsigned char *data, size_t len) {
  uint8_t crc = 0xff;
  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (size_t j = 0; j < 8; j++) {
      if ((crc & 0x80) != 0)
        crc = static_cast<uint8_t>((crc << 1) ^ 0x31);
      else
        crc <<= 1;
    }
  }
  return crc;
}
