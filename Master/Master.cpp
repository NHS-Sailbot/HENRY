#include <Henry/Arduino.hpp>
#include <Henry/Camera.hpp>

#include <iostream>

static inline constexpr double rcRange(const unsigned short aVal, const unsigned short aMinVal, const unsigned short aMidVal,
                                       const unsigned short aMaxVal) {
    if (aVal > aMidVal) {
        if (aVal > aMaxVal) return 0;
        return static_cast<double>(aVal - aMidVal) / (aMidVal - aMinVal);
    } else {
        if (aVal < aMinVal) return 0;
        return static_cast<double>(aVal - aMidVal) / (aMaxVal - aMidVal);
    }
}

int main() {
    struct MasterR {
        unsigned int mWindSensorReading;
        int mMotorReading;
    };
    struct MasterT {
        int mMotorDesired;
    };
    struct RemoteR {
        unsigned short mRC[6];
    };
    struct RemoteT {};

    Henry::Arduino masterArd(sizeof(MasterR), sizeof(MasterT), 115200, 0xA5);
    MasterR *masterR = nullptr;
    MasterT *masterT = nullptr;
    Henry::Arduino remoteArd(sizeof(RemoteR), sizeof(RemoteT), 115200, 0xBD);
    RemoteR *remoteR = nullptr;
    RemoteT *remoteT = nullptr;

    Henry::Camera camera(1280, 720);

    enum StatusFlag { NONE = 0, MASTER_ARD = 1, REMOTE_ARD = 2, CAMERA = 4, ALL = 7 };
    unsigned char statusFlags = ALL;

    if (!masterArd.isConnected()) {
        std::cout << "Unable to connect with the motors Arduino\n";
        statusFlags &= ~MASTER_ARD;
    } else {
        masterR = reinterpret_cast<MasterR *>(masterArd.mRdata);
        masterT = reinterpret_cast<MasterT *>(masterArd.mTdata);
    }

    if (!remoteArd.isConnected()) {
        std::cout << "Unable to connect with the remote Arduino\n";
        statusFlags &= ~REMOTE_ARD;
    } else {
        remoteR = reinterpret_cast<RemoteR *>(remoteArd.mRdata);
        remoteT = reinterpret_cast<RemoteT *>(remoteArd.mTdata);
    }

    if (!camera.isOpen()) {
        std::cout << "Unable to open the camera\n";
        statusFlags &= ~CAMERA;
    }

    if (statusFlags == ALL) {
        int motorDesired = 0;

        while (true) {
            if (remoteArd.isValid() && masterArd.isValid()) {
                auto tJoystick1 = rcRange(remoteR->mRC[0], 991, 1503, 2015);
                auto tMotorReading = masterR->mMotorReading;
                if (tJoystick1 > 0.2 || tJoystick1 < -0.2)
                    masterT->mMotorDesired = tMotorReading + tJoystick1 * 100, motorDesired = tMotorReading;
                else
                    masterT->mMotorDesired = motorDesired;
            }
        }
    }
}
