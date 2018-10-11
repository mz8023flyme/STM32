#include "bsp_tetris.h"

u16 Tetris[19]={0x0F00,0x4444,0x0660,0x4460,0x02E0,0x6220,0x0740,0x2260,0x0E20,0x6440,0x0470,0x0C60,0x2640,0x0360,0x4620,0x04E0,0x2620,0x0E40,0x4640};
u8 Board[19][10] = {{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{1,1,1,1,1,1,1,1,1,1}};
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
        Shape_Structer.y = 0 ;
        temp =  Tetris[Shape_Structer.CurNum ];
        
        //取出形状放在模型框中
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if(temp & 0x8000)
                        {
                                Model[i][j]= 1 * Color;
//                                printf("*");
                        }
                        else
                        {
                                Model[i][j]= 0;
//                                printf("-");
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
        for(i=0;i<18;i++)
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
//        for(i=0;i<16;i++)
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
        u8 i,j; 
        
        Shape_Structer.y ++;
        
        //将原来的图案清空
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if(Model[i][j]!=0)
                        {
                                Board[i+Shape_Structer.y-1][j+Shape_Structer.x]=0;
                        }
                }
        }
        
        
        
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
                                Clean_Line();
                                Clean_Line();
                                Clean_Line();
                                Clean_Line();
//                                printf("[%s][%d]\r\n", __func__, __LINE__);
                                Down_Flag=0;
                                Creat_Shap();
                                return;
                        }
                }
                
        }
        

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

        
}


void  Tetris_Rotate(void)
{
        u8 i,j;
        u16 temp[4][4];
        //檫除原来图形
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if(Model[i][j]!=0)
                        {
                                Board[i+Shape_Structer.y][j+Shape_Structer.x]=0;
                        }
                }
        }
        //旋转
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        temp[i][j] =  Model[j][3-i];
                }
        }
        
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if( temp[i][j]!=0 && Board[i+Shape_Structer.y][j+Shape_Structer.x]!=0)
                        {
                                //如果不可以旋转  那就还原
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
                                return;
                        }
                }
                
        }
        //如果可以旋转
        
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        Model[i][j]=temp[i][j];
                        if(temp[i][j]!=0)
                        {
                                Board[i+Shape_Structer.y][j+Shape_Structer.x]=temp[i][j];
                        }
                        
                }
        }
}


void  Tetris_Right(void)
{
        u8 i,j;
        
        
        //檫除原来图形
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if(Model[i][j]!=0)
                        {
                                Board[i+Shape_Structer.y][j+Shape_Structer.x]=0;
                        }
                }
        }
        
        
        if(Shape_Structer.x==6)
        {
                if(Shape_Structer.CurNum!=0)
                {
                        for(i=0;i<4;i++)
                        {
                                if(Model[i][3]!=0)
                                {
                                        //还原
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
                                        return ;
                                }
                        }
                        for(i=0;i<4;i++)
                        {
                                for(j=3;j>0;j--)
                                {
                                        Model[i][j]= Model[i][j-1];
                                }
                        }
                        Model[0][0]=Model[1][0]=Model[2][0]=Model[3][0]=0;
                        for(i=0;i<4;i++)
                        {
                                for(j=0;j<4;j++)
                                {
                                        Board[i+Shape_Structer.y][j+Shape_Structer.x]=Model[i][j];
                                }
                        }
                        return ;
                }
                else
                {
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
                        return ;
                }
        }
        
        
        else if (Shape_Structer.x==0)
        {
                if(Shape_Structer.CurNum!=0)
                {
                        for(i=0;i<4;i++)
                        {
                                if(Model[i][0]!=0)
                                {
                                        
                                        for(i=0;i<4;i++)
                                        {
                                                for(j=3;j>0;j--)
                                                {
                                                        Model[i][j]= Model[i][j-1];
                                                }
                                        }
                                        Model[0][0]=Model[1][0]=Model[2][0]=Model[3][0]=0;
                                        for(i=0;i<4;i++)
                                        {
                                                for(j=0;j<4;j++)
                                                {
                                                        Board[i+Shape_Structer.y][j+Shape_Structer.x]=Model[i][j];
                                                }
                                        }
                                        return;
                                }
                        }
                }
        }
        
        Shape_Structer.x++;
        
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if( Model[i][j]!=0 && Board[i+Shape_Structer.y][j+Shape_Structer.x]!=0)
                        {
                                //右边已经有东西了 还原刚被清空的图形 
                                for(i=0;i<4;i++)
                                {
                                        for(j=0;j<4;j++)
                                        {
                                                if(Model[i][j]!=0)
                                                {
                                                        Board[i+Shape_Structer.y][j+Shape_Structer.x-1]=Model[i][j];
                                                }
                                        }
                                }
                                return;
                        }
                }
                
        }
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
        
}



