#include "hidapi.h"
#include <stdio.h>
#include <array>
#include <cstring>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include <ratio>
//#include <optional>

#define PROCON_DRIVER_VERSION "0.2"

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

    static const uchar byte_button_value(ProController::BUTTONS button)
    {
        switch (button)
        {
        case d_left:
            return 0x08;
            break;
        case d_right:
            return 0x04;
            break;
        case d_up:
            return 0x02;
            break;
        case d_down:
            return 0x01;
            break;
        case A:
            return 0x08;
            break;
        case B:
            return 0x04;
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
            return 0x10;
            break;
        case share:
            return 0x20;
            break;
        case L1:
            return 0x40;
            break;
        case L2:
            return 0x80;
            break;
        case L3:
            return 0x08;
            break;
        case R1:
            return 0x40;
            break;
        case R2:
            return 0x80;
            break;
        case R3:
            return 0x04;
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

    void timer()
    {
        // using namespace std::chrono;

        // high_resolution_clock::time_point t2 = high_resolution_clock::now();

        // duration<double, std::milli> time_span = t2 - t1;

        // high_resolution_clock::time_point t1 = t2;

        // printf("Time since last call ms: %u\n", time_span.count());
        // //std::cout << "It took me " << time_span.count() << " milliseconds.";
        // //std::cout << std::endl;

        using namespace std;
        clock_t now = clock();

        double elapsed_secs = double(now - last_time) / CLOCKS_PER_SEC;

        last_time = now;

        printf("Time for last %u polls: %f seconds\n", n_print_cycle, elapsed_secs);
        printf("Bad 0x00: %u\nBad 0x30: %u\n\n", n_bad_data_thirty, n_bad_data_zero);

        print_cycle_counter = 0;
        n_bad_data_thirty = 0;
        n_bad_data_zero = 0;
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
    exchange_array exchange(std::array<uchar, length> const &data, bool timed = false)
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
        if (!timed)
            hid_read(controller_ptr, ret.data(), exchange_length);
        else
        {
            if (hid_read_timeout(controller_ptr, ret.data(), exchange_length, 100) == 0)
            {
                red();
                //printf("Timeout! Let's try that again!\n");
                normal();
                fflush(stdout);
                close_device();
                open_device(ven_id, prod_id, NULL, n_controller);
                usleep(1000 * 100);
            }
        }
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

    void print_sticks(const uchar &data0, const uchar &data1, const uchar &data2, const uchar &data3, const uchar &data4, const uchar &data5)
    {
        uchar left_x = ((data1 & 0x0F) << 4) | ((data0 & 0xF0) >> 4);
        uchar left_y = data2;
        uchar right_x = ((data4 & 0x0F) << 4) | ((data3 & 0xF0) >> 4);
        uchar right_y = data5;

        yellow();
        printf("left_x %d\n", left_x);
        printf("left_y %d\n", left_y);
        printf("right_x %d\n", right_x);
        printf("right_y %d\n", right_y);
        normal();

        // if(left_x == 0x00 || left_y == 0x00 || right_x == 0x00 || right_y == 0x00 ) {
        //     return -1;
        // }
        // return 0;
    }

    void print_buttons(const uchar &left, const uchar &mid, const uchar &right)
    {
        //uchar left = buttons[0];
        //uchar mid = buttons[1];
        //uchar right = buttons[2];

        if (left & byte_button_value(d_left))
            printf("d_left\n");
        if (left & byte_button_value(d_right))
            printf("d_right\n");
        if (left & byte_button_value(d_up))
            printf("d_up\n");
        if (left & byte_button_value(d_down))
            printf("d_down\n");
        if (left & byte_button_value(L1))
            printf("L1\n");
        if (left & byte_button_value(L2))
            printf("L2\n");
        if (mid & byte_button_value(L3))
            printf("L3\n");
        if (mid & byte_button_value(R3))
            printf("R3\n");
        if (mid & byte_button_value(home))
            printf("home\n");
        if (mid & byte_button_value(share))
            printf("share\n");
        if (mid & byte_button_value(plus))
            printf("plus\n");
        if (mid & byte_button_value(minus))
            printf("minus\n");
        if (right & byte_button_value(A))
            printf("A\n");
        if (right & byte_button_value(B))
            printf("B\n");
        if (right & byte_button_value(X))
            printf("X\n");
        if (right & byte_button_value(Y))
            printf("Y\n");
        if (right & byte_button_value(R1))
            printf("R1\n");
        if (right & byte_button_value(R2))
            printf("R2\n");
    }

    void clear_console()
    {
        system("clear");
    }

    int poll_input()
    {
        print_cycle_counter++;
        if(print_cycle_counter++ > n_print_cycle) {
            timer();
        }
        if (!controller_ptr)
        {
            printf("%sERROR: Controller pointer is nullptr%s\n", KRED, KNRM);
            return -1;
        }

        auto dat = send_command(get_input, empty);

        //clear_console();
        // if (!dat)
        // {
        //     red();
        //     printf("ERROR: Data is not valid!\n");
        //     normal();
        //     return;
        // }
        //fill_new_array(dat);
        //print_new_array(dat);

        // if (dat[0x10] == 0x00)
        // {
        //     print_exchange_array(dat);
        //     if (detect_useless_data(dat[0]))
        //     {
        //         printf("BÖSE!!\n");
        //     }
        //     usleep(1000 * 1000);
        // }

        if (detect_useless_data(dat[0]))
        {
            //printf("detected useless data!\n");
            return 0;
        }

        detect_bad_data(dat[0], dat[1]);
        if (bad_data_detected)
        {
            red();
            //printf("\n-----------------------------------------------------------\n----------------   DETECTED BAD DATA!!!!!   ---------------\n-----------------------------------------------------------\n");
            normal();
            //return -1;
            close_device();
            usleep(1000 * 100);
            open_device(ven_id, prod_id, NULL, n_controller);
            bad_data_detected = false;
        }

        std::array<uchar, 3> buttons;
        //buttons[0] = dat[0x0f]; //left
        //buttons[1] = dat[0x0e]; //mid
        //buttons[2] = dat[0x0d]; //right

        print_buttons(dat[0x0f], dat[0x0e], dat[0x0d]);
        //print_sticks(dat[0x10], dat[0x11], dat[0x12], dat[0x13], dat[0x14], dat[0x15]);
        //print_exchange_array(dat);
        return 0;
    }
    /* Hackishly detects when the controller is trapped in a bad loop. 
    Nothing to do here, need to restart driver :(*/
    void detect_bad_data(const uchar &dat1, const uchar &dat2)
    {
        bad_data_detected = (dat2 == 0x01 && dat1 == 0x81) ? true : bad_data_detected;
    }

    bool detect_useless_data(const uchar &dat)
    {
        if(dat == 0x30) 
            n_bad_data_thirty++;
        if(dat == 0x00) 
            n_bad_data_zero++;
        return (dat == 0x30 || dat == 0x00);
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
        is_opened = true;

        if (!controller_ptr)
        {
            return -1;
        }

        n_controller = n_controll;
        ven_id = vendor_id;
        prod_id = product_id;

        // if (false)
        // { //!exchange(handshake)) { //need std::optional
        //     red();
        //     printf("ERROR: exchange handshake failed!\n");
        //     normal();
        // }

        //set_non_blocking();

        exchange(switch_baudrate);
        exchange(handshake);
        exchange(hid_only_mode, true);

        send_subcommand(0x1, ledCommand, led);

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
            //printf("Closed controller nr. %u\n", n_controller);
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
    std::clock_t last_time;

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

    unsigned short ven_id;
    unsigned short prod_id;
    unsigned short n_controller;

    uint n_print_cycle = 1000;
    uint print_cycle_counter = 0;
    uint n_bad_data_zero = 0;
    uint n_bad_data_thirty = 0;

    bool is_opened = false;
};
