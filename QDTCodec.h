#ifndef QDTCODEC_H
#define QDTCODEC_H

#include <iostream>
#include "Utils.h"

static const unsigned char SYNC1 = 0xAD;
static const unsigned char SYNC2 = 0xEE;

namespace SeekerGM
{
    enum CDCRTYPE : uint8_t {
        NO_RESPONSE = 0x00,
        RES_RESPONSE = 0x01,
        REQ_RESPONSE = 0x03
    };

    class QDTCodec
    {
    public:
        short       key;
        uint16_t    ack_type;
        CDCRTYPE    cdcr;
        std::vector<unsigned char> data;
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

    public:
        QDTCodec() {

        }

        QDTCodec(short _key, uint16_t _ack_type, CDCRTYPE _cdcr, unsigned char * _data, uint8_t _len) {
            key = _key;
            ack_type = _ack_type;
            cdcr = _cdcr;
            data = std::vector<unsigned char>(_data, _data + _len + QDT_ACK_LEN + QDT_CDCR_LEN);
        }

        QDTCodec(short _key, CDCRTYPE _cdcr, unsigned char * _data, uint8_t _len) {
            key = _key;
            ack_type = 0;
            cdcr = _cdcr;
            data = std::vector<unsigned char>(_data, _data + _len + QDT_ACK_LEN + QDT_CDCR_LEN);
        }

        QDTCodec(short _key, uint16_t _ack_type, CDCRTYPE _cdcr, std::vector<unsigned char> _data) {
            key = _key;
            ack_type = _ack_type;
            cdcr = _cdcr;
            data = _data;
        }

        QDTCodec(short _key, CDCRTYPE _cdcr, std::vector<unsigned char> _data) {
            key = _key;
            ack_type = 0;
            cdcr = _cdcr;
            data = _data;
        }

        ~QDTCodec() {}

    public:
        std::vector<unsigned char> encode() {
            std::vector<unsigned char> buffer;
            buffer.push_back(SYNC1);
            buffer.push_back(SYNC2);

            std::vector<unsigned char> s_key;
            s_key = Utils::toByte<short>(key);
            buffer.insert(buffer.end(), s_key.begin(), s_key.end());

            std::vector<unsigned char> s_len;
            s_len = Utils::toByte<uint8_t>(static_cast<unsigned char>(data.size()));
            buffer.insert(buffer.end(), s_len.begin(), s_len.end());

            std::vector<unsigned char> s_ack_type;
            s_ack_type = Utils::toByte<uint16_t>(ack_type);
            buffer.insert(buffer.end(), s_ack_type.begin(), s_ack_type.end());

            std::vector<unsigned char> s_cdcr;
            s_cdcr = Utils::toByte<CDCRTYPE>(cdcr);
            buffer.insert(buffer.end(), s_cdcr.begin(), s_cdcr.end());

            buffer.insert(buffer.end(), data.begin(), data.end());
            unsigned char m_gencrc = gencrc(buffer.data() + QDT_SYNC_LEN, data.size() + QDT_KEY_LENGTH_ACK_CDCR_LEN);
            buffer.push_back(m_gencrc);

//            //-- Debug
//            printf("Sent %d bytes: ", static_cast<int>(buffer.size()));
//            for(size_t i=0; i < buffer.size(); i++){
//                printf(" %02X", buffer.at(i));
//            }
//            printf("\r\n");

            return buffer;
        }

        static uint8_t gencrc(unsigned char *data, size_t len) {
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
    };
};
#endif // QDTCODEC_H
