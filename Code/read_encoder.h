#include "stm32f10x.h"                  // Device header
#include "gp_drive.h"
#include <stdlib.h>

//Cac chan doc ENCODER
#define ENCODER_A_port		PA
#define ENCODER_B_port		PA
#define ENCODER_A_pin			4
#define ENCODER_B_pin			5

typedef struct{
	char encoder_A_port, encoder_A_pin;
	char encoder_B_port, encoder_B_pin;
}ENCODER;

static long pluse_Encoder = 0;

void EXTI4_IRQHandler(void);

void Init_Encoder(ENCODER *encoder, char channe_A_port, char channe_A_pin, char channe_B_port, char channe_B_pin);

long Pluse_Return(void);




