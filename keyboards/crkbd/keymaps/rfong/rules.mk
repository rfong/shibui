# https://github.com/manna-harbour/qmk_firmware/blob/crkbd/keyboards/crkbd/keymaps/manna-harbour/readme.org

SPLIT_KEYBOARD=yes

BOOTLOADER=qmk-dfu
#BOOTLOADER=atmel-dfu
#BOOTLOADER=caterina

### WPM config
WPM_ENABLE=yes
WPM_ALLOW_COUNT_REGRESSION=yes
WPM_SAMPLE_SECONDS=5
WPM_SAMPLE_PERIODS=5
WPM_LAUNCH_CONTROL=yes

### Tap dance
TAP_DANCE_ENABLE = yes

### Characters
UNICODE_ENABLE = yes

### RGB
#RGB_MATRIX_ENABLE = yes
#RGB_MATRIX_DRIVER = WS2812

### Trackpoint config
#MH_MODULE=trackpoint
#MH_MODULE=oled
#MH_MODULE=oled_old
MH_MODULE=no

MH_AUTO_BUTTONS=no
OLED_ENABLE=yes

ifeq ($(strip $(MH_MODULE)), trackpoint)
  MH_AUTO_BUTTONS=yes
  OLED_ENABLE=no
endif

DEBUG=yes
MH_DEBUG=yes
LTO_ENABLE=yes

MH_LOW_MOUSE=no
MH_OLED_MODE=no
MH_OLED_IMAGE=no


this_dir=keyboards/crkbd/keymaps/rfong


ifeq ($(strip $(MH_MODULE)), trackpoint)
  PS2_MOUSE_ENABLE = yes
  PS2_USE_INT = yes
  OPT_DEFS += -DMH_MODULE_TRACKPOINT
else ifeq ($(strip $(MH_MODULE)), oled)
  OLED_DRIVER_ENABLE = yes
else ifeq ($(strip $(MH_MODULE)), oled_old)
  SRC += ./lib/glcdfont.c
  SRC += ./lib/logo_reader.c
  OPT_DEFS += -DSSD1306OLED
endif

ifeq ($(strip $(MH_AUTO_BUTTONS)), yes)
  MOUSEKEY_ENABLE = yes
  OPT_DEFS += -DMH_AUTO_BUTTONS
endif

ifeq ($(strip $(MH_LOW_MOUSE)), yes)
  OPT_DEFS += -DMH_LOW_MOUSE
endif

ifeq ($(strip $(MH_OLED_MODE)), static)
  OPT_DEFS += -DMH_OLED_MODE_STATIC
else ifeq ($(strip $(MH_OLED_MODE)), caps)
  OPT_DEFS += -DMH_OLED_MODE_CAPS
endif

ifeq ($(strip $(MH_DEBUG)), yes)
  CONSOLE_ENABLE=yes
  OPT_DEFS += -DMH_DEBUG
endif

