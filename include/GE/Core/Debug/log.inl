#include "log.hpp"
#include "glad.h"

inline 
void Engine::Core::Debug::SLog::release() noexcept
{
    fileLog.close();

    if (releaseLogFile)
    {
        std::remove(fileLogPath.c_str());
    }
}

inline
void Engine::Core::Debug::SLog::logAddMsg (std::string msg) noexcept
{
    std::streambuf* coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(fileLog.rdbuf()); //redirect std::cout to log.txt

    std::cout << msg;

    std::cout.rdbuf(coutbuf); //reset to standard output again
}


inline
void Engine::Core::Debug::SLog::logHeading () noexcept
{
    std::string msgLog;
    msgLog += "================================\n";
    msgLog += "| LOG\n";
    msgLog += "| Date : ";
    msgLog += getDateStr() + " " + getTimeStr() + "\n";
    msgLog += "================================\n\n";

    logAddMsg (msgLog);
}

inline 
void Engine::Core::Debug::SLog::log (std::string msg) noexcept
{
    std::string msgLog;
    msgLog += "GE: "; 
    msgLog += displayDate ? getDateStr() + " " : "";
    msgLog += displayHour ? getTimeStr() + " " : "";
    msgLog += msg;
    msgLog += "\n";

    if (printAlwayLogFile) 
        releaseLogFile = false;

    std::cout << msgLog;
    logAddMsg (msgLog);
}

inline 
void Engine::Core::Debug::SLog::logError (std::string msg) noexcept
{
    std::string msgLog;
    msgLog += "GE: "; 
    msgLog += displayDate ? getDateStr() + " " : "";
    msgLog += displayHour ? getTimeStr() + " " : "";

    if(displayWithColor)
    {
        logAddMsg (msgLog);
        std::cout << msgLog;
        msgLog.clear();

        msgLog += FRED(BOLD("ERROR: "));
        std::cout << msgLog;
        msgLog.clear();

        msgLog += "ERROR: ";
        logAddMsg (msgLog);
        msgLog.clear();
    }
    else
    {
        msgLog += "ERROR: ";
    }
    
    msgLog += msg;
    msgLog += "\n";

    if (printLogFileIfErrorHappend) 
        releaseLogFile = false;

    std::cout << msgLog;
    logAddMsg (msgLog);
}

inline 
void Engine::Core::Debug::SLog::GlCheckAndLogError (std::string msg) noexcept
{
    switch (glGetError())
    {
    case GL_NO_ERROR:
        break;

    case GL_INVALID_ENUM:
        logError(std::string("GL_INVALID_ENUM") + msg);
        break;

    case GL_INVALID_VALUE:
        logError(std::string("GL_INVALID_VALUE") + msg);
        break;

    case GL_INVALID_OPERATION:
        logError(std::string("GL_INVALID_OPERATION") + msg);
        break;

    case GL_INVALID_FRAMEBUFFER_OPERATION:
        logError(std::string("GL_INVALID_FRAMEBUFFER_OPERATION") + msg);
        break;

    case GL_OUT_OF_MEMORY:
        logError(std::string("GL_OUT_OF_MEMORY") + msg);
        break;

    case GL_STACK_UNDERFLOW:
        logError(std::string("GL_STACK_UNDERFLOW") + msg);
        break;

    case GL_STACK_OVERFLOW:
        logError(std::string("GL_STACK_OVERFLOW") + msg);
        break;
    
    default:
        logError(std::string("Unknow openGl error") + msg);
        break;
    }
}

inline 
void Engine::Core::Debug::SLog::logWarning (std::string msg) noexcept
{
    std::string msgLog;
    msgLog += "GE: "; 
    msgLog += displayDate ? getDateStr() + " " : "";
    msgLog += displayHour ? getTimeStr() + " " : "";

    if(displayWithColor)
    {
        logAddMsg (msgLog);
        std::cout << msgLog;
        msgLog.clear();

        msgLog += FYEL(BOLD("WARNING: "));
        std::cout << msgLog;
        msgLog.clear();
        
        msgLog += "WARNING: ";
        logAddMsg (msgLog);
        msgLog.clear();
    }
    else
    {
        msgLog += "WARNING: ";
    }

    msgLog += msg;
    msgLog += "\n";

    if (printLogFileIfWarningHappend) 
        releaseLogFile = false;

    std::cout << msgLog;
    logAddMsg (msgLog);
}

