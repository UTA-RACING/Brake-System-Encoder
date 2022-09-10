/*
*Pedal Position Sensor Code Using TIVA C for 3 separate analog inputs
*Author: ELijah Cavanaugh
*/

//Libraries
#include<stdint.h>
#include<stdbool.h>
#include"inc/hw_memmap.h"
#include"inc/tm4c123gh6pm.h"
#include"driverlib/sysctl.h"
#include"driverlib/gpio.h"
#include"driverlib/timer.h"
#include"driverlib/interrupt.h"
#include"driverlib/adc.h"

uint32_t x = 0;
uint32_t y = 0;
uint32_t z = 0;
uint32_t r = 0;
uint32_t s = 0;
uint32_t t = 0;
uint32_t a = 0;
uint32_t b = 0;
uint32_t c = 0;

int main(void) {
    //Set microcontroller clock
    SysCtlClockSet(SYSCTL_XTAL_16MHZ|SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN);
    //Enable LEDs
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    //Enable ADC
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    //Enable analog input pin for E3, E2, E1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1);
    //Set Sampling Sequence
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    //Pin E3
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);
    //Pin E2
    ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 2);
    ADCIntClear(ADC0_BASE, 2);
    //Pin E1
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH2|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 1);
    ADCIntClear(ADC0_BASE, 1);

    while(1)
    {
        ADCProcessorTrigger(ADC0_BASE, 3);

        //PE3
        while(!ADCIntStatus(ADC0_BASE, 3, false))
        {
        }
        ADCIntClear(ADC0_BASE, 3);
        ADCSequenceDataGet(ADC0_BASE, 3, &s);

        //PE2
        ADCProcessorTrigger(ADC0_BASE, 2);
        while(!ADCIntStatus(ADC0_BASE, 2, false))
        {
        }
        ADCIntClear(ADC0_BASE, 2);
        ADCSequenceDataGet(ADC0_BASE, 2, &r);

        //PE1
        ADCProcessorTrigger(ADC0_BASE, 1);
        while(!ADCIntStatus(ADC0_BASE, 1, false))
        {
        }
        ADCIntClear(ADC0_BASE, 1);
        ADCSequenceDataGet(ADC0_BASE, 1, &t);

        /*//Compare PE3 and PE2
        x = r/s;
        if (x < 1){
            x = 1-x;
        }else{
            x = s/r;
            x = 1-x;
        }

        //Compare PE2 and PE1
        y = r/t;
        if (y < 1){
            y = 1-y;
        }else{
            y = t/r;
            y = 1-y;
        }

        //Compare PE3 and PE1
        z = s/t;
        if (z < 1){
            z = 1-z;
        }else{
            z = t/s;
            z = 1-z;
        }*/


        //If 2 out of 3 pins are less than 10% apart, then light up LED (LED to prove its working and check functionality)
        if (0 < r ){
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,10);

                //Percent Pressed when starting from 0V
                a = (r/4095)*100;

                if( r>1024 && t>25){
                        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,10);
                        if(r<205)
                            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,10);

                }


                //Percent Pressed when starting from 3.3V
                // a = (1-(r/4095))*100;

        /*}else if (0 < y < 0.10){
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,10);

                //Percent Pressed when starting from 0V
                b = (r/4095)*100;

                //Percent Pressed when starting from 3.3V
                // b = (1-(r/4095))*100;

        //}else if (0 < z < 0.10){
                //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,10);

                //Percent Pressed when starting from 0V
                //c = (s/4095)*100;

                //Percent Pressed when starting from 3.3V
                // c = (1-(s/4095))*100;*/

            }else{
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,0);

                a = 0;
                b = 0;
                c = 0;
                // Open Shutdown Circuit
                //send error message via can
                //needs code
            }

        //send a, b, or c value to motor controller & dash via CAN

        //needs code

    }
}
