#ifndef IR_EM_H
#define IR_EM_H

void IRemettitorInit();
int readEmettitor(int *sensor);
void sendInfos(int sequence[]);
void sendSequence(void);
void high(void);
void low(void);
void lowLevel(void);
int binaryTodecimal(char *binary);
int setBitSensor(int *sensorNumber);
int clearBitSensor(int *sensorNumber);

#endif