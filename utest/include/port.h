#ifndef _TESTSUITE_INCLUDE_PORT_H_
#define _TESTSUITE_INCLUDE_PORT_H_

#include <iostream>

namespace testing
{
    class Message
    {
    private:
        /* data */
    public:
        Message(){};
        ~Message()
        {
            std::cout << std::endl;
        }
        // Streams a custom failure message into this object.
        template <typename T>
        Message &operator<<(const T &value)
        {
            std::cout << value;

            return *this;
        }
    };

}

#endif
