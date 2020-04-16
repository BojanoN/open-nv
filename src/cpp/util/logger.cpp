#include "logger.hpp"
namespace Logger {
void setLevel(int level)
{
    log_set_level(level);
};
}
