#include "user_wifi.h"
#include <Wifi.h>
#include "systemData.h"
#include <Arduino.h>

// #define SSID "HNTR"
// #define PASSWORD "tuoren@123"

#include <WiFi.h>
#include <vector>

// WiFi配置
const char* ssid = "Your_SSID";        // 默认连接的SSID
const char* password = "Your_Password"; // 默认密码

// 结构体存储WiFi网络信息
struct WiFiNetwork {
  String ssid;
  int32_t rssi;
  uint8_t encryptionType;
  bool isConnected;
};

// 存储扫描到的网络
std::vector<WiFiNetwork> networks;

// 连接状态变量
bool isConnected = false;
unsigned long lastConnectAttempt = 0;
const unsigned long CONNECT_TIMEOUT = 30000; // 30秒连接超时

void wifi_setup_test() {
  Serial.begin(115200);
  
  // 初始化WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  Serial.println("\nESP32 WiFi扫描与连接演示");
  Serial.println("==========================");
  
  // 扫描可用网络
  scanNetworks();
  
  // 显示扫描结果
  displayNetworks();
  
  // 尝试连接到默认网络
  connectToWiFi();
}

void wifi_loop() {
  // 检查连接状态
  checkConnection();
  
  // 每30秒检查一次网络状态
  static unsigned long lastScan = 0;
  if (millis() - lastScan > 30000) {
    if (!isConnected) {
      Serial.println("\n重新扫描网络...");
      scanNetworks();
      displayNetworks();
      connectToWiFi();
    }
    lastScan = millis();
  }
  
  delay(1000);
}

// 扫描可用WiFi网络
void scanNetworks() {
  Serial.println("开始扫描WiFi网络...");
  
  networks.clear();
  
  int numNetworks = WiFi.scanNetworks();
  Serial.printf("找到 %d 个网络\n", numNetworks);
  
  if (numNetworks == 0) {
    Serial.println("未找到任何网络");
    return;
  }
  
  for (int i = 0; i < numNetworks; i++) {
    WiFiNetwork network;
    network.ssid = WiFi.SSID(i);
    network.rssi = WiFi.RSSI(i);
    network.encryptionType = WiFi.encryptionType(i);
    network.isConnected = false;
    
    networks.push_back(network);
  }
  
  // 按信号强度排序
  std::sort(networks.begin(), networks.end(), [](const WiFiNetwork& a, const WiFiNetwork& b) {
    return a.rssi > b.rssi;
  });
  
  WiFi.scanDelete();
}

// 显示扫描到的网络
void displayNetworks() {
  Serial.println("\n可用网络列表:");
  Serial.println("No. | SSID | 信号强度 | 加密方式 | 状态");
  Serial.println("----------------------------------------");
  
  for (size_t i = 0; i < networks.size() && i < 15; i++) { // 只显示前15个
    String encryption;
    switch(networks[i].encryptionType) {
      case WIFI_AUTH_OPEN: encryption = "开放"; break;
      case WIFI_AUTH_WEP: encryption = "WEP"; break;
      case WIFI_AUTH_WPA_PSK: encryption = "WPA"; break;
      case WIFI_AUTH_WPA2_PSK: encryption = "WPA2"; break;
      case WIFI_AUTH_WPA_WPA2_PSK: encryption = "WPA/WPA2"; break;
      default: encryption = "未知";
    }
    
    String status = networks[i].isConnected ? "已连接" : "可用";
    
    Serial.printf("%2d | %-20s | %4d dBm | %-8s | %s\n", 
                  i+1, 
                  networks[i].ssid.c_str(), 
                  networks[i].rssi, 
                  encryption.c_str(),
                  status.c_str());
  }
}

