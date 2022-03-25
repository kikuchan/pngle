// Sketch to fetch images from web and display on TFT

#include <TFT_eSPI.h>              // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();         // Invoke custom library

#include <HTTPClient.h>

#include "pngle.h"

#define WIFI_SSID "XXXXXXXX"
#define WIFI_PASS "XXXXXXXX"

// Clear screen and reset text cursor etc
void cls()
{
  tft.fillScreen(TFT_WHITE);

  tft.setCursor(0, 0);
  tft.setTextColor(TFT_BLACK);
  tft.setTextFont(4);
}

// ===================================================
// pngle example for TFT_eSPI
// ===================================================

double g_scale = 1.0;
// pngle callback, called during decode process for each pixel
void pngle_on_draw(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4])
{
  // Convert to RGB 565 format
  uint16_t color = tft.color565(rgba[0], rgba[1], rgba[2]);

  // If alpha > 0 then draw
  if (rgba[3]) {
    if (g_scale <= 1.0) tft.drawPixel(x, y, color);
    else {
      x = ceil(x * g_scale);
      y = ceil(y * g_scale);
      w = ceil(w * g_scale);
      h = ceil(h * g_scale);
      tft.fillRect(x, y, w, h, color);
    }
  }

}

void load_png(const char *url, double scale = 1.0)
{
  HTTPClient http;

  http.begin(url);

  int httpCode = http.GET();
  if (httpCode != HTTP_CODE_OK) {
    tft.printf("HTTP ERROR: %u\n", httpCode);
    http.end();
    return ;
  }

  int total = http.getSize();

  WiFiClient *stream = http.getStreamPtr();

  pngle_t *pngle = pngle_new();
  pngle_set_draw_callback(pngle, pngle_on_draw);
  g_scale = scale;

  uint8_t buf[2048];
  int remain = 0;
  uint32_t timeout = millis();
  while (http.connected()  && (total > 0 || remain > 0)) {

    // Break out of loop after 10s
    if ((millis() - timeout) > 10000UL)
    {
      tft.printf("HTTP request timeout\n");
      break;
    }

    size_t size = stream->available();

    if (!size) { delay(1); continue; }

    if (size > sizeof(buf) - remain) {
      size = sizeof(buf) - remain;
    }

    int len = stream->readBytes(buf + remain, size);
    if (len > 0) {
      int fed = pngle_feed(pngle, buf, remain + len);
      if (fed < 0) {
        cls();
        tft.printf("ERROR: %s\n", pngle_error(pngle));
        break;
      }
      total -= len;
      remain = remain + len - fed;
      if (remain > 0) memmove(buf, buf + fed, remain);
    } else {
      delay(1);
    }
  }

  pngle_destroy(pngle);

  http.end();
}
// ===================================================

void setup()
{
  Serial.begin(115200);

  tft.begin();
  cls();

  tft.printf("Welcome.\n");

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  tft.printf("WiFi connected.\n");
}

void loop()
{
  cls();
  load_png("https://raw.githubusercontent.com/kikuchan/pngle/master/tests/pngsuite/PngSuite.png");
  delay(5000);

  cls();
  load_png("https://raw.githubusercontent.com/kikuchan/pngle/master/tests/pngsuite/tbrn2c08.png", 7);
  delay(5000);

  cls();
  load_png("https://avatars3.githubusercontent.com/u/17420673?s=240");
  delay(5000);
}
