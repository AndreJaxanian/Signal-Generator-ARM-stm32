#include <stm32f4xx.h>
#include <math.h>

#define M_TWOPI 6.283185307179586476925286766559


void INIT_PORTA(void);
void INIT_PORTB(void);
void INIT_PORTC(void);
void delayMs(int ms);
char USART2_read(void);
void USART2_write (int ch);
int sgn(double input);

int u(double input){
	if(input >= 0) return 1;
	return 0;
}


void INIT_TIMER() {
	 RCC->APB1ENR |= 1;              /* enable TIM2 clock */
   TIM2->PSC = 2 - 1;          		 /* divided by 200 */
   TIM2->ARR = 4 - 1;          		/* divided by 400 */
   TIM2->CNT = 0;                  /* clear timer counter */
   TIM2->CR1 = 1;                  /* enable TIM2 */
}

unsigned int opcode;
unsigned int duration;
unsigned int frequency;
int period ;
int curr_index_per;
int main()
{
	unsigned int packet1;
	unsigned int packet2;
	unsigned int packet3;
	unsigned int packet4;
	unsigned int packet5;
	unsigned int time = 1;
	double omega;
	int i = 0;
	
	INIT_PORTA();
	INIT_PORTB();
	INIT_PORTC();
	while (1)
	{

		packet1 = USART2_read();
		packet2 = USART2_read();
		packet3 = USART2_read();
		packet4 = USART2_read();
		packet5 = USART2_read();

    	opcode = packet1;
    	duration = packet2 | packet3 << 8;
		frequency = packet4  | packet5 << 8;
		omega = M_TWOPI / frequency;


		switch (opcode)
		{
			case 0:
				
			
				while (time < duration*10)
				{
					GPIOC->ODR = (int)(10000*(sin((M_TWOPI/360)*frequency*time / 1000)+1)) <<8 ;
					time++;
					//delayMs(1);
					delayMs(1);
				}
				break;
				
				
				
			case 1:
				
			
			
				while (time < duration*10)
				{
					GPIOC->ODR = (64 * sgn(sin(frequency * M_TWOPI * time / 1000))) << 8;
					time++;
					delayMs(1);
				}
				break;
				
				
				
			case 2:
					period=(1000/frequency);
					while (time < duration * 100){
					curr_index_per = time % period;
					if(curr_index_per < period/2)
						GPIOC->ODR = (int)(((curr_index_per * 1.0 / (period/2)) + 1 ) * 16384.0);
					else if (curr_index_per < (3*period/2))
						GPIOC->ODR = (int)((1 - ((curr_index_per - period/2) * 1.0 / (period/2)) + 1) * 16384.0);
					else
						GPIOC->ODR = (int)(((((curr_index_per - (3*period/2)) * 1.0 / (period/2)) + 1 ) - 1) * 16384.0);
					time++;
					delayMs(1);
						}
			
				break;
						
						
						
			case 3:
				
			while(time < duration *100) {
				GPIOC->ODR = (int)(16384*(fabs(sin((M_TWOPI/360)*time*frequency/100))));	
				time++;
				
	}
			break;
			case 4:
				
			
				  while (time < duration * 100) {
		curr_index_per = time % period;
		GPIOC->ODR = (int)((u(curr_index_per - 1  * period / 20 ) + 
												u(curr_index_per - 2  * period / 20 ) + 
												u(curr_index_per - 3  * period / 20 ) +
												u(curr_index_per - 4  * period / 20 ) +
												u(curr_index_per - 5  * period / 20 ) +
												u(curr_index_per - 6  * period / 20 ) +
												u(curr_index_per - 7  * period / 20 ) +
												u(curr_index_per - 8  * period / 20 ) +
												u(curr_index_per - 9  * period / 20 ) +
												u(curr_index_per - 10 * period / 20 ) -
												u(curr_index_per - 11 * period / 20 ) -
												u(curr_index_per - 12 * period / 20 ) -
												u(curr_index_per - 13 * period / 20 ) -
												u(curr_index_per - 14 * period / 20 ) -
												u(curr_index_per - 15 * period / 20 ) -
												u(curr_index_per - 16 * period / 20 ) -
												u(curr_index_per - 17 * period / 20 ) -
												u(curr_index_per - 18 * period / 20 ) -
												u(curr_index_per - 19 * period / 20 ) -
												u(curr_index_per - 20 * period / 20 )) * 32768 / 20);
	
		time++;
		delayMs(1);
	             
	}
					
	
				break;
			case 5:
				
					while (time < duration * 100) {
					curr_index_per = time % period;
					if(curr_index_per < period/2)
						GPIOC->ODR = (int)(((curr_index_per *  1.0 / (period/2)) + 1 ) * 16384.0);
					else
						GPIOC->ODR = (int)((((curr_index_per - (period/2)) * 1.0 / (period/2) + 1 ) - 1) * 16384.0);
					time++;
					delayMs(1);
	}
			
				break;
		}

		
		GPIOC->ODR = 0x0000;
		time = 1;
		USART2_write('E');
		
		
	
		
	}
	


	
	
}

int sgn(double input)
{
	if (input > 0) return 2;
	if (input < 0) return 0;
	return 1;
}

/* Write a character to USART2 */
void USART2_write (int ch) {
    while (!(USART2->SR & 0x0080)) {}   // wait until Tx buffer empty
    USART2->DR = (ch & 0xFF);
		delayMs(1000);
}



char USART2_read(void) {
    while (!(USART2->SR & 0x0020)) {}   // wait until char arrives
    return USART2->DR;
}


void INIT_PORTA()
{
	//clock for GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= 0x20000;    /* Enable USART2 clock */

    /* Configure PA3 for USART2 RX */
    GPIOA->AFR[0] &= ~0xF000;
    GPIOA->AFR[0] |=  0x7000;   /* alt7 for USART2 */
    GPIOA->MODER  &= ~0x00C0;
    GPIOA->MODER  |=  0x0080;   /* enable alternate function for PA3 */

    USART2->BRR = 0x0683;       /* 115200 baud @ 16 MHz */
    USART2->CR1 = 0x0004;       /* enable Rx, 8-bit data */
    USART2->CR2 = 0x0000;       /* 1 stop bit */
    USART2->CR3 = 0x0000;       /* no flow control */
    USART2->CR1 |= 0x2000;      /* enable USART2 */


	 /* Configure PA2 for USART2_TX */
    GPIOA->AFR[0] &= ~0x0F00;
    GPIOA->AFR[0] |=  0x0700;   /* alt7 for USART2 */
    GPIOA->MODER  &= ~0x0030;
    GPIOA->MODER  |=  0x0020;   /* enable alternate function for PA2 */

    USART2->BRR = 0x0683;       /* 9600 baud @ 16 MHz */
    USART2->CR1 = 0x0008;       /* enable Tx, 8-bit data */
    USART2->CR2 = 0x0000;       /* 1 stop bit */
    USART2->CR3 = 0x0000;       /* no flow control */
    USART2->CR1 |= 0x2000;      /* enable USART2 */

}

void INIT_PORTB()
{
	//clock for GPIOB
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	//MODER PORT B
	GPIOB->MODER = 0x55555555;
}

void INIT_PORTC()
{
	//clock for GPIOC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER = 0x55555555;
}


void delayMs(int ms)
{
	int i = 0;
	int j = 0;
	for(;j<ms;j++)
	{
		for(;i<20000;i++)
		{
			
		}
	}
}



