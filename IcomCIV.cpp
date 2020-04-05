#include "Arduino.h"
#include "IcomCIV.h"




//global variables - defaults are just dummy values for troubleshooting
	char radio_addr = 0x00; 
	int level = 128;
	int level2 = 0;     	//temporary value for constrained level limits
	int operation = 12;   //what is to be done  
	int freq = 27185000;  //
	char radio = 706MK2G;     //
	int freqdelta=0;  	// will be dynamically set eventually.
	int oldfreq;    		// for frequency scanning using the VFO knob 
	int newfreq;    		// for frequency scanning using the VFO knob 
	int freqstep;     	//tuning step size
	int capability = 0;	//
	char capability_array[8];
	
void select_Radio()
{

  switch(radio)
  {

	case IC735: 
		radio_addr = 0x04; // icom IC735 default   
		capability = 160   
	break;
		
	case ICR7000: 
		radio_addr = 0x08; // icom ICR7000 default   
		capability = 160  
	break;
		
	case IC275: 
		radio_addr = 0x10; // icom IC275 default   
		capability = 160 
	break;
		
	case IC375: 
		radio_addr = 0x12; // icom IC375 default   
		capability = 160  
	break;
	
	case IC475: 
		radio_addr = 0x14; // icom IC475 default   
		capability = 160  
	break;
	
	case IC575: 
		radio_addr = 0x16; // icom IC575 default   
		capability = 160  
	break;
	
	case IC1275: 
		radio_addr = 0x18; // icom IC1275 default   
		capability = 160  
	break;
	
	case ICR71: 
		radio_addr = 0x1A; // icom ICR71 default   
		capability = 160  
	break;
	
	case IC751A: 
		radio_addr = 0x1C; // icom IC751A default   
		capability = 160 
	break;
	
	case IC761: 
		radio_addr = 0x1E; // icom IC761 default   
		capability = 160  
	break;
	
	case IC271: 
		radio_addr = 0x20; // icom IC271 default   
		capability = 160  
	break;
	
	case IC471: 
		radio_addr = 0x22; // icom IC471 default   
		capability = 160   
	break;
	
	case IC1271: 
		radio_addr = 0x24; // icom IC1271 default   
		capability = 160  
	break;
	
	case IC781: 
		radio_addr = 0x26; // icom IC781 default   
		capability = 160 
	break;
	
	case IC725: 
		radio_addr = 0x28; // icom IC725 default   
		capability = 160 
	break;
	
	case ICR9000: 
		radio_addr = 0x2A; // icom ICR9000 default   
		capability = 160 
	break;
	
	case IC765: 
		radio_addr = 0x2C; // icom IC765 default   
		capability = 160   
	break;
	
	case IC970: 
		radio_addr = 0x2E; // icom IC970 default   
		capability = 160  
	break;
	
	case IC726: 
		radio_addr = 0x30; // icom IC726 default   
		capability = 160  
	break;
	
	case ICR72: 
		radio_addr = 0x32; // icom ICR72 default   
		capability = 160 
	break;
	
	case ICR7100: 
		radio_addr = 0x34; // icom ICR7100 default   
		capability = 160   
	break;
	
	case IC728: 
		radio_addr = 0x38; // icom IC728 default   
		capability = 160 
	break;
	
	case IC729: 
		radio_addr = 0x3A; // icom IC729 default   
		capability = 160 
	break;
	
	case IC737: 
		radio_addr = 0x3C; // icom IC737 default   
		capability = 160 
	break;
	
	case IC707: 
		radio_addr = 0x3E; // icom IC707 default   
		capability = 160  
	break;
	
	case IC736: 
		radio_addr = 0x40; // icom IC736 default   
		capability = 160  
	break;
	
	case IC820: 
		radio_addr = 0x42; // icom IC820 default   
		capability = 160  
	break;
	
	case IC738: 
		radio_addr = 0x44; // icom IC738 default   
		capability = 160 
	break;
	
	case IC775: 
		radio_addr = 0x46; // icom IC775 default   
		capability = 176  
	break;
	
	case IC706: 
		radio_addr = 0x48; // icom IC706 default   
		capability = 176  
	break;
	
	case ICR8500: 
		radio_addr = 0x4A; // icom ICR8500 default   
		capability = 176  
	break;
	
	case IC821: 
		radio_addr = 0x4C; // icom IC821 default   
		capability = 160  
	break;
	
	case IC706MK2: 
		radio_addr = 0x4E; // icom IC706MK2 default   
		capability = 176  
	break;
	
	case IC756: 
		radio_addr = 0x50; // icom IC756 default   
		capability = 160  
	break;
	
	case ICR10: 
		radio_addr = 0x52; // icom ICR10 default   
		capability = 160  
	break;
	
	case IC746: 
		radio_addr = 0x56; // icom IC746 default  
		capability = 176  
	break;
	
	case IC706MK2G: 
		radio_addr = 0x58; // icom IC706MK2G default 
		capability = 176 
	break;
	
	case ICR75: 
		radio_addr = 0x5A; // icom ICR75 default  
		capability = 160 
	break;
	
	case IC756Pro: 
		radio_addr = 0x5C; // icom IC756Pro default
		capability = 176  
	break;
	
	case IC718: 
		radio_addr = 0x5E; // icom IC718 default  
		capability = 176  
	break;
	
	case IC910: 
		radio_addr = 0x60; // icom IC910 default  
		capability = 160  
	break;
	
	case IC78: 
		radio_addr = 0x62; // icom IC78 default  
		capability = 160  
	break;
	
	case IC756Pro2: 
		radio_addr = 0x64; // icom IC756Pro2 default 
		capability = 240  
	break;
	
	case IC746Pro: 
		radio_addr = 0x66; // icom IC746Pro default  
		capability = 240  
	break;
	
	case IC703: 
		radio_addr = 0x68; // icom IC703 default   
		capability = 176 
	break;
	
	case IC7800: 
		radio_addr = 0x6A; // icom IC7800 default  
		capability = 240 
	break;
	
	case ICR20: 

		radio_addr = 0x6C; // icom ICR20 default   
		capability = 160  
	break;

	case IC756Pro3: 
		radio_addr = 0x6E; // icom IC756Pro3 default  
		capability = 240  
	break;

	case IC7000: 
		radio_addr = 0x70; // icom IC7000 default   
		capability = 224 
	break;

	case ICR9500: 
		radio_addr = 0x72; // icom ICR9500 default  
		capability = 224 
	break;

	case IC7700: 
		radio_addr = 0x74; // icom IC7700 default   
		capability = 224 
	break;

	case IC7200: 
		radio_addr = 0x76; // icom IC7200 default   
		capability = 224 
	break;

	case ICRX7: 
		radio_addr = 0x78; // icom ICRX7 default   
		capability = 224  
	break;

	case IC7600: 
		radio_addr = 0x7A; // icom IC7600 default   
		capability = 224  
	break;

	case IC9100: 
		radio_addr = 0x7C; // icom IC9100 default  
		capability = 224  
	break;

	case IC7410: 
		radio_addr = 0x80; // icom IC7410 default   
		capability = 224  
	break;

	case ID51: 
		radio_addr = 0x86; // icom ID51 default   
		capability = 224  
	break;

	case IC7100: 
		radio_addr = 0x88; // icom IC7100 default   
		capability = 252   
	break;

	case IC7850: 
		radio_addr = 0x8E; // icom IC7850 default   
		capability = 252   
	break;

	case IC7851: 
		radio_addr = 0x8E; // icom IC7851 default   
		capability = 252   
	break;

	case IC7300: 
		radio_addr = 0x94; // icom IC7300 default   
		capability = 252   
	break;

	case ICR8600: 
		radio_addr = 0x96; // icom ICR8600 default   
		capability = 224   
	break;

	case IC7610: 
		radio_addr = 0x98; // icom IC7610 default   
		capability = 252   
	break;

	case ID4100: 
		radio_addr = 0x9A; // icom ID4100 default   
		capability = 252   
	break;

	case IC9700: 
		radio_addr = 0xA2; // icom IC9700 default   
		capability = 252   
	break;


    default:
      //do nothing
    break;
  }
  
  //EEPROM.write(0,radio_addr)
}
  
