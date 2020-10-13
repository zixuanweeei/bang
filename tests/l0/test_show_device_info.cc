#include <iostream>
#include <vector>

#include "ze_api.h"
#include "zes_api.h"

void ShowDeviceInfo(zes_device_handle_t hSysmanDevice) {
  zes_device_properties_t devProps;
  zes_device_state_t devState;
  if (zesDeviceGetProperties(hSysmanDevice, &devProps) == ZE_RESULT_SUCCESS) {
    std::cout << "    UUID:           ";
    uint8_t *id = devProps.core.uuid.id;
    for (size_t i = 0; i < ZE_MAX_DEVICE_UUID_SIZE; ++i)
      std::cout << static_cast<uint32_t>(id[i]);
    std::cout << '\n';

    std::cout << "    #subdevices:    " << devProps.numSubdevices << '\n';
    std::cout << "    brand:          " << devProps.brandName << '\n';
    std::cout << "    model:          " << devProps.modelName << '\n';
  }
  if (zesDeviceGetState(hSysmanDevice, &devState) == ZE_RESULT_SUCCESS)
    std::cout << "    Was repaired:   %s"
              << ((devState.repaired == ZES_REPAIR_STATUS_PERFORMED) ? "yes"
                                                                     : "no");
  if (devState.reset != 0) {
    std::cout << "DEVICE RESET REQUIRED:\n";
    if (devState.reset & ZES_RESET_REASON_FLAG_WEDGED)
      std::cout << "- Hardware is wedged\n";
    if (devState.reset & ZES_RESET_REASON_FLAG_REPAIR)
      std::cout << "- Hardware needs to complete repairs\n";
  }
}

int main(int argv, char **argc) {
  if (zeInit(0) != ZE_RESULT_SUCCESS) {
    std::cout << "Can't initialize the API";
  } else { // Discover all the drivers
    uint32_t driversCount = 0;
    zeDriverGet(&driversCount, nullptr);
    std::vector<ze_driver_handle_t> allDrivers(driversCount);
    zeDriverGet(&driversCount, allDrivers.data());
    std::cout << "Found " << driversCount << " driver(s).\n";

    ze_driver_handle_t hDriver = nullptr;
    for (int i = 0; i < driversCount; ++i) { // Discover devices in a driver
      uint32_t deviceCount = 0;
      zeDeviceGet(allDrivers[i], &deviceCount, nullptr);

      std::vector<ze_device_handle_t> allDevices(deviceCount);
      zeDeviceGet(allDrivers[i], &deviceCount, allDevices.data());
      std::cout << "Found " << deviceCount << " device(s).\n";

      for (int devIndex = 0; devIndex < deviceCount; ++devIndex) {
        ze_device_properties_t device_properties;
        zeDeviceGetProperties(allDevices[devIndex], &device_properties);
        std::cout << "Found device - " << device_properties.name << '\n';
        if (ZE_DEVICE_TYPE_GPU != device_properties.type)
          continue;
        // Get the Sysman device handle
        zes_device_handle_t hSysmanDevice =
            (zes_device_handle_t)allDevices[devIndex];
        // Start using hSysmanDevice to manage the device
        ShowDeviceInfo(hSysmanDevice);
      }
    }
  }
}