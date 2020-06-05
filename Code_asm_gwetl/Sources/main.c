// filename ******** Main.C ************** 
// Example main program for SCI 9S12C32 gadlfy example   
// 9S12C32 SCI connected to PC com port
// does not run in BOOT mode, only in RUN mode

//  This example accompanies the books
//   "Embedded Microcomputer Systems: Real Time Interfacing", 
//     Thomsen, copyright (c) 2006,
//   "Introduction to Embedded Microcomputer Systems: 
//    Motorola 6811 and 6812 Simulation", Brooks-Cole, copyright (c) 2002,
//    Jonathan W. Valvano 3/17/07

// You may use, edit, run or distribute this file 
//    as long as the above copyright notices remain

//**********************************************************************************
// Adapted for the Technological Arts EsduinoXtreme board by Carl Barnes, 11/28/2014
//**********************************************************************************

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"  /* derivative information */
#include "SCI.h"


//char string[20];
unsigned short num;
unsigned int start, mode, theta_digital, angle;
//float angle;
unsigned const char LED[5] = {0x00, 0x08, 0x0C, 0x0E, 0x0F};
  
 

void setClk(void);
void delay1ms(unsigned int numTimes);
void OutCRLF(void);

void main(void) {
  setClk();
   /* DEFAULT COMMENCING VALUES */ 
  
  start = 0;
  mode = 1;
  
  /*                            */
 
  //Configure Port I/O's//
  
  //PORT J: On-board LED to flash when data acquisition is "ON"//
  DDRJ = 0x01;
  PTJ = 0x00;
  
  //: CONFIGURE PT0AD & PT1AD FOR BCD DIG OUTPUTS 
  DDR1AD = 0x0F;
  PER1AD = 0x0F;
  DDR0AD = 0xFF;
  PER0AD = 0xFF;
  
  //: CONFIGURE PT1AD FOR ANALOG INPUTS  [AD 5:0]
  ATDCTL1 = 0x2F; // 10-bit resolution
  ATDCTL3 = 0x88; //right-justified 
  ATDCTL4 = 0x02;  //prescaler = 6/(2*(PRS+1))
  
  /* The next six assignment statements configure the Timer Input Capture 
	 */ 
	 	 
  TSCR1 = 0x90; //Timer System Control Register 1 
 	// TSCR1[7] = TEN:  Timer Enable (0-disable, 1-enable) 	
 	// TSCR1[6] = TSWAI:  Timer runs during WAI (0-enable, 1-disable) 	
 	// TSCR1[5] = TSFRZ:  Timer runs during WAI (0-enable, 
 	// TSCR1[4] = TFFCA:  Timer Fast Flag Clear All (0-normal 1-read/write clears interrupt flags) 
	// TSCR1[3] = PRT:  Precision Timer (0-legacy, 1-precision) 
	// TSCR1[2:0] not used 
  
  TSCR2 = 0x04; //Timer System Control Register 2 
	 	// TSCR2[7] = TOI: Timer Overflow Interrupt Enable (0-inhibited, 1-hardware irq when TOF=1) 
	 	// TSCR2[6:3] not used 
	 	// TSCR2[2:0] = Timer Prescaler Select: See Table22-12 of MC9S12G Family Reference Manual r1.25 (set for bus/1) 
  
 	 
  TIOS = 0xFC;  //Timer Input Capture or Output capture 
	             	//set TIC[0] and input (similar to DDR) 
  PERT = 0x03;  //Enable Pull-Up resistor on TIC[0] 
  
  TCTL3 = 0x00; //TCTL3 & TCTL4 configure which edge(s) to capture 
  TCTL4 = 0x0A; //Configured for falling edge on TIC[0] 
  
/* 
*	The next one assignment statement configures the Timer Interrupt Enable 
 	 
	 */ 	 
  
  TIE = 0x03;   //Timer Interrupt Enable //  TIE = 0x03; for two interrrupts 
  
/* 
*	The next one assignment statement configures the ESDX to catch Interrupt 
	Requests 	 
	 */ 	 
  
  EnableInterrupts; //CodeWarrior's method of enabling interrupts 

  
  
  SCI_Init(38400);
  
 
  
     
      for(;;){
      
        while (start == 1){ 
          
          theta_digital = ATDDR0;
          
           /* MODE 0 */
           
          if (mode == 0){
          
            PTJ = 0b00000001;
            delay1ms(250);
            
            if (theta_digital >= 510 && theta_digital <=612){ //avoid outputting randomly produced negative values
            
              //SCI_OutString("X = "); SCI_OutUDec(theta_digital);
              
     
              angle = ((theta_digital-510)*90)/102; 
              //SCI_OutString(" X = "); 
              SCI_OutUDec(angle);
              OutCRLF();
              delay1ms(500);
              
              
              /* LED BCD DISPLAY*/
              
              //Most Significant Bit of Angle
              if (angle/10 == 0) { PT0AD = 0b00000000;}
              if (angle/10 == 1) { PT0AD = 0b00000001;}
              if (angle/10 == 2) { PT0AD = 0b00000010;}
              if (angle/10 == 3) { PT0AD = 0b00000011;}
              if (angle/10 == 4) { PT0AD = 0b00000100;}
              if (angle/10 == 5) { PT0AD = 0b00000101;}
              if (angle/10 == 6) { PT0AD = 0b00000110;}
              if (angle/10 == 7) { PT0AD = 0b00000111;}
              if (angle/10 == 8) { PT0AD = 0b00001000;}
              if (angle/10 == 9) { PT0AD = 0b00001001;}
              
              //Least Signficant Bit of Angle
              if (angle%10 == 0) { PT1AD = 0b00000000;}
              if (angle%10 == 1) { PT1AD = 0b00000001;}
              if (angle%10 == 2) { PT1AD = 0b00000010;}
              if (angle%10 == 3) { PT1AD = 0b00000011;}
              if (angle%10 == 4) { PT1AD = 0b00000100;}
              if (angle%10 == 5) { PT1AD = 0b00000101;}
              if (angle%10 == 6) { PT1AD = 0b00000110;}
              if (angle%10 == 7) { PT1AD = 0b00000111;}
              if (angle%10 == 8) { PT1AD = 0b00001000;}
              if (angle%10 == 9) { PT1AD = 0b00001001;}
              
                 
            }//end Mode 0
            
            
            /* MODE 1 */
            
          } else if (mode == 1){
              PTJ = 0b00000001;
              delay1ms(100);
              PTJ = 0b00000000;
              delay1ms(100);              
              
              if (theta_digital >=511 && theta_digital <=620){
                
                //SCI_OutString("Y = "); SCI_OutUDec(theta_digital);
               
                angle =((theta_digital-511)*90)/109;
                //SCI_OutString(" Y = "); 
                SCI_OutUDec(angle);
                OutCRLF();
                delay1ms(500);
              
                
                    /* LED BCD DISPLAY*/
              
              //Most Significant Bit of Angle
              if (angle/10 == 0) { PT0AD = 0b00000000;}
              if (angle/10 == 1) { PT0AD = 0b00000001;}
              if (angle/10 == 2) { PT0AD = 0b00000010;}
              if (angle/10 == 3) { PT0AD = 0b00000011;}
              if (angle/10 == 4) { PT0AD = 0b00000100;}
              if (angle/10 == 5) { PT0AD = 0b00000101;}
              if (angle/10 == 6) { PT0AD = 0b00000110;}
              if (angle/10 == 7) { PT0AD = 0b00000111;}
              if (angle/10 == 8) { PT0AD = 0b00001000;}
              if (angle/10 == 9) { PT0AD = 0b00001001;}
              
              //Least Signficant Bit of Angle
              if (angle%10 == 0) { PT1AD = 0b00000000;}
              if (angle%10 == 1) { PT1AD = 0b00000001;}
              if (angle%10 == 2) { PT1AD = 0b00000010;}
              if (angle%10 == 3) { PT1AD = 0b00000011;}
              if (angle%10 == 4) { PT1AD = 0b00000100;}
              if (angle%10 == 5) { PT1AD = 0b00000101;}
              if (angle%10 == 6) { PT1AD = 0b00000110;}
              if (angle%10 == 7) { PT1AD = 0b00000111;}
              if (angle%10 == 8) { PT1AD = 0b00001000;}
              if (angle%10 == 9) { PT1AD = 0b00001001;} 
              }
              
              
           
            
          }
          
          
  
        }
          
          
          
            
         
    }
} //end main




