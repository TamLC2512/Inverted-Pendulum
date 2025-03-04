#include "adc_drive.h" 

char adc_init(char adc, short port, short pin){
	char channel = 0; 
	char result = 0;
	if(port == PA){
		if(pin < 8){
			result = 1;
			channel = (char)pin;
		}
	}
	else if(port == PB){
		if(pin < 2){
			result = 1;
			channel = (char)pin + 8;
		}
	}
	else if(port == PC){
		if(pin < 6){
			result = 1;
			channel = (char)pin + 10;
		}
	}
	if(result){
		init_GP((unsigned short)port, (unsigned short)pin, IN, I_AN);
		if(adc == adc1){
			//Bat clock, kich hoat ADC1
			RCC->APB2ENR |= 0x201;
			//Tat ADC de cau hinh khong bi loi
			ADC1->CR2 = 0;
			//Chon kenh doc ADC
			ADC1->SQR3 = channel;
			//Bat lai ADC
			ADC1->CR2 |= 1;
			DelayMs(100);
			//Bat lai lan nua de bat dau qua trinh hieu chuan
			ADC1->CR2 |= 1;
			//Bat che do chuyen doi lien tuc
			ADC1->CR2 |= 2;
		}
		else if(adc == adc2){
			RCC->APB2ENR |= 0x401;
			ADC2->CR2 = 0;
			ADC2->SQR3 = channel;
			ADC2->CR2 |= 1;
			DelayMs(100);
			ADC2->CR2 |= 1;
			ADC2->CR2 |= 2;
		}
	}
	return result;
}

//Reading the flag that says the data is ready
char adc_check(char adc){
	char check = 0;
	
	if(adc == adc1){
		//kiem tra da hoan thanh chuyen doi chua
		//bit EOC cua thanh ghi SR
		if(ADC1->SR & 2){
			check = 1;
		}
	}
	else if(adc == adc2){
		if(ADC2->SR & 2){
			check = 1;
		}
	}
	return check;
}

//Reading the ADC value
int adc_rx(char adc){
	unsigned int result = 0;
	unsigned int data = 0;
	if(adc == adc1){
		data = ADC1->DR;
	}
	else if(adc == adc2){
		data = ADC2->DR;
	}
//	result = (data*1000)/0xFFF;
	result = data;
	return (int)result;
}
void adc_irq(char adc, char port, char pin)
{
	adc_init(adc, port, pin);// Initialize the ADC: adc_init(ADC1 or ADC2, Port, Pin)
	if(adc == 1)
	{
		ADC1->CR1 |= 0x20; // Setting the Interrupt
		ADC1->CR1 |= 0x40;
	}
	else if(adc == 2)
	{
		ADC2->CR1 |= 0x20; // Setting the Interrupt
		ADC2->CR1 |= 0x40;
	}
	
	__disable_irq();
	NVIC_EnableIRQ(ADC1_2_IRQn);
	__enable_irq();
}

void adc_wd(char adc, char port, char pin, short htr, short ltr)
{
		adc_init(adc, port, pin);// Initialize the ADC: adc_init(ADC1 or ADC2, Port, Pin)

		if(adc == 1)
	{
		ADC1->CR1 |= 0x800000; // Enable the watchdog
		ADC1->HTR = (unsigned int)htr;
		ADC1->LTR = (unsigned int)ltr;
		ADC1->CR1 |= 0x40;
	}
	else if(adc == 2)
	{
		ADC2->CR1 |= 0x800000; // Enable the watchdog
		ADC2->HTR = (unsigned int)htr;
		ADC2->LTR = (unsigned int)ltr;
		ADC2->CR1 |= 0x40;
	}
	
	__disable_irq();
	NVIC_EnableIRQ(ADC1_2_IRQn);
	__enable_irq();
}

void adc_multi_ch_init(char adc, char channels, char * adc_channels){
	int i = 0;
	//Initiate the pins
	for(i = 0; i<channels; i++){
		if(adc_channels[i]<8){
			init_GP(PA, adc_channels[i], IN, I_AN);
		} 
		else if(adc_channels[i]<10){
			init_GP(PB, adc_channels[i]-8, IN, I_AN);
		}
		else if(adc_channels[i]<16){
			init_GP(PC, adc_channels[i]-10, IN, I_AN); 
		}
	}
	if(adc == 1){
		//Setup the control registers
		RCC->APB2ENR |= 0x201; 
		ADC1->CR2 = 0;
		ADC1->CR2 |= 1;
		DelayMs(100);
		ADC1->SQR3 = adc_channels[1];
		ADC1->CR2 |= 2; //Continous reading
		ADC1->CR2 |= 1; //Starting the conversion
	}
	else if(adc == 2){
		//Setup the control registers
		RCC->APB2ENR |= 0x401; 
		ADC2->CR2 = 0;
		ADC2->CR2 |= 1;
		DelayMs(100);
		ADC2->SQR3 = adc_channels[1];
		ADC2->CR2 |= 2; //Continous reading
		ADC2->CR2 |= 1; //Starting the conversion
	}
}

void adc_multi_ch_rx(char adc, char channels, char* adc_channels, int* analog_rx){
	static unsigned int temp_rx = 0;
	int i = 0;
	
	if(adc == 1){
		while(1){
			if(adc_check(adc1)){
				temp_rx = ADC1->DR;
//				analog_rx[i] = (temp_rx*1000)/0xFFF;
				analog_rx[i] = (int)temp_rx ;
				i++;
				if(i == channels){
					i = 0;
					ADC1->SQR3 = adc_channels[i];
					break;
				}
				else {
					ADC1->SQR3 = adc_channels[i];
				}
			}
			
		}
	}
	else if(adc == 2){
		while(1){
			if(adc_check(adc1)){
				temp_rx = ADC2->DR;
//				analog_rx[i] = (temp_rx*1000)/0xFFF;
				analog_rx[i] = (int)temp_rx ;
				i++;
				if(i == channels){
				i = 0;
					ADC2->SQR3 = adc_channels[i];
					break;
				}
				else {
					ADC2->SQR3 = adc_channels[i];
				}
			}
			
		}
	}
	
}




