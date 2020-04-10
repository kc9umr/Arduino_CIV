
//  Disclaimers, credits, etc.:
//  free for personal use.  Not to be used for commercial purposes.
//  borrowed some code and inspiration from efforts described here:  https://github.com/darkbyte-ru/ICOM-CI-V-cat
//the BCD encoding was also borrowed from another project - source needs to be cited.

#include  <SoftwareSerial.h>
#include "Arduino.h"
#include "stdlib.h"   //used for some math functions
#include "Encoder.h"  //used for rotary encoder
#include "EEPROM.h"   //used to write some non volatile things to the EEPROM

#define rxPin 2     //used for softwareserial
#define txPin 3     //used for softwareserial
SoftwareSerial CAT(rxPin, txPin); // RX, TX



/*Command structure:  
 * The Icom CI-V is arranged into a set of different commands and sub commands - we'll call them "layers"
 * The protocol works in a PREAMBLE-TO-FROM-COMMAND-POSTAMBLE structure (FE FE 94 01 CMD FD)
 * The command may be more than a single byte
 * 
 * simple commands use a single byte.  for example the READ FREQUENCY command - just tell the radio 0x03 as a CMD, 
 * and it will respond with the frequency
 * 
 * most commands use at least TWO bytes  -  generally a base command and a sub command.  
 * I have taken to calling these "layers"
 * 
 * Some commands also add data - think "SEND FREQUENCY" - you need to tell the radio where to go.  This is a data payload
 * herein, it's referred to as the Command payload
 * 
 * The variables below define the number of layers (commands can be up to 4 bytes, plus the payload), whether a paylaod is present
 * and what is in the payload.
 */
int CMD_LAYERS = 0;                                 //define the depth of commands (always resets to zero as a failsafe)
byte CMD_DATA[4] = {0x00,0x00,0x00,0x00};           //data is defined in an array, which will be sequentially sent to the radio

int CMD_PAYLOAD_PRESENT = 0;                        //defines if a payload is present.  if not, send payload portions of commands will be ignored
byte CMD_PAYLOAD[5] = {0x00,0x00,0x00,0x00,0x00};   //max payload size is 5 bytes (mostly for frequency)
     







//=============Frequency Conversion variables=========================
//==       Used to convert betweeen decimal and BCD                ==
byte FREQ_BCD[6] = {0x00,0x00,0x00,0x00,0x00,0x00}; 
int MHZ_100;
int MHZ;
int KHZ;
int HZ;
const uint32_t decMulti[]    = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};





//=============Serial READ DATA variables=========================
//==       Used to read data sent from the radio                ==
boolean newData = false;
const byte numBytes = 76;
byte rcvFromRadio[numBytes];  // The array that store the response from the radio
byte numReceived = 0;
byte cwMem1[70];

String model;
byte capabilities;


//=================RADIO LIST SETUP AND TABLE==================================================================
#define ARRAYSIZE (52)  //Radio Array Size - adjust as more radios added

typedef struct  {  //DEFINE RADIO DEFINITION
    String model; //user readable string giving the Icom model number
    byte address; //the default address for each radio
    byte capabilities;  //a byte, which, when converted to binary, defines what operations the radio can handle (some radios only have one preamp, others don't have auto-notch) 
  } radios;

