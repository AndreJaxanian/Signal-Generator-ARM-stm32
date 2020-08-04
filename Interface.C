#include <stm32f4xx.h>
#include <math.h>
#include <stdio.h>


unsigned int mode,ANALREAD,FREQ,TIME;
unsigned int Stud_id[] = {9,6,2,4,3, 0, 0, 2 };


void Delay_generat(int ms) {
	int i = 0;
	int j = 0;
	for(;j<ms;j++) {
		for(;i<20000;i++){}
	}
}
void LCD(unsigned char cmd) {
	  GPIOB->BSRR = (0x0020 | 0x0010) << 	16; 
    GPIOC->ODR = cmd;          			 
    GPIOB->BSRR = 0x0040;               	
    Delay_generat(0);
    GPIOB->BSRR = 0x0040 << 16;         	

    if (cmd < 4)
        Delay_generat(2);        		 
    else
        Delay_generat(1);        		 
}


void Data_LCD(unsigned char d){
    GPIOB->BSRR = 0x0020;              
    GPIOB->BSRR = 0x0010 << 16;        
    GPIOC->ODR = d;             		 
    GPIOB->BSRR = 0x0040;              
    Delay_generat(0);
    GPIOB->BSRR = 0x0040 << 16;        

    Delay_generat(1);
}


void LCD_STRING (char* msg)			//display String in LCD
{
	for(int i=0 ; i <= strlen(msg) ; i++){
		Data_LCD(msg[i]);
		LCD(0x06);
	}
	Delay_generat(1000000);	

}


void setcurser_first(int shiftam){
	for (int i= 0 ; i <=shiftam ; i++){
		LCD(0x10);
	}
}

void setcurser_Right(int shiftam){		//move curser Right one time
	for (int i= 0 ; i <=shiftam ; i++){
		LCD(0x06);
	}
}

void delayUs(int n)
{
	int i; int j;
	for(i = 0 ; i < n; i++) {
	for(j = 0; j < 8; j++) ;
}
}


void USART_INIT(){
	//clock for GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |=  0xC;           /* PA1 analog */

    /* setup ADC1 */
    RCC->APB2ENR |= 0x00000100;     /* enable ADC1 clock */
    ADC1->CR2 = 0;                  /* SW trigger */
    ADC1->SQR3 = 1;                 /* conversion sequence starts at ch 1 */
    ADC1->SQR1 = 0;                 /* conversion sequence length 1 */
    ADC1->CR2 |= 1;                 /* enable ADC1 */




    RCC->APB1ENR |= 0x20000;    /* Enable USART2 clock */

       /* Configure PA3 for USART2 RX */
    GPIOA->AFR[0] &= ~0xF000;
    GPIOA->AFR[0] |=  0x7000;   /* alt7 for USART2 */
    GPIOA->MODER  &= ~0x00C0;
    GPIOA->MODER  |=  0x0080;   /* enable alternate function for PA3 */

    USART2->BRR = 0x0683;       /* 115200 baud @ 16 MHz */
    USART2->CR1 = 0x0004;       /* enable Rx, 8-bit Data_LCD */
    USART2->CR2 = 0x0000;       /* 1 stop bit */
    USART2->CR3 = 0x0000;       /* no flow control */
    USART2->CR1 |= 0x2000;      /* enable USART2 */


	 /* Configure PA2 for USART2_TX */
    GPIOA->AFR[0] &= ~0x0F00;
    GPIOA->AFR[0] |=  0x0700;   /* alt7 for USART2 */
    GPIOA->MODER  &= ~0x0030;
    GPIOA->MODER  |=  0x0020;   /* enable alternate function for PA2 */

    USART2->BRR = 0x0683;       /* 9600 baud @ 16 MHz */
    USART2->CR1 = 0x0008;       /* enable Tx, 8-bit Data_LCD */
    USART2->CR2 = 0x0000;       /* 1 stop bit */
    USART2->CR3 = 0x0000;       /* no flow control */
    USART2->CR1 |= 0x2000;      /* enable USART2 */

}

void LCD_INIT(){
			//INITIALIZE LCD
		Delay_generat(30); 		
		LCD(0x30);
		Delay_generat(10);
		LCD(0x30);
		Delay_generat(1);
		LCD(0x30);
		LCD(0x38); 
		LCD(0x06);
		LCD(0x01);
		LCD(0x0F);
}

void PAB_INIT(){			//init port a and b 
	//clock for GPIOC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER = 0x55555555;
	
	//clock for GPIOB
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	//MODER PORT B
	GPIOB->MODER = 0x55005555;
}

int PotenMeter_Read(){
		unsigned int result;
		ADC1->CR2 |= 0x40000000;        /* start a conversion */
  	while(!(ADC1->SR & 2)) {}       /* wait for conv complete */
  	result = ADC1->DR;              /* read conversion result */
  	return result;             /* read conversion result */
}

/* Write a character to USART2 */
void USART2_write (int ch) {
    while (!(USART2->SR & 0x0080)) {}   // wait until Tx buffer empty
    USART2->DR = (ch & 0xFF);
	LCD(0X01);
	Data_LCD(ch);
	Delay_generat(1000);
}

