#include "bsp_tetris.h"

u16 Tetris[19]={0x0F00,0x4444,0x0660,0x4460,0x02E0,0x6220,0x0740,0x2260,0x0E20,0x6440,0x0470,0x0C60,0x2640,0x0360,0x4620,0x04E0,0x2620,0x0E40,0x4640};
u8 Board[19][10] = {{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{1,1,1,1,1,1,1,1,1,1}};
u8 Model[4][4]={0};
u8 Preview[4][4]={0};


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
        temp=Tetris[Shape_Structer.CurNum];
        Shape_Structer.x = 3 ;
        Shape_Structer.y = 0 ;
        Set_Tetris_Color();
        //取出形状放在模型框中
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if(temp & 0x8000)
                        {
                                Model[i][j]= Shape_Structer.Color;
                        }
                        else
                        {
                                Model[i][j]= 0;
                        }
                        temp<<=1;
                        
                }
        }
        //将模型框中的形状变化到显示层
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if(Model[i][j]!=0)
                        {
                                if(Board[i][j+Shape_Structer.x]!=0)
                                {
                                        printf("Gameover\n");
                                        LCD_SetTextColor(GREEN);
                                        LCD_DisplayStringEx(11,140,32,32,"游戏结束",0);
                                        TIM_Cmd(BASIC_TIM, DISABLE);
                                        Game_Over_flag=1;
                                }
                                else
                                {
                                        Board[i][j+3] = Model[i][j];
                                }
                        }
                }
        }
        //取出形状放在预览框中
        temp=Tetris[Shape_Structer.NextNum];
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if(temp & 0x8000)
                        {
                                Preview[i][j]= Shape_Structer.NextColor;
                        }
                        else
                        {
                                Preview[i][j]= 0;
                        }
                        temp<<=1;
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
                        switch(Board[i][j])
                        {
                                case 1 :
                                        LCD_SetTextColor(RED);
                                break;
                                case 2 :
                                        LCD_SetTextColor(WHITE);
                                break;
                                case 3 :
                                        LCD_SetTextColor(BLUE);
                                break;
                                case 4 :
                                        LCD_SetTextColor(YELLOW);
                                break;
                                case 5 :
                                        LCD_SetTextColor(GREEN);
                                break;
                                case 6 :
                                        LCD_SetTextColor(GBLUE);
                                break;
                                case 7 :
                                        LCD_SetTextColor(MAGENTA);
                                break;
                                default :
                                        LCD_SetTextColor(BLACK);
                                break;
                        }
                        LCD_DrawBlock(j*15,i*15+50);
                }
        }
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        switch(Preview[i][j])
                        {
                                case 1 :
                                        LCD_SetTextColor(RED);
                                break;
                                case 2 :
                                        LCD_SetTextColor(WHITE);
                                break;
                                case 3 :
                                        LCD_SetTextColor(BLUE);
                                break;
                                case 4 :
                                        LCD_SetTextColor(YELLOW);
                                break;
                                case 5 :
                                        LCD_SetTextColor(GREEN);
                                break;
                                case 6 :
                                        LCD_SetTextColor(GBLUE);
                                break;
                                case 7 :
                                        LCD_SetTextColor(MAGENTA);
                                break;
                                default :
                                        LCD_SetTextColor(BLACK);
                                break;
                        }
                        LCD_DrawBlock(j*15+165,i*15+150);
                }
        }
}




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
        if(Shape_Structer.CurNum==2)
        {
                //如果是正方形不需要变形
                return ;
        }
        
        
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
        //确定了可以旋转则处理下长条的特殊情况，否则长条可能不能到最边上
        if(Shape_Structer.CurNum==0)
        {
                Shape_Structer.CurNum=1;
        }
        else if(Shape_Structer.CurNum==1)
        {
                Shape_Structer.CurNum=0;
        }
}

