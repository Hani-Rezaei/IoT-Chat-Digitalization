/* Generated file do not edit */
#ifdef DOXYGEN
#define RIOT_VERSION "<YEAR_OF_RELEASE>.<MONTH_OF_RELEASE>-<POSTFIX>"
#define RIOT_VERSION_CODE   RIOT_VERSION_NUM(<YEAR>,<MONTH>,<PATCH>,<EXTRA>)
#define RIOT_APPLICATION    "<RIOT_APP_NAME>"
#define RIOT_BOARD          "<BOARD_NAME>"
#define RIOT_CPU            "<CPU_FOLDER_NAME>"
#define CPU_RAM_SIZE        /* RAM Size in Bytes */
#endif /* DOXYGEN */
#if defined(DOXYGEN)
#  define MACRO_DEPRECATED    /* implementation */
#elif defined(__GNUC__) || defined(__clang__)
#  define MACRO_DEPRECATED _Pragma("GCC warning \"Code is using a deprecated macro\"")
#else
#  define MACRO_DEPRECATED
#endif
#define RIOT_MCU        MACRO_DEPRECATED RIOT_CPU
#define CONFIG_NIMBLE_AUTOADV_DEVICE_NAME "NimBLE GATT Example"
#define CONFIG_NIMBLE_AUTOADV_START_MANUALLY 1
#define DEVELHELP 1
#define CPU_FAM_NRF52 1
#define CPU_MODEL_NRF52840XXAA 1
#define CPU_CORE_CORTEX_M4F 1
#define RIOT_APPLICATION "nimble_gatt"
#define BOARD_NRF52840DK "nrf52840dk"
#define RIOT_BOARD BOARD_NRF52840DK
#define CPU_NRF52 "nrf52"
#define RIOT_CPU CPU_NRF52
#define CPU_RAM_BASE 0x20000000
#define CPU_RAM_SIZE 0x40000
#define RTT_FREQUENCY RTT_MAX_FREQUENCY
#define NIMBLE_CFG_CONTROLLER 1
#define MYNEWT_VAL_OS_CPUTIME_FREQ 32768
#define MYNEWT_VAL_BLE_TRANSPORT_EVT_SIZE 45
#define NRF52840_XXAA 1
#define RIOT_VERSION "2025.01-devel-2-g94bb7"
#define RIOT_VERSION_CODE RIOT_VERSION_NUM(2025,01,0,0)
#define MODULE_AUTO_INIT 1
#define MODULE_AUTO_INIT_ZTIMER 1
#define MODULE_BLUETIL_AD 1
#define MODULE_BLUETIL_ADDR 1
#define MODULE_BOARD 1
#define MODULE_BOARD_COMMON_INIT 1
#define MODULE_BOARDS_COMMON_NRF52XXXDK 1
#define MODULE_CMSIS 1
#define MODULE_CORE 1
#define MODULE_CORE_INIT 1
#define MODULE_CORE_LIB 1
#define MODULE_CORE_MSG 1
#define MODULE_CORE_PANIC 1
#define MODULE_CORE_THREAD 1
#define MODULE_CORE_THREAD_FLAGS 1
#define MODULE_CORTEXM_COMMON 1
#define MODULE_CORTEXM_COMMON_PERIPH 1
#define MODULE_CORTEXM_FPU 1
#define MODULE_CPU 1
#define MODULE_CPU_COMMON 1
#define MODULE_DIV 1
#define MODULE_EVENT 1
#define MODULE_EVENT_CALLBACK 1
#define MODULE_FMT 1
#define MODULE_FRAC 1
#define MODULE_LIBC 1
#define MODULE_MALLOC_THREAD_SAFE 1
#define MODULE_MPU_STACK_GUARD 1
#define MODULE_NEWLIB 1
#define MODULE_NEWLIB_NANO 1
#define MODULE_NEWLIB_SYSCALLS_DEFAULT 1
#define MODULE_NIMBLE 1
#define MODULE_NIMBLE_AUTOADV 1
#define MODULE_NIMBLE_CONTROLLER 1
#define MODULE_NIMBLE_DRIVERS_NRF5X 1
#define MODULE_NIMBLE_HOST 1
#define MODULE_NIMBLE_HOST_STORE_RAM 1
#define MODULE_NIMBLE_HOST_UTIL 1
#define MODULE_NIMBLE_NPL_RIOT 1
#define MODULE_NIMBLE_PORTING_NIMBLE 1
#define MODULE_NIMBLE_RIOT_CONTRIB 1
#define MODULE_NIMBLE_SVC_GAP 1
#define MODULE_NIMBLE_SVC_GATT 1
#define MODULE_NIMBLE_TRANSPORT 1
#define MODULE_NRF52_VECTORS 1
#define MODULE_NRF5X_COMMON_PERIPH 1
#define MODULE_NRFX 1
#define MODULE_PERIPH 1
#define MODULE_PERIPH_COMMON 1
#define MODULE_PERIPH_GPIO 1
#define MODULE_PERIPH_GPIO_LL_DISCONNECT 1
#define MODULE_PERIPH_GPIO_LL_INPUT_PULL_DOWN 1
#define MODULE_PERIPH_GPIO_LL_INPUT_PULL_UP 1
#define MODULE_PERIPH_GPIO_LL_IRQ_EDGE_TRIGGERED_BOTH 1
#define MODULE_PERIPH_GPIO_LL_IRQ_UNMASK 1
#define MODULE_PERIPH_GPIO_LL_OPEN_DRAIN 1
#define MODULE_PERIPH_GPIO_LL_OPEN_DRAIN_PULL_UP 1
#define MODULE_PERIPH_GPIO_LL_OPEN_SOURCE 1
#define MODULE_PERIPH_GPIO_LL_OPEN_SOURCE_PULL_DOWN 1
#define MODULE_PERIPH_INIT 1
#define MODULE_PERIPH_INIT_GPIO 1
#define MODULE_PERIPH_INIT_LED0 1
#define MODULE_PERIPH_INIT_LED1 1
#define MODULE_PERIPH_INIT_LED2 1
#define MODULE_PERIPH_INIT_LED3 1
#define MODULE_PERIPH_INIT_LED4 1
#define MODULE_PERIPH_INIT_LED5 1
#define MODULE_PERIPH_INIT_LED6 1
#define MODULE_PERIPH_INIT_LED7 1
#define MODULE_PERIPH_INIT_LEDS 1
#define MODULE_PERIPH_INIT_PM 1
#define MODULE_PERIPH_INIT_RTT 1
#define MODULE_PERIPH_INIT_UART 1
#define MODULE_PERIPH_PM 1
#define MODULE_PERIPH_RTT 1
#define MODULE_PERIPH_UART 1
#define MODULE_PREPROCESSOR 1
#define MODULE_PREPROCESSOR_SUCCESSOR 1
#define MODULE_SEMA 1
#define MODULE_STDIO 1
#define MODULE_STDIO_UART 1
#define MODULE_SYS 1
#define MODULE_TINYCRYPT 1
#define MODULE_VDD_LC_FILTER_REG0 1
#define MODULE_VDD_LC_FILTER_REG1 1
#define MODULE_ZTIMER 1
#define MODULE_ZTIMER_CONVERT 1
#define MODULE_ZTIMER_CONVERT_FRAC 1
#define MODULE_ZTIMER_CONVERT_SHIFT 1
#define MODULE_ZTIMER_CORE 1
#define MODULE_ZTIMER_EXTEND 1
#define MODULE_ZTIMER_INIT 1
#define MODULE_ZTIMER_MSEC 1
#define MODULE_ZTIMER_PERIPH_RTT 1
