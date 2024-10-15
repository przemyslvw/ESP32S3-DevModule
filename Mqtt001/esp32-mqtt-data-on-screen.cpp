#include <WiFi.h>
#include <PubSubClient.h>
#include "lvgl.h"
#include "TFT_eSPI.h"

#define LVGL_TICK_PERIOD 20
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Dane logowania do WiFi
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
// Adres IP brokera MQTT
const char* mqtt_server = "192.168.0.12";

WiFiClient espClient;
PubSubClient client(espClient);
TFT_eSPI tft = TFT_eSPI(); 
lv_disp_draw_buf_t draw_buf;
lv_color_t buf[SCREEN_WIDTH * 10];

// Funkcja obsługująca tick LVGL
void lv_tick_handler(void) {
    lv_tick_inc(LVGL_TICK_PERIOD);
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    
    // Czekanie na połączenie z WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Łączenie z WiFi...");
    }
    
    Serial.println("Połączono z WiFi");
    
    client.setServer(mqtt_server, 1883); // Ustawienie brokera MQTT
    client.setCallback(callback); // Ustawienie funkcji callback do wiadomości MQTT
    
    tft.begin(); // Inicjalizacja wyświetlacza TFT
    tft.setRotation(1); // Ustawienie rotacji wyświetlacza
    lv_init(); // Inicjalizacja biblioteki LVGL
    
    // Inicjalizacja bufora wyświetlacza dla LVGL
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, SCREEN_WIDTH * 10);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    
    // Ustawienie okresowego timera dla tick LVGL
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_handler,
        .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    esp_timer_create(&periodic_timer_args, &periodic_timer);
    esp_timer_start_periodic(periodic_timer, LVGL_TICK_PERIOD * 1000);
}

// Funkcja callback obsługująca przychodzące wiadomości MQTT
void callback(char* topic, byte* message, unsigned int length) {
    char msg[length + 1];
    strncpy(msg, (char*)message, length);
    msg[length] = '\0'; // Dodanie zakończenia null do wiadomości
    Serial.print("Odebrano wiadomość: ");
    Serial.println(msg);

    // Utworzenie etykiety i ustawienie jej tekstu na odebraną wiadomość
    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, msg);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

// Funkcja do ponownego połączenia z brokerem MQTT, jeśli połączenie zostało zerwane
void reconnect() {
    while (!client.connected()) {
        Serial.print("Próba połączenia z MQTT...");
        if (client.connect("ESP32Client")) {
            Serial.println("połączono");
            client.subscribe("your_topic"); // Subskrybowanie tematu
        } else {
            Serial.print("nieudane, rc=");
            Serial.print(client.state());
            Serial.println(" ponowna próba za 5 sekund");
            delay(5000); // Czekanie 5 sekund przed ponowną próbą
        }
    }
}

void loop() {
    if (!client.connected()) {
        reconnect(); // Ponowne połączenie z brokerem MQTT, jeśli połączenie zostało zerwane
    }
    client.loop(); // Przetwarzanie przychodzących wiadomości MQTT
    lv_task_handler(); // Obsługa zadań LVGL
    delay(5); // Krótkie opóźnienie, aby umożliwić LVGL przetwarzanie zadań
}

// Funkcja flush do aktualizowania wyświetlacza za pomocą LVGL
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    tft.pushColors(&color_p->full, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();
    lv_disp_flush_ready(disp); // Wskazanie, że flush jest zakończone
}