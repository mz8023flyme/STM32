#ifndef __DATA_PRTOCOL_H
#define __DATA_PRTOCOL_H

#include "stm32f10x.h"

void data_scope_write_buffer(u8 channel, float data);           // 写通道数据至待发送帧数据缓存区
int data_scope_ready_to_send(u8 channel);                       // 发送帧数据生成函数
void data_scope_wave(int num, ...);                             // 发送波形数据到上位机

#endif