inline 
void Engine::Core::Debug::SLog::logHelp (std::string msg) noexcept
{
    std::string msgLog;
    msgLog += "GE: "; 
    msgLog += displayDate ? getDateStr() + " " : "";
    msgLog += displayHour ? getTimeStr() + " " : "";

    if(displayWithColor)
    {
        logAddMsg (msgLog);
        std::cout << msgLog;
        msgLog.clear();

        msgLog += FGRN(BOLD("HELP: "));
        std::cout << msgLog;
        msgLog.clear();
        
        msgLog += "HELP: ";
        logAddMsg (msgLog);
        msgLog.clear();
    }
    else
    {
        msgLog += "HELP: ";
    }

    msgLog += msg;
    msgLog += "\n";

    if (printAlwayLogFile) 
        releaseLogFile = false;

    std::cout << msgLog;
    logAddMsg (msgLog);
}

inline 
void Engine::Core::Debug::SLog::logInitializationStart (std::string elem) noexcept
{
    log((std::string(elem) + " being initialized...").c_str());
}

inline 
void Engine::Core::Debug::SLog::logInitializationStep   (std::string elem, unsigned int count) noexcept
{
    log((std::string(elem) + " initialization [" + std::to_string(count) + "%]").c_str());
}

inline 
void Engine::Core::Debug::SLog::logInitializationEnd   (std::string elem) noexcept
{
    log((std::string(elem) + " initialization completed").c_str());
}

inline 
void Engine::Core::Debug::SLog::setSetting	(E_logSetting setting, bool data) noexcept
{
    switch (setting)
    {
    case E_logSetting::E_DISPLAY_DATE:
        displayDate = data;
        break;

    case E_logSetting::E_DISPLAY_HOUR:
        displayHour = data;
        break;

    case E_logSetting::E_DISPLAY_WITH_COLOR:
        displayWithColor = data;
        break;

    case E_logSetting::E_ALWAY_PRINT_LOG_FILE:
        printAlwayLogFile = data;
        break;

    case E_logSetting::E_PRINT_LOG_FILE_WRNG:
        displayWithColor = data;
        break;

    case E_logSetting::E_ALWAY_PRINT_LOG_ERR:
        printLogFileIfErrorHappend = data;
        break;

    default:
        assert(false);
        break;
    }
}

inline
bool Engine::Core::Debug::SLog::getSetting	(E_logSetting setting) noexcept
{
    switch (setting)
    {
    case E_logSetting::E_DISPLAY_DATE:
        return displayDate;
        break;

    case E_logSetting::E_DISPLAY_HOUR:
        return displayHour;
        break;

    case E_logSetting::E_DISPLAY_WITH_COLOR:
        return displayWithColor;
        break;
    
    case E_logSetting::E_ALWAY_PRINT_LOG_FILE:
        return printAlwayLogFile;
        break;

    case E_logSetting::E_PRINT_LOG_FILE_WRNG:
        return displayWithColor;
        break;

    case E_logSetting::E_ALWAY_PRINT_LOG_ERR:
        return printLogFileIfErrorHappend;
        break;

    default:
        assert(false);
        break;
    }
}

inline
std::string Engine::Core::Debug::SLog::getDateAndTimeStr() noexcept
{
    return getDateStr() + std::string(" ") + getTimeStr();
}

inline
std::string Engine::Core::Debug::SLog::getTimeStr() noexcept
{
    time_t t = time(0);   // get time now
    tm* now = localtime(&t);

    std::string rst;
    rst += std::to_string(now->tm_hour) + std::string(":"); 
    rst += std::to_string(now->tm_min) + std::string(":"); 
    rst += std::to_string(now->tm_sec);
    return rst;
}

inline
std::string Engine::Core::Debug::SLog::getDateStr() noexcept
{
    time_t t = time(0);   // get time now
    tm* now = localtime(&t);


    std::string rst;
    rst += std::to_string(now->tm_mday) + std::string("-"); 
    rst += std::to_string(now->tm_mon + 1) + std::string("-");
    rst += std::to_string(now->tm_year + 1900);
    return rst;
}