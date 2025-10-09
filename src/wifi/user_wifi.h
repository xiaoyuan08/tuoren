#ifndef _USER_WIFI_H
#define _USER_WIFI_H


void scanNetworks();
void wifi_setup_test();
void wifi_loop();
void displayNetworks();
void connectToWiFi();
void checkConnection();
bool connectToSpecificNetwork(const char* targetSSID, const char* targetPassword);
void printConnectionInfo();


#endif





