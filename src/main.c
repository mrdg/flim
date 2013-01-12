#include "server.h"
#include "flim.h"

int main(int argc, char *argv[])
{
    struct flim *flim = flm_new();
    flm_server_start(flim);
}
