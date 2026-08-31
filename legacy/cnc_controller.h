#ifndef CNC_CONTROLLER_H
#define CNC_CONTROLLER_H

#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include "config.h"

struct Position {
    double x, y, z, a;
    Position() : x(0), y(0), z(0), a(0) {}
};

struct GCodeCommand {
    char code;
    std::vector<double> parameters;
    std::string comment;
};

class CNCController {
public:
    CNCController();
    ~CNCController();
    
    bool initialize();
    void shutdown();
    
    bool moveTo(double x, double y, double z, double a, double feedrate);
    bool moveRelative(double dx, double dy, double dz, double da, double feedrate);
    bool homeAxis(int axis);
    bool homeAll();
    void stop();
    void emergencyStop();
    
    bool loadGCodeFile(const std::string& filename);
    bool executeGCode();
    bool executeCommand(const std::string& command);
    void pause();
    void resume();
    
    Position getCurrentPosition();
    bool isRunning() const { return m_running; }
    bool isPaused() const { return m_paused; }
    bool isEmergencyStopped() const { return m_emergencyStop; }
    std::string getStatusMessage() const { return m_statusMessage; }
    
    void setSpindleSpeed(int rpm);
    void spindleOn();
    void spindleOff();
    
    void setStepsPerMM(double x, double y, double z, double a);
    void setMaxSpeed(double x, double y, double z, double a);
    void setAcceleration(double accel);
    
private:
    void stepMotor(uint8_t stepPin, uint8_t dirPin, int steps, int direction, int delayUs);
    void stepperThread();
    void monitorEndstops();
    bool parseGCodeLine(const std::string& line, GCodeCommand& cmd);
    bool executeGCodeCommand(const GCodeCommand& cmd);
    
    Position m_currentPos;
    Position m_targetPos;
    Position m_homePos;
    Position m_stepsPerMM;
    Position m_maxSpeed;
    Position m_acceleration;
    
    std::vector<std::string> m_gcodeBuffer;
    size_t m_currentLine;
    
    std::atomic<bool> m_running;
    std::atomic<bool> m_paused;
    std::atomic<bool> m_stopRequested;
    std::atomic<bool> m_emergencyStop;
    std::atomic<bool> m_homing;
    
    std::thread m_stepperThread;
    std::thread m_endstopThread;
    std::mutex m_mutex;
    
    int m_spindleSpeed;
    bool m_spindleOn;
    std::string m_statusMessage;
    
    bool m_endstops[4];
    uint8_t m_stepPins[4];
    uint8_t m_dirPins[4];
    uint8_t m_limitPins[4];
};

#endif
