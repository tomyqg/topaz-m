#ifndef _CRC_H
#define _CRC_H

#ifdef __cplusplus
extern "C" {
#endif

//#define UCHAR unsigned char
typedef unsigned char UCHAR;
typedef unsigned short  USHORT;
//#define USHORT unsigned short

USHORT crc16(UCHAR * pucFrame, USHORT usLen);

//unsigned short getCRC16();

#ifdef __cplusplus
}
#endif

#endif