radios radio_attrib[ARRAYSIZE] =   {  //ICOM RADIO DIRECTORY (lookup)
    //note:  receive only radios removed.
    //radio, hex address, capability
    {"IC735", 0x04 , 160},
    {"IC275", 0x10 , 160},
    {"IC375", 0x12 , 160},
    {"IC475", 0x14 , 160},
    {"IC575", 0x16 , 160},
    {"IC1275", 0x18 , 160},
    {"IC751A", 0x1C , 160},
    {"IC761", 0x1E , 160},
    {"IC271", 0x20 , 160},
    {"IC471", 0x22 , 160},
    {"IC1271", 0x24 , 160},
    {"IC781", 0x26 , 160},
    {"IC725", 0x28 , 160},
    {"IC765", 0x2C , 160},
    {"IC970", 0x2E , 160},
    {"IC726", 0x30 , 160},
    {"IC728", 0x38 , 160},
    {"IC729", 0x3A , 160},
    {"IC737", 0x3C , 160},
    {"IC707", 0x3E , 160},
    {"IC736", 0x40 , 160},
    {"IC820", 0x42 , 160},
    {"IC738", 0x44 , 160},
    {"IC775", 0x46 , 176},
    {"IC706", 0x48 , 176},
    {"IC821", 0x4C , 160},
    {"IC706MK2", 0x4E , 176},
    {"IC756", 0x50 , 160},
    {"IC746", 0x56 , 176},
    {"IC706MK2G", 0x58 , 176},
    {"IC756Pro", 0x5C , 176},
    {"IC718", 0x5E , 176},
    {"IC910", 0x60 , 160},
    {"IC78", 0x62 , 160},
    {"IC756Pro2", 0x64 , 240},
    {"IC746Pro", 0x66 , 240},
    {"IC703", 0x68 , 176},
    {"IC7800", 0x6A , 240},
    {"IC756Pro3", 0x6E , 240},
    {"IC7000",0x70 , 224},
    {"IC7700",0x74 , 224},
    {"IC7200",0x76 , 224},
    {"IC7600",0x7A , 224},
    {"IC9100",0x7C , 224},
    {"IC7410",0x80 , 224},
    {"IC7100",0x88 , 252},
    {"IC7850",0x8E , 252},
    {"IC7851",0x8E , 252},
    {"IC7300",0x94 , 252},
    {"IC7610",0x98 , 252},
    {"ID4100",0x9A , 252},
    {"IC9700",0xA2 , 252}
};



// Need to put magic decoder ring for radio capability byte




//=============Command definition constants=========================

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
#define READ_FREQ      0x03

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
#define RADIO_ID2       0x00 // Get Transciever ID (Sub command)

#define MISC_FUNC        0x1A // THere's a TON of operations in this set
#define MISC_IPPL        0x07 // Turn IP+ On/Off 
//================================================================================================================================



//======Various variables used within the program==================================================================================

long frequency = 27155000; //frequency variable, default loads a non-transmitting frequency
//char radio_addr = 0x00;       // default.  Real value will be set based on selection of radio
byte xcvr;                    //radio address byte (hex stuff - not human readable)

int dummycount = 0;   //used as a limiting constant for the basic loop - may be deleted later
long buffer_freq;     //frequency buffer for VFO swapping functions
long buffer_freq2;    //a second frequency buffer
//================================================================================================================================


//====FUNCTION DEFINITIONS 1=============================================================================================================
//====FUNCTION PREAMBLE/POSTABLE=========================================================================================================

void send_preamble()
{
    CAT.write(0xFE);
    CAT.write(0xFE);
    CAT.write(xcvr);    
    CAT.write(CONTROLLER_ADDRESS);
    Serial.print(" Preamble ");
}

/*
void send_preamble2()
{
    CAT.write(0xFE);
    CAT.write(0xFE);
    CAT.write(xcvr);    
    CAT.write(CONTROLLER_ADDRESS);

    Serial.print("Try this:  ");
    Serial.print(0xFE, HEX);
    Serial.print(0xFE, HEX);
    Serial.print(xcvr, HEX);    
    Serial.print(CONTROLLER_ADDRESS, HEX);
    Serial.print("-");
}
*/

void send_post()
{
  byte POST[4] = {0xFD};
  for(int f=0; f<1; f++)
  {
    CAT.write(POST[f]);
    Serial.print(" Post ");
    Serial.println();
  }
} 


void READ_DATA()  //    Read data back from the radio
{
    static boolean recvInProgress = false;
    static byte index = 0;
    byte startMarker = 0xFE;
    byte endMarker = 0xFD;
    byte rb;                        // read one byte from Serial

        Serial.println(); 
        Serial.print("Incoming:  ");  
    
    while (CAT.available() > 0 && newData == false) {
        rb = CAT.read();    

        if (recvInProgress == true) 
        {
          
            if (rb != endMarker) 
            {
              rcvFromRadio[index] = rb;
              Serial.print(rcvFromRadio[index], HEX);
              Serial.print("-");            
              index++;
              if (index >= numBytes )
              {
                index = numBytes -1;
              }
            }
            else {
              rcvFromRadio[index] = '\0'; // terminate the string
              recvInProgress = false;
              numReceived = index;  // save the number for use when printing
              index = 0;
              newData = true;
            }
        }
        else if (rb == startMarker) {
            recvInProgress = true;
        }
    } 
            Serial.println();   
}


