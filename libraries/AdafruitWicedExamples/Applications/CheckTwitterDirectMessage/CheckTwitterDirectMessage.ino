/*********************************************************************
 This is an example for our Feather WIFI modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include "adafruit_feather.h"
#include "adafruit_http.h"
#include "adafruit_sha1.h"
#include "adafruit_base64.h"
#include "adafruit_urlencode.h"
#include "adafruit_twitter.h"

/* This example demonstrates how to use AdafrtuiTwitter class
 * to send out a tweet
 *
 * To run this demo:
 * 1. Goto https://apps.twitter.com/ and login
 * 2. Create an application to use with this WICED Feather
 * 3. Change the access level to give the applicaion permission to send 
 * Direct Message (If not enabled already)
 * 4. In the app management click "manage keys and access tokens"
 * and then click "Create my access token"
 * 5. Change CONSUMER_KEY, CONSUMER_SECRET, TOKEN_ACCESS, TOKEN_SECRET accordingly
 * 6. Change your DM_SCREENNAME (twitter account e.g "adafruit"), and your DM_MESSAGE.
 * Note: you can only send DM to follower's account.
 * 7. Compile and run, if you run this sketch too often, Twitter server may reject
 * your connection request, just wait a few minutes and try again.
 */

// Network
#define WLAN_SSID               "yourSSID"
#define WLAN_PASS               "yourPassword"

// Twitter Account
#define CONSUMER_KEY            "YOUR_CONSUMER_KEY"
#define CONSUMER_SECRET         "YOUR_CONSUMER_SECRET"

#define TOKEN_ACCESS            "YOUR_TOKEN_ACCESS"
#define TOKEN_SECRET            "YOUR_TOKEN_SECRET"

#define DM_SCREENNAME           "Account Recieve DM"
#define DM_MESSAGE              "Hello from Adafruit WICED Feather"

AdafruitTwitter Twitter;

/**************************************************************************/
/*!
    @brief  The setup function runs once when reset the board
*/
/**************************************************************************/
void setup()
{
  Serial.begin(115200);

  // Wait for the Serial Monitor to open
  while (!Serial)
  {
    /* Delay required to avoid RTOS task switching problems */
    delay(1);
  }

  Serial.println("Twitter Check Direct Message Example\r\n");

  // Print all software versions
  Feather.printVersions();

  while ( !connectAP() )
  {
    delay(500); // delay between each attempt
  }

  // Connected: Print network info
  Feather.printNetwork();

  Twitter.begin(CONSUMER_KEY, CONSUMER_SECRET, TOKEN_ACCESS, TOKEN_SECRET);
  Twitter.err_actions(true, true);

  Serial.println("Set callback for New Direct Message");
  Twitter.setDirectMessageRecvCallback(direct_message_received);
}


/**************************************************************************/
/*!
    @brief  Callback when a Direct Message is retrieved
*/
/**************************************************************************/
void direct_message_received(TwitterDM dm)
{
  Serial.print("Message ID: "); Serial.println(dm.id);
  Serial.print("Sender    : "); Serial.println(dm.sender);
  Serial.print("Text      : "); Serial.println(dm.text);
  Serial.print("Created at: "); Serial.println(dm.created_at);
}

/**************************************************************************/
/*!
    @brief  The loop function runs over and over again forever
*/
/**************************************************************************/
void loop()
{
  Twitter.checkDirectMessage();
  Serial.println("Wait 10 seconds before next check");
  delay(10000);
}

/**************************************************************************/
/*!
    @brief  Connect to defined Access Point
*/
/**************************************************************************/
bool connectAP(void)
{
  // Attempt to connect to an AP
  Serial.print("Please wait while connecting to: '" WLAN_SSID "' ... ");

  if ( Feather.connect(WLAN_SSID, WLAN_PASS) )
  {
    Serial.println("Connected!");
  }
  else
  {
    Serial.printf("Failed! %s (%d)", Feather.errstr(), Feather.errnum());
    Serial.println();
  }
  Serial.println();

  return Feather.connected();
}
