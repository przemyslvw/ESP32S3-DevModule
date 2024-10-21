#include <TFT_eSPI.h>  // TFT library
#include <SPI.h>
#include <SD.h>

// TFT screen setup
TFT_eSPI tft = TFT_eSPI();
#define TFT_WIDTH 240
#define TFT_HEIGHT 320

// SD card setup
#define SD_CS 5  // Pin for SD card Chip Select

// Touch screen variables
#define TOUCH_THRESHOLD 40

// Form data
String name = "";
String age = "";
int selectedGender = -1; // 0 - male, 1 - female

bool keyboardActive = false;
String *currentInputField = nullptr;

// Keyboard setup
const char* keyboardRows[] = {"123", "456", "789", "ABC"};
String currentInput = "";

void setup() {
  Serial.begin(115200);

  // TFT Screen initialization
  tft.init();
  tft.setRotation(1); // Landscape
  tft.fillScreen(TFT_BLACK);

  // SD card initialization
  if (!SD.begin(SD_CS)) {
    Serial.println("SD Card Mount Failed");
    return;
  }
  Serial.println("SD Card initialized.");

  // Draw form UI
  drawForm();
}

void loop() {
  // Add touch handling here for interaction
}

// Function to draw the form UI
void drawForm() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  
  // Draw "Name" input field
  tft.setCursor(10, 20);
  tft.setTextSize(2);
  tft.print("Name:");
  tft.fillRect(10, 40, 220, 30, TFT_WHITE);
  tft.setCursor(15, 45);
  tft.setTextColor(TFT_BLACK);
  tft.print(name);

  // Draw "Age" input field
  tft.setCursor(10, 80);
  tft.setTextColor(TFT_WHITE);
  tft.print("Age:");
  tft.fillRect(10, 100, 220, 30, TFT_WHITE);
  tft.setCursor(15, 105);
  tft.setTextColor(TFT_BLACK);
  tft.print(age);

  // Draw radio buttons for gender
  tft.setCursor(10, 140);
  tft.setTextColor(TFT_WHITE);
  tft.print("Gender:");
  
  // Male radio button
  tft.fillRect(10, 160, 20, 20, TFT_WHITE);
  tft.setCursor(35, 165);
  tft.setTextColor(TFT_WHITE);
  tft.print("Male");
  
  // Female radio button
  tft.fillRect(100, 160, 20, 20, TFT_WHITE);
  tft.setCursor(125, 165);
  tft.setTextColor(TFT_WHITE);
  tft.print("Female");

  // Draw "Save" button
  tft.fillRect(10, 250, 220, 40, TFT_GREEN);
  tft.setCursor(60, 260);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.print("Save");
}

// Function to draw on-screen keyboard
void drawKeyboard() {
  keyboardActive = true;
  tft.fillRect(0, TFT_HEIGHT - 100, TFT_WIDTH, 100, TFT_DARKGREY); // Keyboard background
  
  for (int row = 0; row < 4; row++) {
    tft.setCursor(10, 220 + row * 25);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.print(keyboardRows[row]);
  }
}

// Function to save form data to SD card
void saveToSD() {
  File file = SD.open("/form_data.txt", FILE_WRITE);

  if (file) {
    file.println("Form Data:");
    file.print("Name: ");
    file.println(name);
    file.print("Age: ");
    file.println(age);
    file.print("Gender: ");
    if (selectedGender == 0) file.println("Male");
    else if (selectedGender == 1) file.println("Female");
    else file.println("Unknown");
    
    file.close();
    Serial.println("Data saved to SD card.");
  } else {
    Serial.println("Error opening file for writing.");
  }
}

// Function to handle touch events (placeholder)
void handleTouch(int x, int y) {
  // Check if user tapped on the "Name" field
  if (x > 10 && x < 230 && y > 40 && y < 70) {
    currentInputField = &name;
    drawKeyboard();
  }
  
  // Check if user tapped on the "Age" field
  if (x > 10 && x < 230 && y > 100 && y < 130) {
    currentInputField = &age;
    drawKeyboard();
  }
  
  // Check if user tapped on gender radio buttons
  if (x > 10 && x < 30 && y > 160 && y < 180) {
    selectedGender = 0; // Male selected
    drawForm();
  }
  
  if (x > 100 && x < 120 && y > 160 && y < 180) {
    selectedGender = 1; // Female selected
    drawForm();
  }
  
  // Check if user tapped on the "Save" button
  if (x > 10 && x < 230 && y > 250 && y < 290) {
    saveToSD();
  }
}
