#ifndef _HMI_USART_H_
#define _HMI_USART_H_
#include "bsp_usart_to_hmi.h"
/*注意:
 * （1）要使用该函数，必须提供一个原型为：
 * void Usart_SendString( USART_TypeDef * pUSARTx, char *str);的函数接口
 * （2）对于成员Control_Param，不能直接调用sprintf()函数进行赋值，
 *  而应该向一个已经分配内存的char数组，写入实际的字符串，再让Control_Param
 *  指向该数组
 */

typedef struct{
        /* 控件名称 */
        const char *Control_Name;
                /* 控件的属性 */
        const char *Control_Property;
                /* 控件的属性参数 */
        char *Control_Param;
}HMI_Control_Typtdef;

void HMI_Updata_Control_Property(HMI_Control_Typtdef* controlture);
void HMI_Send_Command(char * Command);



#endif /* HMI_USART_H_ */