// INTERRUPTS //


//ISR1 - TIC0 - Vtimch0 - start 

  interrupt VectorNumber_Vtimch0 void ISR_Vtimch0(void){
        unsigned int temp;

        delay1ms(500);
        
        if(start == 0){
          
          start = 1;
          SCI_OutString("Serial Communication Started");
          OutCRLF();
        } 
        
        

        else{
        
          start = 0;
          SCI_OutString("Serial Communication Halted.");
          PTJ = 0b00000000;
          PT0AD = 0b00000000;
          PT1AD = 0b00000000;
          OutCRLF();
        }
        
        /* RESETS INTERRUPT (Don't Edit)*/
        temp = TC0;
    }


  //ISR2 - TIC1 - Vtimch1 - for modes 
  interrupt VectorNumber_Vtimch1 void ISR_Vtimch1(void){
  
      unsigned int temp; 
     
      delay1ms(250);
      
      if (mode == 0){
          
          mode = 1;
          SCI_OutString("This is mode 1");
          OutCRLF();
          ATDCTL5 = 0x25; //MODE 1: AN5 
          
      }
      
      //delay1ms(250);
      
      else if (mode == 1){
        
          mode = 0;
          SCI_OutString("This is mode 0");
          OutCRLF();
          ATDCTL5 = 0x24; //MODE 0 : AN4 (as prescribed)
         
      }
      temp = TC1;
      
    } //end interrupts
 
 
    

  