void send_preamble()
 {
  Serial.write(0xFE);       //preamble
  Serial.write(0xFE);       //preamble
  Serial.write(radio_addr);       //radio address
  Serial.write(0x01);       //Controller Address
}

void send_post()
{
  Serial.write(0xFD);       //preamble
} 

void send_testfreq()        //test case frequency
{
    send_preamble();
    Serial.write(0x00);     //10 Hz,  1 Hz
    Serial.write(0x00);     // 1 kHz,   100 Hz
    Serial.write(0x50);     // 100 kHz, 10 kHz
    Serial.write(0x18);     //10 MHZ,   1 MHz
    Serial.write(0x27);     //GHz ,   100 MHz
    send_post();
} 
  
void send_freq(long freq)
{
  char f[16],*fp;
  char dump[16];

  int i;
  unsigned char ob;
  char ichar;
  char is[16];
  char radio_addr = "0x58";   //eventually, this will be an address passed between functions
 
  send_preamble();    //preamble
  Serial.write(0x05);       //set freq command
  //send_testfreq();        //commented out - troubleshooting measure.
  sprintf(f,"%08ld",freq);    //sprintf will format the data from "freq" into the "f" array
  fp = &f[6];
  for(i=0;i<4;i++)        // For now send all 8 digits
  {
    ob = (*fp++ & 0x0f) << 4; // convert the ascii digits to pairs of BCD digits in one byte
    ob |= *fp++ & 0x0f;
    Serial.write(ob);
    fp -= 4;
  }

  send_post();          //send post command
  delay(30);
  while(Serial.available()>0)   //Clear Read Buffers
  {dump[0] = Serial.read();}    //Clear Read Buffers

}

