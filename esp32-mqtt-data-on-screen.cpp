#include <WiFi.h>
#include <PubSubClient.h>
#include "lvgl.h"
#include "TFT_eSPI.h"

#define LVGL_TICK_PERIOD 20
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* mqtt_server = "192.168.0.12";

WiFiClient espClient;
PubSubClient client(espClient);
TFT_eSPI tft = TFT_eSPI(); 
lv_disp_draw_buf_t draw_buf;
lv_color_t buf[SCREEN_WIDTH * 10];

void lv_tick_handler(void) {
    lv_tick_inc(LVGL_TICK_PERIOD);
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    
    Serial.println("Connected to WiFi");
    
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    
    tft.begin();
    tft.setRotation(1);
    lv_init();
    
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, SCREEN_WIDTH * 10);
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_handler,
        .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    esp_timer_create(&periodic_timer_args, &periodic_timer);
    esp_timer_start_periodic(periodic_timer, LVGL_TICK_PERIOD * 1000);
}

void callback(char* topic, byte* message, unsigned int length) {
    char msg[length + 1];
    strncpy(msg, (char*)message, length);
    msg[length] = '\0';
    Serial.print("Message arrived: ");
    Serial.println(msg);

    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, msg);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32Client")) {
            Serial.println("connected");
            client.subscribe("your_topic");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    lv_task_handler();
    delay(5);
}

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    tft.pushColors(&color_p->full, (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1), true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}