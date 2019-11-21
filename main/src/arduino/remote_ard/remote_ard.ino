#include <EnableInterrupt.h>

static constexpr uint32_t       //
    SERIAL_PORT_SPEED = 115200; //

static constexpr uint8_t //
    RC_NUM_CHANNELS = 6, //

    RC_CH1 = 0, //
    RC_CH2 = 1, //
    RC_CH3 = 2, //
    RC_CH4 = 3, //
    RC_CH5 = 4, //
    RC_CH6 = 5, //

    RC_CH1_INPUT = A0, //
    RC_CH2_INPUT = A1, //
    RC_CH3_INPUT = A2, //
    RC_CH4_INPUT = A3, //
    RC_CH5_INPUT = A4, //
    RC_CH6_INPUT = A5; //

struct TData {
    uint16_t rc_values[RC_NUM_CHANNELS];
};

struct RData {};

unsigned char tdata_buffer[sizeof(TData) + 2];
unsigned char rdata_buffer[sizeof(RData) + 2];
TData *const tdata = reinterpret_cast<TData *>(tdata_buffer + 1);
RData *const rdata = reinterpret_cast<RData *>(rdata_buffer + 1);

uint16_t rc_values[RC_NUM_CHANNELS];
uint32_t rc_start[RC_NUM_CHANNELS];
volatile uint16_t rc_shared[RC_NUM_CHANNELS];

static inline void rc_read_values() {
    noInterrupts();
    for (uint8_t i = 0; i < RC_NUM_CHANNELS; ++i)
        tdata->rc_values[i] = rc_shared[i];
    interrupts();
}

static inline void calc_input(uint8_t channel, uint8_t input_pin) {
    if (digitalRead(input_pin) == HIGH) {
        rc_start[channel] = micros();
    } else {
        rc_shared[channel] = (uint16_t)(micros() - rc_start[channel]);
    }
}

static inline void calc_ch1() { calc_input(RC_CH1, RC_CH1_INPUT); }
static inline void calc_ch2() { calc_input(RC_CH2, RC_CH2_INPUT); }
static inline void calc_ch3() { calc_input(RC_CH3, RC_CH3_INPUT); }
static inline void calc_ch4() { calc_input(RC_CH4, RC_CH4_INPUT); }
static inline void calc_ch5() { calc_input(RC_CH5, RC_CH5_INPUT); }
static inline void calc_ch6() { calc_input(RC_CH6, RC_CH6_INPUT); }

void setup() {
    Serial.begin(SERIAL_PORT_SPEED);

    const unsigned char key = 0xBD;
    tdata_buffer[0] = key, tdata_buffer[sizeof(TData) + 1] = key;

    pinMode(RC_CH1_INPUT, INPUT);
    pinMode(RC_CH2_INPUT, INPUT);
    pinMode(RC_CH3_INPUT, INPUT);
    pinMode(RC_CH4_INPUT, INPUT);
    pinMode(RC_CH5_INPUT, INPUT);
    pinMode(RC_CH6_INPUT, INPUT);

    enableInterrupt(RC_CH1_INPUT, calc_ch1, CHANGE);
    enableInterrupt(RC_CH2_INPUT, calc_ch2, CHANGE);
    enableInterrupt(RC_CH3_INPUT, calc_ch3, CHANGE);
    enableInterrupt(RC_CH4_INPUT, calc_ch4, CHANGE);
    enableInterrupt(RC_CH5_INPUT, calc_ch5, CHANGE);
    enableInterrupt(RC_CH6_INPUT, calc_ch6, CHANGE);
}

void loop() {
    rc_read_values();

    if (Serial.available() > sizeof(rdata_buffer) - 1) {
        while (Serial.available() > 0) {
            Serial.readBytes(reinterpret_cast<unsigned char *>(&rdata_buffer), sizeof(rdata_buffer));
            Serial.write(reinterpret_cast<unsigned char *>(&tdata_buffer), sizeof(tdata_buffer));
            Serial.flush();
        }
    }
}
