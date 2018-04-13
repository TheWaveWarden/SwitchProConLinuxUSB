#include "hidapi.h"
#include <stdio.h>

#define PROCON_DRIVER_VERSION "0.1"

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#define PROCON_ID 0x2009
#define NINTENDO_ID 0x057E

#define MAX_N_CONTROLLERS 4

class ProController
{
    public:

    //ProController() {}
    //~ProController() {}

    int open_device(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number, unsigned short n_controller)
    {
        controller_ptr = hid_open(vendor_id, product_id, serial_number);
        if (!controller_ptr)
        {
            return -1;
        }
        printf("%sSuccessfully opened controller nr. %u with serial number NULL%s\n", KYEL, n_controller, KNRM);
        is_opened = true;
        return 0;
    }

    void close_device() {
        is_opened = false;
    }

    hid_device *controller_ptr;
    unsigned short serial_number;
    bool is_opened = false;
};
