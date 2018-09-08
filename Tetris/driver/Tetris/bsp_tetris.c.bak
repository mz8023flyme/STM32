#include "bsp_tetris.h"

u16 Tetris[19]={0x0F00,0x4444,0x0660,0x4460,0x02E0,0x6220,0x0740,0x2260,0x0E20,0x6440,0x0470,0x0C60,0x2640,0x0360,0x4620,0x04E0,0x2620,0x0E40,0x4640};
u8 Board[15][11]={{0}0};
u8 Model[4][4]={0};
u8 Color=1;

extern Shape Shape_Structer;

static void LCD_DrawBlock(u8 x,u16 y)
{
        LCD_DrawRectangle(x,y,15,15,1);
}


void Creat_Shap(void)
{
        u8 i,j;  //customary+Model[i][j]
        u16 temp;
        Shape_Structer.CurNum = Shape_Structer.NextNum;
        Shape_Structer.NextNum = Make_Random();
        Shape_Structer.x = 3 ;
        Shape_Structer.y = 0;
        temp =  Tetris[Shape_Structer.CurNum ];
        
        //取出形状放在模型框中
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if(temp & 0x8000)
                        {
                                Model[i][j]= 1 * Color;
                                printf("*");
                        }
                        else
                        {
                                Model[i][j]= 0;
                                printf("-");
                        }
                        temp<<=1;
                        
                }
                printf("\n");
        }
        //将模型框中的形状变化到显示层
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if( (Model[i][j]!=0) && (Board[i][j+3]!=0))
                        {
                                printf("Gameover\n");
                                
                        }
                        else
                        {
                                Board[i][j+3] = Model[i][j];
                                
                        }
                }
                 
        }  
        
        
}

void LCD_Tetris_Show(void)
{
        u8 i,j; 
        for(i=0;i<15;i++)
        {
                for(j=0;j<10;j++)
                {
                        if(Board[i][j]!=0)
                        {
                                //添加变化颜色的代码
                                LCD_SetTextColor(RED);
                                LCD_DrawBlock(j*15,i*15+50);
                        }
                        else
                        {
                                LCD_SetTextColor(BLACK);
                                LCD_DrawBlock(j*15,i*15+50);
                        }
                }
        }
//        for(i=0;i<15;i++)
//        {
//                for(j=0;j<10;j++)
//                {
//                        if(Board[i][j]!=0)
//                        {
//                                printf("*");
//                        }else{
//                                printf("-");
//                        }
//                }
//                printf("\n");
//        }
}


//void Tetris_Game(void)
//{
//        u8 i,j,temp; 
//        
//        for(i=0;i<4;i++)
//        {
//                for(j=0;j<4;j++)
//                {
//                        
//                }
//                
//        }
//        
//}




void Tetris_Dowm()
{
        u8 i,j,flag; 
        printf("[%s][%d]\r\n", __func__, __LINE__);
        //将原来的图案清空
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if(Model[i][j]!=0)
                        {
                                Board[i+Shape_Structer.y][j+Shape_Structer.x]=0;
//                                printf("%d %d",i+Shape_Structer.x,i+Shape_Structer.y);
                        }
                }
        }
        
        printf("[%s][%d]\r\n", __func__, __LINE__);
        Shape_Structer.y ++;
        
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if( Model[i][j]!=0 && Board[i+Shape_Structer.y][j+Shape_Structer.x]!=0)
                        {
                                //下到不可再下 还原刚被清空的图形 开始创建新图形
                                for(i=0;i<4;i++)
                                {
                                        for(j=0;j<4;j++)
                                        {
                                                if(Model[i][j]!=0)
                                                {
                                                        Board[i+Shape_Structer.y-1][j+Shape_Structer.x]=Model[i][j];
                                                }
                                        }
                                }
                                Creat_Shap();
                                return;
                        }
//                        else
//                        {
//                                
//                                flag=1;
////                                flag=0;
////                                return ;
//                        }
                }
                
        }
        printf("[%s][%d]\r\n", __func__, __LINE__);
        //可以往下走
//        if(flag)
//        {
                for(i=0;i<4;i++)
                {
                        for(j=0;j<4;j++)
                        {
                                if(Model[i][j]!=0)
                                {
                                        Board[i+Shape_Structer.y][j+Shape_Structer.x]=Model[i][j];
                                }
                        }
                }
//        }
        
}










