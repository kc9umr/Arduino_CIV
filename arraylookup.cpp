#include "Arduino.h"
//#include "IcomCIV.h"




//global variables - defaults are just dummy values for troubleshooting
  
  int level = 128;
  int level2 = 0;       //temporary value for constrained level limits
  int operation = 12;   //what is to be done  
  int freq = 27185000;  //
  String radio = IC706MK2G;      //
  String radio_addr = 0x00; 
  String capability = 0; //
  int freqdelta=0;    // will be dynamically set eventually.
  int oldfreq;        // for frequency scanning using the VFO knob 
  int newfreq;        // for frequency scanning using the VFO knob 
  int freqstep;       //tuning step size
  char capability_array[8];

#define ARRAYSIZE (70*5)

String radios[ARRAYSIZE] = 	
{	//radio, address, capability, spare, spare
"IC735, 0x04 , 10100000,"" , ""
"ICR7000, 0x08 , 10100000,"" , ""
"IC275, 0x10 , 10100000,"" , ""
"IC375, 0x12 , 10100000,"" , ""
"IC475, 0x14 , 10100000,"" , ""
"IC575, 0x16 , 10100000,"" , ""
IC1275, 0x18 , 10100000,"" , ""
ICR71, 0x1A , 10100000,"" , ""
IC751A, 0x1C , 10100000,"" , ""
IC761, 0x1E , 10100000,"" , ""
IC271, 0x20 , 10100000,"" , ""
IC471, 0x22 , 10100000,"" , ""
IC1271, 0x24 , 10100000,"" , ""
IC781, 0x26 , 10100000,"" , ""
IC725, 0x28 , 10100000,"" , ""
ICR9000, 0x2A , 10100000,"" , ""
IC765, 0x2C , 10100000,"" , ""
IC970, 0x2E , 10100000,"" , ""
IC726, 0x30 , 10100000,"" , ""
ICR72, 0x32 , 10100000,"" , ""
ICR7100, 0x34 , 10100000,"" , ""
IC728, 0x38 , 10100000,"" , ""
IC729, 0x3A , 10100000,"" , ""
IC737, 0x3C , 10100000,"" , ""
IC707, 0x3E , 10100000,"" , ""
IC736, 0x40 , 10100000,"" , ""
IC820, 0x42 , 10100000,"" , ""
IC738, 0x44 , 10100000,"" , ""
IC775, 0x46 , 10110000,"" , ""
IC706, 0x48 , 10110000,"" , ""
ICR8500, 0x4A , 10110000,"" , ""
IC821, 0x4C , 10100000,"" , ""
IC706MK2, 0x4E , 10110000,"" , ""
IC756, 0x50 , 10100000,"" , ""
ICR10, 0x52 , 10100000,"" , ""
IC746, 0x56 , 10110000,"" , ""
IC706MK2G, 0x58 , 10110000,"" , ""
ICR75, 0x5A , 10100000,"" , ""
IC756Pro, 0x5C , 10110000,"" , ""
IC718, 0x5E , 10110000,"" , ""
IC910, 0x60 , 10100000,"" , ""
IC78, 0x62 , 10100000,"" , ""
IC756Pro2, 0x64 , 11110000,"" , ""
IC746Pro, 0x66 , 11110000,"" , ""
IC703, 0x68 , 10110000,"" , ""
IC7800, 0x6A , 11110000,"" , ""
ICR20, 0x6C , 10100000,"" , ""
IC756Pro3, 0x6E , 11110000,"" , ""
IC7000, 0x70 , 11100000,"" , ""
ICR9500, 0x72 , 11100000,"" , ""
IC7700, 0x74 , 11100000,"" , ""
IC7200, 0x76 , 11100000,"" , ""
ICRX7, 0x78 , 11100000,"" , ""
IC7600, 0x7A , 11100000,"" , ""
IC9100, 0x7C , 11100000,"" , ""
IC7410, 0x80 , 11100000,"" , ""
ID51, 0x86 , 11100000,"" , ""
IC7100, 0x88 , 11111100,"" , ""
IC7850, 0x8E , 11111100,"" , ""
IC7851, 0x8E , 11111100,"" , ""
IC7300, 0x94 , 11111100,"" , ""
ICR8600, 0x96 , 11100000,"" , ""
IC7610, 0x98 , 11111100,"" , ""
ID4100, 0x9A , 11111100,"" , ""
IC9700, 0xA2 , 11111100,"" , ""};



 
  
void select_Radio()
{

int wantedval = -1;
int wantedpos;
for (int i=0; i<ARRAYSIZE; i++) 
	{
		if (wantedval = radios[i]) 
		{
			wantedpos = i;
		break;
   }

	radio = radios[i];      //
	radio_addr = radios[i+1]; 
    capability = radios[i+2]; //
	//spare 1 = radios[i+3];
	//spare 2 = radios[i+4];

  
  //EEPROM.write(0,radio);
  //EEPROM.write(2,radio_addr);
  //EEPROM.write(4,capability);
  //EEPROM.write(6,spare_1);
  //EEPROM.write(8,spare_2);
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

