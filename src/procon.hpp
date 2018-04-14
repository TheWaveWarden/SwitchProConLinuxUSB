#include "hidapi.h"
#include <stdio.h>
#include <array>
#include <cstring>
#include <unistd.h>
//#include <optional>

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

    enum BUTTONS
    {
        d_left,
        d_right,
        d_up,
        d_down,
        A,
        B,
        X,
        Y,
        plus,
        minus,
        home,
        share,
        L1,
        L2,
        L3,
        R1,
        R2,
        R3,
        None
    };

    using uchar = unsigned char;
    static constexpr uchar ledCommand{0x30};
    static constexpr uchar get_input{0x1f};
    static constexpr size_t exchange_length{0x400};
    using exchange_array = std::array<uchar, exchange_length>; // might need std::optional<>
                                                               // static constexpr size_t buflen{ 80 };
                                                               // std::array<char, buflen> buffer;

  public:
    //ProController() {}
    //~ProController() {}

    static const uchar bit_position(ProController::BUTTONS button)
    {
        switch (button)
        {
        case d_left:
            return 0x04;
            break;
        case d_right:
            return 0x03;
            break;
        case d_up:
            return 0x02;
            break;
        case d_down:
            return 0x01;
            break;
        case A:
            return 0x04;
            break;
        case B:
            return 0x03;
            break;
        case X:
            return 0x02;
            break;
        case Y:
            return 0x01;
            break;
        case plus:
            return 0x02;
            break;
        case minus:
            return 0x01;
            break;
        case home:
            return 0x05;
            break;
        case share:
            return 0x06;
            break;
        case L1:
            return 0x07;
            break;
        case L2:
            return 0x08;
            break;
        case L3:
            return 0x04;
            break;
        case R1:
            return 0x07;
            break;
        case R2:
            return 0x08;
            break;
        case R3:
            return 0x03;
            break;
        case None:
            return 0x00;
            break;
        default:
            red();
            printf("ERROR: Tried to find bitpos of unknown button!\n");
            normal();
            return 0x00;
            break;
        }
    }

    static const uchar data_address(ProController::BUTTONS button)
    {
        switch (button)
        {
        case d_left:
            return 0x0f;
            break;
        case d_right:
            return 0x0f;
            break;
        case d_up:
            return 0x0f;
            break;
        case d_down:
            return 0x0f;
            break;
        case A:
            return 0x0d;
            break;
        case B:
            return 0x0d;
            break;
        case X:
            return 0x0d;
            break;
        case Y:
            return 0x0d;
            break;
        case plus:
            return 0x0e;
            break;
        case minus:
            return 0x0e;
            break;
        case home:
            return 0x0e;
            break;
        case share:
            return 0x0e;
            break;
        case L1:
            return 0x0f;
            break;
        case L2:
            return 0x0f;
            break;
        case L3:
            return 0x0e;
            break;
        case R1:
            return 0x0d;
            break;
        case R2:
            return 0x0d;
            break;
        case R3:
            return 0x0e;
            break;
        case None:
            return 0x00;
            break;
        default:
            red();
            printf("ERROR: Tried to find data adress of unknown button!\n");
            normal();
            return 0x00;
            break;
        }
    }

    template <size_t length>
    exchange_array send_command(uchar command, std::array<uchar, length> const &data)
    {
        std::array<uchar, length + 0x9> buffer;
        buffer.fill(0);
        buffer[0x0] = 0x80;
        buffer[0x1] = 0x92;
        buffer[0x3] = 0x31;
        buffer[0x8] = command;
        if (length > 0)
        {
            memcpy(buffer.data() + 0x9, data.data(), length);
        }
        return exchange(buffer);
    }

    template <size_t length>
    exchange_array exchange(std::array<uchar, length> const &data)
    {
        if (!controller_ptr)
        {
            red();
            printf("ERROR: controller_ptr is nullptr!\n");
            normal();
            return {};
        }

        if (hid_write(controller_ptr, data.data(), length) < 0)
        {
            red();
            printf("ERROR: read() returned -1!\n");
            normal();
            return {};
        }

        std::array<uchar, exchange_length> ret;
        ret.fill(0);
        hid_read(controller_ptr, ret.data(), exchange_length);
        return ret;
    }

    template <size_t length>
    exchange_array send_subcommand(uchar command, uchar subcommand, std::array<uchar, length> const &data)
    {
        std::array<uchar, length + 10> buffer{
            static_cast<uchar>(rumble_counter++ & 0xF),
            0x00,
            0x01,
            0x40,
            0x40,
            0x00,
            0x01,
            0x40,
            0x40,
            subcommand};
        if (length > 0)
        {
            memcpy(buffer.data() + 10, data.data(), length);
        }
        return send_command(command, buffer);
    }

    void poll_input()
    {
        if (!controller_ptr)
        {
            printf("%sERROR: Controller pointer is nullptr%s\n", KRED, KNRM);
            return;
        }

        auto dat = send_command(get_input, empty);
        // if (!dat)
        // {
        //     red();
        //     printf("ERROR: Data is not valid!\n");
        //     normal();
        //     return;
        // }
        fill_new_array(dat);
        print_new_array(dat);
        print_exchange_array(dat);
        detect_bad_data(dat);
        if (bad_data_detected)
        {
            red();
            printf("\n-----------------------------------------------------------\n----------------   DETECTED BAD DATA!!!!!   ---------------\n-----------------------------------------------------------\n");
            normal();
        }

        if (dat[data_address(A)] == 0x08)
        {
            yellow();
            printf("A BUTTON PRESSED!!!!!! :_))))))))))))\n");
            normal();
        }
        //print_exchange_array(dat);
    }
    /* Hackishly detects when the controller is trapped in a bad loop. 
    Nothing to do here, need to restart driver :(*/
    void detect_bad_data(exchange_array arr)
    {
        bad_data_detected = (arr[1] == 0x01 && arr[0] == 0x81) ? true : bad_data_detected;
    }

    void print_exchange_array(exchange_array arr)
    {
        bool redcol = false;
        if (arr[0] != 0x30)
            yellow();
        else
        {
            red();
            redcol = true;
        }
        for (size_t i = 0; i < 20; ++i)
        {
            if (arr[i] == 0x00)
            {
                blue();
            }
            else
            {
                if (redcol)
                {
                    red();
                }
                else
                {
                    yellow();
                }
            }
            printf("%02X ", arr[i]);
        }
        normal();
        printf("\n");
        fflush(stdout);
    }

    void fill_new_array(exchange_array arr)
    {
        for (size_t i = 0; i < 20; ++i)
        {
            if (arr[i] == 0x00)
                continue;

            if (first[i] == 0x00)
            {
                first[i] = arr[i];
                continue;
            }

            if (first[i] == arr[i])
            {
                continue;
            }

            if (second[i] == 0x00)
            {
                second[i] = arr[i];
                continue;
            }

            if (second[i] == arr[i])
            {
                continue;
            }

            if (third[i] == 0x00)
            {
                third[i] = arr[i];
            }

            if (third[i] == arr[i])
            {
                continue;
            }

            if (fourth[i] == 0x00)
            {
                fourth[i] = arr[i];
            }

            if (fourth[i] == arr[i])
            {
                continue;
            }

            if (fifth[i] == 0x00)
            {
                fifth[i] = arr[i];
            }

            if (fifth[i] == arr[i])
            {
                continue;
            }

            if (sixth[i] == 0x00)
            {
                sixth[i] = arr[i];
            }
        }
    }

    void print_new_array(exchange_array arr)
    {
        system("clear");

        red();
        for (size_t i = 0; i < 20; ++i)
        {
            printf("%02X ", i);
        }
        printf("\n");

        for (size_t i = 0; i < 20; ++i)
        {
            if (first[i] == 0x00)
                blue();
            else
                yellow();
            printf("%02X ", first[i]);
        }
        printf("\n");
        for (size_t i = 0; i < 20; ++i)
        {
            if (second[i] == 0x00)
                blue();
            else
                yellow();
            printf("%02X ", second[i]);
        }
        printf("\n");
        for (size_t i = 0; i < 20; ++i)
        {
            if (third[i] == 0x00)
                blue();
            else
                yellow();
            printf("%02X ", third[i]);
        }
        printf("\n");
        for (size_t i = 0; i < 20; ++i)
        {
            if (fourth[i] == 0x00)
                blue();
            else
                yellow();
            printf("%02X ", fourth[i]);
        }
        printf("\n");
        for (size_t i = 0; i < 20; ++i)
        {
            if (fifth[i] == 0x00)
                blue();
            else
                yellow();
            printf("%02X ", fifth[i]);
        }
        printf("\n");
        for (size_t i = 0; i < 20; ++i)
        {
            if (sixth[i] == 0x00)
                blue();
            else
                yellow();
            printf("%02X ", sixth[i]);
        }
        printf("\n\n");
    }

    int read(hid_device *device, unsigned char *data, size_t size)
    {
        int ret = hid_read(device, data, size);
        if (ret < 0)
        {
            printf("%sERROR: Couldn't read from device nr. %u%s\n", KRED, n_controller, KNRM);
        }
        return ret;
    }

    int open_device(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number, unsigned short n_controll)
    {
        controller_ptr = hid_open(vendor_id, product_id, serial_number);
        if (!controller_ptr)
        {
            return -1;
        }

        if (false)
        { //!exchange(handshake)) { //need std::optional
            red();
            printf("ERROR: exchange handshake failed!\n");
            normal();
        }

        exchange(switch_baudrate);
        exchange(handshake);
        exchange(hid_only_mode);

        printf("%sSuccessfully opened controller nr. %u with serial number NULL%s\n", KYEL, n_controll, KNRM);
        n_controller = n_controll;

        send_subcommand(0x1, ledCommand, led);

        is_opened = true;

        usleep(100 * 1000);

        return 0;
    }

    void set_non_blocking()
    {
        if (hid_set_nonblocking(controller_ptr, 1) < 0)
        {
            printf("%sERROR: Couldn't set controller %u to non-blocking%s\n", KRED, n_controller, KNRM);
        }
    }

    void set_blocking()
    {
        if (hid_set_nonblocking(controller_ptr, 0) < 0)
        {
            printf("%sERROR: Couldn't set controller %u to blocking%s\n", KRED, n_controller, KNRM);
        }
    }

    void close_device()
    {
        if (!is_opened)
            return;
        is_opened = false;
        if (controller_ptr)
        {
            hid_close(controller_ptr);
            blue();
            printf("Closed controller nr. %u\n", n_controller);
            normal();
        }
    }

    static const void red()
    {
        printf("%s", KRED);
    }
    static const void normal()
    {
        printf("%s", KNRM);
    }
    static const void blue()
    {
        printf("%s", KBLU);
    }
    static const void yellow()
    {
        printf("%s", KYEL);
    }

    std::array<uchar, 20> first{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    std::array<uchar, 20> second{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    std::array<uchar, 20> third{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    std::array<uchar, 20> fourth{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    std::array<uchar, 20> fifth{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    std::array<uchar, 20> sixth{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    uchar rumble_counter{0};
    const std::array<uchar, 1> led{0x8};
    const std::array<uchar, 0> empty{};
    const std::array<uchar, 2> handshake{0x80, 0x02};
    const std::array<uchar, 2> switch_baudrate{0x80, 0x03};
    const std::array<uchar, 2> hid_only_mode{0x80, 0x04};

    bool bad_data_detected = false;

    hid_device *controller_ptr;
    unsigned short n_controller;
    unsigned short serial_number;
    bool is_opened = false;
};
