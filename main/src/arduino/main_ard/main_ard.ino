static constexpr uint32_t SERIAL_PORT_SPEED = 115200;

static constexpr uint8_t  //
    WIND_SENSOR_PIN = A0; //

struct TData {
    uint32_t wind_sensor_reading;
};

struct RData {
    uint32_t m1_dir, m2_dir, m1_pow, m2_pow;
};

unsigned char tdata_buffer[sizeof(TData) + 2];
unsigned char rdata_buffer[sizeof(RData) + 2];
TData *const tdata = reinterpret_cast<TData *>(tdata_buffer + 1);
RData *const rdata = reinterpret_cast<RData *>(rdata_buffer + 1);

static inline uint16_t get_wind_sensor() {
    // return 1024;
    return analogRead(WIND_SENSOR_PIN);
}

void setup() {
    Serial.begin(SERIAL_PORT_SPEED);

    const unsigned char key = 0xA5;
    tdata_buffer[0] = key, tdata_buffer[sizeof(TData) + 1] = key;
}

void loop() {
    tdata->wind_sensor_reading = get_wind_sensor();

    if (Serial.available() > sizeof(rdata_buffer) - 1) {
        while (Serial.available() > 0) {
            Serial.readBytes(reinterpret_cast<unsigned char *>(&rdata_buffer), sizeof(rdata_buffer));
            Serial.write(reinterpret_cast<unsigned char *>(&tdata_buffer), sizeof(tdata_buffer));
            Serial.flush();
        }
    }
}
