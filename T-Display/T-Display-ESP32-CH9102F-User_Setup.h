//                            USER DEFINED SETTINGS
//   Set driver type, fonts to be loaded, pins used and SPI control method etc.

// User defined information reported by "Read_User_Setup" test & diagnostics example
#define USER_SETUP_INFO "User_Setup"

// ##################################################################################
//
// Section 1. Call up the right driver file and any options for it
//
// ##################################################################################

#define ST7789_DRIVER      // Full configuration option, define additional parameters below for this display

// For ST7735, ST7789 and ILI9341 ONLY, define the colour order IF the blue and red are swapped on your display
#define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

// For ST7789, ST7735, ILI9163 and GC9A01 ONLY, define the pixel width and height in portrait orientation
#define TFT_WIDTH  135 // ST7789 135 x 240
#define TFT_HEIGHT 240 // ST7789 135 x 240

#define TFT_INVERSION_OFF

// ##################################################################################
//
// Section 2. Define the pins that are used to interface with the display here
//
// ##################################################################################

// For ESP32 Dev board (only tested with ILI9341 display)
#define TFT_CS   5  // Chip select control pin
#define TFT_DC   16  // Data Command control pin
#define TFT_RST  23  // Reset pin (could connect to Arduino RESET pin)

// ##################################################################################
//
// Section 3. Define the fonts that are to be used here
//
// ##################################################################################

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

// ##################################################################################
//
// Section 4. Other options
//
// ##################################################################################

// For RP2040 processor

