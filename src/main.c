#include "server.h"
#include "flim.h"

int main(int argc, char *argv[])
{
    struct flim *flim = flm_new();
    server_run(flim);
}
