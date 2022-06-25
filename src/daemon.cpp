// service daemon
//
#include "platform.hpp"
#include "aloraShare.hpp"
#include <ctime>

int main()
{
    aloraShare app;
    app.update_identity("abhiraj ranjan");
    app.run();

    std::time_t time = std::time(nullptr);
    while(1)
    {
        std::time_t ctime = std::time(nullptr);
        
        for(auto dev: app.get_devices())
        {
            std::cout << dev.info["name"] << std::endl;
        }
        sleep(1);
    }
}