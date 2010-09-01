#include "oauth2.h"

int main(int argc, char** argv)
{
    oauth2_config* conf = oauth2_init("foo", "bar");

    oauth2_cleanup(conf);

    return 0;
}