void set_VFO_mode()
{
  send_preamble();
  Serial.write(0x07);       //set VFO mode
  send_post();
}

void set_VFO_A()
{
  send_preamble();
  Serial.write(0x07);       //set VFO mode
  Serial.write(0x00);       //set VFO A 
  send_post();
}

void Radio_OFF()
{
  send_preamble();
  Serial.write(0x18);       
  Serial.write(0x00);       //set Power_OFF (not 706)
  send_post();
}

void Mode_WFM()
{
  send_preamble();
  Serial.write(0x06);       //set mode
  Serial.write(0x06);       //set WFM 
  send_post();
}

void Mode_FM()
{
  send_preamble();
  Serial.write(0x06);       //set mode
  Serial.write(0x05);       //set FM 
  send_post();
}

void Mode_AM()
{
  send_preamble();
  Serial.write(0x06);       //set mode
  Serial.write(0x02);       //set AM 
  send_post();
}

void Mode_USB()
{
  send_preamble();
  Serial.write(0x06);       //set mode
  Serial.write(0x01);       //set AM 
  send_post();
}

void Mode_LSB()
{
  send_preamble();
  Serial.write(0x06);       //set mode
  Serial.write(0x00);       //set AM 
  send_post();
}

void Mode_RTTY()
{
  send_preamble();
  Serial.write(0x06);       //set mode
  Serial.write(0x04);       //set AM 
  send_post();
}

void set_VFO_B()
{
  send_preamble();
  Serial.write(0x07);       //set VFO mode
  Serial.write(0x01);       //set VFO B 
  send_post();
}

void set_VFO_equal()
{
  send_preamble();
  Serial.write(0x07);       //set VFO mode
  Serial.write(0xA0);       //set VFOs Equal  
  send_post();
}

void swap_VFOs()              // Swap A and B VFOs
{
  send_preamble();
  Serial.write(0x07);       //set VFO mode
  Serial.write(0xB0);       //set VFO B 
  send_post();
}

void set_20dB_Atten_on()         //turn on Attenuator
{
  send_preamble();
  Serial.write(0x11);       //set Atten
  Serial.write(0x20);       //set Atten ON  
  send_post();
}

void set_20dB_Atten_off()        //Turn off Attenuator
{
  send_preamble();
  Serial.write(0x11);       //set Atten
  Serial.write(0x00);       //set Atten OFF
  send_post();
}

void set_preamp1_on()            //turn Preamp 1 ON (valid for newer radios)
{
  //for radios with only a single preamp, this one works
  send_preamble();
  Serial.write(0x16);       // RX command 
  Serial.write(0x02);       // Preamp Subcommand
  Serial.write(0x01);       // Preamp 1 ON
  send_post();
}

void set_preamp2_on()            //turn Preamp 2 ON (valid for newer radios)
{
  //need to verify what radios this is applicable for!
  //for radios with only a single preamp, modify commands as necessary to just turn preamp ON?
  send_preamble();
  Serial.write(0x16);       // RX command 
  Serial.write(0x02);       // Preamp Subcommand
  Serial.write(0x02);       // Preamp 2 ON
  send_post();
}

void set_preamp_off()            //Turn Preamps OFF
{
  send_preamble();
  Serial.write(0x16);       // RX command 
  Serial.write(0x02);       // Preamp Subcommand
  Serial.write(0x00);       // Preamp OFF
  send_post();
}

void set_NB_ON()              //turn Noise Reduction ON
{
  send_preamble();
  Serial.write(0x16);       // RX command
  Serial.write(0x22);       // NB Subcommand
  Serial.write(0x01);       // NB ON
  send_post();
}

void set_NB_OFF()               //turn Noise Blanker Off
{
  send_preamble();
  Serial.write(0x16);       // RX command
  Serial.write(0x22);       // NB Subcommand
  Serial.write(0x00);       // NB OFF
  send_post();
}

