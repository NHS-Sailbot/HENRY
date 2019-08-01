struct Vec2 {
    float x, y;
};

struct RData {
    unsigned char m1_dir, m2_dir;
    unsigned int m1_pow, m2_pow;
};

struct TData {
    Vec2 gps, rc_left, rc_right;
    float wind_direction;
};

void setup() { Serial.begin(57600); }

// I have ordered them in (long, lat) order instead of the standard (lat, long)
TData tdata = {{-70.863186, 42.80212}, {0.2, 0.4}, {0.6, 0.8}, 0.0};
RData rdata;

void loop() {
    while (Serial.available() > 0) {
        Serial.readBytes(reinterpret_cast<unsigned char *>(&rdata), sizeof(RData));
        for (unsigned int i = 0; i < sizeof(TData); ++i)
            Serial.write(*(reinterpret_cast<unsigned char *>(&tdata) + i));
    }
    Serial.flush();
}
