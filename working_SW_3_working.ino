
//  Disclaimers, credits, etc.:
//  free for personal use.  Not to be used for commercial purposes.
//  borrowed some code and inspiration from efforts described here:  https://github.com/darkbyte-ru/ICOM-CI-V-cat
//the BCD encoding was also borrowed from another project - source needs to be cited.

#include  <SoftwareSerial.h>
#include "Arduino.h"
#include "stdlib.h"
#include "Encoder.h"
#include "EEPROM.h"

#define rxPin 2
#define txPin 3
SoftwareSerial CAT(rxPin, txPin); // RX, TX

int CMD_LAYERS = 0;
//byte CMD_LAYER1 = 0x00;
//byte CMD_LAYER2 = 0x00;
byte CMD_DATA[4] = {0x00,0x00,0x00,0x00}; 


int CMD_PAYLOAD_PRESENT = 0;
byte CMD_PAYLOAD[5] = {0x00,0x00,0x00,0x00,0x00}; 

char radio_addr = 0x94;       // default.  Real value will be set based on selection of radio

#define CONTROLLER_ADDRESS   0xE0 //Controller address

#define START_BYTE     0xFE //Start byte
#define STOP_BYTE      0xFD //Stop byte

#define FUNC_OFF       0x00  //Generic OFF for ON/OFF items
#define FUNC_ON        0x01  //Generic ON for ON/OFF items

#define VFO_FUNC       0x07 //VFO operations
#define VFO_SWAP       0xB0 
#define VFO_EQUAL      0xA0
#define VFO_SELA       0x00 
#define VFO_SELB       0x01 

#define SET_FREQ       0x05 
#define READ_FREQ      0x04

#define MEM_FUNC       0x08 //memory
#define MEM_WRITE      0x09 
#define MEM_TO_VFO     0x0A
#define MEM_CLEAR      0x0B 

#define SPLIT_FUNC     0x0F //split operation
 
#define ATTEN_FUNC     0x11 //attenuator
#define ATTEN_ON       0x20
#define ATTEN_OFF      0x00

#define VSYNTH_FUNC     0x13 //speech synthesizer
#define VSYNTH_ALL      0x00 //speech synthesizer
#define VSYNTH_FRQ_RSSI 0x01 //speech synthesizer
#define VSYNTH_MODE     0x02 //speech synthesizer

#define LEV_FUNC       0x14 // Control Level Adjustments - 2 tier command + data payload
#define LEV_AFG        0x01 //AF Gain Control 0000 - 0255 data load
#define LEV_RFG        0x02 //RF Gain Control 0000 - 0255 data load
#define LEV_SQL        0x03 //SQL Control 0000 - 0255 data load
#define LEV_NR         0x06 //NR Control 0000 - 0255 data load
#define LEV_PBT_IN     0x07 //Inner PBT Control 0000 - 0255 data load
#define LEV_PBT_OUT    0x08 //Outer PBT Control 0000 - 0255 data load
#define LEV_CWP        0x09 //CW PITCH Control 0000 - 0255 data load
#define LEV_RFP        0x0A //RF Power Control 0000 - 0255 data load 
#define LEV_MIC        0x0A //Mic Gain Control 0000 - 0255 data load 
#define LEV_KEYSPEED   0x0A //CW Keyer speed Control 0000 - 0255 data load 
#define LEV_NOTCH      0x0A //Notch position Control 0000 - 0255 data load 
#define LEV_COMP       0x0A //COMP Level Control 0000 - 0255 data load 
#define LEV_QSK_DLY    0x0A //QSK Delay Control 0000 - 0255 data load 
#define LEV_NB         0x0A //NB Level Control 0000 - 0255 data load 
#define LEV_MON        0x0A //Monitor Gain Power Control 0000 - 0255 data load 
#define LEV_VOX        0x0A // VOX Gain Control 0000 - 0255 data load 
#define LEV_AVOX       0x0A //Anti-vox Control 0000 - 0255 data load 
#define LEV_BRIGHT     0x0A //Bright Level Control 0000 - 0255 data load 

#define METER_FUNC     0x15 // Metering functions- 2 tier command with  data payload response
#define METER_STS      0x01 // Read noise or S-meter Squelch Status - return 00/01
#define METER_RSSI     0x02 // Read S-meter level - return 0-255
#define METER_STS2     0x05 // Read various squelch function Status - return 00/01
#define METER_PO       0x11 // Read PO meter level - return 0-255
#define METER_SWR      0x12 // Read SWR meter level - return 0-255
#define METER_ALC      0x13 // Read ALC meter level - return 0-255
#define METER_COMP     0x14 // Read COMP meter level - return 0-255
#define METER_VD       0x15 // Read VD meter level - return 0-255
#define METER_ID       0x16 // Read ID meter level - return 0-255