void set_NR_ON()              //turn Noise Reduction On
{
  send_preamble();
  Serial.write(0x16);       // RX command
  Serial.write(0x40);       // NR Subcommand
  Serial.write(0x01);       // NB ON
  send_post();
}

void set_NR_OFF()               //turn Noise Reduction Off
{
  send_preamble();
  Serial.write(0x16);       // RX command
  Serial.write(0x40);       // NR Subcommand
  Serial.write(0x00);       // NB ONFF
  send_post();
}

void set_AutoNotch_ON()          //turn auto notch ON
{
  send_preamble();
  Serial.write(0x16);       // RX command
  Serial.write(0x41);       // AN Subcommand
  Serial.write(0x01);       // AN ON
  send_post();
}

void set_AutoNotch_OFF()         //turn Auto Notch OFF
{
  send_preamble();
  Serial.write(0x16);       // RX command
  Serial.write(0x41);       // AN Subcommand
  Serial.write(0x00);       // AN OFF
  send_post();
}

void set_RFGain(int level)    //set RF Gain Level
{
  int Hexlevel=0;
  level2 = constrain(level, 0, 0255);
  Hexlevel = char(level2, HEX);

  
  send_preamble();
  Serial.write(0x14);       // RX Levels command
  Serial.write(0x02);       // RF Gain Subcommand
  Serial.write(Hexlevel);     // Send Gain Level
  send_post();
}

void set_NRLevel(int level)    //set Noise Reduction Level
{
  int Hexlevel=0;
  level2 = constrain(level, 0, 0255);
  Hexlevel = char(level2, HEX);

  send_preamble();
  Serial.write(0x14);       // RX Levels command
  Serial.write(0x06);       // NoiseReduction Subcommand
  Serial.write(Hexlevel);     // Send NR Level
  send_post();
}

void set_RFPower(int level)  //set RF power
{
  int Hexlevel=0;
  level2 = constrain(level, 0, 0255);
  Hexlevel = char(level2, HEX);
  
  send_preamble();
  Serial.write(0x14);       // RX Levels command
  Serial.write(0x0A);       // RF Power Subcommand
  Serial.write(Hexlevel);     // Send RF Power Level
  send_post();
}

void set_NBLevel(int level)     //set NB Level
{
  int Hexlevel=0;
  level2 = constrain(level, 0, 0255);
  Hexlevel = char(level2, HEX);
  
  send_preamble();
  Serial.write(0x14);       // RX Levels command
  Serial.write(0x12);       // Noise Blanker Subcommand
  Serial.write(Hexlevel);     // Send NB Level
  send_post();
}

void set_VoiceKeyerInterval(int interval) //set keyer interval
{
  int Hexlevel=0;
  int level = constrain(interval, 01, 15);
  Hexlevel = char(level, HEX);
  //note:  may need to verify that this is consistent across new models (Voice Keyer not available on older radios)
  //Read Radio Capability Byte
  //Check Bit 3 - if Bit 3 is 1, then proceed, otherwise, skip this operation.
  
  send_preamble();
  Serial.write(0x1A);       // Voice Keyer command
  Serial.write(0x05);       // Interval Subcommand
  Serial.write(0x01);       // Interval Subcommand
  Serial.write(0x81);       // Interval Subcommand
  Serial.write(Hexlevel);        // Send keyer interval
  send_post();
}

void ActivateVoiceKeyer(int keyerloc)   //select voice keyer location
{
  int loc=0;

  int level = constrain(keyerloc, 0x01, 0x08);
  loc = char(level, keyerloc);  // convert to hex....   necesssary?
  //note:  may need to verify that this is consistent across new models (Voice Keyer not available on older radios)
  //Read Radio Capability Byte
  //Check Bit 3 - if Bit 3 is 1, then proceed, otherwise, skip this operation.
  
  send_preamble();
  Serial.write(0x2B);       // Voice Keyer command
  Serial.write(0x00);       //  Subcommand
  Serial.write(loc);        // Send desired location
  send_post();
}

