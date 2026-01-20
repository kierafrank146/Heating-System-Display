<<<<<<< HEAD

/*
All INPUT sensors output in degrees C (AIRo range could be -50.0 C to +50.0 C for Air.  All other sensors can be from -5.0 C to +60.0 C)
INPUT_HPi;		       Heat pump in temp
INPUT_Hpo;		       Heat pump out temp in C
INPUT_TANKi;		     Tank in temp
INPUT_TANKo;		     Tank out temp
INPUT_GNDi;		       Ground in temp
INPUT_GNDo;		       Ground out temp
INPUT_AIRo;		       Air out temp

MODE;		             0 - Off, 1 - Heat,  2 - Cool, 3 - Test, 4 - Critical Error
MODE_heat_floor;		 1st/2nd Floor Pump:  0 - Floor Pump Off, 1 - Floor Pump On & HP On during Off-Peak only, 2 - Floor Pump On & HP On during Mid/Off-Peak only, 3 - Floor Pump On & HP On during Any peak
MODE_heat_forced;		 0 - Off, 1 - Off-Peak only, 2 - Mid/Off-Peak only, 3 - Any peak
MODE_cool_forced;		 0 - Off-Peak only, 1 - Mid/Off-Peak only, 2 - Any peak
MODE_proactive;		   0 - Off, 1 - Heat/Chill tank before next TOU

INPUT_PUMP_PWR_HP;		 Pump Power (in Watts.  Range can be from 0.0 Watts to 120.0 Watts) 

// States can be 1/0 (or HIGH/LOW)
STATE_HP_O;		         Cooling 
STATE_HP_Y;		         Compressor
STATE_PUMP_HP;		     PUMP HP
STATE_PUMP_BMNT;		   PUMP BASEMENT
STATE_PUMP_1ST2ND;		 PUMP 1ST/2ND
STATE_AH_W;		         AH Aux Heat
STATE_AH_O;		         AH Cool/Heat		High (cooling), Low (heating)
STATE_AH_Y;		         AH Fan		High (fan on), Low (fan off)
STATE_AH_G;		         AH Fan High

CriticalFlag;		       0 – No Alarm, 1 - Alarm
PowerAlarmLevelFlag;	 0 - n/a, 1 - Warning,  2 – Critical
TempAlarmLevelFlag;		 0 - n/a, 1 - Warning,  2 – Critical
RelayAlarmLevelFlag;	 0 - n/a, 1 - Warning,  2 – Critical

*/

float INPUT_HPi[] = {10.7, 10.8, 10.9, 11.3, 11.5,   11.6, 11.9, 12.4, 12.6, 13.1,  13.5, 14.2, 14.9, 15.6, 16.5,   16.6, 16.7, 16.1, 16.0, 15.8,   15.5, 15.2, 14.9, 14.5, 14.1,   13.7, 13.4, 13.0, 12.6, 12.3,    11.9, 11.5, 11.1, 10.8, 10.5 };
float INPUT_HPo[] = {14.1, 14.5, 14.9, 15.5, 15.9,   16.3, 16.5, 16.6, 16.9, 17.2,  17.5, 17.9, 18.3, 18.5, 18.9,   19.3, 19.7, 19.5, 19.3, 19.1,   18.8, 18.3, 18.1, 17.9, 17.4,   16.9, 16.5, 16.1, 15.9, 15.7,    15.4, 15.2, 14.9, 14.6, 14.5 };

float INPUT_TANKi[] = {9.7,  9.8,  9.9, 10.2, 10.3,   10.5, 10.6, 10.5, 10.9, 11.0,  11.2, 11.5, 11.6, 12.0, 12.3,   12.8, 13.0, 13.1, 13.0, 12.8,   12.6, 12.4, 12.3, 12.2, 12.3,   12.0, 11.8, 11.6, 11.3, 11.2,    10.9, 10.7, 10.5, 10.4, 10.2 };
float INPUT_TANKo[] = {10.2, 10.4, 10.6, 10.8, 10.9,   11.1, 11.5, 11.8, 12.1, 12.5,  13.1, 13.8, 14.4, 15.2, 16.0,   16.1, 16.2, 16.1, 16.0, 15.8,   15.5, 15.2, 14.9, 14.5, 14.1,   13.7, 13.4, 13.0, 12.6, 12.3,    11.9, 11.5, 11.1, 10.8, 10.5 };