#define FEAT_FUNC       0x16 // Turn features on/off  - 2 tier command + data payload
#define FEAT_PREAMP     0x02 // Preamp Adjust - 00, 01, 02
#define FEAT_AGC        0x12 // AGC Adjust - 00, 01, 02, 03
#define FEAT_NB         0x22 // NB ON/OFF
#define FEAT_NR         0x40 // NR ON/OFF
#define FEAT_AN         0x41 // Auto Notch ON/OFF
#define FEAT_PL         0x42 // Repeater Tone ON/OFF
#define FEAT_TSQ        0x43 // Tone Squelch ON/OFF
#define FEAT_COMP       0x44 // Speech Compressor ON/OFF
#define FEAT_MON        0x45 // Monitor Function ON/OFF
#define FEAT_VOX        0x46 // VOX ON/OFF
#define FEAT_QSK        0x47 // Break-In ON/OFF
#define FEAT_NOTCH      0x48 // Manual Notch ON/OFF
#define FEAT_TWNPK      0x4F // Twin Peak Filter ON/OFF
#define FEAT_DLOCK      0x50 // Dial Lock ON/OFF
#define FEAT_DSP        0x56 // DSP Filter Type - 00 Sharp, 01 Soft
#define FEAT_NOTCHWID   0x57 // Manual Notch Width - 00, 01, 02
#define FEAT_SSBWID     0x58 // SSB TX Width - 00, 01, 02

#define FEAT_FUNC       0x16 // Turn features on/off  - 2 tier command + data payload

#define VKEY_FUNC       0x28 // Voice Keyer  - 2 tier command + data payload
#define VKEY_PLAY       0x00 // Voice Keyer  - 2 tier command + data payload
#define VKEY_CANCEL     0x00 // Voice Keyer  - 2 tier command + data payload

#define POWER_FUNC      0x18 // radio power button ON/OFF (2 layer CMD)

#define RADIO_ID        0x19 // Get Transciever ID

#define MISC_FUNC        0x1A // THere's a TON of operations in this set
#define MISC_IPPL        0x07 // Turn IP+ On/Off 








int dummycount;
long freq = 27155000; //default test case loads a non-transmitting frequency



void setup() {
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    CAT.begin(19200);
    Serial.begin(19200);
    Serial.flush();
    CAT.flush();
    pinMode(LED_BUILTIN, OUTPUT);
    dummycount = 0;
}

void send_preamble()
{
  CAT.write(0xFE);       //preamble
  CAT.write(0xFE);       //preamble
  CAT.write(0x94);       //radio address
  CAT.write(0x01);       //Controller Address
}

void send_post()
{
  CAT.write(0xFD);       //preamble
} 


void SWAP_VFOS()  //Switch A/B VFOs
{
  CMD_LAYERS = 2;
  CMD_DATA[0] = VFO_FUNC;       
  CMD_DATA[1] = VFO_SWAP;
  SEND_CMD();
}


void IP_TOGGLE(int sw)  //Toggle IP+
  {
      FEATURE_TOGGLE(MISC_FUNC, MISC_IPPL, sw);
  }


void PREAMP_TOGGLE(int sw) //Toggle Preamp
{

    CMD_LAYERS=2;
    CMD_DATA[0] = FEAT_FUNC;       
    CMD_DATA[1] = FEAT_PREAMP;
    CMD_PAYLOAD_PRESENT = 1;
    if(sw=2)
    {
        CMD_PAYLOAD[0] = 0x02;
    }
    else if(sw=1)
    {
          CMD_PAYLOAD[0] = 0x01;
    }
    else
    {
          CMD_PAYLOAD[0] = FUNC_OFF;
    }
    
    SEND_CMD();
}


void NB_TOGGLE(int sw)  //Toggle Noise Blanker
  {
      FEATURE_TOGGLE(FEAT_FUNC, FEAT_NB, sw);
  }

void NR_TOGGLE(int sw)  //Toggle Noise Reduction
  {
      FEATURE_TOGGLE(FEAT_FUNC, FEAT_NR, sw);
  }

void ANOTCH_TOGGLE(int sw)  //Toggle Auto Notch
  {
      FEATURE_TOGGLE(FEAT_FUNC, FEAT_AN, sw);
  }

void RPTRTONE_TOGGLE(int sw)  //Toggle Repeater Tone
  {
      FEATURE_TOGGLE(FEAT_FUNC, FEAT_PL, sw);
  }

