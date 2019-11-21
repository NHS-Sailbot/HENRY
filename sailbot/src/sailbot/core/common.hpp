#pragma once

#ifdef __linux__
#define SAILBOT_PLATFORM platform_linux
#else
#define SAILBOT_PLATFORM
#error UNSUPPORTED PLATFORM!
#endif
