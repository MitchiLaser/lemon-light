/*
 * Include dependencies (Libraries)
 */

#include <ESP8266WiFi.h>        // Library to use the wireless interface from the ESP8266

#include <ESP8266WebServer.h>   // Library to create a web-interfach that can be acessed over http via a browser

#include <ESP8266WebServer.h>   // Library to run a web-server on the ESP8226




// ------------------------------------ CONFIGURATION -----------------------------------------------------

/*
 * Global set of the Baud rate, the Boxes are using to communicate with each other
 * Default is 500000
 */
 #define Serial_Baud 500000


/*
 * Configurations for the Wireless Interface:
 * - WIFI_SSID:   SSID of the network the server should connect to or the name of the network the server should create
 * - WIFI_PSK:    The WPA2 password to connect to the existing network or to the network the server should create
 * - WIFI_MASTER_EN:   If set to "false", the server is trying to connect to an existung network. 
 *                If set to "true", the server is creating an own network.
 *                The default value is "false"
 */
#define WIFI_SSID       "MyNetwork"
#define WIFI_PSK        "MySecretPassword"

#define WIFI_MASTER_EN  false


/*
 * Configurations for the Web-interface:
 * - HTTP_PORT:   The port, the web interface can be reached over. Default is 80 (http) 
 */
#define HTTP_PORT   80

// Initialise the web server as a global object
// The Server Port can be config with the global config parameter HTTP_PORT
ESP8266WebServer server(HTTP_PORT);




// ------------------------------------ IMPLEMENTED FUNCTIONS -----------------------------------------------------

/*
 * SetupWireless
 * Return value: void
 * Parameters: void
 * Description: According to the user-config this function initialises the wireless functionality of the Server.
 *              The Server can be configured as a Wireless client or an accesspoint. 
 *              The Configurations are:
 *                       - WIFI_SSID:   SSID of the network the server should connect to or the name of the network the server should create
 *                       - WIFI_PSK:    The WPA2 password to connect to the existing network or to the network the server should create
 *                       - MASTER_EN:   If set to "false", the server is trying to connect to an existung network. 
 *                                      If set to "true", the server is creating an own network
 */

void SetupWireless( void )
{
    if(WIFI_MASTER_EN)
    { // if server should create an acesspoint
        WiFi.mode(WIFI_AP);
        WiFi.softAP(WIFI_SSID, WIFI_PSK);
    }
    else
    { // if server should connect to a existing acesspoint
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WIFI_PSK);
    }
}


/*
 * SetupWebInterface
 * Return value: void
 * Parameters: void
 * Description: According to the user-config this function initialises the web interface of the server.
 *              The web interface can be configures with the following parameters:
 *                       - HTTP_PORT:   Sets the port, the web interface can be reached over. Default is 80 (http)
 */
void SetupWebInterface( void )
{
    // init server handle for the main page,
    // that delivers the web interface via http
    server.on("/", handleRoot);

    // init the server handle to receive the text that should be displayed on the LEDs
    server.on("/power", handleText);

    // init the Error-404 http handle
    server.onNotFound([]() {
        server.send(404, "text/html", "");
    });
}

/*
 * the following function delivers the main web interface for the server via http
 * it refers to "SetupWebInterface"
 */
void handleRoot( void )
{
    // TODO
}

/*
 * the following function receives the text that should be rendered on the LEDs  via http
 * it refers to "SetupWebInterface"
 */
void handleText( void )
{
    // TODO
}




// ------------------------------------ MAIN PROGRAM -----------------------------------------------------

void setup()
{
  // Initialise Serial Connection
  Serial.begin( Serial_Baud );

  // Set up the wireless interface of the ESP8266
  SetupWireless();

  // Set up the Web interface
  SetupWebInterface();
}

void loop()
{
  
}
