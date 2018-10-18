#ifndef  __BSP_TETRIS_H
#define  __BSP_TETRIS_H

#include "main.h"

typedef struct
{
        u8 x; 
        u8 y; 
        u16 Right; 
        u16 Down; 
        u8 CurNum; 
        u8 NextNum; 
}Shape;



void LCD_Tetris_Show(void);
void Creat_Shap(void);
void Tetris_Dowm(void);
void Tetris_Rotate(void);
void Tetris_Right(void);
void Tetris_Left(void);
void Clean_Line(void);
void Clean_Reset(void);


#endif /*__BSP_TETRIS_H*/

