/**
 * CMP1 Generated Driver Source File
 * 
 * @file      cmp1.c
 *            
 * @ingroup   cmpdriver
 *            
 * @brief     This is the generated driver source file for CMP1 driver
 *
 * @skipline @version   PLIB Version 1.0.1
 *            
 * @skipline  Device : dsPIC33AK128MC106
*/


/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

// Section: Included Files

#include <xc.h>
#include "../cmp1.h"

#ifndef CMP_FPDMDAC_ADDRESS
#define CMP_FPDMDAC_ADDRESS 0x7F20B0UL
#endif //CMP_FPDMDAC_ADDRESS

// Section: File specific functions

static void (*CMP1_EventHandler)(void) = NULL;

// Section: Driver Interface
static const struct DAC_DC_INTERFACE dac1_dc_interface = {
    .Enable = &CMP1_DACEnable,
    .Disable = &CMP1_DACDisable,
    .DataWrite = &CMP1_DACDataWrite,
};

const struct CMP_INTERFACE CMP_DAC1 = {
    .Initialize = &CMP1_Initialize,
    .Deinitialize = &CMP1_Deinitialize,
    .Enable = &CMP1_Enable,
    .Disable = &CMP1_Disable,
    .StatusGet = &CMP1_StatusGet,
    .Calibrate = &CMP1_Calibrate,
    
    .EventCallbackRegister = &CMP1_EventCallbackRegister,
    .Tasks = &CMP1_Tasks,
    .cmp_dac_dc_interface = &dac1_dc_interface,
    .cmp_dac_slope_interface = NULL,
};

// Section: CMP1 Module APIs

void CMP1_Initialize(void)
{           
    // Comparator Register settings
    DACCTRL1 = 0x3F7F0000UL; //FCLKDIV 1:1; DNLADJ 0x0; SIDL disabled; ON disabled; NEGINLADJ 127; POSINLADJ 63; RREN disabled; 
    DACCTRL2 = 0x8A0001UL; //TMODTIME 1; SSTIME 138; 
    DAC1CON = 0x8200UL; //HYSSEL No hysteresis is selected; HYSPOL Rising Edge; INSEL CMP1A; CMPPOL Non Inverted; FLTREN disabled; DACOEN enabled; CBE disabled; UPDTMDIS disabled; EXTUPD disabled; IRQM Interrupts are disabled; DACEN enabled; TMCB 0; 

    //Slope Settings
    DAC1DAT = 0xF3200CDUL; //DACLOW 205; DACDAT 3890; 
    SLP1CON = 0x0UL; //SLPSTRT None; SLPSTOPB None; SLPSTOPA None; HCFSEL None; FFSEN disabled; PSE Negative; TWME disabled; HME disabled; SLOPEN disabled; 
    SLP1DAT = 0x0UL; //SLPDAT 0; 
    
    CMP1_EventCallbackRegister(&CMP1_EventCallback);
    
    DACCTRL1bits.ON = 1U;
}

void CMP1_Deinitialize(void)
{ 
    DACCTRL1bits.ON = 0;
    
    
    // Comparator Register settings
    DACCTRL1 = 0x3F7F0000UL;
    DACCTRL2 = 0x8A0001UL;
    DAC1CON = 0x0UL;

    //Slope Settings
    DAC1DAT = 0x0UL;
    SLP1CON = 0x0UL;
    SLP1DAT = 0x0UL;
}

void CMP1_Calibrate(void)
{
    uint32_t *fpdmdac = (uint32_t*)CMP_FPDMDAC_ADDRESS;
    DACCTRL1bits.POSINLADJ = ((*fpdmdac) & _DACCTRL1_POSINLADJ_MASK) >> _DACCTRL1_POSINLADJ_POSITION;
    DACCTRL1bits.NEGINLADJ = ((*fpdmdac) & _DACCTRL1_NEGINLADJ_MASK) >> _DACCTRL1_NEGINLADJ_POSITION;
    DACCTRL1bits.DNLADJ = ((*fpdmdac) & _DACCTRL1_DNLADJ_MASK) >> _DACCTRL1_DNLADJ_POSITION;
    DACCTRL1bits.RREN = 1U;
}

void CMP1_EventCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CMP1_EventHandler = handler;
    }
}

void __attribute__ ((weak)) CMP1_EventCallback(void)
{ 
   
}
 
void CMP1_Tasks(void)
{
    if(IFS2bits.CMP1IF == 1U)
    {
        // CMP1 callback function 
        if(NULL != CMP1_EventHandler)
        {
            (*CMP1_EventHandler)();
        }
    
        // clear the CMP1 interrupt flag
        IFS2bits.CMP1IF = 0U;
    }
}

/**
 End of File
*/