/*
 * Config.h
 *
 *  Created on: Jul 20, 2025
 *      Author: aresi
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "stm32f439xx.h"
#include "stdint.h"
#include <string.h>

#define true	1
#define false	0

typedef enum
{
	NVIC_ISER0 = 0xE000E100,
	NVIC_IP0 = 0xE000E400
}NVIC_Address;

typedef enum
{
	TIF = 0x80,
	RIF = 0x20
}USART_Flags;

typedef enum
{
	EXTI_PA	= 0,
	EXTI_PB	= 1,
	EXTI_PC	= 2,
	EXTI_PD	= 3,
	EXTI_PE	= 4,
	EXTI_PF	= 5,
	EXTI_PG	= 6,
	EXTI_PH	= 7,
	EXTI_PI	= 8,
	EXTI_PJ	= 9,
	EXTI_PK	= 10
}EXTICR_Enum;

void LEDGREEN_Blink()
{
	GPIOB->ODR ^= GPIO_ODR_OD0;
}

void LEDBLUE_Blink()
{
	GPIOB->ODR ^= GPIO_ODR_OD7;
}

void LEDRED_BlinkUp()
{
	GPIOB->ODR |= GPIO_ODR_OD14;
}

void LEDRED_BlinkDown()
{
	GPIOB->ODR &= ~GPIO_ODR_OD14;
}

void NVIC_InteConfig(uint8_t Pos, const uint8_t Prio)
{
	volatile uint32_t * const NVIC_ISERx 	= (volatile uint32_t * const)( NVIC_ISER0 + ((Pos/32)*4) );
	volatile uint32_t * const NVIC_IPx 		= (volatile uint32_t * const)(NVIC_IP0 + ((Pos/4)*4) );

	*NVIC_ISERx |= ( 1 << Pos % 32 );
	*NVIC_IPx |= ( Prio << (8*((Pos) % 4) + 4) );

}

uint16_t BaudRate(uint32_t f_ck, uint32_t Bauds, _Bool OVER8)
{
	uint32_t Denominador = (8*Bauds)*(2 - OVER8);
	uint64_t Enumerador = f_ck;

	uint32_t DIV_Mantissa = Enumerador/Denominador, USARTDIV = Enumerador*1000/Denominador;

	uint32_t DIV_Fraction =  (16)*(USARTDIV - DIV_Mantissa*1000);

	if ( DIV_Fraction >= 500 )
		DIV_Fraction = (DIV_Fraction/1000) + 1;

	return (DIV_Mantissa << 4) | DIV_Fraction;

}

uint8_t ShiftCalculate(uint8_t const Pin)
{
	uint8_t Shift = Pin;

	while (Shift > 3) Shift -=4;

	return Shift;
}

void SYSCFG_EXTICRConfig(uint8_t Pin, uint8_t EXTICR_Port)
{
	uint8_t const Shift = ShiftCalculate(Pin);

	if (Pin < 4)
	{
		SYSCFG->EXTICR[0] &= ~(0xFF << 4*Shift);
		SYSCFG->EXTICR[0] |= (EXTICR_Port << 4*Shift);
		return;

	} else if ((Pin >= 4) && (Pin < 8))
	{
		SYSCFG->EXTICR[1] &= ~(0xFF << 4*Shift);
		SYSCFG->EXTICR[1] |= (EXTICR_Port << 4*Shift);
		return;

	} else if ((Pin >= 8) && (Pin < 12))
	{
		SYSCFG->EXTICR[2] &= ~(0xFF << 4*Shift);
		SYSCFG->EXTICR[2] |= (EXTICR_Port << 4*Shift);
		return;

	} else if (Pin >= 12)
	{
		SYSCFG->EXTICR[3] &= ~(0xFF << 4*Shift);
		SYSCFG->EXTICR[3] |= (EXTICR_Port << 4*Shift);
		return;

	}
}

void ConfigClock()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

}

void GpioConfig_TaskPush()
{
	GPIOC->MODER &= ~GPIO_MODER_MODER13;
	GPIOC->PUPDR |= GPIO_PUPDR_PUPD13_1;

	EXTI->PR |=  EXTI_PR_PR13;
	EXTI->RTSR |= EXTI_RTSR_TR13;
	EXTI->IMR |= EXTI_IMR_IM13;

	SYSCFG_EXTICRConfig(13, EXTI_PC);
	NVIC_InteConfig(EXTI15_10_IRQn, 15);

}

void GpioConfig_TaskGreen()
{
	GPIOB->MODER &= ~(GPIO_MODER_MODE0);
	GPIOB->MODER |= GPIO_MODER_MODE0_0;
}

void GpioConfig_TaskLedBlue()
{
	GPIOD->MODER &= ~(GPIO_MODER_MODE1);
	GPIOD->MODER |= GPIO_MODER_MODE1_0;

	EXTI->PR |= EXTI_PR_PR1;
	EXTI->IMR |= EXTI_IMR_IM1;

	SYSCFG_EXTICRConfig(1, EXTI_PD);
	NVIC_InteConfig(EXTI1_IRQn, 14);

	GPIOB->MODER &= ~(GPIO_MODER_MODE7);
	GPIOB->MODER |= GPIO_MODER_MODE7_0;

}

void GpioConfig_TaskLedRed()
{
	GPIOB->MODER &= ~(GPIO_MODER_MODE14);
	GPIOB->MODER |= GPIO_MODER_MODE14_0;

}

void UsartConfig_DMA()
{
	DMA1_Stream3->CR &= ~DMA_SxCR_EN;
	while ( (DMA1_Stream3->CR & DMA_SxCR_EN) );

	DMA1_Stream3->CR |= DMA_SxCR_CHSEL_2; //Canal 4

	DMA1_Stream3->CR |= DMA_SxCR_PL_0; //Prioridad media

	DMA1_Stream3->CR &= ~(DMA_SxCR_MSIZE | DMA_SxCR_PSIZE); // Tamaño de dato a enviar y recibir de 1 Byte

	DMA1_Stream3->CR |= DMA_SxCR_MINC; // Incrementar puntero en memoria a la siguiente localidad, cada que se transmite un dato

	DMA1_Stream3->CR |= DMA_SxCR_DIR_0; // Transmitir de memoria al periférico

	DMA1_Stream3->CR |= DMA_SxCR_TCIE; // Interrupción al generarse transmisión completa

	NVIC_InteConfig(DMA1_Stream3_IRQn, 11); // función alterna y prioridad de ISR
}

void UsartConfig()
{
	//Función alterna de GPIO
	GPIOD->MODER &= ~ GPIO_MODER_MODE8 | GPIO_MODER_MODE9;
	GPIOD->MODER |= GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1;
	GPIOD->AFR[1] |= GPIO_AFRH_AFSEL8_0 | GPIO_AFRH_AFSEL8_1 | GPIO_AFRH_AFSEL8_2 | GPIO_AFRH_AFSEL9_0 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_2;

	//Configuración de USART3
	USART3->CR1 &= ~ USART_CR1_UE;
	USART3->BRR = BaudRate(16000000, 115200, 0);
	USART3->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;

	//Configuración de canal 3 de DMA1, para transmitir por USART3
	UsartConfig_DMA();

	NVIC_InteConfig(USART3_IRQn, 12);

	USART3->CR1 |= USART_CR1_UE;

}

/*
 * Mode_Transmit = 0: habilitar transmisión de UART por poleo
 * Mode_Transmit = 1: habilitar transmisión de UART por interrupción del periférico
 * Mode_Transmit = 2: habilitar transmisión de UART por DMA
 * */
