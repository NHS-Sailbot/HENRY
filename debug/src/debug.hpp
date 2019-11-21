#pragma once

#ifndef NDEBUG
#define DEBUG_ENABLE_TIMING
#define DEBUG_ENABLE_LOGGING
#define DEBUG_ENABLE_PROFILING
#endif

namespace debug { namespace timer {
#ifdef DEBUG_ENABLE_TIMING
    // This function returns the time, either since the program
    // started, or since the last time debug::time::reset() was
    // called.
    double now();
    // This function resets the internal time point of reference
    // so that the next time debug::time::now() is called, the
    // time returned is relative to the time this is called.
    void reset();
    // This function pauses execution by timing a while loop using
    // timer::now() to get the elapsed time since the function was
    // initially called.
    void sleep(const double s);
#else
    // This function returns 0.0 in release mode.
    static inline double now(...) { return 0.0; }
    // This function does nothing in release mode.
    static inline void reset(...) {}
    // This function does nothing in release mode.
    static inline void sleep(...) {}
#endif
}} // namespace debug::timer

namespace debug { namespace log {
#ifdef DEBUG_ENABLE_LOGGING
    // This function is a wrapper for the printf function.
    void info(const char *const msg, ...);
    // This function writes the given string to the console in gray.
    void message(const char *const msg, ...);
    // This function writes the given string to the console in
    // green, prefaced with the tag "[SUCCESS]".
    void success(const char *const msg, ...);
    // This function writes the given string to the console in
    // yellow, prefaced with the tag "[WARNING]".
    void warning(const char *const msg, ...);
    // This function writes the given string to the console in
    // red, prefaced with the tag "[ERROR]".
    void error(const char *const msg, ...);
#else
    // This function does nothing in release mode.
    static inline void info(...) {}
    // This function does nothing in release mode.
    static inline void message(...) {}
    // This function does nothing in release mode.
    static inline void warning(...) {}
    // This function does nothing in release mode.
    static inline void error(...) {}
#endif
}} // namespace debug::log

namespace debug { namespace profile {
    struct ScopedProfile {
        const char *const m_name;
        long long m_start_time;
        bool m_stopped;
        ScopedProfile(const char *const function_name);
        ~ScopedProfile();
        void end();
    };
    struct Session {
        const char *const m_name, *const m_output_filename;
        bool m_stopped;
        Session(const char *const session_name, const char *const session_file = "results.json");
        ~Session();
        void end();
    };
#ifdef DEBUG_ENABLE_PROFILING
#define DEBUG_BEGIN_PROFILE_SESSION(name, filename) ::debug::profile::Session db_##name(#name, filename)
#define DEBUG_END_PROFILE_SESSION(name) db_##name.end()
#define DEBUG_BEGIN_PROFILE(name) ::debug::profile::ScopedProfile db_profile_##name(#name)
#define DEBUG_END_PROFILE(name) db_profile_##name.end()
#define DEBUG_BEGIN_FUNC_PROFILE ::debug::profile::ScopedProfile db_profile_##__PRETTY_FUNCTION__(__PRETTY_FUNCTION__)
#define DEBUG_END_FUNC_PROFILE db_profile_##__PRETTY_FUNCTION__.end()
#else
    // This function does nothing in release mode.
    static inline void begin(...) {}
    // This function does nothing in release mode.
    static inline void end(...) {}
#define DEBUG_BEGIN_PROFILE_SESSION(name, filename)
#define DEBUG_END_PROFILE_SESSION(name)
#define DEBUG_BEGIN_PROFILE(name)
#define DEBUG_END_PROFILE(name)
#define DEBUG_BEGIN_FUNC_PROFILE
#define DEBUG_END_FUNC_PROFILE
#endif
}} // namespace debug::profile
