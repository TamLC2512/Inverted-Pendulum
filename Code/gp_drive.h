//dia chi cua RCC->APB2ENR
//dia chi cua RCC: 0x4002 1000 (trang 50), APB2ENR: 0x18 (trang 146)
//nen dia chi cua RCC->APB2ENR: 0x4002 1018
#define RCC_APB2ENR (*((volatile unsigned long *) 0x40021018))
	
//dia chi cua GPIO A,B,C (trang 51)
#define GPIO_A  (*((volatile unsigned long *) 0x40010800))
#define GPIO_B  (*((volatile unsigned long *) 0x40010C00))
#define GPIO_C  (*((volatile unsigned long *) 0x40011000))
	
//List of PORTS
#define PA	1
#define PB	2
#define PC	3

//Cac che do Input va Output (trang 171)
#define IN	0
#define OUT10	 1  //dau ra 10 MHz
#define OUT2	 2  //dau ra 2 MHz
#define OUT50	 3  //dau ra 50 MHz

//Chi tiet ben trong che do Input (trang 171)
#define I_AN	0		//Analog Mode
#define I_F		1		//Floating input (reset state)
#define I_PP	2		//Input with pull-up/pull-down

//Chi tiet ben trong che do Onput (trang 171)
#define O_GP_PP 0U	//General purpose output push-pull
#define	O_GP_OD 1U //General purpose output Open-drain
#define O_AF_PP 2U //Alternate function output Push-pull
#define O_AF_OD 3U //Alternate function output Open-drain

//State of PIN
#define LOW  0
#define HIGH 1

void init_GP(unsigned short port, unsigned short pin, unsigned short dir, unsigned short opt);
int R_GP(unsigned short port, unsigned short pin);
void W_GP(unsigned short port, unsigned short pin, unsigned long state);
void toggle_GP(unsigned short port, unsigned short pin);

void BLED(unsigned short state);
void B_init(void);
void Digital_Input(unsigned short PORT,unsigned short PIN);
void Digital_Output(unsigned short PORT,unsigned short PIN);
void PINc(unsigned short pin, unsigned short STATUS);