void GET_RADIO_ID()  //Get Radio ID
{
    CAT.write(0xFE);
    CAT.write(0xFE);
    CAT.write(byte(0x00));
    CAT.write(CONTROLLER_ADDRESS);
    CAT.write(0x19);
    CAT.write(byte(0x00));
    CAT.write(0xFD);

    delay(100);
    READ_DATA();
  //need to format the received byte into a radio address.
    xcvr = rcvFromRadio[5];

  Serial.println();
  Serial.print("Address received = ");  
  Serial.print(xcvr, HEX);
  Serial.println();

  for(int r=0; r<ARRAYSIZE; r++)
  {
    if(radio_attrib[r].address == xcvr)
    {
      Serial.print(" HEX Address (lookup) ");
      Serial.print(radio_attrib[r].address, HEX);
      Serial.print(" - ");
      Serial.print(xcvr, HEX);
      Serial.print(" (reported) ");
      Serial.println();  
     
      Serial.print("Radio detected;  Radio = ");
      Serial.print(radio_attrib[r].model);
      Serial.println();  
  
      model = radio_attrib[r].model;
      capabilities = radio_attrib[r].capabilities;
    }
  }
}





//====FUNCTION DEFINITIONS 4=============================================================================================================
//====Core program operations   =========================================================================================================

void SEND_CMD()         //send a command and data to the radio
{
  if(CMD_LAYERS>0)      //if no layers are present, end the function.  a failsafe.
  {
    send_preamble();

    for(int j=0;j<CMD_LAYERS;j++)            
        {
          CAT.write(CMD_DATA[j]);     //Send Commands
          Serial.print(CMD_DATA[j], HEX);
          Serial.print("-");
        }
    
    if (CMD_PAYLOAD_PRESENT > 0)    //Check if a data payload is expected, if not, skip
    {
        for(int k=0;k<CMD_PAYLOAD_PRESENT;k++)            
        {
          CAT.write(CMD_PAYLOAD[k]);  //Send Data payload, if applicale
          Serial.print(CMD_PAYLOAD[k], HEX);
          Serial.print("-");
        }
    }
  
    send_post();
  }
    //now clear the command and data payload sizes (did not clear actual buffers, in case previous command needs to be verified
    CMD_LAYERS = 0;
    CMD_PAYLOAD_PRESENT = 0;
}








//====FUNCTION DEFINITIONS 2=============================================================================================================
//====RADIO OPERATION FUNCTIONS =========================================================================================================
void SWAP_VFOS()  //Switch A/B VFOs
{
  CMD_LAYERS = 2;
  CMD_DATA[0] = VFO_FUNC;       
  CMD_DATA[1] = VFO_SWAP;
  SEND_CMD();
}







void VOICE_PLAY(int location)  
{
  if(location<9)                //There's only 8 voice memories.  failsafe to keep from sending bad addresses
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
    Serial.print("Voice Keyer Address Error");    //sebug helper
  }
}



void GET_FREQ()  //Get Radio ID
{
  CMD_LAYERS = 1;
  CMD_PAYLOAD_PRESENT = 0;
  CMD_DATA[0] = READ_FREQ;
  SEND_CMD();
  delay(10);
  READ_DATA();

  //Frequency calculation algorythm borrowed from https://github.com/darkbyte-ru/ICOM-CI-V-cat/blob/master/icom-ic820-cat.ino
  frequency = 0;  //initialize frequency before we calculate it
  for (uint8_t i = 0; i < 5; i++) 
  {
    frequency += (rcvFromRadio[9 - i] >> 4) * decMulti[i * 2];
    frequency += (rcvFromRadio[9 - i] & 0x0F) * decMulti[i * 2 + 1];
  }
  frequency = frequency * 100;
  Serial.print(frequency);

}