char USART2_read(void) {
    while (!(USART2->SR & 0x0020)) {}   // wait until char arrives
    return USART2->DR;
}
 
 int bounding(int range , int mul , int div , int sum){
	 range = ((range *mul) / div ) + sum ;
	 
	 return range;
 }
 
 
int KeyPad_Ipnut() {
	int chosen = 12  ;
	GPIOB->ODR = 0xEFFF;
	int input = GPIOB->IDR;
	switch(input) {
		case 0xEEFF:   chosen = 1 ; break;		
		case 0xEDFF:   chosen = 2 ; break;
		case 0xEBFF:   chosen = 3 ; break;
	}
	
	GPIOB->ODR = 0xDFFF;
	input = GPIOB->IDR;
	switch(input) {
		case 0xDEFF: chosen = 4 ; break;
		case 0xDDFF: chosen = 5 ; break;
		case 0xDBFF:  chosen = 6 ; break;
	}
		
	GPIOB->ODR = 0x7FFF;
	input = GPIOB->IDR;
	switch(input) {
		case 0x7BFF:   chosen = 11 ; break;
	}
	return chosen;
}
void Greeting_Input(){
	
	
		char* word = "Hello*_*";
		LCD(0x80);
		LCD_STRING(word);
		setcurser_first(8);
		word = "please press:";
		LCD(0xC0);
		LCD_STRING(word);
		setcurser_first(13);
		LCD(0x01);
	  
	
		word = "1_Sine";
		LCD(0x80);
		LCD_STRING(word);
		setcurser_first(6);
		word = "2_Square";
		LCD(0xC0);
		LCD_STRING(word);
		setcurser_first(8);
		LCD(0x01	);
		
		
		
		
		
		word = "3_Triangle";
		LCD(0x80);
		LCD_STRING(word);
		setcurser_first(10);
		LCD(0xC0);
		word = "4_Sine_ABS";
		LCD_STRING(word);
		setcurser_first(10);
		LCD(0x01	);
		
		

		
		word = "5_Step_wave";
		LCD(0x80);
		LCD_STRING(word);
		setcurser_first(11);		
		LCD(0xC0);
		word = "6_SawTeeth";
		LCD_STRING(word);
		setcurser_first(10);
		LCD(0x01	);
		
}

char * conv(int integer){
char* number;
sprintf(number,"%ld",integer);
return number;
}

void Disp_Time(int t , char* a ,char* b){
		char* num;
		LCD_STRING(a);
		//setcurser_Right(7);
		num = conv(t);
		LCD(0x80);
		for (int i = 0; i < strlen(num); i++){
																
			Data_LCD(num[i]);
		}
		LCD_STRING(b);
		Delay_generat(1000000);
		LCD(0x01);
}
void USART2_write_str (char* text, char status) {
	for (int i = 0; text[i] != '\0'; ++i) {
		USART2_write(status);
		Delay_generat(50);
		USART2_write(text[i]);
		Delay_generat(50);
	}
}
char *convert_int2str(int number) {
	char* result;
	sprintf(result, "%d", number);
	return result;
}

int main() {
	int time;
	int freq;
	PAB_INIT();
	
	//GPIOC USART CONFIGURATION
	USART_INIT();
	
	LCD_INIT();
 
	
	
		//while(1) {
		LCD(1);  //clear display 
		Delay_generat(500);
		LCD(0x80);  //set cursor at first line 

		Data_LCD(Stud_id[0] + '0');
		Data_LCD(Stud_id[1] + '0');
		Data_LCD(Stud_id[2] + '0');
		Data_LCD(Stud_id[3] + '0');	
		Data_LCD(Stud_id[4] + '0');
		Data_LCD(Stud_id[5] + '0');
		Data_LCD(Stud_id[6] + '0');
		Data_LCD(Stud_id[7] + '0');

		Delay_generat(1000000);
		LCD(0x01	);
		

		
		LCD(0x01);
		
		char* word ;
		while(1){
			Greeting_Input();

			do{
				mode = KeyPad_Ipnut()-1;
			}while(	mode >= 5	);
	
		word = "Enter Time:";
		LCD_STRING(word);
		setcurser_first(16);
		LCD(0x01);
			
		do{
			time = PotenMeter_Read();
			time = bounding(time , 9500 , 4095 ,500);
			if(time!=TIME){
				TIME = time;
				Disp_Time(TIME , "time :" , "MS");
			}
		}while(KeyPad_Ipnut()!=11);

		word = "Enter frequency:";
		LCD_STRING(word);
		setcurser_first(16);
		LCD(0x01);
		
		do{
			freq = PotenMeter_Read();
			freq = bounding(freq , 999 , 4095 ,1);
			if(freq!=FREQ){
				FREQ = freq;
				Disp_Time(FREQ,"frequency: " , "HZ");
			}
		}while(KeyPad_Ipnut()!=11);

		word = "making packets: ";
		LCD_STRING(word);
		setcurser_first(16);
		LCD(0x01);
		
	USART2_write(0xFF & mode); 
	USART2_write(0xFF & TIME );
	USART2_write(TIME >> 8); 
	USART2_write(0xFF & FREQ ); 
	USART2_write(FREQ >> 8);
	
	while(USART2_read() != 'E'){

	}
			
}

}

















