#include "read_encoder.h"

void Init_Encoder(ENCODER *encoder, char channel_A_port, char channel_A_pin, char channel_B_port, char channel_B_pin){
	encoder->encoder_A_port = channel_A_port;
	encoder->encoder_A_pin  = channel_A_pin;
	
	encoder->encoder_B_port = channel_B_port;
	encoder->encoder_B_pin  = channel_B_pin;	
	
	init_GP(encoder->encoder_A_port, encoder->encoder_A_pin, IN, I_PP);
	init_GP(encoder->encoder_B_port, encoder->encoder_B_pin, IN, I_PP);
	
	__disable_irq();						
	AFIO->EXTICR[1] = 0x10;	
	EXTI->IMR |= 0x10;			
	EXTI->RTSR |= 0x10;			
	NVIC_EnableIRQ(EXTI4_IRQn);	
	__enable_irq();	
}

void EXTI4_IRQHandler(void){
	EXTI->PR |= 1;	
	int x = (int)(pluse_Encoder/1000);
	if(R_GP(ENCODER_B_port, ENCODER_B_pin)==1){
		if(pluse_Encoder >= 1000) pluse_Encoder -= abs(x)*1000;
		pluse_Encoder++;			
	}
	else{
		if(pluse_Encoder <= -1000) pluse_Encoder += abs(x)*1000;
		pluse_Encoder--;			
	}
}

long Pluse_Return(void){
	return pluse_Encoder;
}


