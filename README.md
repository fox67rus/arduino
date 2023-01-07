# arduino

## Памятка
### Скачать и установить Arduino IDE
[Скачать](https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE)
### Настройка IDE для плат ESP8266
Для поддержки плат ESP добавить строки в пункте меню File - Preferences - 
Additional Boards Manager URLs. Адреса вводить через запятую. Нажать ОК.
```
http://arduino.esp8266.com/stable/package_esp8266com_index.json, https://dl.espressif.com/dl/package_esp32_index.json
```
В пункте меню Tools (Инструменты) -> Board (Плата) выбираем Boards manager (Менеджер плат). Ввести в поиске 
ESP8266 и нажать Install. 

Выбрать плату в настройках. NodeMCU 1.0 (ESP-12E)

