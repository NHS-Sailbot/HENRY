#include <Debug/Debug.hpp>

#include <Henry/Arduino.hpp>
#include <Henry/Camera.hpp>

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
    struct RemoteT {
        //
    };

    Henry::Arduino masterArd(sizeof(MasterR), sizeof(MasterT), 115200, 0xA5);
    MasterR *masterR = nullptr;
    MasterT *masterT = nullptr;
    Henry::Arduino remoteArd(sizeof(RemoteR), sizeof(RemoteT), 115200, 0xBD);
    RemoteR *remoteR = nullptr;
    RemoteT *remoteT = nullptr;

    Henry::Camera camera(1280, 720);

    enum StatusFlag { NONE, MASTER_ARD, REMOTE_ARD, CAMERA, ALL = MASTER_ARD | REMOTE_ARD | CAMERA };
    unsigned char statusFlags = ALL;

    if (!masterArd.IsConnected()) {
        Debug::Log::error("Unable to connect with the motors Arduino");
        statusFlags &= ~MASTER_ARD;
    } else {
        masterR = reinterpret_cast<MasterR *>(masterArd.mRData);
        masterT = reinterpret_cast<MasterT *>(masterArd.mTData);
    }

    if (!remoteArd.IsConnected()) {
        Debug::Log::error("Unable to connect with the remote Arduino");
        statusFlags &= ~REMOTE_ARD;
    } else {
        remoteR = reinterpret_cast<RemoteR *>(remoteArd.mRData);
        remoteT = reinterpret_cast<RemoteT *>(remoteArd.mTData);
    }

    if (!camera.IsOpen()) {
        Debug::Log::error("Unable to open the camera");
        statusFlags &= ~CAMERA;
    }

    if (statusFlags == ALL) {
        int motorDesired = 0;

        while (true) {
            if (remoteArd.IsValid() && masterArd.IsValid()) {
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
