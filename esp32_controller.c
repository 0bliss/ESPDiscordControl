#include <WiFi.h>
#include <HTTPClient.h>

// ==== [ MODIFY BELOW THIS LINE ] ====
String api_address = "";
String api_port = "";
String api_endpoint = "";

// SSID of network
const char* SSID = "";

// Wi-Fi network password
const char* PASSWORD = "";

// This can be modified, a request will be sent this often
// good reason to keep the API local, also for security
const uint32_t CHECK_INTERVAL = 1000; // set milliseconds here

// Do something with the string received from server
void parse_response(String response_text) {
  // Debugging action
  Serial.println(response_text); 
}


// ==== [ EVERYTHING HERE SHOULD NOT BE MODIFIED ] ====

HTTPClient http; // used for API requests
String response = ""; // Filled for each request
String ip_address = ""; // Filled by DHCP
String api_url = ""; // Constructed automatically later
const unsigned long BAUD = 115200; // keep this, it's the most common esp speed

// Easy form url to make more legible
String mkurl(String address, String endpoint, String  port="80") {

  String url = "http://" + address + ":" + port + "/" + endpoint;
}

// Sleep milliseconds
void delay_ms(uint32_t milliseconds) {
  vTaskDelay(milliseconds / portTICK_PERIOD_MS);
}

// This pauses runtime until a network
// connection is available
void await_network_connection() {
  int f_iter = 0; // failed iterations for serial warnings

  // Set Wi-Fi mode
  WiFi.mode(WIFI_STA);
  
  // Start connection
  WiFi.begin(SSID, PASSWORD);
    
  // Await network connection
  Serial.print("- Connecting");
  while( WiFi.status() != WL_CONNECTED) {
    // Show progress
    Serial.print(".");
    // Sleep for a second
    delay_ms(1000);
    f_iter++;

    // Every 10 iterations
    if (f_iter % 10) {
      Serial.println("");
      Serial.println("Network seems unresponsive. Still waiting...");
      Serial.print("- Connecting");
    }
  }
  Serial.println("");

  // Log serial once connected
  ip_address = WiFi.localIP().toString();
  Serial.println ("- Connected to wireless network.");
  Serial.print   ("- DHCP IPv4 Address: ");
  Serial.println (ip_address); 
}



void setup() {
  
  // Begin serial
  Serial.begin(BAUD);

  // Wait for serial to open
  delay(1000);

  // Connect to le interwebs
  await_network_connection(); 

  // Construct URL of control server
  api_url = mkurl(api_address, api_endpoint, api_port);
}

void loop() {

  // Each time this loops a HTTP GET
  // request is sent to the URL. The server will
  // tell us anything we need to know
  
  // Wait a bit
  delay_ms(CHECK_INTERVAL);

  // HTTP GET the URL
  http.begin(api_url);
  http.GET();

  // Read server response
  response = http.getString();

  // Do something depending on what it says
  if (!response.isEmpty() && response.c_str() != "\n") {
    parse_response(response);    
  }
}
