#include <stdint.h>
#include "tm4c123gh6pm.h"

#define RED_LED (1U << 1)   // Red LED on port F, pin 1]
#define SW1 (1U << 4)           // switch pin 4

void initial_setup(void);
void GPIO_Handler(void);

int main(void){
   initial_setup();

   while(1){

   }
}

void GPIO_Handler(void) {
    if (GPIO_PORTF_MIS_R & 0x10){
        GPIO_PORTF_DATA_R ^= RED_LED;  // Toggle RED LED
    }
}

void initial_setup(void) {
    // Enable clock for GPIO port F
    SYSCTL_RCGC2_R |= 0x00000020;       /* enable clock to GPIOF */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     /* unlock commit register */
    GPIO_PORTF_CR_R = 0x1F;             /* make PORTF0 configurable */
    GPIO_PORTF_DEN_R = 0x1F;    //1E        /* set PORTF pins 4 pin */
    GPIO_PORTF_DIR_R = 0x0E;            /* set PORTF4 pin as input user switch pin */
    GPIO_PORTF_PUR_R = 0x11;    //10        /* PORTF4 is pulled up */

    // Interrupt configuration
    GPIO_PORTF_IS_R &= ~SW1;    // Make PF4 edge-sensitive
    GPIO_PORTF_IBE_R &= ~SW1;   // Disable both edges, we use IEV
    GPIO_PORTF_IEV_R &= ~SW1;   // Falling edge trigger (when switch is pressed)
    GPIO_PORTF_ICR_R |= SW1;    // Clear any prior interrupt
    GPIO_PORTF_IM_R |= SW1;     // Unmask interrupt for SW1

    // Enable interrupt in NVIC
    NVIC_EN0_R |= (1 << 30);    // Enable IRQ30 (Port F)

}
