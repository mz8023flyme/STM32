#ifndef  __BSP_OLED_H
#define  __BSP_OLED_H

#define OLED_ADDRESS    0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78


void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y,char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N ,unsigned char* F16x16);


#endif  /*__BSP_OLED_H*/