float INPUT_GNDi[] = {14.9, 14.2, 13.2, 11.8, 10.2,   10.1, 10.1, 9.8, 10.1, 10.0,  10.1, 10.0, 9.9, 10.2, 10.0,   10.1, 10.2, 10.1, 10.2, 10.8,   11.0, 11.3, 11.5, 11.6, 11.7,    12.0, 12.2, 12.5, 12.7, 13.0,    13.3, 13.6, 13.9, 14.3, 14.6 };
float INPUT_GNDo[] = {14.3, 13.9, 13.0, 11.0, 9.5,    8.6,  8.2,  7.7,  7.5,  7.2,   7.0,  7.1, 6.9,  7.0,  6.8,    6.8,  6.7,  6.6,  6.8,  7.0,    7.5,  7.7,  8.0,  8.5,  9.0,     9.3,  9.5,  9.8, 10.1, 10.5,    11.0, 12.0, 12.5, 13.2, 13.9 };

float INPUT_AIRo[] = {-11.3, -11.2, -11.3, -11.2, -11.5,    -11.3, -11.2, -11.3, -11.2, -11.3,    -11.3, -11.1, -11.3, -11.2, -11.1,    -11.3, -11.2, -11.5, -11.2, -11.3,   -11.3, -11.2, -11.3, -11.0, -11.3,  -11.3, -11.2, -11.3, -11.1, -11.3,   -11.0, -11.2, -11.3,-11.2, -11.3};