char const * ptrDataTransmit_Int = (void*)0;
char * ptrDataTransmit_DMA = (void*)0;
char ArrayDestination[100];
void Usart_Transmit(char const* Data, uint8_t const Mode_Transmit)
{

	if ( Mode_Transmit == 1 )
	{
		ptrDataTransmit_Int = Data;
		USART3->CR1 |= USART_CR1_TXEIE;
		return;
	} else if (Mode_Transmit == 2)
	{
		strcpy(ArrayDestination, Data);

		ptrDataTransmit_DMA = ArrayDestination;

		DMA1_Stream3->CR &= ~DMA_SxCR_EN;
		while ( (DMA1_Stream3->CR & DMA_SxCR_EN) );

		DMA1_Stream3->PAR = 0x40004800 + 0x04; // Escribir la dirección del registro de datos
		DMA1_Stream3->M0AR = (uint32_t)(ptrDataTransmit_DMA); // Escribir la dirección de la localidad de memoria inicial de la cadena a transmitir
		DMA1_Stream3->NDTR = ( strlen(ptrDataTransmit_DMA) ); // Cantidad de Bytes a transmitir

		USART3->CR3 |= USART_CR3_DMAT; // Habilitar USART para transmitir por DMA

		DMA1_Stream3->CR |= DMA_SxCR_EN; // habilitar trasmisión
		return;
	}

	while (*Data)
	{
		while ( !(USART3->SR & USART_SR_TXE) );
		USART3->DR |= *Data++;
	}
}

#endif /* CONFIG_H_ */
