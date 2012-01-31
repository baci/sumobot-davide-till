#include <avr/io.h>
#include <string.h>
#include <math.h>
#include "sumobot.h"
#include "IRemettitor.h"
#include "timer.h"

//frame in the beginning of each message
const char headerFrame[4] = "0101";
// ID of the broadcast and multicast
const char Broad_Multi_UniqueID[3] = "111";
const char BroadcastGlobalID[2] = "11";

//Sumobot Unique ID and Team ID
const char BotID[3] = "001";	// BotID is 001
const char TeamGID[2] = "00";	// Bot's team 02

/* UID	GID
 * 111	11	--> Broadcast (to every sumobots in the field) 
 * 111	01	--> Multicast (to every sumobots of the team 1)
 * 001	01	--> Singlecast (Sumobot 001 in team 01)
 */

void IRemettitorInit(){
	//PB1 - left
	DDRB &= ~(1<<DD1);
	//PB3 - right
	DDRB &= ~(1<<DD3);
}

int readEmettitor(int *sensor){
	int counter = 0;
	char bodyFrame[14];
	int bit = 0;
	int times = 0;
	
	while(1){
		//reset counter
		counter = 0;
		
		while(setBitSensor(&sensor) && counter <= 150){
			counter++;
		}
		
		if(counter >= 100){
			bit = 0;
			times++;
		}
		
		if(times >= 3)
			return -1;
			
		//reset counter
		counter = 0;
		
		while(clearBitSensor(&sensor) && counter <= 50){
			counter++;
		}	
		
		if(counter >= 25){
			bodyFrame[bit] = '1';
		} else {
			bodyFrame[bit] = '0';
		}
		
		//reset counter
		counter = 0;
		
		// check header of the frame
		if(bit <= 3 && bodyFrame[bit] != headerFrame[bit]){
			bit = 0;
		} else {
			bit++;
		}
			
		// check the whole information
		if(bit == 14){
			int auxBit;
			int parityNumber = 0;
			
			/*
			  check parity		
			  parity number == 1 -> odd
			  parity number == 0 -> even
			*/
			for(auxBit = 0; auxBit <= 12; auxBit++){
				if(bodyFrame[auxBit] == '1'){
					parityNumber++;
				}
			}
		
			if((parityNumber % 2 == 0 && bodyFrame[13] == '0') ||
		   	   (parityNumber % 2 != 0 && bodyFrame[13] == '1'))	{ }
			else {
				return -3;
			}	
	
			//Checking sumobot GroupID 
			for(auxBit = 7; auxBit <= 8; auxBit++){
				if(bodyFrame[auxBit] == BroadcastGlobalID[auxBit - 7] ||
		   		   bodyFrame[auxBit] == TeamGID[auxBit - 7]){ }
				else {
					return -4;
				}
			}
	
			// conversion binary to decimal
			char binary[5];
			int numberToConvert = 0;
			strncpy(binary, &bodyFrame[9], 4);
			binary[4] = '\0';
	
			numberToConvert = binaryTodecimal(binary);
			return numberToConvert;
		}
	}
	return 0;
}

//sending an array of bits
void sendInfos(int sequence[]){
	int i = 0;
	for(i = 0; i <= 13; i++){
		if(sequence[i] == 1)
			high();
		else
			low();
		
		lowLevel();
	}
}

void sendSequence(void){
	int i;
	int sequence[14] = {0,1,0,1,0,0,0,0,0,1,1,1,1,0};
	
	// send the frame 3 times
	for(i = 0; i <= 3; i++){
		i++;
		sendInfos(sequence);
	}
	TimerWait(1000);
}

/* If in the frame there is a 1, switch on the green LED
 * the output is at 39.5 KHz signal during 2 ms 
 * which it will be interpreted as a 1 from the receiver
 */

void high(void){
	DDRB |= 1 << DD4; // left light
	TimerWait(2);
}

/* If in the frame there is a 0, switch on the red LED
 * the output is at 39.5 KHz signal during 1 ms 
 * which it will be interpreted as a 0 from the receiver
 */
void low(void){
	DDRB |= 1 << DD4; // left light
	TimerWait(1);	
}

// no LED output when changing data direction register
void lowLevel(void){
	DDRB &= ~(1 << DD4); // left light
	TimerWait(1);
}

int binaryTodecimal(char *binary){
	int n, b, k;
	int length, sum = 0;
	
	length = strlen(binary) - 1;
	for(k = 0; k <= length; k++){
		b = 1;
		n = (binary[k] - '0');
		if((n > 1) || (n < 0)){
			return (0);
		}
		b = b << (length - k);
		sum = sum + n * b;
	}
	return (sum);
}

int setBitSensor(int *sensorNumber){
	switch(*sensorNumber){
		case 1:
			if(bit_is_set(PINB, PINB1)) // left
				return 1;
			break;
		case 2:
			if(bit_is_set(PINB, PINB3)) // right
				return 1;
			break;
		default:
			return 0;
	}
	return 0;
}

int clearBitSensor(int *sensorNumber){
	switch(*sensorNumber){
		case 1:
			if(bit_is_clear(PINB, PINB1)) // left
				return 1;
			break;
		case 2:
			if(bit_is_clear(PINB, PINB3)) // right
				return 1;
			break;
		default:
			return 0;
	}
	return 0;	
}
