#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char * apSsid = "Smart Config NBY";
const char * apPassword = "12345678";

AsyncWebServer server(80);

const String html = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><script src=\"https:\/\/cdn.tailwindcss.com\"><\/script><title>Smart Config<\/title><\/head><body><div class=\"w-full h-screen py-20 md:py-40 p-2\" style=\"background-image:url(https:\/\/www.linkpicture.com\/q\/sc_1.webp);background-size:cover\"><div class=\"container max-w-lg mx-auto p-4 bg-sky-100 bg-opacity-30 rounded-lg shadow-lg\"><h1 class=\"text text-2xl text-center text-sky-500 font-bold tracking-widest\">SMART CONFIG<\/h1><div class=\"my-8\"><\/div><form method=\"post\" action=\"\/smart-config\"><div class=\"flex flex-col\"><label for=\"ssid-input\" class=\"text text-lg text-sky-500\">SSID<\/label><input type=\"text\" name=\"ssid\" id=\"ssid-input\" class=\"shadow-lg outline-none rounded-lg p-2 focus:ring-1 ring-sky-500 placeholder:text-sky-300\" placeholder=\"Enter SSID\"><\/div><div class=\"my-2\"><\/div><div class=\"flex flex-col\"><label for=\"password-input\" class=\"text text-lg text-sky-500\">Password<\/label><input type=\"password\" name=\"password\" id=\"password-input\" class=\"shadow-lg outline-none rounded-lg p-2 focus:ring-1 ring-sky-500 placeholder:text-sky-300\" placeholder=\"Enter Password\"><\/div><div class=\"my-6\"><\/div><div class=\"flex justify-center w-full\"><button type=\"submit\" class=\"shadow-lg bg-sky-500 text-white rounded-lg p-2 hover:bg-sky-600 duration-100 focus:scale-110\">SUBMIT<\/button><\/div><\/form><\/div><\/div><\/body><\/html>";

void setup() {
    Serial.begin(115200);

    WiFi.softAP(apSsid, apPassword);
    Serial.print("AP started with IP: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
        request -> send(200, "text/plain", "Hello, world");
    });

    server.on("/smart-config", HTTP_GET, [](AsyncWebServerRequest * request) {
        request -> send(200, "text/html", html);
    });

    server.on("/smart-config", HTTP_POST, [](AsyncWebServerRequest * request) {
        Serial.println("POST");
        Serial.println(request -> arg("ssid"));
        Serial.println(request -> arg("password"));

        WiFi.begin(request -> arg("ssid").c_str(), request -> arg("password").c_str());
        Serial.print("\nConnecting to WiFi..");
        unsigned long start = millis();
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
            if (millis() - start > 10000) {
                Serial.println("Can not connect to the WiFi network");
                request -> send(200, "text/html", "Can not connect to the WiFi network");
                break;
            }
        }
        Serial.print("\n\nConnected to the WiFi network with local IP: ");
        Serial.println(WiFi.localIP());

        request -> send(200, "text/html", "Connected to the WiFi network with local IP: " + WiFi.localIP().toString());
    });

    // Start server
    server.begin();
    Serial.println("Server started");
}

void loop() {}