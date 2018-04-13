#include "procon.hpp"

//#define DEBUG

int main(int argc, char **argv)
{
  printf("\n-------------------------------------------------------------------------------------------\n");
  printf("| %sNintendo Switch Pro-Controller USB driver for linux based systems. %sCurrent version: ", KGRN, KNRM);
  printf(PROCON_DRIVER_VERSION);
  printf(" |\n-------------------------------------------------------------------------------------------");
  printf("\n\n%s", KNRM);

  ProController controllers[MAX_N_CONTROLLERS];

  hid_init();

  hid_device *controller_ptr;
  hid_device_info *devs = hid_enumerate(NINTENDO_ID, PROCON_ID); // Don't trust hidapi, returns non-matching devices sometimes (*const to prevent compiler from optimizing away)
  hid_device_info *iter = devs;

  unsigned short n_controller = 0;
  do
  {
    if (iter != nullptr)
    {
#ifdef DEBUG
      printf("prod_id: 0x%x\n", iter->product_id);
      printf("vend_id: 0x%x\n", iter->vendor_id);
      printf("serial_numb: 0x%x\n", iter->serial_number);
#endif
      if (iter->product_id == PROCON_ID && iter->vendor_id == NINTENDO_ID)
      {
        if (controllers[n_controller].open_device(iter->vendor_id, iter->product_id, iter->serial_number, n_controller + 1) < 0)
        {
          printf("%sERROR: Failed to open controller nr %u!%s\n", KRED, n_controller + 1, KNRM);
          return -1;
        }
      }
      iter = iter->next;
      n_controller++;
    }
  } while (iter != nullptr && n_controller < MAX_N_CONTROLLERS);

  hid_free_enumeration(devs);

  //hid_device *controller_ptr = hid_open_path(device_name);

  hid_exit;
  return 0;
}