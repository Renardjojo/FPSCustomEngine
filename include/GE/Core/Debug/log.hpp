//Project : Engine
//Editing by Six Jonathan
//Date : 06/01/2020 - 16 h 13

#ifndef _GE_LOG_H
#define _GE_LOG_H

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <ctime> 
#include <cstdio>

namespace Engine::Core::Debug
{      
    #define LOG_FOLDER "./log/"

    enum E_logSetting
    {
        E_DISPLAY_DATE,             //display date befor message (2)
        E_DISPLAY_HOUR,             //display hour befor message (3)
        E_DISPLAY_WITH_COLOR,       //display log with color for each even
        E_ALWAY_PRINT_LOG_FILE,     //never destroy log file
        E_PRINT_LOG_FILE_WRNG,      //print log file if warning happend
        E_ALWAY_PRINT_LOG_ERR       //print log file if error happend
    };

    //singleton class
    class SLog
    {
        public :

        /**
         * @brief Macro can by use to modify text properties.
         * @example :   cout << FBLU("I'm blue.") << endl;
         *              cout << BOLD(FBLU("I'm blue-bold.")) << endl;
         */
        /* TEXT PROPERTIES */
        #define RST         "\033[0m"
        #define BOLD(x)     "\033[1m" x RST
        #define ITLC(x)     "\033[3m" x RST
        #define UNDL(x)     "\033[4m" x RST
        #define INVS(x)     "\033[7m" x RST //inverses foreground & background colors
        #define STKTHGH(x) "\033[9m" x RST

        /* FOREGROUND COLOR TEXT */
        #define FBLK(x) "\033[30m" x RST
        #define FRED(x) "\033[31m" x RST
        #define FGRN(x) "\033[32m" x RST
        #define FYEL(x) "\033[33m" x RST
        #define FBLU(x) "\033[34m" x RST
        #define FMAG(x) "\033[35m" x RST
        #define FCYN(x) "\033[36m" x RST
        #define FWHT(x) "\033[37m" x RST

        /* BACKGROUND COLOR TEXT */
        #define BBLK(x) "\033[40m" x RST
        #define BRED(x) "\033[41m" x RST
        #define BGRN(x) "\033[42m" x RST
        #define BYEL(x) "\033[43m" x RST
        #define BBLU(x) "\033[44m" x RST
        #define BMAG(x) "\033[45m" x RST
        #define BCYN(x) "\033[46m" x RST
        #define BWHT(x) "\033[47m" x RST

        #define functGlCheckAndLogError()            \
                Engine::Core::Debug::SLog::GlCheckAndLogError (   \
                (std::string(" in : ") + __FILE__  +    \
                ':' + std::to_string(__LINE__) +        \
                ": function \"" + __FUNCTION__ + "()" + \
                "\"").c_str());

        #define functError(msg)                         \
                Engine::Core::Debug::SLog::logError (        \
                (std::string(__FILE__) +                \
                ':' + std::to_string(__LINE__) +        \
                ": function \"" + __FUNCTION__ + "()" + \
                "\" : " + msg).c_str());

        #define functWarning(msg)                       \
                Engine::Core::Debug::SLog::logWarning (        \
                (std::string(__FILE__) +                \
                ':' + std::to_string(__LINE__) +        \
                ": function \"" + __FUNCTION__ + "()" + \
                "\" :"  + msg).c_str());

        public:

            #pragma region constructor/destructor

            SLog ()                     = delete;
            SLog (const SLog& other)    = delete;
            ~SLog ()                    = delete;

            /**
             * @brief Create log file
             */
            static 
            void init() noexcept;

            /**
             * @brief Save log file if error happen else destroy log file
             */
            static inline 
            void release() noexcept;

            #pragma endregion //!constructor/destructor

            #pragma region static methods

            /**
             * @brief print message in log event
             * 
             * @param os 
             */
            static inline
            void logAddMsg (std::string msg) noexcept;

            /**
             * @brief display log's heading
             */
            static inline 
            void logHeading () noexcept;

            /**
             * @brief display message in current stream
             * 
             * @param msg 
             */
            static inline 
            void log (std::string msg) noexcept;

            /**
             * @brief display message with prefix "Error : " in current stream
             * 
             * @param msg 
             */
            static inline 
            void logError (std::string msg) noexcept;

            /**
             * @brief display error of openGl with prefix "Error : " in current stream
             * 
             * @param msg 
             */
            static inline 
            void GlCheckAndLogError (std::string msg) noexcept;

            /**
             * @brief display message with prefix "Warning : " in current stream
             * 
             * @param msg 
             */
            static inline 
            void logWarning (std::string msg) noexcept;

            /**
             * @brief display message with prefix "Help : " in current stream
             * 
             * @param msg 
             */
            static inline 
            void logHelp (std::string msg) noexcept;

            /**
             * @brief display message with elem initialized in current stream
             * 
             * @param elem initialized 
             */
            static inline 
            void logInitializationStart (std::string elem) noexcept;

            /**
             * @brief display message with count of elem initialize in current stream
             * 
             * @param elem 
             * @param count in %
             */
            static inline 
            void logInitializationStep   (std::string elem, unsigned int count) noexcept;

        /**
             * @brief display message with elem initialized in current stream
             * 
             * @param elem initialized 
             */
            static inline 
            void logInitializationEnd   (std::string elem) noexcept;

            /**
             * @brief Set the Setting object
             * 
             * @param setting
             *         
             * @param data 
             */
            static inline
            void setSetting	(E_logSetting setting, bool data) noexcept;

            /**
             * @brief Get the Setting object
             * 
             * @param setting 
             * @return true 
             * @return false 
             */
            static inline 
            bool getSetting	(E_logSetting setting) noexcept;

            /**
             * @brief Get the Date And Time Str
             * 
             * @return std::string 
             */        static inline 
            std::string getTimeStr() noexcept;
            static inline 
            std::string getDateStr() noexcept;
            static inline 
            std::string getDateAndTimeStr() noexcept;

            #pragma endregion //!static methods

        protected:

            #pragma region static attribut

            static bool displayDate;        // default false
            static bool displayHour;        // default true
            static bool displayWithColor;   // default true
            static bool printAlwayLogFile;              // default false
            static bool printLogFileIfWarningHappend;   // default false
            static bool printLogFileIfErrorHappend;     // default true

            static bool             releaseLogFile;     //true if log file in'st keep
            static std::string      fileLogPath;
            static std::ofstream    fileLog;

            #pragma endregion //! static attribut

        private:
    };
} // namespace Engine::Core::Debug

#include "log.inl"

#endif // _GE_LOG_H