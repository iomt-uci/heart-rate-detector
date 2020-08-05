/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-many-to-one-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int patient_id; // must be unique for each sender board
    int device_id;
    char patient_name[11];
    int bpm;
    int alarm_signal;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;
//struct_message board3;

// Create an array with all the structures
struct_message boardsStruct[2] = {board1, board2};

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.device_id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.device_id-1].patient_id = myData.patient_id;
  boardsStruct[myData.device_id-1].device_id = myData.device_id;
  strcpy(boardsStruct[myData.device_id-1].patient_name, myData.patient_name);
  boardsStruct[myData.device_id-1].bpm = myData.bpm;
  boardsStruct[myData.device_id-1].alarm_signal = myData.alarm_signal;

  Serial.printf("x patient_id: %d \n", boardsStruct[myData.device_id-1].patient_id);
  Serial.printf("x device_id: %d \n", boardsStruct[myData.device_id-1].device_id);
  Serial.print("x patient_name: ");
  Serial.println(boardsStruct[myData.device_id-1].patient_name);
  Serial.printf("x bpm: %d \n", boardsStruct[myData.device_id-1].bpm);
  Serial.printf("x alarm_signal: %d \n", boardsStruct[myData.device_id-1].alarm_signal);
  Serial.println();
}
 
void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
  
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  // Acess the variables for each board
  /*int board1X = boardsStruct[0].x;
  int board1Y = boardsStruct[0].y;
  int board2X = boardsStruct[1].x;
  int board2Y = boardsStruct[1].y;
  int board3X = boardsStruct[2].x;
  int board3Y = boardsStruct[2].y;*/

//  delay(1000);  
}