void TONESQL_TOGGLE(int sw)  //Toggle Tone Squelch
  {
      FEATURE_TOGGLE(FEAT_FUNC, FEAT_TSQ, sw);
  }

void COMP_TOGGLE(int sw)  //Toggle Speech COmpressor
  {
      FEATURE_TOGGLE(FEAT_FUNC, FEAT_COMP, sw);
  }

void MON_TOGGLE(int sw)  //Toggle Monitor Function
  {
      FEATURE_TOGGLE(FEAT_FUNC, FEAT_MON, sw);
  }

void VOX_TOGGLE(int sw)  //Toggle Monitor Function
  {
      FEATURE_TOGGLE(FEAT_FUNC, FEAT_VOX, sw);
  }

void QSK_TOGGLE(int sw)  //Toggle Break-In Function
  {
      FEATURE_TOGGLE(FEAT_FUNC, FEAT_QSK, sw);
  }

void MAN_NOTCH_TOGGLE(int sw)  //Toggle MAnual Notch Function
  {
      FEATURE_TOGGLE(FEAT_FUNC, FEAT_NOTCH, sw);
  }

void TWN_FILT_TOGGLE(int sw)  //Toggle Twin Peak Filter Function
  {
      FEATURE_TOGGLE(FEAT_FUNC, FEAT_TWNPK, sw);
  }

void DIALLOCK_TOGGLE(int sw)  //Toggle Dial Lock Function
  {
      FEATURE_TOGGLE(FEAT_FUNC, FEAT_DLOCK, sw);
  }
  
void FEATURE_TOGGLE(byte command, byte Feature, int sw)  //Toggle Features (populate command variables)
{
    CMD_LAYERS=2;
    CMD_DATA[0] = command;       
    CMD_DATA[1] = Feature;
    CMD_PAYLOAD_PRESENT = 1;
    if(sw=1)
    {
          CMD_PAYLOAD[0] = FUNC_ON;
    }
    else
    {
          CMD_PAYLOAD[0] = FUNC_OFF;
    }  
    SEND_CMD();
}


void VOICE_PLAY(int location)  
{
  if(location<9)
  {
    CMD_LAYERS = 2;
    CMD_DATA[0] = VKEY_FUNC;       
    CMD_DATA[1] = VKEY_PLAY;
    CMD_PAYLOAD_PRESENT = 1;
    CMD_PAYLOAD[0] = byte(location);
    SEND_CMD();
    Serial.print("Voice Keyer Address ");
    Serial.print(location);
    Serial.println();

  }
  else
  {
    Serial.print("Voice Keyer Address Error");
  }
}




void SEND_FREQ(long freq)
{
  char f[16],*fp;
  char lev[4],*levp;

  int k;
  unsigned char ob;
  char ichar;
  char is[16];
  
  CMD_DATA[0] = SET_FREQ;       
  CMD_LAYERS = 1;
  CMD_PAYLOAD_PRESENT = 4;

  sprintf(f,"%08ld",freq);    //sprintf will format the data from "freq" into the "f" array
  fp = &f[6];

  for(k=0;k<CMD_PAYLOAD_PRESENT;k++)            // For now send all 8 digits
  {
    ob = (*fp++ & 0x0f) << 4;   // convert the ascii digits to pairs of BCD digits in one byte
    ob |= *fp++ & 0x0f;
    CMD_PAYLOAD[k]= ob;
    fp -= 4;
  }

 SEND_CMD();
}




void SEND_CMD()
{
  int k;  
  int j; 
  if(CMD_LAYERS>0)
  {
    send_preamble();

    for(j=0;j<CMD_LAYERS;j++)            
        {
          CAT.write(CMD_DATA[j]);     //Send Commands
          Serial.print(CMD_DATA[j]);
          Serial.print("-");
        }
    
    if (CMD_PAYLOAD_PRESENT > 0) 
    {
        for(k=0;k<CMD_PAYLOAD_PRESENT;k++)            
        {
          CAT.write(CMD_PAYLOAD[k]);  //Send Data payload, if applicale
          Serial.print(CMD_PAYLOAD[k]);
          Serial.print("-");
        }
    }
  
    send_post();
  }
    //now clear the command and data payload sizes (did not clear actual buffers, in case previous command needs to be verified
    CMD_LAYERS = 0;
    CMD_PAYLOAD_PRESENT = 0;
}



void loop() 
{
  // put your main code here, to run repeatedly:


  while(dummycount<1)
  {
    VOICE_PLAY(8);
    delay(10000);  
    SWAP_VFOS();
    dummycount++;
  } 
}
