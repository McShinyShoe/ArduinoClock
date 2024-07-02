#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <math.h>

struct LCSegment {
    uint8_t size, lines, x, y;
};
struct LCSeperator {
    uint8_t x, y;
    char c;
};
template <size_t segment_size, size_t seperator_size> class LiquidCrystalSegments : public LiquidCrystal_I2C {
  public:
    LCSegment segments[segment_size];
    LCSeperator seperators[segment_size];

  public:
    void init(const LCSegment *segments, const LCSeperator *seperators) {
        LiquidCrystal_I2C::init();
        memcpy(this->segments, segments, sizeof(LCSegment) * segment_size);
        memcpy(this->seperators, seperators, sizeof(LCSeperator) * seperator_size);
    }
    template <typename print_type> void segmentPrint(size_t segment, const print_type &var) {
        String str = String(var);
        LCSegment &seg = segments[segment];
        for (size_t i = 0; i < ceil((double)str.length() / seg.size); i++) {
            if (i >= seg.lines) break;
            LiquidCrystal_I2C::setCursor(seg.x, seg.y + i);
            LiquidCrystal_I2C::print(str.substring(i * seg.size, (seg.size + (i * seg.size)) < str.length() ? (seg.size + (i * seg.size)) : str.length()));
        }
    }
    void seperatorPrint() {
        for (size_t i = 0; i < seperator_size; i++) {
            LiquidCrystal_I2C::setCursor(seperators[i].x, seperators[i].y);
            LiquidCrystal_I2C::print(seperators[i].c);
        }
    }

  public:
    LiquidCrystalSegments(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows) : LiquidCrystal_I2C(lcd_Addr, lcd_cols, lcd_rows){};
};