void PREAMP_TOGGLE(int sw) //Toggle Preamp
{
    CMD_LAYERS=2;
    CMD_DATA[0] = FEAT_FUNC;       
    CMD_DATA[1] = FEAT_PREAMP;
    CMD_PAYLOAD_PRESENT = 1;
    if(sw=2)      {CMD_PAYLOAD[0] = 0x02;     } //Preamp 2
    else if(sw=1) {CMD_PAYLOAD[0] = 0x01;     } //Preamp 1
    else          {CMD_PAYLOAD[0] = FUNC_OFF; } //Preamp OFF
    SEND_CMD();
}




//====FUNCTION DEFINITIONS 3=============================================================================================================
//====helper functions    ===============================================================================================================


  
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



//=====ON/OFF Commands==================================
//  These call the FEATURE TOGGLE helper, and send core command family, specific command, and a switch (0 / 1 = off / on)

void IP_TOGGLE(int sw)  //Toggle IP+
  { FEATURE_TOGGLE(MISC_FUNC, MISC_IPPL, sw);}
  
void NB_TOGGLE(int sw)  //Toggle Noise Blanker
  { FEATURE_TOGGLE(FEAT_FUNC, FEAT_NB, sw); }

void NR_TOGGLE(int sw)  //Toggle Noise Reduction
  { FEATURE_TOGGLE(FEAT_FUNC, FEAT_NR, sw); }

void ANOTCH_TOGGLE(int sw)  //Toggle Auto Notch
  { FEATURE_TOGGLE(FEAT_FUNC, FEAT_AN, sw);  }

void RPTRTONE_TOGGLE(int sw)  //Toggle Repeater Tone
  { FEATURE_TOGGLE(FEAT_FUNC, FEAT_PL, sw);  }

void TONESQL_TOGGLE(int sw)  //Toggle Tone Squelch
  { FEATURE_TOGGLE(FEAT_FUNC, FEAT_TSQ, sw);  }

void COMP_TOGGLE(int sw)  //Toggle Speech COmpressor
  { FEATURE_TOGGLE(FEAT_FUNC, FEAT_COMP, sw); }

void MON_TOGGLE(int sw)  //Toggle Monitor Function
  { FEATURE_TOGGLE(FEAT_FUNC, FEAT_MON, sw);  }

void VOX_TOGGLE(int sw)  //Toggle Monitor Function
  { FEATURE_TOGGLE(FEAT_FUNC, FEAT_VOX, sw);  }

void QSK_TOGGLE(int sw)  //Toggle Break-In Function
  { FEATURE_TOGGLE(FEAT_FUNC, FEAT_QSK, sw);  }

void MAN_NOTCH_TOGGLE(int sw)  //Toggle MAnual Notch Function
  { FEATURE_TOGGLE(FEAT_FUNC, FEAT_NOTCH, sw);  }

void TWN_FILT_TOGGLE(int sw)  //Toggle Twin Peak Filter Function
  { FEATURE_TOGGLE(FEAT_FUNC, FEAT_TWNPK, sw);  }

void DIALLOCK_TOGGLE(int sw)  //Toggle Dial Lock Function
  { FEATURE_TOGGLE(FEAT_FUNC, FEAT_DLOCK, sw);  }









void SEND_FREQ(long freq)
{
  char f[16],*fp;
  char lev[4],*levp;
  int k = 0;
  char ob;

  //set up the command and payload
  CMD_DATA[0] = SET_FREQ;       
  CMD_LAYERS = 1;
  CMD_PAYLOAD_PRESENT = 4;

  //reformat the frequency variable into icom comparible BCD
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


//====ARDUINO BASIC OPERATION=============================================================================================================
//====Setup and Loop functions   =========================================================================================================
void setup() {
    pinMode(rxPin, INPUT);    //configure the software serial receive pin
    pinMode(txPin, OUTPUT);   //configure the software serial transmit pin
    CAT.begin(19200);         //configure the software serial com speed
    Serial.begin(19200);      //configure the hardware serial com speed (used for debugging - the serial terminal)
 
    GET_RADIO_ID();
    dummycount=0;
//  Serial.print("No setup functions");

}

void loop() 
{
  // put your main code here, to run repeatedly:
  while(dummycount<1)
  {
      SWAP_VFOS();
      SEND_FREQ(14200000);
//      VOICE_PLAY(1);
//    Serial.print("No loop functions");
      dummycount++;
  } 
}

  
