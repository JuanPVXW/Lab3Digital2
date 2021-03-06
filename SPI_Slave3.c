/*
 * File:   SPI_Slave3.c
 * Author: acer
 *
 * Created on 29 de julio de 2021, 01:26 AM
 */


#include <xc.h>
#include "SPI_Slave3.h"

int banderas;
void spiInit(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge)
{
    TRISC5 = 0;
    if(sType & 0b00000100) //If Slave Mode
    {
        SSPSTAT = sTransmitEdge;
        TRISC3 = 1;
    }
    else              //If Master Mode
    {
        SSPSTAT = sDataSample | sTransmitEdge;
        TRISC3 = 0;
    }
    
    SSPCON = sType | sClockIdle;
}

static void spiReceiveWait()
{
    while ( !SSPSTATbits.BF ); // Wait for Data Receive complete
}

void spiWrite(char dat)  //Write data to SPI bus
{
    SSPBUF = dat;
}

unsigned spiDataReady() //Check whether the data is ready to read
{
    if(SSPSTATbits.BF)
        return 1;
    else
        return 0;
}

char spiRead() //REad the received data
{
    /*if(banderas == 0)
    {
    spiReceiveWait();        // wait until the all bits receive
    }
    else
    {*/
        spiReceiveWait();
    //}
    return(SSPBUF); // read the received data from the buffer
}
