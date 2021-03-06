/*
 * File:   Lab03Master.c
 * Author: acer
 *
 * Created on 29 de julio de 2021, 01:46 AM
 */
//Configuración PIC16F887
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Progra
//****DirectivasCompliador*****//
#define _XTAL_FREQ 4000000
//*****IMPORTACION DE LIBRERIAS******
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <pic16f887.h>
#include <math.h>
#include <stdlib.h>
#include "UART3.h"
#include "SPI_Master3.h"
#include <string.h>
uint8_t contador = 0;       /*Declaración variables*/
char string2[20];
uint8_t var1;
uint8_t var2;
int cambio = 0; 
uint8_t bandera = 0;
int banderas = 0;
float v1;
float v2;
float v3 = 0;
char s[20];
//***********Prototipos de funciones************
void setup(void);           /*funcion principal */
void valorsensores(void);
void configuracionUART(void);
//**************Interrupciones**************
void __interrupt()isr(void) /*interrupciones*/
{

}
//*********************************funcionPrincipal**********
void main(void) {
    setup();                /*funcion de configuracion principal*/
    //*********************************LoopPrincipal*********
    while(1)
    {
        //PORTA = contador;
        v1 = (var1*0.0196);             //Operacion para rango 0-5v
        v2 = (var2*0.0196);
        uint8_t bandera = 0;
        PORTCbits.RC2 = 0;       //Slave Select
         __delay_ms(1);
       
        spiWrite(bandera);
        if(banderas == 0)
        {
            var1 = spiRead();
            //var1 = string2;
            //sscanf(string2, "%d", &var1);
            banderas = 1;
        }
        else
        {
            var2 = spiRead();
            banderas = 0;
        }
        __delay_ms(1);
        PORTCbits.RC2 = 1;       //Slave Deselect 
        if (PIR1bits.RCIF)
        {
            PORTA = RCREG;
            contador = PORTA;
        }
        
        valorsensores();
    }
    //return;
}
//*************Funciones************************
void setup(void)
{ 
    //*******configuración io entradas y salidas****
    ANSEL = 0x00;
    ANSELH = 0x00;     //Salidas Digitales
    
    TRISCbits.TRISC2 = 0;
    TRISE = 0x00;  
    TRISA = 0x00;
    TRISB = 0x00;
    TRISD = 0x00;
    //TRISCbits.TRISC2 = 0;
    PORTA = 0x00; 
    //PORTC = 0x00; 
    PORTE = 0x00;
    PORTB = 0x00;
    PORTD = 0x00;
    
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV16, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    //configuración del reloj (oscilador interno)
    OSCCONbits. IRCF2 = 1; 
    OSCCONbits. IRCF1 = 1;
    OSCCONbits. IRCF0 = 0;  //4Mhz
    OSCCONbits. SCS = 1;
    //configuración Tmr0

    //CONFIGURACIOS ADC
    /*ADCON1bits.ADFM = 0;        //justificado a la izquierda 
    ADCON1bits.VCFG0 = 0;       //Voltaje VDD referencia
    ADCON1bits.VCFG1 = 0;       //Voltaje Vss referencia
    
    ADCON0bits.ADCS = 1;        //ADC Clock FOSC = 4Mhz/8
    ADCON0bits.CHS = 5;         //Canal 5
    __delay_us(100);
    ADCON0bits.ADON = 1;        //Habiliar Modulo de ADC*/
    //*****ConfiguraciónUART TX Y RX ***********
    configuracionUART();
    //configuración interrupción
    //INTCONbits. GIE = 1;        //Globales
    //INTCONbits.PEIE = 1;        //Perifericas
    
}
void configuracionUART(void)
{
    TXSTAbits.SYNC = 0;             //Modo asíncrono
    TXSTAbits.BRGH = 1;             //Seleccion BAUD RATE
    BAUDCTLbits.BRG16 = 0; 
    
    SPBRG = 25;                     //Registros para valor BAUD RATE
    SPBRGH = 0; 
    
    RCSTAbits.SPEN = 1;         //Habilitar puerto serial asíncrono
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;         //Habilitar recepción de datos 

    TXSTAbits.TXEN = 1;         //Habilitar transmision
}
