#include "stm32f10x.h"                  // Device header

//tham khao trang 171, 172
#include "gp_drive.h"

void init_GP(unsigned short port, unsigned short pin, unsigned short dir, unsigned short opt){
	volatile unsigned long * CR=0;
	unsigned short tPIN = pin;
	unsigned short offset = 0x00; //dia chi cua CRL trong bo nho GPIOx
	
	//do CRH nam sau CRL 4 byte nen offset=0x01
	if(pin>7){	//truong hop su dung cac port tu 8-15
		tPIN -= 8;//vi du pin=13 thi tPIN = 5
		offset = 0x01;
	}
	if(port == 1){
		RCC_APB2ENR |= 4;	//Enabling PORT A
		//gan CR = dia chi GPIOA + offset
		//neu dung port tu 0-7 thi CR = &GPIOA + 0x00
		//neu dung port tu 8-15 thi CR = &GPIOA + 0x01
		//do ep kieu (unsigned long *) nen khi cong 0x01 se tuong duong 4 byte
		CR = (volatile unsigned long *) (&GPIO_A + offset);
		//ket qua: neu dung CRL -> CR = &GPIOA = 0x40010800
		//				 neu dung CRH -> CR = &GPIOA + 4byte= 0x40010804
	}
	if(port == 2){
		RCC_APB2ENR |= 8;	//Enabling PORT B
		//nguyen tat tuong tu GPIOA
		CR = (volatile unsigned long *) (&GPIO_B + offset);
	}
	if(port == 3){
		RCC_APB2ENR |= 16;	//Enabling PORT C
		//nguyen tat tuong tu GPIOA
		CR = (volatile unsigned long *) (&GPIO_C + offset);
	}
	
	//den day thi CR tuong ung voi CRH hoac CRL 
	
	//Reset the target PIN
	//vi du pin=13 thi tPIN = 5 -> tPIN*4 =20, dung vi tri cua bit MODE13
	*CR &= ~(0xFU << (tPIN)*4);		
	//Setup the direction and the option of the PIN
	//dir dai dien cho cach chon INPUT hay OUTPUT (bit MODEx)
	//opt dai dien cho che do muon su dung (bit CNFx)
	*CR |= (((unsigned long)dir << (tPIN*4)) | ((unsigned long)opt << (tPIN*4+2)));
}

int R_GP(unsigned short port, unsigned short pin){
	volatile unsigned long * IDR = 0;
	unsigned long offset = 0x02; //moi thanh ghi chiem 4 byte nen 0x02 <=> 8 byte
	int state;
	
	if(port == 1){
		IDR = (volatile unsigned long *)(&GPIO_A + offset);
	}
	else if(port == 2){
		IDR = (volatile unsigned long *)(&GPIO_B + offset);
	}
	else if(port == 3){
		IDR = (volatile unsigned long *)(&GPIO_C + offset);
	}
	state = (int)((*IDR & (1<<pin)) >> pin);
	//(1<<pin): dich bit 1 sang vi tri thu (pin) can doc 
	//*IDR & (1<<pin): dung phep AND de lay gia tri can doc
	//(>>pin): dich gia tri can doc ve vi tri thu 0
	
	//Vi du: pin=5 dang o muc cao -> IDR co the bang 0x20
	//Nen:(*IDR & (1<<5))>>5 = (0x20 & 0x20)>>5 = 0x20 >> 5 = 1 => state = 1
	//Con truong hop pin 5 dang o muc thap thi khong co IDR nen state = 0
	return state;
}

void W_GP(unsigned short port, unsigned short pin, unsigned long state){
	volatile unsigned long * ODR = 0;
	unsigned long offset = 0x03; 
	
	if(port == 1){
		ODR = (volatile unsigned long *)(&GPIO_A + offset);
	}
	else if(port == 2){
		ODR = (volatile unsigned long *)(&GPIO_B + offset);
	}
	else if(port == 3){
		ODR = (volatile unsigned long *)(&GPIO_C + offset);
	}
	if (state)
    *ODR |= (1U << pin);  // Set bit
	else
    *ODR &= ~(1U << pin); // Clear bit
}
void toggle_GP(unsigned short port, unsigned short pin){
	if(R_GP(port, pin)){
		W_GP(port, pin, 0);
	}
	else{
		W_GP(port, pin, 1);
	}
}




void PINc(unsigned short pin, unsigned short STATUS)
{
	STATUS ? (GPIOC->ODR |= (1U<<pin)) : (GPIOC->ODR &= ~(1U<<pin));
}

void B_init(void)
{
	init_GP(PC,13,OUT50,O_GP_PP);
}

void BLED(unsigned short state)
{
	W_GP(PC,13,state);
}

void Digital_Input(unsigned short PORT,unsigned short PIN)
{
	init_GP(PORT, PIN, IN, I_PP);
}

void Digital_Output(unsigned short PORT,unsigned short PIN)
{
	init_GP(PORT, PIN, OUT50, O_GP_PP);
}





