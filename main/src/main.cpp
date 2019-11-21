#include <sailbot.hpp>

#define DEBUG_ENABLE_LOGGING
#include <debug.hpp>

struct Bot {
    double rc1, rc2, rc3, rc4, rc5, rc6;
    double wind_angle, lat, lon;
} static bot;

static inline void print_data() {
    debug::log::info("\033[s");
    debug::log::info("┌────────────┬───────────┬─────────┐\n");
    debug::log::info("|    data    |   value   |  units  |\n");
    debug::log::info("├────────────┼───────────┼─────────┤\n");
    debug::log::info("| rc1        | %*.4f |         |\n", 9, bot.rc1);
    debug::log::info("| rc2        | %*.4f |         |\n", 9, bot.rc2);
    debug::log::info("| rc3        | %*.4f |         |\n", 9, bot.rc3);
    debug::log::info("| rc4        | %*.4f |         |\n", 9, bot.rc4);
    debug::log::info("| rc5        | %*.4f |         |\n", 9, bot.rc5);
    debug::log::info("| rc6        | %*.4f |         |\n", 9, bot.rc6);
    debug::log::info("| wind angle | %*.4f | radians |\n", 9, bot.wind_angle);
    debug::log::info("| latitude   | %*.4f | degrees |\n", 9, bot.lat);
    debug::log::info("| longitude  | %*.4f | degrees |\n", 9, bot.lon);
    debug::log::info("└────────────┴───────────┴─────────┘\n");
    debug::log::info("\033[u");
}

struct MainRData {
    unsigned int wind_sensor_reading;
};
struct MainTData {
    unsigned int m1_dir, m2_dir, m1_pow, m2_pow;
};

struct RemoteRData {
    unsigned short rc1, rc2, rc3, rc4, rc5, rc6;
};
struct RemoteTData {};

void main_ard_update(sailbot::plat_linux::Arduino &ard) {
    MainRData &rdata = *reinterpret_cast<MainRData *>(ard.rdata_handle);
    MainTData &tdata = *reinterpret_cast<MainTData *>(ard.tdata_handle);

    bot.wind_angle = static_cast<double>(rdata.wind_sensor_reading) / 512 * 3.14159265359;

    print_data();
}

static inline constexpr double map_range(const unsigned short val, const unsigned short min_val, const unsigned short mid_val,
                                         const unsigned short max_val) {
    if (val > mid_val) {
        return static_cast<double>(val - mid_val) / (mid_val - min_val);
    } else {
        return static_cast<double>(val - mid_val) / (max_val - mid_val);
    }
}

void remote_ard_update(sailbot::plat_linux::Arduino &ard) {
    RemoteRData &rdata = *reinterpret_cast<RemoteRData *>(ard.rdata_handle);
    RemoteTData &tdata = *reinterpret_cast<RemoteTData *>(ard.tdata_handle);

    bot.rc1 = map_range(rdata.rc1, 991, 1503, 2015); // 991 - 1503 - 2015 l-
    bot.rc2 = map_range(rdata.rc2, 984, 1506, 2008); // 984 - 1506 - 2008 r|
    bot.rc3 = map_range(rdata.rc3, 978, 1451, 2001); // 978 - 1451 - 2001 l|
    bot.rc4 = map_range(rdata.rc4, 991, 1503, 2014); // 991 - 1503 - 2014 r-
    bot.rc5 = map_range(rdata.rc5, 985, 1503, 2009); // 985 - 1503 - 2009
    bot.rc6 = map_range(rdata.rc6, 978, 1506, 2003); // 978 - 1506 - 2003

    print_data();
}

int main() {
    DEBUG_BEGIN_PROFILE_SESSION(Setup, "setup.json");

    sailbot::plat_linux::Arduino main_ard(sizeof(MainRData), sizeof(MainTData), 115200, 0xA5);

    sailbot::plat_linux::Arduino remote_ard(sizeof(RemoteRData), sizeof(RemoteTData), 115200, 0xBD);

    if (!main_ard.is_valid) return -1;
    main_ard.on_update(main_ard_update);

    if (!remote_ard.is_valid) return -2;
    remote_ard.on_update(remote_ard_update);

    DEBUG_END_PROFILE_SESSION(Setup);
    DEBUG_BEGIN_PROFILE_SESSION(Loop, "loop.json");

    while (true) {
        main_ard.update();
        remote_ard.update();
    }

    DEBUG_END_PROFILE_SESSION(Loop);
}
