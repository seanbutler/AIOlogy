
#pragma once


#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>


namespace Utils {


    namespace Time {



        
        std::string HumanReadableTimeNowMillis() {

            auto now = std::chrono::system_clock::now();
            std::time_t now_time = std::chrono::system_clock::to_time_t(now);

            // Get milliseconds part
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()
            ) % 1000;

            std::ostringstream oss;
            // as "YYYY-MM-DD HH:MM:SS.mmm"
            oss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S")
                << '.' << std::setfill('0') << std::setw(3) << ms.count();

            return oss.str();

        };


};      // namespace Time
};      // namespace Utils