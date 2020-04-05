    /*==========================================================*/
   /*   IcomCIV.h	- file for controlling Icom amateur radios */
  /*   Created by Ken Cechura, 9 Aug 2019					  */
 /*   Released into the public domain.						 */
/*==========================================================*/
 
 
#ifndef IcomCIV_h
#define IcomCIV_h

#include "Arduino.h"

class IcomCIV
{
	public:
		void select_radio();
		void send_testfreq();
		void set_VFO_mode();
		void set_VFO_A();
		void set_VFO_B();
		void set_VFO_equal();
		void swap_VFOs();         
		void set_20dB_Atten_on();         
		void set_20dB_Atten_off();          
		void set_preamp1_on();            
		void set_preamp2_on();            
		void set_preamp_off();            
		void set_NB_ON();             
		void set_NB_OFF();              
		void set_NR_ON();             
		void set_NR_OFF();              
		void set_AutoNotch_ON();          
		void set_AutoNotch_OFF();         
		void set_RFGain(level);        
		void set_NRLevel(level);        
		void set_RFPower(level);      
		void set_NBLevel(level);       
		void set_VoiceKeyerInterval(level);
		void ActivateVoiceKeyer(level);
		void CancelVoiceKeyer();  
		void Mode_FM();  
		void Mode_WFM();  
		void Mode_AM();  
		void Mode_USB();  
		void Mode_LSB();  
		void Mode_RTTY();  
	private:
		//nothing
};

#endif