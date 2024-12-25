#include <sstream>

class Debug {
public:
    // Overload the << operator for general types
    template<typename T>
    Debug& operator<<(const T& value) {
        #ifdef DEBUG
        std::cout << value;
        #endif

        return *this;
    }

    // Optional: Overload for manipulators like std::endl
    Debug& operator<<(std::ostream& (*manip)(std::ostream&)) {
        #ifdef DEBUG
        std::cout << manip;
        #endif

        return *this;
    }
};
