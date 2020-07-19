

/**
 * main.c
 */
#include "tm4c123gh6pm.h"
int count = 1;
int flag = 0;
int flag1 = 0;

void redLight(void);
void yellowLight(void);
void greenLight(void);
void timer0(void);
void timer1(void);
void timer2(void);
void switch1_interrupt(void);

int main(void)
{

        /*Enable for PORTF Clock gate*/
        SYSCTL_RCGCGPIO_R |= 1 << 5;

        /*Unlock PortF0*/
        GPIO_PORTF_LOCK_R = 0x4C4F434B;

        /*Set the Led Direction pin to output*/
        GPIO_PORTF_DIR_R |= 1 << 2;
        GPIO_PORTF_DIR_R |= 1 << 1;
        GPIO_PORTF_DIR_R |= 1 << 3;

        /*Set the switches direction to input*/
        GPIO_PORTF_DIR_R &= ~( (1 << 4));

        /*Set Led Pin mode to GPIO*/
        GPIO_PORTF_AFSEL_R &= ~(1 << 2);
        GPIO_PORTF_AFSEL_R &= ~(1 << 4);

        /*set switches and LEDs mode to GPTIO*/
        GPIO_PORTF_AFSEL_R &= ~((1 << 1)| (1 << 2)| (1 << 3)| (1 << 4) );

        /*Set maximum output current for the LED pin to 2 mA*/
        GPIO_PORTF_DR2R_R |= 1 << 2;

        /*Enable to the Pull down resistor of the LED pin*/
        GPIO_PORTF_PDR_R |= 1 << 2;
        GPIO_PORTF_PDR_R |= 1 << 1;
        GPIO_PORTF_PDR_R |= 1 << 3;

        /*Enable pull up resistor for the Switches pin*/
        GPIO_PORTF_PUR_R |=  (1 << 4);

        /*Enable the Digital functionality of the LED pin*/
        GPIO_PORTF_DEN_R |= 1 << 2;
        GPIO_PORTF_DEN_R |= 1 << 1;
        GPIO_PORTF_DEN_R |= 1 << 3;

        /*Enable the digital functionality of Switches*/
        GPIO_PORTF_DEN_R |=  (1 << 4);

        /*Set interrupt sense of the switch to edge*/
        GPIO_PORTF_IS_R &= ~(1 << 4);

        /*Set interrupt edge to single edge*/
        GPIO_PORTF_IBE_R &= ~(1 << 4);

        /*Set the interrupt event to falling edge*/
        GPIO_PORTF_IEV_R &= ~(1 << 4);

        /*unmask the GPIO pin interrupt*/
        GPIO_PORTF_IM_R |= (1 << 4);

        /*enable the PORTF interrupt*/
        NVIC_EN0_R |= (1 << 30);
        NVIC_EN4_R |= (1 << 30);

        /*Enable Timer Clock gate*/
        SYSCTL_RCGCTIMER_R |= 1 << 0;
        SYSCTL_RCGCTIMER_R |= 1 << 1;
        SYSCTL_RCGCTIMER_R |= 1 << 2;

        /*Select 16 bit timer*/
        TIMER0_CFG_R = 0x04;
        //select 32bit timer
        TIMER1_CFG_R = 0x00;
        TIMER2_CFG_R = 0x00;

        /*Set timer mode to periodic mode*/
        TIMER0_TAMR_R |= 0x02;
        TIMER1_TAMR_R |= 0x02;
        TIMER2_TAMR_R |= 0x02;

        //set count direction to count down
        //TIMER_TAMR_TACDIR &= ~(1 << 4);

        /*Set maximum count in the Load inteterval*/
        TIMER0_TAILR_R = 640000;
        TIMER1_TAILR_R = 211000000;
        TIMER2_TAILR_R = 700000000;


        /*Set prescaler value*/
        TIMER0_TAPR_R = 250;
        TIMER1_TAPR_R = 250;
        TIMER2_TAPR_R = 250;

        /*Enable Timerout interrupt mask     */
        TIMER0_IMR_R |= 1 << 0;
        TIMER1_IMR_R |= 1 << 0;
        TIMER2_IMR_R |= 1 << 0;

        /*Enable and start timer 0 A*/
        TIMER0_CTL_R |= 1 << 0;
        TIMER1_CTL_R |= 1 << 0;
        TIMER2_CTL_R |= 1 << 0;

        /*Enable Timer Interrupt at the NVIC*/
        NVIC_EN0_R |= 1 << 19;
        NVIC_EN0_R |= 1 << 21;
        NVIC_EN0_R |= 1 << 23;



        while(1){
            if(count == 1)
                //enable pull up resistor for the switch
                  GPIO_PORTF_PUR_R |=  (1 << 4);

            else
                //disable pull up resistor for the switch
                GPIO_PORTF_PUR_R &=  ~(1 << 4);



        }

    return 0;
}

void greenLight(void){

    static unsigned char state = 0x00;

    state ^= 0xff;
    //RED light oFF
    GPIO_PORTF_DATA_BITS_R(1 << 1) = 0x00;
    //GREEN light ON
    GPIO_PORTF_DATA_BITS_R(1 << 3) = state;
}

void yellowLight(void){


    static unsigned char state = 0x00;

    state ^= 0xff;
    //RED LIGHT ON
    GPIO_PORTF_DATA_BITS_R(1 << 1) = state;
    //GREEN light ON
    GPIO_PORTF_DATA_BITS_R(1 << 3) = state;



}


void redLight(void){


   static unsigned char state = 0x00;

   state ^= 0xff;
   //RED light ON
   GPIO_PORTF_DATA_BITS_R(1 << 1) = state;
   //GREEN light OFF
   GPIO_PORTF_DATA_BITS_R(1 << 3) = 0x00;

}


// one second timer
void timer0(void){
    TIMER0_ICR_R |= 1 << 0;

    if(count == 1)
        redLight();

    if(count == 2){

        yellowLight();

        flag = 1;

    }


    if(count >= 3){
        greenLight();


    }

}

// 10 second timer
void timer1(void){
    TIMER1_ICR_R |= 1 << 0;

    if(flag == 1){

        count++;
        flag1 = 1;
        greenLight();

     }
}

// 30 second timer
void timer2(void){

    TIMER2_ICR_R |= 1 << 0;

    if(flag1 == 1){
        redLight();
        flag = 0;
        flag1 = 0;
        count = 1;
    }

}


void switch1_interrupt(void){

    TIMER0_ICR_R |= 1 << 0;

    GPIO_PORTF_ICR_R |= (1 << 4);


    if(GPIO_PORTF_DATA_BITS_R(1 << 4) == 0x00){
         count++;
    }




}