void  Tetris_Right(void)
{
        u8 i,j;
//        u8 count;
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
                                if(Model[i][3]!=0||Board[i+Shape_Structer.y][9]!=0)
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
                                        if(Model[i][j]!=0)
                                        {
                                                Board[i+Shape_Structer.y][j+Shape_Structer.x]=Model[i][j];
                                        }
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
//                                        count=0;
                                        for(i=0;i<4;i++)
                                        {
                                                for(j=2;j<4;j++)
                                                {
                                                        if(Model[i][j-1]!=0 && Board[i+Shape_Structer.y][j]!=0)
                                                        {
//                                                                break;
                                                                goto AAA;
                                                        }
//                                                        count++;
                                                }
                                        }
//                                        printf("count = %d\n",count);
//                                        if(count==8)
//                                        {
                                                for(i=0;i<4;i++)
                                                {
                                                        for(j=3;j>0;j--)
                                                        {
                                                                Model[i][j]= Model[i][j-1];
                                                        }
                                                }
                                                Model[0][0]=Model[1][0]=Model[2][0]=Model[3][0]=0;
//                                        }
                                        
                                        AAA:   for(i=0;i<4;i++)
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
        }
        
        Shape_Structer.x++;
        
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if( Model[i][j]!=0 && Board[i+Shape_Structer.y][j+Shape_Structer.x]!=0)
                        {
                                //右边已经有东西了 还原刚被清空的图形 
                                Shape_Structer.x--;
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
                                if(Model[i][0]!=0||Board[i+Shape_Structer.y][0]!=0)
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
                                        if(Model[i][j]!=0)
                                        {
                                                Board[i+Shape_Structer.y][j+Shape_Structer.x]=Model[i][j];
                                        }
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
                                                for(j=0;j<2;j++)
                                                {
                                                        if(Model[i][j+1]!=0 && Board[i+Shape_Structer.y][j+Shape_Structer.x]!=0)
                                                        {
                                                                goto BBB;
                                                        }
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
                                        
                                        BBB:
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
        }
        
        Shape_Structer.x--;
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if( Model[i][j]!=0 && Board[i+Shape_Structer.y][j+Shape_Structer.x]!=0)
                        {
                                //左边边已经有东西了 还原刚被清空的图形 
                                Shape_Structer.x++;
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
        LCD_Clear(0,51,150,270,BLACK);
                        
        for(i=0;i<18;i++)
        {
                for(j=0;j<10;j++)
                {
                        Board[i][j]=0;
                }
        }
//        for(j=0;j<10;j++)
//        {
//                Board[18][j]=1;
//        }
        Shape_Structer.y = 0;
        Shape_Structer.x = 3;
        Shape_Structer.CurNum = Make_Random();
        Shape_Structer.NextNum = Make_Random();
        temp =  Tetris[Shape_Structer.CurNum ];
        Set_Tetris_Color();
        //取出形状放在模型框中
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if(temp & 0x8000)
                        {
                                Model[i][j]= Shape_Structer.Color;
                                Board[i+Shape_Structer.y][j+Shape_Structer.x]=Model[i][j];
                        }
                        else
                        {
                                Model[i][j]= 0;
                        }
                        temp<<=1;
                }
        }
        
        temp=Tetris[Shape_Structer.NextNum];
        for(i=0;i<4;i++)
        {
                for(j=0;j<4;j++)
                {
                        if(temp & 0x8000)
                        {
                                Preview[i][j]= Shape_Structer.NextColor;
                        }
                        else
                        {
                                Preview[i][j]= 0;
                        }
                        temp<<=1;
                }
        }
}

