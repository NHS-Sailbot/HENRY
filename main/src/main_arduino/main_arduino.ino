struct Vec2 {float x, y;};
struct Vec3 {float x, y, z;};

struct RData {
    unsigned char m1_dir, m2_dir;
    unsigned int m1_pow, m2_pow;
};

struct TData {
    long long gps_lat, gps_lon;
    Vec2 rc_left, rc_right;
    Vec3 mag;
    float wind_direction;
};

void setup() { Serial.begin(115200); }

TData tdata = {42813356, -70886233, {0.2, 0.4}, {0.6, 0.8}, {0.0, 0.0, 0.0}, 0.0};
RData rdata;

void loop() {
    while (Serial.available() > 0) {
        Serial.readBytes(reinterpret_cast<unsigned char *>(&rdata), sizeof(RData));
        for (unsigned int i = 0; i < sizeof(TData); ++i)
            Serial.write(*(reinterpret_cast<unsigned char *>(&tdata) + i));
    }
    Serial.flush();
    tdata.gps_lat += rand() % 3 - 1;
    tdata.gps_lon += rand() % 3 - 1;
}