// 连接到WiFi
void connectToWiFi() {
  if (strlen(ssid) == 0) {
    Serial.println("未设置SSID，请配置网络信息");
    return;
  }
  
  Serial.printf("\n尝试连接到: %s\n", ssid);
  
  WiFi.begin(ssid, password);
  
  lastConnectAttempt = millis();
  isConnected = false;
  
  Serial.print("连接中");
  
  while (WiFi.status() != WL_CONNECTED && 
         millis() - lastConnectAttempt < CONNECT_TIMEOUT) {
    delay(500);
    Serial.print(".");
    
    if (WiFi.status() == WL_CONNECT_FAILED) {
      Serial.println("\n连接失败，请检查密码");
      return;
    }
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    isConnected = true;
    Serial.println("\n连接成功!");
    Serial.printf("IP地址: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("MAC地址: %s\n", WiFi.macAddress().c_str());
    
    // 更新网络列表中的连接状态
    for (auto& network : networks) {
      if (network.ssid == ssid) {
        network.isConnected = true;
        break;
      }
    }
  } else {
    Serial.println("\n连接超时");
    WiFi.disconnect();
  }
}

// 检查连接状态
void checkConnection() {
  static unsigned long lastCheck = 0;
  
  if (millis() - lastCheck > 5000) { // 每5秒检查一次
    if (isConnected && WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi连接断开!");
      isConnected = false;
      
      // 尝试重新连接
      Serial.println("尝试重新连接...");
      connectToWiFi();
    }
    lastCheck = millis();
  }
}

// 手动连接指定网络（可选功能）
bool connectToSpecificNetwork(const char* targetSSID, const char* targetPassword) {
  Serial.printf("\n手动连接到: %s\n", targetSSID);
  
  WiFi.disconnect();
  delay(1000);
  
  WiFi.begin(targetSSID, targetPassword);
  
  unsigned long startTime = millis();
  Serial.print("连接中");
  
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < CONNECT_TIMEOUT) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n连接成功!");
    ssid = targetSSID;
    password = targetPassword;
    isConnected = true;
    return true;
  } else {
    Serial.println("\n连接失败");
    return false;
  }
}

// 获取当前连接信息
void printConnectionInfo() {
  if (isConnected) {
    Serial.println("\n=== 当前连接信息 ===");
    Serial.printf("SSID: %s\n", WiFi.SSID().c_str());
    Serial.printf("信号强度: %d dBm\n", WiFi.RSSI());
    Serial.printf("IP地址: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("子网掩码: %s\n", WiFi.subnetMask().toString().c_str());
    Serial.printf("网关: %s\n", WiFi.gatewayIP().toString().c_str());
    Serial.printf("DNS: %s\n", WiFi.dnsIP().toString().c_str());
    Serial.println("====================\n");
  } else {
    Serial.println("当前未连接到任何网络");
  }
}










// void wifi_setup(void)
// {
//   //初始化Wifi物理连接
//   Serial.println("Connecting to WiFi\n");
//   Serial.print("SSID: ");
//   Serial.println(SSID);
//   WiFi.begin(SSID, PASSWORD);
//   int attempts = 0;
//   while (WiFi.status() != WL_CONNECTED && attempts < 10) 
//   {
//     delay(250);
//     Serial.print(".");
//     attempts++;
//   }
  
//   if (WiFi.status() == WL_CONNECTED)
//   {
//     Serial.println("wifi连接成功\n");
//     Serial.print("IP: \n");
//     Serial.println(WiFi.localIP());
//   } 
//   else
//   {
//     Serial.println("wifi连接失败\n");
//   }
// }

// void wifi_task(void *pvParameters) 
// {
//   const TickType_t xFrequency = pdMS_TO_TICKS(1000);// 周期为500毫秒
//   TickType_t xLastWakeTime;
//   xLastWakeTime = xTaskGetTickCount();
//   int rssi_total;
//   for(;;)
//   {
//     //WIFI重连
//     if (WiFi.status() == WL_CONNECTED)
//     {
//       rssi_total=0;
//       for(uint16_t i=0;i<500;i++)
//       {
//         rssi_total = rssi_total+WiFi.RSSI();  //通过RSSI()函数获取wifi信号强度
//         vTaskDelay(1);
//       }
//       system_data.hardware_data.rssi = rssi_total/500;
//     } 
//     else
//     {
//       Serial.println("WIFI断开\n");
//       system_data.hardware_data.rssi=-1000;
//       WiFi.begin(SSID, PASSWORD);
//     }
//     vTaskDelayUntil(&xLastWakeTime, xFrequency);
//   }
// }



