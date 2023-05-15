#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

class Logger {
    public:
        Logger(std::ostream& os) : m_os(os) {}

        template<typename T>
            Logger& operator<<(const T& data) {
                std::time_t t = std::time(nullptr);
                char time_str[32];
                std::strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
                m_os << time_str << " " << data;
                return *this;
            }

    private:
        std::ostream& m_os;
};

int main() {
    std::ofstream ofs("log.txt");
    Logger logger(ofs);
    logger << "Hello, world!\n";
    logger << "The answer is " << 42 << '\n';
    return 0;
}