byte MODE[] = {1, 1, 1, 1, 1,   1, 1, 1, 1, 1,  1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1};
byte MODE_heat_floor[] = {2, 2, 2, 2, 2,   2, 2, 2, 2, 2,   2, 2, 2, 2, 2,   2, 2, 2, 2, 2,   2, 2, 2, 2, 2,   2, 2, 2, 2, 2,   2, 2, 2, 2, 2};
byte MODE_heat_forced[] = {1, 1, 1, 1, 1,   1, 1, 1, 1, 1,  1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1};
byte MODE_cool_forced[] = {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte MODE_proactive[] = {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};

float INPUT_PUMP_PWR_HP[] = {65.5, 103.2, 102.3, 103.1, 103.3,   105.5, 105.1, 105.2, 104.2, 103.5,   102.2, 99.9, 102.2, 103.3, 103.2,  102.9, 103.2, 45.5, 0.0, 0.0,   0.0, 0.0, 0.0, 0.0, 0.0,   0.0, 0.0, 0.0, 0.0, 0.0,  0.0, 0.0, 0.0, 0.0, 0.0, };

byte STATE_HP_O[] = {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte STATE_HP_Y[] = {0, 0, 0, 0, 0,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte STATE_PUMP_HP[]= {0, 0, 0, 0, 0,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte STATE_PUMP_BMNT[]= {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte STATE_PUMP_1ST2ND[]= {1, 1, 1, 1, 1,   1, 1, 1, 1, 1,  1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1};
byte STATE_AH_W[]= {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte STATE_AH_O[]= {1, 1, 1, 1, 1,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte STATE_AH_Y[]= {0, 0, 0, 0, 0,   1, 1, 1, 1, 1,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte STATE_AH_G[]= {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   1, 1, 1, 1, 1,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};

byte CriticalFlag[] = {1, 1, 0, 0, 0,   0, 0, 0, 0, 0,   1, 1, 0, 0, 0,   0, 0, 0, 0, 0,   1, 1, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte PowerAlarmLevelFlag[] = {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   1, 1, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte TempAlarmLevelFlag[] = {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   1, 1, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte RelayAlarmLevelFlag[] = {1, 1, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};

int count = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {

  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage l Serial.print("INPUT_HPi=");

  Serial.print(INPUT_HPi[count]);
  Serial.print(" ");
  Serial.print(INPUT_HPo[count]);
  Serial.print(" ");
  Serial.print(INPUT_TANKi[count]);
  Serial.print(" ");
  Serial.print(INPUT_TANKo[count]);
  Serial.print(" ");
  Serial.print(INPUT_GNDi[count]);
  Serial.print(" ");
  Serial.print(INPUT_GNDo[count]);
  Serial.print(" ");
  Serial.print(INPUT_AIRo[count]);
  Serial.print(" ");
  Serial.print(MODE[count]);
  Serial.print(" ");
  Serial.print(MODE_heat_floor[count]);
  Serial.print(" ");  
  Serial.print(MODE_heat_forced[count]);
  Serial.print(" "); 
  Serial.print(MODE_cool_forced[count]);
  Serial.print(" "); 
  Serial.print(MODE_proactive[count]);
  Serial.print(" "); 
  Serial.print(INPUT_PUMP_PWR_HP[count]);
  Serial.print(" "); 
  Serial.print(STATE_HP_O[count]);
  Serial.print(" "); 
  Serial.print(STATE_HP_Y[count]);
  Serial.print(" "); 
  Serial.print(STATE_PUMP_HP[count]);
  Serial.print(" "); 
  Serial.print(STATE_PUMP_BMNT[count]);
  Serial.print(" "); 
  Serial.print(STATE_PUMP_1ST2ND[count]);
  Serial.print(" "); 
  Serial.print(STATE_AH_W[count]);
  Serial.print(" "); 
  Serial.print(STATE_AH_O[count]);
  Serial.print(" "); 
  Serial.print(STATE_AH_Y[count]);
  Serial.print(" "); 
  Serial.print(STATE_AH_G[count]);
  Serial.print(" "); 

  Serial.print(CriticalFlag[count]);
  Serial.print(" "); 
  Serial.print(PowerAlarmLevelFlag[count]);
  Serial.print(" "); 
  Serial.print(TempAlarmLevelFlag[count]);
  Serial.print(" "); 
  Serial.println(RelayAlarmLevelFlag[count]);


  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second

  count++;

  if (count > 34){
    count=0;
  }
}
=======

/*
All INPUT sensors output in degrees C (AIRo range could be -50.0 C to +50.0 C for Air.  All other sensors can be from -5.0 C to +60.0 C)
INPUT_HPi;		       Heat pump in temp
INPUT_Hpo;		       Heat pump out temp in C
INPUT_TANKi;		     Tank in temp
INPUT_TANKo;		     Tank out temp
INPUT_GNDi;		       Ground in temp
INPUT_GNDo;		       Ground out temp
INPUT_AIRo;		       Air out temp

MODE;		             0 - Off, 1 - Heat,  2 - Cool, 3 - Test, 4 - Critical Error
MODE_heat_floor;		 1st/2nd Floor Pump:  0 - Floor Pump Off, 1 - Floor Pump On & HP On during Off-Peak only, 2 - Floor Pump On & HP On during Mid/Off-Peak only, 3 - Floor Pump On & HP On during Any peak
MODE_heat_forced;		 0 - Off, 1 - Off-Peak only, 2 - Mid/Off-Peak only, 3 - Any peak
MODE_cool_forced;		 0 - Off-Peak only, 1 - Mid/Off-Peak only, 2 - Any peak
MODE_proactive;		   0 - Off, 1 - Heat/Chill tank before next TOU

INPUT_PUMP_PWR_HP;		 Pump Power (in Watts.  Range can be from 0.0 Watts to 120.0 Watts) 

// States can be 1/0 (or HIGH/LOW)
STATE_HP_O;		         Cooling 
STATE_HP_Y;		         Compressor
STATE_PUMP_HP;		     PUMP HP
STATE_PUMP_BMNT;		   PUMP BASEMENT
STATE_PUMP_1ST2ND;		 PUMP 1ST/2ND
STATE_AH_W;		         AH Aux Heat
STATE_AH_O;		         AH Cool/Heat		High (cooling), Low (heating)
STATE_AH_Y;		         AH Fan		High (fan on), Low (fan off)
STATE_AH_G;		         AH Fan High

CriticalFlag;		       0 – No Alarm, 1 - Alarm
PowerAlarmLevelFlag;	 0 - n/a, 1 - Warning,  2 – Critical
TempAlarmLevelFlag;		 0 - n/a, 1 - Warning,  2 – Critical
RelayAlarmLevelFlag;	 0 - n/a, 1 - Warning,  2 – Critical

*/

float INPUT_HPi[] = {10.7, 10.8, 10.9, 11.3, 11.5,   11.6, 11.9, 12.4, 12.6, 13.1,  13.5, 14.2, 14.9, 15.6, 16.5,   16.6, 16.7, 16.1, 16.0, 15.8,   15.5, 15.2, 14.9, 14.5, 14.1,   13.7, 13.4, 13.0, 12.6, 12.3,    11.9, 11.5, 11.1, 10.8, 10.5 };
float INPUT_HPo[] = {14.1, 14.5, 14.9, 15.5, 15.9,   16.3, 16.5, 16.6, 16.9, 17.2,  17.5, 17.9, 18.3, 18.5, 18.9,   19.3, 19.7, 19.5, 19.3, 19.1,   18.8, 18.3, 18.1, 17.9, 17.4,   16.9, 16.5, 16.1, 15.9, 15.7,    15.4, 15.2, 14.9, 14.6, 14.5 };

float INPUT_TANKi[] = {9.7,  9.8,  9.9, 10.2, 10.3,   10.5, 10.6, 10.5, 10.9, 11.0,  11.2, 11.5, 11.6, 12.0, 12.3,   12.8, 13.0, 13.1, 13.0, 12.8,   12.6, 12.4, 12.3, 12.2, 12.3,   12.0, 11.8, 11.6, 11.3, 11.2,    10.9, 10.7, 10.5, 10.4, 10.2 };
float INPUT_TANKo[] = {10.2, 10.4, 10.6, 10.8, 10.9,   11.1, 11.5, 11.8, 12.1, 12.5,  13.1, 13.8, 14.4, 15.2, 16.0,   16.1, 16.2, 16.1, 16.0, 15.8,   15.5, 15.2, 14.9, 14.5, 14.1,   13.7, 13.4, 13.0, 12.6, 12.3,    11.9, 11.5, 11.1, 10.8, 10.5 };

float INPUT_GNDi[] = {14.9, 14.2, 13.2, 11.8, 10.2,   10.1, 10.1, 9.8, 10.1, 10.0,  10.1, 10.0, 9.9, 10.2, 10.0,   10.1, 10.2, 10.1, 10.2, 10.8,   11.0, 11.3, 11.5, 11.6, 11.7,    12.0, 12.2, 12.5, 12.7, 13.0,    13.3, 13.6, 13.9, 14.3, 14.6 };
float INPUT_GNDo[] = {14.3, 13.9, 13.0, 11.0, 9.5,    8.6,  8.2,  7.7,  7.5,  7.2,   7.0,  7.1, 6.9,  7.0,  6.8,    6.8,  6.7,  6.6,  6.8,  7.0,    7.5,  7.7,  8.0,  8.5,  9.0,     9.3,  9.5,  9.8, 10.1, 10.5,    11.0, 12.0, 12.5, 13.2, 13.9 };

float INPUT_AIRo[] = {-11.3, -11.2, -11.3, -11.2, -11.5,    -11.3, -11.2, -11.3, -11.2, -11.3,    -11.3, -11.1, -11.3, -11.2, -11.1,    -11.3, -11.2, -11.5, -11.2, -11.3,   -11.3, -11.2, -11.3, -11.0, -11.3,  -11.3, -11.2, -11.3, -11.1, -11.3,   -11.0, -11.2, -11.3,-11.2, -11.3};

byte MODE[] = {1, 1, 1, 1, 1,   1, 1, 1, 1, 1,  1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1};
byte MODE_heat_floor[] = {2, 2, 2, 2, 2,   2, 2, 2, 2, 2,   2, 2, 2, 2, 2,   2, 2, 2, 2, 2,   2, 2, 2, 2, 2,   2, 2, 2, 2, 2,   2, 2, 2, 2, 2};
byte MODE_heat_forced[] = {1, 1, 1, 1, 1,   1, 1, 1, 1, 1,  1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1};
byte MODE_cool_forced[] = {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte MODE_proactive[] = {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};

float INPUT_PUMP_PWR_HP[] = {65.5, 103.2, 102.3, 103.1, 103.3,   105.5, 105.1, 105.2, 104.2, 103.5,   102.2, 99.9, 102.2, 103.3, 103.2,  102.9, 103.2, 45.5, 0.0, 0.0,   0.0, 0.0, 0.0, 0.0, 0.0,   0.0, 0.0, 0.0, 0.0, 0.0,  0.0, 0.0, 0.0, 0.0, 0.0, };

byte STATE_HP_O[] = {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte STATE_HP_Y[] = {0, 0, 0, 0, 0,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte STATE_PUMP_HP[]= {0, 0, 0, 0, 0,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte STATE_PUMP_BMNT[]= {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte STATE_PUMP_1ST2ND[]= {1, 1, 1, 1, 1,   1, 1, 1, 1, 1,  1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1,   1, 1, 1, 1, 1};
byte STATE_AH_W[]= {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte STATE_AH_O[]= {1, 1, 1, 1, 1,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte STATE_AH_Y[]= {0, 0, 0, 0, 0,   1, 1, 1, 1, 1,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte STATE_AH_G[]= {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   1, 1, 1, 1, 1,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};

byte CriticalFlag[] = {1, 1, 0, 0, 0,   0, 0, 0, 0, 0,   1, 1, 0, 0, 0,   0, 0, 0, 0, 0,   1, 1, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte PowerAlarmLevelFlag[] = {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   1, 1, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte TempAlarmLevelFlag[] = {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   1, 1, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};
byte RelayAlarmLevelFlag[] = {1, 1, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0};

int count = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {

  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage l Serial.print("INPUT_HPi=");

  Serial.print(INPUT_HPi[count]);
  Serial.print(" ");
  Serial.print(INPUT_HPo[count]);
  Serial.print(" ");
  Serial.print(INPUT_TANKi[count]);
  Serial.print(" ");
  Serial.print(INPUT_TANKo[count]);
  Serial.print(" ");
  Serial.print(INPUT_GNDi[count]);
  Serial.print(" ");
  Serial.print(INPUT_GNDo[count]);
  Serial.print(" ");
  Serial.print(INPUT_AIRo[count]);
  Serial.print(" ");
  Serial.print(MODE[count]);
  Serial.print(" ");
  Serial.print(MODE_heat_floor[count]);
  Serial.print(" ");  
  Serial.print(MODE_heat_forced[count]);
  Serial.print(" "); 
  Serial.print(MODE_cool_forced[count]);
  Serial.print(" "); 
  Serial.print(MODE_proactive[count]);
  Serial.print(" "); 
  Serial.print(INPUT_PUMP_PWR_HP[count]);
  Serial.print(" "); 
  Serial.print(STATE_HP_O[count]);
  Serial.print(" "); 
  Serial.print(STATE_HP_Y[count]);
  Serial.print(" "); 
  Serial.print(STATE_PUMP_HP[count]);
  Serial.print(" "); 
  Serial.print(STATE_PUMP_BMNT[count]);
  Serial.print(" "); 
  Serial.print(STATE_PUMP_1ST2ND[count]);
  Serial.print(" "); 
  Serial.print(STATE_AH_W[count]);
  Serial.print(" "); 
  Serial.print(STATE_AH_O[count]);
  Serial.print(" "); 
  Serial.print(STATE_AH_Y[count]);
  Serial.print(" "); 
  Serial.print(STATE_AH_G[count]);
  Serial.print(" "); 

  Serial.print(CriticalFlag[count]);
  Serial.print(" "); 
  Serial.print(PowerAlarmLevelFlag[count]);
  Serial.print(" "); 
  Serial.print(TempAlarmLevelFlag[count]);
  Serial.print(" "); 
  Serial.println(RelayAlarmLevelFlag[count]);


  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second

  count++;

  if (count > 34){
    count=0;
  }
}
>>>>>>> 720697e (Update site and data)
