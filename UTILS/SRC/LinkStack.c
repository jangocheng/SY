/*
*********************************************************************************************************
* @file    	LinkStack.c
* @author  	SY
* @version 	V1.0.0
* @date    	2016-9-2 10:16:49
* @IDE	 	V4.70.0.0
* @Chip    	STM32F407VE
* @brief   	��ʽ��ջԴ�ļ�
*********************************************************************************************************
* @attention
*	ջ��������һ�ν��в��������ɾ�����������Ա����׳ƶ�ջ�����������в�����һ�˳�Ϊ��ջ������
*	��һ�̶��˳�Ϊ��ջ�ס�����ջ��û��Ԫ�س�Ϊ����ջ�����ص㣺�Ƚ������FILO����
*
* 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private Includes
*********************************************************************************************************
*/
#include "utils.h"
#include "LinkStack.h"


/*
*********************************************************************************************************
*                              				Private define
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                              				Private typedef
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private constants
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private macro
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private variables
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private function prototypes
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                              				Private functions
*********************************************************************************************************
*/
/*
*********************************************************************************************************
* Function Name : CreateLinkStack
* Description	: ������ʽ��ջ(�ڴ�ռ����ڲ��ṩ����̬�ڴ�)
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
LINK_STACK_TypeDef *CreateLinkStack( void )
{
	/* ����ջ���ڵ� */
	LINK_STACK_TypeDef *top = (LINK_STACK_TypeDef *)calloc(1,sizeof(LINK_STACK_TypeDef));
	if (top == NULL)
	{
		return NULL;
	}
	
	top->data = NULL;
	top->next = NULL;

	return top;
}

/*
*********************************************************************************************************
* Function Name : LinkStackIsEmpty
* Description	: ��ʽ��ջ�Ƿ�Ϊ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM LinkStackIsEmpty( LINK_STACK_TypeDef *top )
{
	if (top == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (top->next == NULL)
	{
		return STATUS_DATA_STRUCT_TRUE;
	}
	else
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
}

/*
*********************************************************************************************************
* Function Name : ClearLinkStack
* Description	: �����ʽ��ջ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void ClearLinkStack( LINK_STACK_TypeDef *top )
{
	if (top != NULL)
	{
		LINK_STACK_TypeDef *oldNode = NULL;
		
		while (top->next != NULL)
		{
			oldNode = top->next;
			top->next = oldNode->next;
			oldNode->next = NULL;
			
			if (oldNode->data != NULL)
			{
				free(oldNode->data);
				oldNode->data = NULL;
			}
			
			free(oldNode);
			oldNode = NULL;				
		}
	}
}

/*
*********************************************************************************************************
* Function Name : DestoryLinkStack
* Description	: ������ʽ��ջ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void DestoryLinkStack( LINK_STACK_TypeDef **top )
{
	LINK_STACK_TypeDef *this = *top;
	
	if (this != NULL)
	{
		ClearLinkStack(this);
		
		/* �ͷ�ջ���ڵ� */
		free(this);
		
		*top = NULL;
	}
}

/*
*********************************************************************************************************
* Function Name : PushLinkStack
* Description	: ��ջ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PushLinkStack( LINK_STACK_TypeDef *top, void *dataIn, uint32_t dataSize )
{
	if (top == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	/* �����½ڵ� */
	LINK_STACK_TypeDef *pNode = (LINK_STACK_TypeDef *)calloc(1,sizeof(LINK_STACK_TypeDef));
	if (pNode == NULL)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	pNode->next = top->next;	
	top->next = pNode;
	
	/* �洢�û����� */
	pNode->data = (void *)calloc(1,dataSize);
	if (pNode->data == NULL)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}	
	memcpy(pNode->data,dataIn,dataSize);
	
	return STATUS_DATA_STRUCT_TRUE;
}

/*
*********************************************************************************************************
* Function Name : PopLinkStack
* Description	: ��ջ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM PopLinkStack( LINK_STACK_TypeDef *top, void *dataOut, uint32_t dataSize )
{
	if (top == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (LinkStackIsEmpty(top) == STATUS_DATA_STRUCT_TRUE)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	LINK_STACK_TypeDef *oldNode = top->next;	
	top->next = oldNode->next;
	oldNode->next = NULL;

	DATA_STRUCT_STATUS_ENUM isDataOK = STATUS_DATA_STRUCT_FALSE;
	if (oldNode->data != NULL)
	{
		memcpy(dataOut,oldNode->data,dataSize);
		free(oldNode->data);
		oldNode->data = NULL;
		isDataOK = STATUS_DATA_STRUCT_TRUE;
	}
	
	free(oldNode);
	oldNode = NULL;
	
	return isDataOK;
}

/*
*********************************************************************************************************
* Function Name : GetLinkStackElement
* Description	: ��ȡ��ʽ��ջԪ��
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
DATA_STRUCT_STATUS_ENUM GetLinkStackElement( LINK_STACK_TypeDef *top, void *dataOut, uint32_t dataSize )
{
	if (top == NULL)
	{
		return STATUS_DATA_STRUCT_UNDEFINED;
	}
	
	if (LinkStackIsEmpty(top) == STATUS_DATA_STRUCT_TRUE)
	{
		return STATUS_DATA_STRUCT_FALSE;
	}
	
	DATA_STRUCT_STATUS_ENUM isDataOK = STATUS_DATA_STRUCT_FALSE;
	LINK_STACK_TypeDef *oldNode = top->next;	
	if (oldNode->data != NULL)
	{
		memcpy(dataOut,oldNode->data,dataSize);
		isDataOK = STATUS_DATA_STRUCT_TRUE;
	}
	
	return isDataOK;
}

/*
*********************************************************************************************************
* Function Name : TraverseLinkStack
* Description	: ������ʽ��ջ
* Input			: None
* Output		: None
* Return		: None
*********************************************************************************************************
*/
void TraverseLinkStack( LINK_STACK_TypeDef *top, void *dataOut, uint32_t dataSize,\
		void (*show_CallBack)( void *dataOut ) )
{
	LINK_STACK_TypeDef stack = *top;
	LINK_STACK_TypeDef *pNode = stack.next;
	
	while (pNode != NULL)
	{
		memcpy(dataOut,pNode->data,dataSize);
		
		if (show_CallBack)
		{
			show_CallBack(dataOut);
		}
		
		pNode = pNode->next;
	}
}

/************************ (C) COPYRIGHT STMicroelectronics **********END OF FILE*************************/