void  Tetris_Left(void)
{
        u8 i,j;
        
        
        //檫除原来图形
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if(Model[i][j]!=0)
                        {
                                Board[i+Shape_Structer.y][j+Shape_Structer.x]=0;
                        }
                }
        }
        
        
        if(Shape_Structer.x==0)
        {
                if(Shape_Structer.CurNum!=0)
                {
                        for(i=0;i<4;i++)
                        {
                                if(Model[i][0]!=0)
                                {
                                        //还原
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
                                        return ;
                                }
                        }
                        for(i=0;i<4;i++)
                        {
                                for(j=0;j<3;j++)
                                {
                                        Model[i][j]= Model[i][j+1];
                                }
                        }
                        Model[0][3]=Model[1][3]=Model[2][3]=Model[3][3]=0;
                        for(i=0;i<4;i++)
                        {
                                for(j=0;j<4;j++)
                                {
                                        Board[i+Shape_Structer.y][j+Shape_Structer.x]=Model[i][j];
                                }
                        }
                        return ;
                }
                else
                {
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
                        return ;
                }
        }
        
        
        else if (Shape_Structer.x==6)
        {
                if(Shape_Structer.CurNum!=0)
                {
                        for(i=0;i<4;i++)
                        {
                                if(Model[i][3]!=0)
                                {
                                        
                                        for(i=0;i<4;i++)
                                        {
                                                for(j=0;j<3;j++)
                                                {
                                                        Model[i][j]= Model[i][j+1];
                                                }
                                        }
                                        Model[0][3]=Model[1][3]=Model[2][3]=Model[3][3]=0;
                                        for(i=0;i<4;i++)
                                        {
                                                for(j=0;j<4;j++)
                                                {
                                                        Board[i+Shape_Structer.y][j+Shape_Structer.x]=Model[i][j];
                                                }
                                        }
                                        return;
                                }
                                
                        }
                }
        }
        
        Shape_Structer.x--;
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if( Model[i][j]!=0 && Board[i+Shape_Structer.y][j+Shape_Structer.x]!=0)
                        {
                                //右边已经有东西了 还原刚被清空的图形 
                                for(i=0;i<4;i++)
                                {
                                        for(j=0;j<4;j++)
                                        {
                                                if(Model[i][j]!=0)
                                                {
                                                        Board[i+Shape_Structer.y][j+Shape_Structer.x+1]=Model[i][j];
                                                }
                                        }
                                }
                                return;
                        }
                }
                
        }
        
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
        
}

void Clean_Reset(void)
{
        u8 i,j ;
        u16 temp;
        LCD_Clear(0,49,150,270,BLACK);
                        
        for(i=0;i<18;i++)
        {
                for(j=0;j<10;j++)
                {
                        Board[i][j]=0;
                }
        }
        for(j=0;j<10;j++)
        {
                Board[18][j]=1;
        }
        Shape_Structer.y = 0;
        Shape_Structer.x = 3;
        Shape_Structer.CurNum = Shape_Structer.NextNum;
        Shape_Structer.NextNum = Make_Random();
        temp =  Tetris[Shape_Structer.CurNum ];
        
        //取出形状放在模型框中
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if(temp & 0x8000)
                        {
                                Model[i][j]= 1 * Color;
                        }
                        else
                        {
                                Model[i][j]= 0;
                        }
                        temp<<=1;
                        
                }

        }
}

void Clean_Line(void)
{
        u8 i,j,k,line;
        //扫描是否可以清行
        for(i=0;i<18;i++)
        {
                for(j=0;j<10;j++)
                {
                        if(Board[i][j] == 0)
                        {
                                break;
                        }
                }
                if(j==10)
                {
//                        clean[count]=i;
//                        count++;
//                        printf("i=%d",i);
                        line=i;
                        printf("line=%d\n",line);
                }
                 
        }
        //新的下降算法
        for(k=line;k>0;k--)
        {
//                printf("[%s][%d]\r\n", __func__, __LINE__);
                for(j=0;j<10;j++)
                {
                        Board[k][j]=Board[k-1][j];
                }
        }
        for(j=0;j<10;j++)
        {
                Board[0][j]=0;
        }

        /*
        switch (count)
        {
                case 1: //分数1
        }
        */
        
}


