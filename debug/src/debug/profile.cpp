#include "../debug.hpp"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <string>

#include <thread>

struct ProfileResult {
    std::string Name;
    long long Start, End;
    uint32_t ThreadID;
};

struct InstrumentationSession {
    std::string Name;
};

class Instrumentor {
  private:
    InstrumentationSession *m_CurrentSession;
    std::ofstream m_OutputStream;
    int m_ProfileCount;

  public:
    Instrumentor() : m_CurrentSession(nullptr), m_ProfileCount(0) {}

    void BeginSession(const std::string &name, const std::string &filepath) {
        m_OutputStream.open(filepath);
        WriteHeader();
        m_CurrentSession = new InstrumentationSession{name};
    }

    void EndSession() {
        WriteFooter();
        m_OutputStream.close();
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
        m_ProfileCount = 0;
    }

    void WriteProfile(const ProfileResult &result) {
        if (m_ProfileCount++ > 0) m_OutputStream << ",";

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{";
        m_OutputStream << "\"cat\":\"function\",";
        m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
        m_OutputStream << "\"name\":\"" << name << "\",";
        m_OutputStream << "\"ph\":\"X\",";
        m_OutputStream << "\"pid\":0,";
        m_OutputStream << "\"tid\":" << result.ThreadID << ",";
        m_OutputStream << "\"ts\":" << result.Start;
        m_OutputStream << "}";

        m_OutputStream.flush();
    }

    void WriteHeader() {
        m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        m_OutputStream.flush();
    }

    void WriteFooter() {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

    static Instrumentor &Get() {
        static Instrumentor *instance = new Instrumentor();
        return *instance;
    }
};

namespace debug { namespace profile {
    using namespace std::chrono;

    Session::Session(const char *const session_name, const char *const session_file)
        : m_stopped(false), m_name(session_name), m_output_filename(session_file) {
        Instrumentor::Get().BeginSession(session_name, session_file);
    }
    Session::~Session() {
        if (!m_stopped) { end(); }
    }
    void Session::end() {
        m_stopped = true;
        Instrumentor::Get().EndSession();
    }

    ScopedProfile::ScopedProfile(const char *const function_name)
        : m_name(function_name),
          m_start_time(time_point_cast<microseconds>(high_resolution_clock::now()).time_since_epoch().count()),
          m_stopped(false) {}
    ScopedProfile::~ScopedProfile() {
        if (!m_stopped) { end(); }
    }
    void ScopedProfile::end() {
        long long end = time_point_cast<microseconds>(high_resolution_clock::now()).time_since_epoch().count();

        uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::Get().WriteProfile({m_name, m_start_time, end, threadID});

        m_stopped = true;
    }
}} // namespace debug::profile

// 972-2024