void Clean_Line(void)
{
        u8 i,j,k,line;
        u8 cnt,Grade=0;
        for(cnt=0;cnt<4;cnt++)
        {
                //扫描是否可以清行
                for(i=17;i>0;i--)
                {
                        for(j=0;j<10;j++)
                        {
                                if(Board[i][j] == 0)//只要进if代表不可以清行
                                {
                                        break;
                                }
                        }
                        if(j==10)//只要进if代表可以清行//下降算法
                        {
                                line=i;
                                printf("line=%d\n",line);
                                Grade++;
                                for(k=line;k>0;k--)
                                {
                                        for(j=0;j<10;j++)
                                        {
                                                Board[k][j]=Board[k-1][j];
                                        }
                                }
                                for(j=0;j<10;j++)
                                {
                                        Board[0][j]=0;
                                }
                                break;
                        }
                }
                
        }
        
        printf("Grade=%d\n",Grade);
        
        switch (Grade)
        {
                case 1:
                        Score+=1;   //分数1
                break;
                case 2:
                        Score+=3;
                break;
                case 3:
                        Score+=5;
                break;
                case 4:
                        Score+=10;
                default:
                        break;
        }
        
        
}

void Set_Tetris_Color(void)
{
        switch(Shape_Structer.CurNum)
        {
                case 0 :
                        Shape_Structer.Color = 1;
                break;
                case 1 :
                        Shape_Structer.Color = 1;
                break;
                case 2 :
                        Shape_Structer.Color = 2;
                break;
                case 3 :
                        Shape_Structer.Color = 3;
                break;
                case 4 :
                        Shape_Structer.Color = 3;
                break;
                case 5 :
                        Shape_Structer.Color = 3;
                break;
                case 6 :
                        Shape_Structer.Color = 3;
                break;
                case 7 :
                        Shape_Structer.Color = 4;
                break;
                case 8 :
                        Shape_Structer.Color = 4;
                break;
                case 9 :
                        Shape_Structer.Color = 4;
                break;
                case 10 :
                        Shape_Structer.Color = 4;
                break;
                case 11 :
                        Shape_Structer.Color = 5;
                break;
                case 12 :
                        Shape_Structer.Color = 5;
                break;
                case 13 :
                        Shape_Structer.Color = 6;
                break;
                case 14 :
                        Shape_Structer.Color = 6;
                break;
                case 15 :
                        Shape_Structer.Color = 7;
                break;
                case 16 :
                        Shape_Structer.Color = 7;
                break;
                case 17 :
                        Shape_Structer.Color = 7;
                break;
                case 18 :
                        Shape_Structer.Color = 7;
                break;
                default :
                        Shape_Structer.Color =0;
                break;
        }
        switch(Shape_Structer.NextNum)
        {
                case 0 :
                        Shape_Structer.NextColor = 1;
                break;
                case 1 :
                        Shape_Structer.NextColor = 1;
                break;
                case 2 :
                        Shape_Structer.NextColor = 2;
                break;
                case 3 :
                        Shape_Structer.NextColor = 3;
                break;
                case 4 :
                        Shape_Structer.NextColor = 3;
                break;
                case 5 :
                        Shape_Structer.NextColor = 3;
                break;
                case 6 :
                        Shape_Structer.NextColor = 3;
                break;
                case 7 :
                        Shape_Structer.NextColor = 4;
                break;
                case 8 :
                        Shape_Structer.NextColor = 4;
                break;
                case 9 :
                        Shape_Structer.NextColor = 4;
                break;
                case 10 :
                        Shape_Structer.NextColor = 4;
                break;
                case 11 :
                        Shape_Structer.NextColor = 5;
                break;
                case 12 :
                        Shape_Structer.NextColor = 5;
                break;
                case 13 :
                        Shape_Structer.NextColor = 6;
                break;
                case 14 :
                        Shape_Structer.NextColor = 6;
                break;
                case 15 :
                        Shape_Structer.NextColor = 7;
                break;
                case 16 :
                        Shape_Structer.NextColor = 7;
                break;
                case 17 :
                        Shape_Structer.NextColor = 7;
                break;
                case 18 :
                        Shape_Structer.NextColor = 7;
                break;
                default :
                        Shape_Structer.NextColor = 0;
                break;
        }
}