void CancelVoiceKeyer()          //disable voice keyer
{
  //note:  may need to verify that this is consistent across new models (Voice Keyer not available on older radios)
  //Read Radio Capability Byte
  //Check Bit 3 - if Bit 3 is 1, then proceed, otherwise, skip this operation.
  
  send_preamble();
  Serial.write(0x2B);       // Voice #include "Arduino.h"
#include "IcomCIV.h"




//global variables - defaults are just dummy values for troubleshooting
	char radio_addr = 0x00; 
	int level = 128;
	int level2 = 0;     	//temporary value for constrained level limits
	int operation = 12;   //what is to be done  
	int freq = 27185000;  //
	char radio = 706MK2G;     //
	int freqdelta=0;  	// will be dynamically set eventually.
	int oldfreq;    		// for frequency scanning using the VFO knob 
	int newfreq;    		// for frequency scanning using the VFO knob 
	int freqstep;     	//tuning step size
	int capability = 0;	//
	char capability_array[8];
	
void select_Radio()
{

  switch(radio)
  {

	case IC735: 
		radio_addr = 0x04; // icom IC735 default   
		capability = 160   
	break;
		
	case ICR7000: 
		radio_addr = 0x08; // icom ICR7000 default   
		capability = 160  
	break;
		
	case IC275: 
		radio_addr = 0x10; // icom IC275 default   
		capability = 160 
	break;
		
	case IC375: 
		radio_addr = 0x12; // icom IC375 default   
		capability = 160  
	break;
	
	case IC475: 
		radio_addr = 0x14; // icom IC475 default   
		capability = 160  
	break;
	
	case IC575: 
		radio_addr = 0x16; // icom IC575 default   
		capability = 160  
	break;
	
	case IC1275: 
		radio_addr = 0x18; // icom IC1275 default   
		capability = 160  
	break;
	
	case ICR71: 
		radio_addr = 0x1A; // icom ICR71 default   
		capability = 160  
	break;
	
	case IC751A: 
		radio_addr = 0x1C; // icom IC751A default   
		capability = 160 
	break;
	
	case IC761: 
		radio_addr = 0x1E; // icom IC761 default   
		capability = 160  
	break;
	
	case IC271: 
		radio_addr = 0x20; // icom IC271 default   
		capability = 160  
	break;
	
	case IC471: 
		radio_addr = 0x22; // icom IC471 default   
		capability = 160   
	break;
	
	case IC1271: 
		radio_addr = 0x24; // icom IC1271 default   
		capability = 160  
	break;
	
	case IC781: 
		radio_addr = 0x26; // icom IC781 default   
		capability = 160 
	break;
	
	case IC725: 
		radio_addr = 0x28; // icom IC725 default   
		capability = 160 
	break;
	
	case ICR9000: 
		radio_addr = 0x2A; // icom ICR9000 default   
		capability = 160 
	break;
	
	case IC765: 
		radio_addr = 0x2C; // icom IC765 default   
		capability = 160   
	break;
	
	case IC970: 
		radio_addr = 0x2E; // icom IC970 default   
		capability = 160  
	break;
	
	case IC726: 
		radio_addr = 0x30; // icom IC726 default   
		capability = 160  
	break;
	
	case ICR72: 
		radio_addr = 0x32; // icom ICR72 default   
		capability = 160 
	break;
	
	case ICR7100: 
		radio_addr = 0x34; // icom ICR7100 default   
		capability = 160   
	break;
	
	case IC728: 
		radio_addr = 0x38; // icom IC728 default   
		capability = 160 
	break;
	
	case IC729: 
		radio_addr = 0x3A; // icom IC729 default   
		capability = 160 
	break;
	
	case IC737: 
		radio_addr = 0x3C; // icom IC737 default   
		capability = 160 
	break;
	
	case IC707: 
		radio_addr = 0x3E; // icom IC707 default   
		capability = 160  
	break;
	
	case IC736: 
		radio_addr = 0x40; // icom IC736 default   
		capability = 160  
	break;
	
	case IC820: 
		radio_addr = 0x42; // icom IC820 default   
		capability = 160  
	break;
	
	case IC738: 
		radio_addr = 0x44; // icom IC738 default   
		capability = 160 
	break;
	
	case IC775: 
		radio_addr = 0x46; // icom IC775 default   
		capability = 176  
	break;
	
	case IC706: 
		radio_addr = 0x48; // icom IC706 default   
		capability = 176  
	break;
	
	case ICR8500: 
		radio_addr = 0x4A; // icom ICR8500 default   
		capability = 176  
	break;
	
	case IC821: 
		radio_addr = 0x4C; // icom IC821 default   
		capability = 160  
	break;
	
	case IC706MK2: 
		radio_addr = 0x4E; // icom IC706MK2 default   
		capability = 176  
	break;
	
	case IC756: 
		radio_addr = 0x50; // icom IC756 default   
		capability = 160  
	break;
	
	case ICR10: 
		radio_addr = 0x52; // icom ICR10 default   
		capability = 160  
	break;
	
	case IC746: 
		radio_addr = 0x56; // icom IC746 default  
		capability = 176  
	break;
	
	case IC706MK2G: 
		radio_addr = 0x58; // icom IC706MK2G default 
		capability = 176 
	break;
	
	case ICR75: 
		radio_addr = 0x5A; // icom ICR75 default  
		capability = 160 
	break;
	
	case IC756Pro: 
		radio_addr = 0x5C; // icom IC756Pro default
		capability = 176  
	break;
	
	case IC718: 
		radio_addr = 0x5E; // icom IC718 default  
		capability = 176  
	break;
	
	case IC910: 
		radio_addr = 0x60; // icom IC910 default  
		capability = 160  
	break;
	
	case IC78: 
		radio_addr = 0x62; // icom IC78 default  
		capability = 160  
	break;
	
	case IC756Pro2: 
		radio_addr = 0x64; // icom IC756Pro2 default 
		capability = 240  
	break;
	
	case IC746Pro: 
		radio_addr = 0x66; // icom IC746Pro default  
		capability = 240  
	break;
	
	case IC703: 
		radio_addr = 0x68; // icom IC703 default   
		capability = 176 
	break;
	
	case IC7800: 
		radio_addr = 0x6A; // icom IC7800 default  
		capability = 240 
	break;
	
	case ICR20: 

		radio_addr = 0x6C; // icom ICR20 default   
		capability = 160  
	break;

	case IC756Pro3: 
		radio_addr = 0x6E; // icom IC756Pro3 default  
		capability = 240  
	break;

	case IC7000: 
		radio_addr = 0x70; // icom IC7000 default   
		capability = 224 
	break;

	case ICR9500: 
		radio_addr = 0x72; // icom ICR9500 default  
		capability = 224 
	break;

	case IC7700: 
		radio_addr = 0x74; // icom IC7700 default   
		capability = 224 
	break;

	case IC7200: 
		radio_addr = 0x76; // icom IC7200 default   
		capability = 224 
	break;

	case ICRX7: 
		radio_addr = 0x78; // icom ICRX7 default   
		capability = 224  
	break;

	case IC7600: 
		radio_addr = 0x7A; // icom IC7600 default   
		capability = 224  
	break;

	case IC9100: 
		radio_addr = 0x7C; // icom IC9100 default  
		capability = 224  
	break;

	case IC7410: 
		radio_addr = 0x80; // icom IC7410 default   
		capability = 224  
	break;

	case ID51: 
		radio_addr = 0x86; // icom ID51 default   
		capability = 224  
	break;

	case IC7100: 
		radio_addr = 0x88; // icom IC7100 default   
		capability = 252   
	break;

	case IC7850: 
		radio_addr = 0x8E; // icom IC7850 default   
		capability = 252   
	break;

	case IC7851: 
		radio_addr = 0x8E; // icom IC7851 default   
		capability = 252   
	break;

	case IC7300: 
		radio_addr = 0x94; // icom IC7300 default   
		capability = 252   
	break;

	case ICR8600: 
		radio_addr = 0x96; // icom ICR8600 default   
		capability = 224   
	break;

	case IC7610: 
		radio_addr = 0x98; // icom IC7610 default   
		capability = 252   
	break;

	case ID4100: 
		radio_addr = 0x9A; // icom ID4100 default   
		capability = 252   
	break;

	case IC9700: 
		radio_addr = 0xA2; // icom IC9700 default   
		capability = 252   
	break;


    default:
      //do nothing
    break;
  }
  
  //EEPROM.write(0,radio_addr)
}
  
void send_preamble()
 {
  Serial.write(0xFE);       //preamble
  Serial.write(0xFE);       //preamble
  Serial.write(radio_addr);       //radio address
  Serial.write(0x01);       //Controller Address
}

void send_post()
{
  Serial.write(0xFD);       //preamble
} 

void send_testfreq()        //test case frequency
{
    send_preamble();
    Serial.write(0x00);     //10 Hz,  1 Hz
    Serial.write(0x00);     // 1 kHz,   100 Hz
    Serial.write(0x50);     // 100 kHz, 10 kHz
    Serial.write(0x18);     //10 MHZ,   1 MHz
    Serial.write(0x27);     //GHz ,   100 MHz
    send_post();
} 
  
void send_freq(long freq)
{
  char f[16],*fp;
  char dump[16];

  int i;
  unsigned char ob;
  char ichar;
  char is[16];
  char radio_addr = "0x58";   //eventually, this will be an address passed between functions
 
  send_preamble();    //preamble
  Serial.write(0x05);       //set freq command
  //send_testfreq();        //commented out - troubleshooting measure.
  sprintf(f,"%08ld",freq);    //sprintf will format the data from "freq" into the "f" array
  fp = &f[6];
  for(i=0;i<4;i++)        // For now send all 8 digits
  {
    ob = (*fp++ & 0x0f) << 4; // convert the ascii digits to pairs of BCD digits in one byte
    ob |= *fp++ & 0x0f;
    Serial.write(ob);
    fp -= 4;
  }

  send_post();          //send post command
  delay(30);
  while(Serial.available()>0)   //Clear Read Buffers
  {dump[0] = Serial.read();}    //Clear Read Buffers

}

void set_VFO_mode()
{
  send_preamble();
  Serial.write(0x07);       //set VFO mode
  send_post();
}

void set_VFO_A()
{
  send_preamble();
  Serial.write(0x07);       //set VFO mode
  Serial.write(0x00);       //set VFO A 
  send_post();
}

void Radio_OFF()
{
  send_preamble();
  Serial.write(0x18);       
  Serial.write(0x00);       //set Power_OFF (not 706)
  send_post();
}

void Mode_WFM()
{
  send_preamble();
  Serial.write(0x06);       //set mode
  Serial.write(0x06);       //set WFM 
  send_post();
}

void Mode_FM()
{
  send_preamble();
  Serial.write(0x06);       //set mode
  Serial.write(0x05);       //set FM 
  send_post();
}

void Mode_AM()
{
  send_preamble();
  Serial.write(0x06);       //set mode
  Serial.write(0x02);       //set AM 
  send_post();
}

void Mode_USB()
{
  send_preamble();
  Serial.write(0x06);       //set mode
  Serial.write(0x01);       //set AM 
  send_post();
}

void Mode_LSB()
{
  send_preamble();
  Serial.write(0x06);       //set mode
  Serial.write(0x00);       //set AM 
  send_post();
}

void Mode_RTTY()
{
  send_preamble();
  Serial.write(0x06);       //set mode
  Serial.write(0x04);       //set AM 
  send_post();
}

void set_VFO_B()
{
  send_preamble();
  Serial.write(0x07);       //set VFO mode
  Serial.write(0x01);       //set VFO B 
  send_post();
}

void set_VFO_equal()
{
  send_preamble();
  Serial.write(0x07);       //set VFO mode
  Serial.write(0xA0);       //set VFOs Equal  
  send_post();
}

void swap_VFOs()              // Swap A and B VFOs
{
  send_preamble();
  Serial.write(0x07);       //set VFO mode
  Serial.write(0xB0);       //set VFO B 
  send_post();
}

void set_20dB_Atten_on()         //turn on Attenuator
{
  send_preamble();
  Serial.write(0x11);       //set Atten
  Serial.write(0x20);       //set Atten ON  
  send_post();
}

void set_20dB_Atten_off()        //Turn off Attenuator
{
  send_preamble();
  Serial.write(0x11);       //set Atten
  Serial.write(0x00);       //set Atten OFF
  send_post();
}

void set_preamp1_on()            //turn Preamp 1 ON (valid for newer radios)
{
  //for radios with only a single preamp, this one works
  send_preamble();
  Serial.write(0x16);       // RX command 
  Serial.write(0x02);       // Preamp Subcommand
  Serial.write(0x01);       // Preamp 1 ON
  send_post();
}

void set_preamp2_on()            //turn Preamp 2 ON (valid for newer radios)
{
  //need to verify what radios this is applicable for!
  //for radios with only a single preamp, modify commands as necessary to just turn preamp ON?
  send_preamble();
  Serial.write(0x16);       // RX command 
  Serial.write(0x02);       // Preamp Subcommand
  Serial.write(0x02);       // Preamp 2 ON
  send_post();
}

void set_preamp_off()            //Turn Preamps OFF
{
  send_preamble();
  Serial.write(0x16);       // RX command 
  Serial.write(0x02);       // Preamp Subcommand
  Serial.write(0x00);       // Preamp OFF
  send_post();
}

void set_NB_ON()              //turn Noise Reduction ON
{
  send_preamble();
  Serial.write(0x16);       // RX command
  Serial.write(0x22);       // NB Subcommand
  Serial.write(0x01);       // NB ON
  send_post();
}

void set_NB_OFF()               //turn Noise Blanker Off
{
  send_preamble();
  Serial.write(0x16);       // RX command
  Serial.write(0x22);       // NB Subcommand
  Serial.write(0x00);       // NB OFF
  send_post();
}

void set_NR_ON()              //turn Noise Reduction On
{
  send_preamble();
  Serial.write(0x16);       // RX command
  Serial.write(0x40);       // NR Subcommand
  Serial.write(0x01);       // NB ON
  send_post();
}

void set_NR_OFF()               //turn Noise Reduction Off
{
  send_preamble();
  Serial.write(0x16);       // RX command
  Serial.write(0x40);       // NR Subcommand
  Serial.write(0x00);       // NB ONFF
  send_post();
}

void set_AutoNotch_ON()          //turn auto notch ON
{
  send_preamble();
  Serial.write(0x16);       // RX command
  Serial.write(0x41);       // AN Subcommand
  Serial.write(0x01);       // AN ON
  send_post();
}

void set_AutoNotch_OFF()         //turn Auto Notch OFF
{
  send_preamble();
  Serial.write(0x16);       // RX command
  Serial.write(0x41);       // AN Subcommand
  Serial.write(0x00);       // AN OFF
  send_post();
}

void set_RFGain(int level)    //set RF Gain Level
{
  int Hexlevel=0;
  level2 = constrain(level, 0, 0255);
  Hexlevel = char(level2, HEX);

  
  send_preamble();
  Serial.write(0x14);       // RX Levels command
  Serial.write(0x02);       // RF Gain Subcommand
  Serial.write(Hexlevel);     // Send Gain Level
  send_post();
}

void set_NRLevel(int level)    //set Noise Reduction Level
{
  int Hexlevel=0;
  level2 = constrain(level, 0, 0255);
  Hexlevel = char(level2, HEX);

  send_preamble();
  Serial.write(0x14);       // RX Levels command
  Serial.write(0x06);       // NoiseReduction Subcommand
  Serial.write(Hexlevel);     // Send NR Level
  send_post();
}

void set_RFPower(int level)  //set RF power
{
  int Hexlevel=0;
  level2 = constrain(level, 0, 0255);
  Hexlevel = char(level2, HEX);
  
  send_preamble();
  Serial.write(0x14);       // RX Levels command
  Serial.write(0x0A);       // RF Power Subcommand
  Serial.write(Hexlevel);     // Send RF Power Level
  send_post();
}

void set_NBLevel(int level)     //set NB Level
{
  int Hexlevel=0;
  level2 = constrain(level, 0, 0255);
  Hexlevel = char(level2, HEX);
  
  send_preamble();
  Serial.write(0x14);       // RX Levels command
  Serial.write(0x12);       // Noise Blanker Subcommand
  Serial.write(Hexlevel);     // Send NB Level
  send_post();
}

void set_VoiceKeyerInterval(int interval) //set keyer interval
{
  int Hexlevel=0;
  int level = constrain(interval, 01, 15);
  Hexlevel = char(level, HEX);
  //note:  may need to verify that this is consistent across new models (Voice Keyer not available on older radios)
  //Read Radio Capability Byte
  //Check Bit 3 - if Bit 3 is 1, then proceed, otherwise, skip this operation.
  
  send_preamble();
  Serial.write(0x1A);       // Voice Keyer command
  Serial.write(0x05);       // Interval Subcommand
  Serial.write(0x01);       // Interval Subcommand
  Serial.write(0x81);       // Interval Subcommand
  Serial.write(Hexlevel);        // Send keyer interval
  send_post();
}

void ActivateVoiceKeyer(int keyerloc)   //select voice keyer location
{
  int loc=0;

  int level = constrain(keyerloc, 0x01, 0x08);
  loc = char(level, keyerloc);  // convert to hex....   necesssary?
  //note:  may need to verify that this is consistent across new models (Voice Keyer not available on older radios)
  //Read Radio Capability Byte
  //Check Bit 3 - if Bit 3 is 1, then proceed, otherwise, skip this operation.
  
  send_preamble();
  Serial.write(0x2B);       // Voice Keyer command
  Serial.write(0x00);       //  Subcommand
  Serial.write(loc);        // Send desired location
  send_post();
}

void CancelVoiceKeyer()          //disable voice keyer
{
  //note:  may need to verify that this is consistent across new models (Voice Keyer not available on older radios)
  //Read Radio Capability Byte
  //Check Bit 3 - if Bit 3 is 1, then proceed, otherwise, skip this operation.
  
  send_preamble();
  Serial.write(0x2B);       // Voice Keyer command
  Serial.write(0x00);       //  Subcommand
  Serial.write(0x00);       // Send cancellation
  send_post();
}

Keyer command
  Serial.write(0x00);       //  Subcommand
  Serial.write(0x00);       // Send cancellation
  send_post();
}

