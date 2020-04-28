#include "log.hpp"

using namespace Engine::Core::Debug;
 
void SLog::init() noexcept
{
    logHeading();
}

bool SLog::displayDate         (false);
bool SLog::displayHour         (true);
bool SLog::displayWithColor    (true);

bool SLog::printAlwayLogFile               (false);           
bool SLog::printLogFileIfWarningHappend    (false);
bool SLog::printLogFileIfErrorHappend      (true); 

bool SLog::releaseLogFile                  (true);
std::string  SLog::fileLogPath             ((   std::string(LOG_FOLDER) + 
                                                std::string("log : ") + 
                                                getDateAndTimeStr() +
                                                std::string(".txt")).c_str());
std::ofstream  SLog::fileLog   (fileLogPath.c_str());