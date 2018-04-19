#ifndef PROCON_DRIVER_H
#define PROCON_DRIVER_H

//#define ROCKET_LEAGUE // Inverts dpad up/down and right stick L/R

#ifdef ROCKET_LEAGUE
#define DRIBBLE_MODE // 
#endif

#include "hidapi.h"
#include <array>
#include <chrono>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <linux/input.h>
#include <linux/uinput.h>
#include <ratio>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdint>

//#include <libevdev/libevdev.h>
// #include <libevdev/libevdev-uinput.h>
//#include <optional>

#define PROCON_DRIVER_VERSION "1.0 alpha"

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

class ProController {

  enum BUTTONS {
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


  static constexpr uint8_t led_command{0x30};
  static constexpr uint8_t get_input{0x1f};
  static constexpr uint8_t center{0x7e};
  static constexpr size_t exchange_length{0x400};
  using exchange_array =
      std::array<uint8_t, exchange_length>; // might need std::optional<>
  // static constexpr size_t buflen{ 80 };
  // std::array<char, buflen> buffer;

public:
  // ProController() {}
  //~ProController() {}

  static const uint8_t bit_position(ProController::BUTTONS button) {
    switch (button) {
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

  static const uint8_t byte_button_value(ProController::BUTTONS button) {
    switch (button) {
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

  static const uint8_t data_address(ProController::BUTTONS button) {
    switch (button) {
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

  // void timer() {

  //   using namespace std;
  //   clock_t now = clock();

  //   double elapsed_secs = double(now - last_time) / CLOCKS_PER_SEC;

  //   last_time = now;

  //   printf("Time for last %u polls: %f seconds\n", n_print_cycle, elapsed_secs);
  //   printf("Bad 0x00: %u\nBad 0x30: %u\n\n", n_bad_data_thirty,
  //          n_bad_data_zero);

  //   print_cycle_counter = 0;
  //   n_bad_data_thirty = 0;
  //   n_bad_data_zero = 0;
  // }

  template <size_t length>
  exchange_array send_command(uint8_t command,
                              std::array<uint8_t, length> const &data) {
    std::array<uint8_t, length + 0x9> buffer;
    buffer.fill(0);
    buffer[0x0] = 0x80;
    buffer[0x1] = 0x92;
    buffer[0x3] = 0x31;
    buffer[0x8] = command;
    if (length > 0) {
      memcpy(buffer.data() + 0x9, data.data(), length);
    }
    return exchange(buffer);
  }

  template <size_t length>
  exchange_array exchange(std::array<uint8_t, length> const &data,
                          bool timed = false, int *status = nullptr) {

    if (!controller_ptr) {
      red();
      printf("ERROR: controller_ptr is nullptr!\n");
      normal();
      return {};
    }

    if (hid_write(controller_ptr, data.data(), length) < 0) {
      red();
      printf("ERROR: read() returned -1!\nDid you disconnect the controller?\n");
      normal();
      throw -1;
      return {};
    }

    std::array<uint8_t, exchange_length> ret;
    ret.fill(0);
    if (!timed)
      hid_read(controller_ptr, ret.data(), exchange_length);
    else {

      if (hid_read_timeout(controller_ptr, ret.data(), exchange_length, 100) ==
          0) {
        // failed to read!
        if (status) {
          *status = -1;
          return {};
        }
      }
    }
    if (status) {
      *status = 0;
    }
    return ret;
  }

  template <size_t length>
  exchange_array send_subcommand(uint8_t command, uint8_t subcommand,
                                 std::array<uint8_t, length> const &data) {
    std::array<uint8_t, length + 10> buffer{
        static_cast<uint8_t>(rumble_counter++ & 0xF),
        0x00,
        0x01,
        0x40,
        0x40,
        0x00,
        0x01,
        0x40,
        0x40,
        subcommand};
    if (length > 0) {
      memcpy(buffer.data() + 10, data.data(), length);
    }
    return send_command(command, buffer);
  }

  // void print_sticks(const uint8_t &data0, const uint8_t &data1, const uint8_t &data2,
  //                   const uint8_t &data3, const uint8_t &data4,
  //                   const uint8_t &data5) {
  //   uint8_t left_x = ((data1 & 0x0F) << 4) | ((data0 & 0xF0) >> 4);
  //   uint8_t left_y = data2;
  //   uint8_t right_x = ((data4 & 0x0F) << 4) | ((data3 & 0xF0) >> 4);
  //   uint8_t right_y = data5;

  //   map_sticks(left_x, left_y, right_x, right_y);

  //   clear_console();
  //   yellow();
  //   printf("left_x %d\n", left_x);
  //   printf("left_y %d\n", left_y);
  //   printf("right_x %d\n", right_x);
  //   printf("right_y %d\n\n", right_y);
  //   normal();

  //   // if(left_x == 0x00 || left_y == 0x00 || right_x == 0x00 || right_y == 0x00
  //   // ) {
  //   //     return -1;
  //   // }
  //   // return 0;
  // }

  // void print_buttons(const uint8_t &left, const uint8_t &mid, const uint8_t &right) {
  //   // uint8_t left = buttons[0];
  //   // uint8_t mid = buttons[1];
  //   // uint8_t right = buttons[2];

  //   if (left & byte_button_value(d_left))
  //     printf("d_left\n");
  //   if (left & byte_button_value(d_right))
  //     printf("d_right\n");
  //   if (left & byte_button_value(d_up))
  //     printf("d_up\n");
  //   if (left & byte_button_value(d_down))
  //     printf("d_down\n");
  //   if (left & byte_button_value(L1))
  //     printf("L1\n");
  //   if (left & byte_button_value(L2))
  //     printf("L2\n");
  //   if (mid & byte_button_value(L3))
  //     printf("L3\n");
  //   if (mid & byte_button_value(R3))
  //     printf("R3\n");
  //   if (mid & byte_button_value(share))
  //     printf("share\n");
  //   if (mid & byte_button_value(home)) {
  //     printf("home\n");
  //   }
  //   if (mid & byte_button_value(plus))
  //     printf("plus\n");
  //   if (mid & byte_button_value(minus))
  //     printf("minus\n");
  //   if (right & byte_button_value(A))
  //     printf("A\n");
  //   if (right & byte_button_value(B))
  //     printf("B\n");
  //   if (right & byte_button_value(X))
  //     printf("X\n");
  //   if (right & byte_button_value(Y))
  //     printf("Y\n");
  //   if (right & byte_button_value(R1))
  //     printf("R1\n");
  //   if (right & byte_button_value(R2))
  //     printf("R2\n");
  // }

  void clear_console() { system("clear"); }

  int poll_input() {
    // print_cycle_counter++;
    // if(print_cycle_counter++ > n_print_cycle) {
    //     timer();
    // }
    if (!controller_ptr) {
      printf("%sERROR: Controller pointer is nullptr%s\n", KRED, KNRM);
      return -1;
    }

    auto dat = send_command(get_input, empty);

    if (detect_useless_data(dat[0])) {
      // printf("detected useless data!\n");
      return 0;
    }

    send_subcommand(0x1, led_command, led_calibrated); // XXX way too often

    if (dat[0x0e] & byte_button_value(home) &&
        dat[0x0e] & byte_button_value(share)) {
      decalibrate();
    }

    uinput_manage_buttons(dat[0x0f], dat[0x0e], dat[0x0d]);
    uinput_manage_joysticks(dat[0x10], dat[0x11], dat[0x12], dat[0x13],
                            dat[0x14], dat[0x15]);
    uinput_manage_dpad(dat[0x0f]);

    // print_buttons(dat[0x0f], dat[0x0e], dat[0x0d]);
    // print_sticks(dat[0x10], dat[0x11], dat[0x12], dat[0x13], dat[0x14],
    // dat[0x15]);
    // print_exchange_array(dat);
    return 0;
  }

#ifdef DRIBBLE_MODE
  void toggle_dribble_mode() { dribble_mode = !dribble_mode; }
#endif

  void calibrate() {
    if (read_calibration_from_file) {
      std::ifstream myReadFile;
      uint8_t output[8];
      myReadFile.open("procon_calibration_data",
                      std::ios::in | std::ios::binary);
      if (myReadFile) {

        // while (!myReadFile.eof())

        myReadFile.read((char *)&left_x_min, sizeof(uint8_t));
        myReadFile.read((char *)&left_x_max, sizeof(uint8_t));
        myReadFile.read((char *)&left_y_min, sizeof(uint8_t));
        myReadFile.read((char *)&left_y_max, sizeof(uint8_t));
        myReadFile.read((char *)&right_x_min, sizeof(uint8_t));
        myReadFile.read((char *)&right_x_max, sizeof(uint8_t));
        myReadFile.read((char *)&right_y_min, sizeof(uint8_t));
        myReadFile.read((char *)&right_y_max, sizeof(uint8_t));

        green();
        printf("Read calibration data from file! ");
        cyan();
        printf("Press 'share' and 'home' to calibrate again or start with "
               "--calibrate or -c.\n");
        normal();

        calibrated = true;
        send_subcommand(0x1, led_command, led_calibrated);

        return;
      }

      myReadFile.close();
    }

    if (!controller_ptr) {
      printf("%sERROR: Controller pointer is nullptr%s\n", KRED, KNRM);
      return;
    }

    auto dat = send_command(get_input, empty);

    if (detect_useless_data(dat[0])) {
      // printf("detected useless data!\n");
      return;
    }

    // print_buttons(dat[0x0f], dat[0x0e], dat[0x0d]);
    // print_sticks(dat[0x10], dat[0x11], dat[0x12], dat[0x13], dat[0x14],
    // dat[0x15]);
    // print_exchange_array(dat);

    send_subcommand(0x1, led_command, led_calibration); // XXX way too often
    if (!share_button_free) {
      if (!(dat[0x0e] & byte_button_value(share))) {
        share_button_free = true;
      }
    } else {
      bool cal = do_calibrate(dat[0x10], dat[0x11], dat[0x12], dat[0x13],
                              dat[0x14], dat[0x15], dat[0x0e]);
      if (cal) {
        calibrated = true;
        send_subcommand(0x1, led_command, led_calibrated);
        // printf("finished calibration\n");
        // usleep(1000000);

        // write calibration data to file
        std::ofstream calibration_file;
        calibration_file.open("procon_calibration_data",
                              std::ios::out | std::ios::binary);
        calibration_file.write((char *)&left_x_min, sizeof(uint8_t));
        calibration_file.write((char *)&left_x_max, sizeof(uint8_t));
        calibration_file.write((char *)&left_y_min, sizeof(uint8_t));
        calibration_file.write((char *)&left_y_max, sizeof(uint8_t));
        calibration_file.write((char *)&right_x_min, sizeof(uint8_t));
        calibration_file.write((char *)&right_x_max, sizeof(uint8_t));
        calibration_file.write((char *)&right_y_min, sizeof(uint8_t));
        calibration_file.write((char *)&right_y_max, sizeof(uint8_t));
        calibration_file.close();
        green();
        printf("Wrote calibration data to file!\n");
        normal();
      }
    }

    // std::ofstream out("calibration_data");
    // if (!out)
    // {
    //     return;
    // }

    // printf("wrote text\n");

    // out.close();
  }

  bool do_calibrate(const uint8_t &stick0, const uint8_t &stick1,
                    const uint8_t &stick2, const uint8_t &stick3,
                    const uint8_t &stick4, const uint8_t &stick5,
                    const uint8_t &mid_buttons) {
    uint8_t left_x = ((stick1 & 0x0F) << 4) | ((stick0 & 0xF0) >> 4);
    uint8_t left_y = stick2;
    uint8_t right_x = ((stick4 & 0x0F) << 4) | ((stick3 & 0xF0) >> 4);
    uint8_t right_y = stick5;

    left_x_min = (left_x < left_x_min) ? left_x : left_x_min;
    left_y_min = (left_y < left_y_min) ? left_y : left_y_min;
    right_x_min = (right_x < right_x_min) ? right_x : right_x_min;
    right_y_min = (right_y < right_y_min) ? right_y : right_y_min;
    left_x_max = (left_x > left_x_max) ? left_x : left_x_max;
    left_y_max = (left_y > left_y_max) ? left_y : left_y_max;
    right_x_max = (right_x > right_x_max) ? right_x : right_x_max;
    right_y_max = (right_y > right_y_max) ? right_y : right_y_max;

    // clear_console();
    // printf("left_x_min: %u\n", left_x_min);
    // printf("left_y_min: %u\n", left_y_min);
    // printf("right_x_min: %u\n", right_x_min);
    // printf("right_y_min: %u\n", right_y_min);
    // printf("left_x_max: %u\n", left_x_max);
    // printf("left_y_max: %u\n", left_y_max);
    // printf("right_x_max: %u\n", right_x_max);
    // printf("right_y_max: %u\n\n", right_y_max);

    return (mid_buttons & byte_button_value(share));
  }

  void decalibrate() {
    left_x_min = center;
    left_x_max = center;
    left_y_min = center;
    left_x_max = center;
    right_x_min = center;
    right_x_max = center;
    right_y_min = center;
    right_x_max = center;
    calibrated = false;
    send_subcommand(0x1, led_command, led_calibration);
    magenta();
    printf("Controller decalibrated!\n");
    cyan();
    printf("%c[%d;%dmPerform calibration again and press the square 'share' "
           "button!\n%c[%dm",
           27, 1, 36, 27, 0);
    normal();
    read_calibration_from_file = false;
    share_button_free = false;
    // usleep(1000*1000);
  }

  const void map_sticks(uint8_t &left_x, uint8_t &left_y, uint8_t &right_x,
                        uint8_t &right_y) {
    left_x = (uint8_t)(clamp((float)(left_x - left_x_min) /
                           (float)(left_x_max - left_x_min) * 255.f));
    left_y = (uint8_t)(clamp((float)(left_y - left_y_min) /
                           (float)(left_y_max - left_y_min) * 255.f));
    right_x = (uint8_t)(clamp((float)(right_x - right_x_min) /
                            (float)(right_x_max - right_x_min) * 255.f));
    right_y = (uint8_t)(clamp((float)(right_y - right_y_min) /
                            (float)(right_y_max - right_y_min) * 255.f));
  }

  static const float clamp(float inp) {
    if (inp < 0.5f)
      return 0.5f;
    if (inp > 254.5f) {
      return 254.5f;
    }
    return inp;
  }
  static const int clamp_int(int inp) {
    if (inp < 0)
      return 0;
    if (inp > 255) {
      return 255;
    }
    return inp;
  }

  int try_read_bad_data() {

    if (!controller_ptr) {
      printf("%sERROR: Controller pointer is nullptr%s\n", KRED, KNRM);
      return -1;
    }

    auto dat = send_command(get_input, empty);

    if (detect_useless_data(dat[0])) {
      return 0;
    }

    if (detect_bad_data(dat[0], dat[1])) {
      // print_exchange_array(dat);
      return -1;
    }

    return 0;
  }

  /* Hackishly detects when the controller is trapped in a bad loop.
  Nothing to do here, need to restart driver :(*/
  bool detect_bad_data(const uint8_t &dat1, const uint8_t &dat2) {
    return (dat2 == 0x01 && dat1 == 0x81) ? true : bad_data_detected;
  }

  bool detect_useless_data(const uint8_t &dat) {
    if (dat == 0x30)
      n_bad_data_thirty++;
    if (dat == 0x00)
      n_bad_data_zero++;
    return (dat == 0x30 || dat == 0x00);
  }

  void print_exchange_array(exchange_array arr) {
    bool redcol = false;
    if (arr[0] != 0x30)
      yellow();
    else {
      red();
      redcol = true;
    }
    for (size_t i = 0; i < 20; ++i) {
      if (arr[i] == 0x00) {
        blue();
      } else {
        if (redcol) {
          red();
        } else {
          yellow();
        }
      }
      printf("%02X ", arr[i]);
    }
    normal();
    printf("\n");
    fflush(stdout);
  }

  int read(hid_device *device, uint8_t *data, size_t size) {
    int ret = hid_read(device, data, size);
    if (ret < 0) {
      printf("%sERROR: Couldn't read from device nr. %u%s\n", KRED,
             n_controller, KNRM);
    }
    return ret;
  }

  int open_device(unsigned short vendor_id, unsigned short product_id,
                  const wchar_t *serial_number, unsigned short n_controll) {
    controller_ptr = hid_open(vendor_id, product_id, serial_number);
    // controller_ptr = hid_open_path("/dev/input/hidraw0");
    is_opened = true;

    if (!controller_ptr) {
      return -1;
    }
    // hid_device_info *info = hid_open(vendor_id, product_id, serial_number);
    // std::cout<< "PATH: " << info->path << std::endl;;

    n_controller = n_controll;
    ven_id = vendor_id;
    prod_id = product_id;

    // if (false)
    // { //!exchange(handshake)) { //need std::optional
    //     red();
    //     printf("ERROR: exchange handshake failed!\n");
    //     normal();
    // }

    // set_non_blocking();

    exchange(switch_baudrate);
    exchange(handshake);

    // the next part will sometimes fail, then need to reopen device via hidapi
    int read_failed;
    exchange(hid_only_mode, true, &read_failed);
    if (read_failed < 0) {
      return -2;
    }

    send_subcommand(0x1, led_command, led_calibration);

    usleep(100 * 1000);

    return 0;
  }

  void set_non_blocking() {
    if (hid_set_nonblocking(controller_ptr, 1) < 0) {
      printf("%sERROR: Couldn't set controller %u to non-blocking%s\n", KRED,
             n_controller, KNRM);
    }
  }

  void set_blocking() {
    if (hid_set_nonblocking(controller_ptr, 0) < 0) {
      printf("%sERROR: Couldn't set controller %u to blocking%s\n", KRED,
             n_controller, KNRM);
    }
  }

  void close_device() {
    if (!is_opened)
      return;
    is_opened = false;
    if (controller_ptr) {
      hid_close(controller_ptr);
      blue();
      // printf("Closed controller nr. %u\n", n_controller);
      normal();
    }
  }

  // void blink() {
  //   if (++blink_counter > blink_length) {
  //     blink_counter = 0;
  //     if (++blink_position >= blink_array.size()) {
  //       blink_position = 0;
  //     }
  //   }
  //   std::array<uint8_t,1> blink_command{{blink_array[blink_position]}};
  //   send_subcommand(0x1, led_command, blink_command);
  // }

  //-------------------------
  //         UINPUT
  //-------------------------

  void uinput_manage_dpad(const char &left) {
    bool b_d_left = left & byte_button_value(d_left);
    bool b_d_right = left & byte_button_value(d_right);
#ifndef ROCKET_LEAGUE // upside down in RL....
    bool b_d_up = left & byte_button_value(d_up);
    bool b_d_down = left & byte_button_value(d_down);
#else
    bool b_d_up = left & byte_button_value(d_down);
    bool b_d_down = left & byte_button_value(d_up);
#endif

    memset(&uinput_event, 0, sizeof(uinput_event));
    gettimeofday(&uinput_event.time, NULL);

    if (b_d_left) {
      uinput_write_single_joystick(-1, ABS_HAT0X);
    } else if (b_d_right) {
      uinput_write_single_joystick(1, ABS_HAT0X);
    } else if (!b_d_left && !b_d_right) {
      uinput_write_single_joystick(0, ABS_HAT0X);
    }
    if (b_d_down) {
      uinput_write_single_joystick(-1, ABS_HAT0Y);
    } else if (b_d_up) {
      uinput_write_single_joystick(1, ABS_HAT0Y);
    } else if (!b_d_down && !b_d_up) {
      uinput_write_single_joystick(0, ABS_HAT0Y);
    }

    // send report
    uinput_event.type = EV_SYN;
    uinput_event.code = SYN_REPORT;
    uinput_event.value = 0;
    write(uinput_fd, &uinput_event, sizeof(uinput_event));
  }

  void uinput_manage_buttons(const char &left, const char &mid,
                             const char &right) {

    // bool b_d_left = left & byte_button_value(d_left);
    // bool b_d_right = left & byte_button_value(d_right);
    // bool b_d_up = left & byte_button_value(d_up);
    // bool b_d_down = left & byte_button_value(d_down);
    bool b_L1 = left & byte_button_value(L1);
    bool b_L2 = left & byte_button_value(L2);
    bool b_L3 = mid & byte_button_value(L3);
    bool b_R1 = right & byte_button_value(R1);
    bool b_R2 = right & byte_button_value(R2);
    bool b_R3 = mid & byte_button_value(R3);
    bool b_share = mid & byte_button_value(share);
    bool b_home = mid & byte_button_value(home);
    bool b_plus = mid & byte_button_value(plus);
    bool b_minus = mid & byte_button_value(minus);
    bool b_a = right & byte_button_value(A);
    bool b_b = right & byte_button_value(B);
    bool b_x = right & byte_button_value(X);
    bool b_y = right & byte_button_value(Y);

    // press
    if (b_a && !last_a)
      uinput_button_down(BTN_EAST);
    if (b_b && !last_b)
      uinput_button_down(BTN_SOUTH);
    if (b_x && !last_x) {
      uinput_button_down(BTN_WEST);
#ifdef DRIBBLE_MODE // toggle off dribble mode
      if (dribble_mode)
        toggle_dribble_mode();
#endif
    }
#ifdef DRIBBLE_MODE // toggle dribble mode
    if (b_y && !last_y)
      toggle_dribble_mode();
    if (b_share && !last_share) // replace button by share
      uinput_button_down(BTN_NORTH);
#else
    if (b_y && !last_y)
      uinput_button_down(BTN_NORTH);
#endif
  
  // if (b_d_down && !last_d_down)
  //   uinput_button_down(BTN_DPAD_DOWN);
  // if (b_d_up && !last_d_up)
  //   uinput_button_down(BTN_DPAD_UP);
  // if (b_d_left && !last_d_left)
  //   uinput_button_down(BTN_DPAD_LEFT);
  // if (b_d_right && !last_d_right)
  //   uinput_button_down(BTN_DPAD_RIGHT);
  if (b_plus && !last_plus)
    uinput_button_down(BTN_START);
  if (b_minus && !last_minus)
    uinput_button_down(BTN_SELECT);
  if (b_L1 && !last_L1)
    uinput_button_down(BTN_TL);
  // if (b_L2 && !last_L2)
  //   uinput_button_down(BTN_TL2);
  if (b_R1 && !last_R1)
    uinput_button_down(BTN_TR);
  // if (b_R2 && !last_R2)
  //   uinput_button_down(BTN_TR2);
  if (b_L3 && !last_L3)
    uinput_button_down(BTN_THUMBL);
  if (b_R3 && !last_R3)
    uinput_button_down(BTN_THUMBR);
  // if (b_L1 && !last_L1)
  //   uinput_button_down(BTN_TL);

  // release
  if (!b_a && last_a)
    uinput_button_release(BTN_EAST);
  if (!b_b && last_b)
    uinput_button_release(BTN_SOUTH);
  if (!b_x && last_x)
    uinput_button_release(BTN_WEST);

#ifdef DRIBBLE_MODE
  if (!b_y && last_y)
    uinput_button_release(BTN_WEST);
  if (!b_share && last_share)
    uinput_button_release(BTN_NORTH);
#else
  if (!b_y && last_y) 
    uinput_button_release(BTN_NORTH);
#endif

// if (!b_d_down && last_d_down)
//   uinput_button_release(BTN_DPAD_DOWN);
// if (!b_d_up && last_d_up)
//   uinput_button_release(BTN_DPAD_UP);
// if (!b_d_left && last_d_left)
//   uinput_button_release(BTN_DPAD_LEFT);
// if (!b_d_right && last_d_right)
//   uinput_button_release(BTN_DPAD_RIGHT);
if (!b_plus && last_plus) uinput_button_release(BTN_START);
if (!b_minus && last_minus)
  uinput_button_release(BTN_SELECT);
if (!b_L1 && last_L1)
  uinput_button_release(BTN_TL);
// if (!b_L2 && last_L2)
//   uinput_button_release(BTN_TL2);
if (!b_R1 && last_R1)
  uinput_button_release(BTN_TR);
// if (!b_R2 && last_R2)
//   uinput_button_release(BTN_TR2);
if (!b_L3 && last_L3)
  uinput_button_release(BTN_THUMBL);
if (!b_R3 && last_R3)
  uinput_button_release(BTN_THUMBR);
// if (!b_L1 && last_L1)
//   uinput_button_release(BTN_TL);

// last_d_left = b_d_left;
// last_d_right = b_d_right;
// last_d_up = b_d_up;
// last_d_down = b_d_down;
last_L1 = b_L1;
// last_L2 = b_L2;
last_L3 = b_L3;
last_R1 = b_R1;
// last_R2 = b_R2;
last_R3 = b_R3;
last_share = b_share;
last_home = b_home;
last_plus = b_plus;
last_minus = b_minus;
last_a = b_a;
last_b = b_b;
last_x = b_x;
last_y = b_y;

// do triggers here as well
int val = b_L2 ? 255 : 0;
uinput_write_single_joystick(val, ABS_Z);
val = b_R2 ? 255 : 0;
uinput_write_single_joystick(val, ABS_RZ);

// send report
uinput_event.type = EV_SYN;
uinput_event.code = SYN_REPORT;
uinput_event.value = 0;
write(uinput_fd, &uinput_event, sizeof(uinput_event));
}

bool calibration_file_exists() {
  std::ifstream conf("procon_calibration_data");
  return conf.good();
}

void uinput_manage_joysticks(const char &dat0, const char &dat1,
                             const char &dat2, const char &dat3,
                             const char &dat4, const char &dat5) {
  // extract data
  uint8_t left_x = ((dat1 & 0x0F) << 4) | ((dat0 & 0xF0) >> 4);
  uint8_t left_y = dat2;
  uint8_t right_x = ((dat4 & 0x0F) << 4) | ((dat3 & 0xF0) >> 4);
  uint8_t right_y = dat5;

  // map data
  map_sticks(left_x, left_y, right_x, right_y);

  // write uinput
  memset(&uinput_event, 0, sizeof(uinput_event));

  // left_x = 0;
  // left_y = 127;
  // right_x = 255;
  // right_y = 200;
  gettimeofday(&uinput_event.time, NULL);

  uinput_write_single_joystick(left_x, ABS_X);
  uinput_write_single_joystick(255 - left_y, ABS_Y);
  uinput_write_single_joystick(right_x, ABS_RX);
#ifndef DRIBBLE_MODE
  uinput_write_single_joystick(right_y, ABS_RY);
#else
  if (dribble_mode) {
    right_y = clamp_int(right_y + dribble_mode_value - 127);
  }
#endif
  uinput_write_single_joystick(right_y, ABS_RY);
  // send report
  uinput_event.type = EV_SYN;
  uinput_event.code = SYN_REPORT;
  uinput_event.value = 0;
  write(uinput_fd, &uinput_event, sizeof(uinput_event));

  // clear_console();
  // printf("left_x: %i\n", (int)left_x);
  // printf("left_y: %i\n", (int)left_y);
  // printf("right_x: %i\n", (int)right_x);
  // printf("right_y: %i\n", (int)right_y);
}

void uinput_write_single_joystick(const int &val, const int &cod) {

  uinput_event.type = EV_ABS;
  uinput_event.code = cod; // BTN_EAST;
  uinput_event.value = (int)val;

  int ret = write(uinput_fd, &uinput_event, sizeof(uinput_event));
  if (ret < 0) {
    red();
    printf("ERROR: write in button_down() returned %i\n", ret);
    normal();
  }
}

void uinput_button_down(const int &cod) {

  // press button
  memset(&uinput_event, 0, sizeof(uinput_event));
  gettimeofday(&uinput_event.time, NULL);
  uinput_event.type = EV_KEY;
  uinput_event.code = cod; // BTN_EAST;
  uinput_event.value = 1;
  int ret = write(uinput_fd, &uinput_event, sizeof(uinput_event));
  if (ret < 0) {
    red();
    printf("ERROR: write in button_down() returned %i\n", ret);
    normal();
  }

  // if (ret < 0)
  // {
  //   red();
  //   printf("ERROR: write in button_down() send report returned %i\n", ret);
  //   normal();
  // }

  // printf("PRessed button %u\n", cod);
}

void uinput_button_release(const int &cod) {
  // release button
  memset(&uinput_event, 0, sizeof(uinput_event));
  gettimeofday(&uinput_event.time, NULL);
  uinput_event.type = EV_KEY;
  uinput_event.code = cod;
  uinput_event.value = 0;
  write(uinput_fd, &uinput_event, sizeof(uinput_event));

  // send report
  uinput_event.type = EV_SYN;
  uinput_event.code = SYN_REPORT;
  uinput_event.value = 0;
  write(uinput_fd, &uinput_event, sizeof(uinput_event));
}

int uinput_create() {
  uinput_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
  uinput_rc = ioctl(uinput_fd, UI_GET_VERSION, &uinput_version);

  memset(&uinput_device, 0, sizeof(uinput_device));

  // strncpy(uinput_device.name, "Nintendo Switch Pro Controller USB",
  //        UINPUT_MAX_NAME_SIZE);
  // strncpy(uinput_device.name, "Nintendo Switch Pro Controller USB",
  //         UINPUT_MAX_NAME_SIZE);
  uinput_device.id.bustype = BUS_USB;
  uinput_device.id.vendor = 0x045e;  // Microsoft
  uinput_device.id.product = 0x028e; // XBOX 360
  uinput_device.id.version = 0x110;  // dunno but xboxdrv uses this
  strncpy(uinput_device.name, "Switch ProController disguised as XBox360",
          UINPUT_MAX_NAME_SIZE);
  // strncpy(
  //    uinput_device.name, "Microsoft X-Box 360 pad",
  //    UINPUT_MAX_NAME_SIZE); // is this trigger fo "being" an xbox
  //    device...?

  // buttons
  ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);

  // sudo xboxdrv --evdev /dev/input/event2 --evdev-absmap
  // ABS_X=x1,ABS_Y=y1,ABS_RX=x2,ABS_RY=y2,ABS_HAT0X=dpad_x,ABS_HAT0Y=dpad_y
  // --axismap -Y1=Y1,-Y2=Y2 --evdev-keymap
  // BTN_SOUTH=a,BTN_EAST=b,BTN_WEST=x,BTN_NORTH=y,BTN_TL=lb,BTN_TR=rb,BTN_TL2=lt,BTN_TR2=rt,BTN_THUMBL=tl,BTN_THUMBR=tr,BTN_SELECT=back,BTN_START=start
  // --silent &

  ioctl(uinput_fd, UI_SET_KEYBIT, BTN_EAST);
  ioctl(uinput_fd, UI_SET_KEYBIT, BTN_SOUTH);
  ioctl(uinput_fd, UI_SET_KEYBIT, BTN_NORTH);
  ioctl(uinput_fd, UI_SET_KEYBIT, BTN_WEST);
  // ioctl(uinput_fd, UI_SET_KEYBIT, BTN_DPAD_DOWN);
  // ioctl(uinput_fd, UI_SET_KEYBIT, BTN_DPAD_UP);
  // ioctl(uinput_fd, UI_SET_KEYBIT, BTN_DPAD_LEFT);
  // ioctl(uinput_fd, UI_SET_KEYBIT, BTN_DPAD_RIGHT);
  ioctl(uinput_fd, UI_SET_KEYBIT, BTN_MODE);
  ioctl(uinput_fd, UI_SET_KEYBIT, BTN_TL);
  // ioctl(uinput_fd, UI_SET_KEYBIT, BTN_TL2);
  ioctl(uinput_fd, UI_SET_KEYBIT, BTN_TR);
  // ioctl(uinput_fd, UI_SET_KEYBIT, BTN_TR2);
  ioctl(uinput_fd, UI_SET_KEYBIT, BTN_THUMBL);
  ioctl(uinput_fd, UI_SET_KEYBIT, BTN_THUMBR);
  ioctl(uinput_fd, UI_SET_KEYBIT, BTN_START);
  ioctl(uinput_fd, UI_SET_KEYBIT, BTN_SELECT);

  // joysticks
  ioctl(uinput_fd, UI_SET_EVBIT, EV_ABS);

  ioctl(uinput_fd, UI_SET_ABSBIT, ABS_X);
  ioctl(uinput_fd, UI_SET_ABSBIT, ABS_Y);
  ioctl(uinput_fd, UI_SET_ABSBIT, ABS_RX);
  ioctl(uinput_fd, UI_SET_ABSBIT, ABS_RY);
  ioctl(uinput_fd, UI_SET_ABSBIT, ABS_Z);  // L2
  ioctl(uinput_fd, UI_SET_ABSBIT, ABS_RZ); // R2
  ioctl(uinput_fd, UI_SET_ABSBIT, ABS_HAT0X);
  ioctl(uinput_fd, UI_SET_ABSBIT, ABS_HAT0Y);

  uinput_device.absmin[ABS_X] = 0;
  uinput_device.absmax[ABS_X] = 255;
  uinput_device.absmin[ABS_Y] = 0;
  uinput_device.absmax[ABS_Y] = 255;
  uinput_device.absmin[ABS_RX] = 0;
  uinput_device.absmax[ABS_RX] = 255;
  uinput_device.absmin[ABS_RY] = 0;
  uinput_device.absmax[ABS_RY] = 255;
  uinput_device.absmin[ABS_Z] = 0;
  uinput_device.absmax[ABS_Z] = 255;
  uinput_device.absmin[ABS_RZ] = 0;
  uinput_device.absmax[ABS_RZ] = 255;
  uinput_device.absmin[ABS_HAT0X] = -1;
  uinput_device.absmax[ABS_HAT0X] = 1;
  uinput_device.absmin[ABS_HAT0Y] = -1;
  uinput_device.absmax[ABS_HAT0Y] = 1;

  write(uinput_fd, &uinput_device, sizeof(uinput_device));

  if (ioctl(uinput_fd, UI_DEV_CREATE)) {
    return -1;
  }

  green();
  printf("Created uinput device!\n");
  normal();

  return 0;
}

void uinput_destroy() {
  ioctl(uinput_fd, UI_DEV_DESTROY);

  close(uinput_fd);

  yellow();
  printf("Destroyed uinput device!\n");
  normal();

  return;
}


static const void red() {
  printf("%s", KRED);
  fflush(stdout);
}
static const void normal() {
  printf("%s", KNRM);
  fflush(stdout);
}
static const void blue() {
  printf("%s", KBLU);
  fflush(stdout);
}
static const void yellow() {
  printf("%s", KYEL);
  fflush(stdout);
}
static const void green() {
  printf("%s", KGRN);
  fflush(stdout);
}
static const void magenta() {
  printf("%s", KMAG);
  fflush(stdout);
}
static const void cyan() {
  printf("%s", KCYN);
  fflush(stdout);
}
std::clock_t last_time;

std::array<uint8_t, 20> first{{0x0}};
std::array<uint8_t, 20> second{{0x0}};
std::array<uint8_t, 20> third{{0x0}};
std::array<uint8_t, 20> fourth{{0x0}};
std::array<uint8_t, 20> fifth{{0x0}};
std::array<uint8_t, 20> sixth{{0x0}};

uint8_t rumble_counter{0};
const std::array<uint8_t, 1> led_calibration{{0xff}};
const std::array<uint8_t, 1> led_calibrated{{0x01}};
const std::array<uint8_t, 0> empty{{}};
const std::array<uint8_t, 2> handshake{{0x80, 0x02}};
const std::array<uint8_t, 2> switch_baudrate{{0x80, 0x03}};
const std::array<uint8_t, 2> hid_only_mode{{0x80, 0x04}};
// const std::array<uint8_t, 4> blink_array{{0x05, 0x10}};//, 0x04, 0x08}};

// uint blink_position = 0;
// size_t blink_counter = 0;
// const size_t blink_length = 50;

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
bool calibrated = false;
bool read_calibration_from_file =
    true; // will be set to false in decalibrate or with flags
bool share_button_free = false; // used for recalibration (press share & home)
uint8_t left_x_min = 0x7e;
uint8_t left_y_min = 0x7e;
uint8_t right_x_min = 0x7e;
uint8_t right_y_min = 0x7e;
uint8_t left_x_max = 0x7e;
uint8_t left_y_max = 0x7e;
uint8_t right_x_max = 0x7e;
uint8_t right_y_max = 0x7e;

// bool last_d_left = false;
// bool last_d_right = false;
// bool last_d_up = false;
// bool last_d_down = false;
bool last_L1 = false;
// bool last_L2 = false;
bool last_L3 = false;
bool last_R1 = false;
// bool last_R2 = false;
bool last_R3 = false;
bool last_share = false;
bool last_home = false;
bool last_plus = false;
bool last_minus = false;
bool last_a = false;
bool last_b = false;
bool last_x = false;
bool last_y = false;

bool dribble_mode = false;
int dribble_mode_value = 205;

// uinput
struct uinput_user_dev uinput_device;
struct input_event uinput_event;
int uinput_version, uinput_rc, uinput_fd;
}
;

#endif

