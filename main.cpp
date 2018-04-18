#include "procon.hpp"

//#define DEBUG
#define TEST_BAD_DATA_CYCLES 10

int main(int argc, char *argv[]) {
  bool help = false;
  bool force_calibration = false;
  bool show_version = false;
#ifdef DRIBBLE_MODE
  int dribble_cam_value;
  bool found_dribble_cam_value = false;
#endif

  for (size_t i = 1; i < argc; ++i) {
    // printf("argv: %d\n",argv[i]);
    // std::cout << argv[i] << std::endl;

    bool option_found = false;
    if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
      help = true;
      option_found = true;
    }
    if (std::string(argv[i]) == "-c" ||
        std::string(argv[i]) == "--calibration") {
      force_calibration = true;
      option_found = true;
    }
    if (std::string(argv[i]) == "--version") {
      show_version = true;
      option_found = true;
    }
#ifdef DRIBBLE_MODE
    if (std::string(argv[i]) == "-d") {
      option_found = true;
      i++;
      dribble_cam_value = std::stoi(argv[i]);
      found_dribble_cam_value = true;
    }
#endif
    if (!option_found) {
      std::cout << "Unknown option " << argv[i]
                << ". For usage, type 'procon_driver --help'" << std::endl;
      return -1;
    }
  }

  if (help) {
    printf("Usage: procon_driver [OPTIONS]\noptions are:\n -h --help           "
           "get help on usage at start\n -c --calibration    force calibration "
           "at start\n");
#ifdef DRIBBLE_MODE
    printf(" -d [VALUE]          pass parameter for dribble cam. Range 0 to "
           "255\n");
#endif
    printf("\n");
    return 0;
  }

  if (show_version) {
    std::cout << "Version is " << PROCON_DRIVER_VERSION << std::endl;
    return 0;
  }

  printf("\n-------------------------------------------------------------------"
         "-----------------------\n");
  printf("| ");
  printf("%c[%d;%dmNintendo Switch Pro-Controller USB driver for linux based "
         "systems.%c[%dm ",
         27, 1, 32, 27, 0);
  printf("%c[%d;%dmVersion: ", 27, 1, 36);
  printf(PROCON_DRIVER_VERSION);
  printf("%c[%dm ", 27, 0);

  printf("%s "
         "|\n------------------------------------------------------------------"
         "------------------------",
         KNRM);
  printf("\n\n%s", KNRM);
#ifdef DRIBBLE_MODE
  printf("%c[%d;%dmDribble mode enabled!%c[%dm \n\n", 27, 1, 36, 27, 0);
// if(found_dribble_cam_value) {
//   printf("VALUE: %i", dribble_cam_value);
// }
#endif
  fflush(stdout);
  ProController controller;
  hid_init();
  hid_device *controller_ptr;
  hid_device_info *devs =
      hid_enumerate(NINTENDO_ID, PROCON_ID); // Don't trust hidapi, returns
                                             // non-matching devices sometimes
                                             // (*const to prevent compiler from
                                             // optimizing away)
  hid_device_info *iter = devs;
  unsigned short n_controller = 0;
  bool controller_found = false;

  bool opened = false;
  bool bad_data = false;

  // pass arguments to controller
  if (force_calibration) {
    controller.read_calibration_from_file = false;
  }
#ifdef DRIBBLE_MODE
  if (found_dribble_cam_value) {
    controller.dribble_mode_value = dribble_cam_value;
  }
#endif

  // OPEN PHASE
  do {
    opened = false;
    bad_data = false;
    if (iter != nullptr) {
      if (iter->product_id == PROCON_ID && iter->vendor_id == NINTENDO_ID) {
        // open & test for timeout in read!
        int ret = controller.open_device(iter->vendor_id, iter->product_id,
                                         iter->serial_number, n_controller + 1);
        opened = ret == 0;
        if (!opened) { // read timed out

          if (ret == -1) {
            ProController::red();
            printf("Invalid device pointer. Aborting!\n");
            ProController::normal();
            return -1;
          }
          ProController::magenta();
          printf("Failed to open controller, error code %d, trying again...\n",
                 ret);
          ProController::normal();
          controller.close_device();
          usleep(1000 * 10);
          continue;
        } else {
          // TEST FOR BAD DATA
          for (size_t i = 0; i < TEST_BAD_DATA_CYCLES; ++i) {
            if (controller.try_read_bad_data() != 0) {
              ProController::magenta();
              printf("Detected bad data stream. Trying again...\n");
              ProController::normal();
              controller.close_device();
              bad_data = true;
              usleep(1000 * 10);
              break;
            }
          }
        }
      }
    } else {
      ProController::red();
      printf("No controller found...\n");
      ProController::normal();
      return -1;
    }

  } while (!opened || bad_data);

  if (controller.is_opened) {
    ProController::green();
    printf("Opened controller!\n");

    if (controller.uinput_create() < 0) {
      ProController::red();
      printf("Failed to open uinput device!\n");
      ProController::normal();
    }

    if (!controller.read_calibration_from_file ||
        !controller.calibration_file_exists()) {
      ProController::blue();
      printf("Now entering calibration mode. \n");
      ProController::cyan();
      printf("%c[%d;%dmMove your analog sticks to the maxima, then press the "
             "square 'share' button!\n%c[%dm",
             27, 1, 36, 27, 0);
      ProController::normal();
    }

    // if(controller.libevdev_test() < 0) {
    //   ProController::red();
    //   printf("Failed to open libevdev device!\n");
    //   ProController::normal();
    // }
  }

  // controller.u_setup();

  while (true) {
    if (!controller.calibrated) {
      while (!controller.calibrated) {
        controller.calibrate();
      }
      ProController::green();
      printf("Calibrated Controller! Now entering input mode!\n");
      ProController::normal();
    }

    if (controller.is_opened) {
      if (controller.poll_input() < 0)
        return -1;
    }
  }

  // hid_exit;
  for (short unsigned i = 0; i < MAX_N_CONTROLLERS; ++i) {
    controller.close_device();
    controller.uinput_destroy();
  }
  printf("\n");
  return 0;
}