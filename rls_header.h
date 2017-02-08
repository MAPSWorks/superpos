#ifndef RLS_HEADER_H
#define RLS_HEADER_H

#define DATA_LEN_4K             16384 // uchar
#define DATA_LEN_SRC_4K         8192 // short
#define DATA_LEN_SPECTR_4K      4096 // float
#define DATA_LEN_SPECTR_4K_16B  4096 // short
#define DATA_LEN_LOG_SPECTR_4K  4096 // uchar

#define MAX_LINE_POS  4096
#define POS_TO_GRAD (360.0 / MAX_LINE_POS)
#define GRAD_TO_POS (1 / POS_TO_GRAD)
#define RAD_TO_GRAD (180.0 / 3.14151945)
#define GRAD_TO_RAD (1 / RAD_TO_GRAD)

#pragma pack(push)
#pragma pack(1)

typedef union {
    unsigned char _data[DATA_LEN_4K]; // 8 K short or 4 K float or 4 K uchar
    short src_data[DATA_LEN_SRC_4K]; //
    float spectr[DATA_LEN_SPECTR_4K]; //
    unsigned char log_spectr[DATA_LEN_LOG_SPECTR_4K]; //
} OUT_DATA_4K_AD;

typedef struct {
    unsigned int line_num;
    unsigned int pos;
} LINE_POS;

typedef struct {
    LINE_POS line_pos;
    OUT_DATA_4K_AD out_data;
} DATA_LINE_4K_AD;

typedef struct {
    //Version
    //unsigned int ver;     // 2 bytes
    //Packet size
    unsigned int size;   // 4 bytes
     //Timestamp
    double time;         // 8 bytes
    //Sender ID
    unsigned int sender; // 4 bytes
    //Signal type
    unsigned int type;   // 4 bytes
    //Signal mode
    unsigned int mode;   // 4 bytes
    //Data block
    DATA_LINE_4K_AD data;
} DATA_PACKAGE_AD;

#pragma pack(pop)

#endif // RLS_HEADER_H