//---------------------OutCRLF--------------------- 
// Output a CR,LF to SCI to go to a new line 
// Input: none 
// Output: none 
 
  
void OutCRLF(void){   
  
  SCI_OutChar(CR); 
  SCI_OutChar(LF);
} 

//SET CLOCK//

void setClk(void){
  CPMUPROT = 0x26;               //Protection of clock configuration is disabled
  
  CPMUCLKS = 0x80;               //PLLSEL=1. Select Bus Clock Source:  PLL clock or Oscillator.
  
  CPMUOSC = 0x00;                //OSCE=0. Select Clock Reference for PLLclk as: 1 MHz.
  
  CPMUSYNR=0x17;                 //Set SYNDIV [5:0] to decimal 23 (fVCO = 2*fREF*(SYNDIV + 1)
  
  CPMUPOSTDIV=0x03;              //Set Post Divider to 3, such that PLL=fVCO/(POSDIV+1) = 12
                                 
  CPMUFLG = 0x08;                //LOCK = 1
  
              //Overall clock speed is fBUS=fPLL/2=6 MHz  
  
 // while (CPMUFLG_LOCK == 0) {}  //Wait for PLL to achieve desired tolerance of target frequency. NOTE: For use when the source clock is PLL. comment out when using external oscillator as source clock 
 
// CPMUPROT = 1;                  //Protection for clock configuration is reenabled 
  //Note: If you change your clock speed you will need to update delay1ms function to give a 1 ms delay
}
//TIME DELAY//
    
void delay1ms(unsigned int time)
{  
    unsigned int j; 
    unsigned int k; 
    
  for(j=0;j<time;j++) 
	 	for(k=0;k<1001;k++); 
} 
  