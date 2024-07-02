#include "LiquidCrystalSegments.h"
#include <EEPROM.h>
#include <RTClib.h>

#define APPEND_ZERO_2_DIGIT_BASE_10(num) (num < 10 ? "0" : "") + String(num)

bool status;
constexpr size_t segmentSize = 8;
constexpr size_t seperatorSize = 3;
/********************
 * 0000000 11:22:33 *
 * 444, 55/666/7777 *
 ********************/
constexpr LCSegment segments[segmentSize] = {
    {7, 1, 0,  0},
    {2, 1, 8,  0},
    {2, 1, 11, 0},
    {2, 1, 14, 0},
    {3, 1, 0,  1},
    {2, 1, 5,  1},
    {3, 1, 8,  1},
    {4, 1, 12, 1}
};
constexpr LCSeperator seperators[seperatorSize] = {
    {10, 0, ':'},
    {13, 0, ':'},
    {3,  1, ','}
};
LiquidCrystalSegments<segmentSize, seperatorSize> lcds(0x27, 16, 4);
RTC_DS1307 rtc;

const char *daysOfTheWeek[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char *monthOfTheYear[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void setup() {
    // Serial Setup
    Serial.begin(115200);

    // RTC_DS1307 Setup
    if (!rtc.begin()) {
        Serial.println("Cant find RTC");
        Serial.flush();
        while (true);
    }

    // Adjust rtc                                        ┌─── Adjusted for upload time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)) + TimeSpan(5));

    // I/O Pin Setup
    pinMode(LED_BUILTIN, OUTPUT);

    // LiquidCrystal_I2C
    lcds.init(segments, seperators);
    lcds.backlight();
    lcds.clear();
    status = HIGH;

    Serial.print("EEPROM Length: ");
    Serial.println(EEPROM.length());
}

void loop() {
    DateTime now = rtc.now();

    digitalWrite(LED_BUILTIN, status = !status);

    lcds.seperatorPrint();
    lcds.segmentPrint(0, ">Clock");
    lcds.segmentPrint(1, APPEND_ZERO_2_DIGIT_BASE_10(now.hour()));
    lcds.segmentPrint(2, APPEND_ZERO_2_DIGIT_BASE_10(now.minute()));
    lcds.segmentPrint(3, APPEND_ZERO_2_DIGIT_BASE_10(now.second()));
    lcds.segmentPrint(4, daysOfTheWeek[now.dayOfTheWeek()]);
    lcds.segmentPrint(5, APPEND_ZERO_2_DIGIT_BASE_10(now.day()));
    lcds.segmentPrint(6, monthOfTheYear[now.month() - 1]);
    lcds.segmentPrint(7, now.year());
    digitalWrite(LED_BUILTIN, status = !status);
}
