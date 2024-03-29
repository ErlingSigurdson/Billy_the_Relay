![Good old ESP32](https://i.imgur.com/U5nZ8fW.png)

[![github-en](https://img.shields.io/badge/github-en-blue)](https://github.com/ErlingSigurdson/Billy_the_Relay/blob/main/README.md)
[![github-ru](https://img.shields.io/badge/github-ru-blue)](https://github.com/ErlingSigurdson/Billy_the_Relay/blob/main/README.ru.md)
[![gitlfic-en](https://img.shields.io/badge/gitflic-en-red)](https://gitflic.ru/project/efimov-d-v/billy_the_relay/blob?file=README.md&branch=main)
[![gitflic-ru](https://img.shields.io/badge/gitflic-ru-red)](https://gitflic.ru/project/efimov-d-v/billy_the_relay/blob?file=README.ru.md&branch=main)

## Новости

**14.01.2024** - Запустил скетч на модуле ESP32-C6 с помощью [альфа-версии 3.0.0-alpha3 ядра Ардуино для ESP32](https://github.com/espressif/arduino-esp32/milestone/4).
Работает хорошо.

**18.03.2024** - Релиз версии 1.1.

# Концепция
Релешка Билли - это Arduino-проект на базе модуля ESP32 или ESP8266. Билли умеет управлять простой (ВКЛ./ВЫКЛ.) нагрузкой, например, светодиодом или (с помощью драйвера) реле.
В свою очередь Билли можно отдавать команды:
- По UART через кабель.
- По Wi-Fi как локальному TCP-серверу, например, с помощью классической Unix-утилиты `netcat` (`nc`) или приложения для Android [Serial Wi-Fi terminal](https://serial-wifi-terminal.en.softonic.com/android).   
- По Wi-Fi как локальному HTTP-серверу, например, с помощью браузера (скетч предоставляет минималистичный веб-интерфейс) или иного приложения для отправки HTTP-запросов.
- По Bluetooth Classic как ведомому устройству[^1], например, с помощью Bluetooth-терминала для рабочего стола или приложения для Android [Serial Bluetooth Controller](https://bluetooth-serial-controller.en.softonic.com/android).
- По Wi-Fi как TCP-клиенту (в том числе через Интернет). Для этого Билли направляет запросы специально настроенному TCP-серверу и в ответ получает от него команды (этот способ более детально описан ниже).

Билли предназначен для работы в локальной сети Wi-Fi в режиме клиента (station, STA). Доступ в Интернет Билли получает через точку доступа, к которой он подключен в рамках сети Wi-Fi.

# Инструкция
### Быстрый старт
Выполните следующие шаги, чтобы настроить и начать использовать Билли:
1. В файле `config_general.h` с помощью директив `#define` определите:
- будет ли использоваться Bluetooth Classic (`BT_CLASSIC_PROVIDED`). Закомментируйте директиву, если ваш модуль ESP32 не поддерживает Bluetooth Classic или вы просто не планируете его использовать. Для ESP8266 игнорируйте;
- пин управления нагрузкой (`LOAD_PIN`). Укажите номер пина в значении директивы;
- пин управления индикаторным светодиодом (`WIFI_INDICATOR_LED_PIN`). Укажите номер пина в значении директивы;
- режим работы нагрузки (`INVERTED_OUTPUT`). Раскомментируйте директиву, если вам нужен инвертированный выход.
2. Убедитесь, что в вашу Arduino IDE (или в SDK Arduino в другой IDE) добавлено соответствующее "ядро" для [ESP32](https://github.com/espressif/arduino-esp32) или [ESP8266](https://github.com/esp8266/Arduino) от компании Espressif Systems.
3. Скомпилируйте скетч и загрузите его в устройство.[^2]
4. Включите устройство и подключитесь к нему по UART через кабель (через USB-UART преобразователь или, если это поддерживается вашим устройством, через UART поверх нативного USB).
5. Дважды введите команду `AT+SETLOAD=TOGGLE` и удостоверьтесь, что Билли переключает текущее состояние нагрузки.
6. С помощью команды `AT+SETLOCALSSID=<значение>` укажите SSID точки доступа вашей сети Wi-Fi.
7. С помощью команды `AT+SETLOCALPSWD=<значение>` укажите пароль точки доступа вашей сети Wi-Fi.
8. С помощью команды `AT+SETLOCALPORT=<значение>` укажите номер порта, который будет использоваться вашим устройством как TCP-сервером для локального соединения.
9. Перезагрузите устройство или введите команду `AT+RSTLOCALCONN`. Удостоверьтесь, что устройство подключилось к сети Wi-Fi (следите за терминалом UART и за индикаторным светодиодом). Запомните выведенный IP-адрес (для его повторного вывода введите команду `AT+PRINTLOCALIP`).
10. Подключитесь к устройству через Wi-Fi, используя ранее присвоенный ему IP-адрес и введённый вами порт.
11. Попробуйте передавать устройству команды (например, `AT+SETLOAD=TOGGLE`) через TCP-соединение. Удостоверьтесь, что Билли вас понимает и слушается.
12. Откройте любой браузер (по моему опыту Firefox менее капризный) на другом устройстве, подключенном к той же сети Wi-Fi, и введите в адресную строку ранее присвоенный локальный IP устройства.
13. С помощью веб-интерфейса попробуйте включить и выключить нагрузку.

Опционально, если ваше устройство поддерживает Bluetooth Classic:

14. С помощью команды `AT+SETBTDEVNAME=<значение>` укажите имя устройства как ведомого устройства Bluetooth.
15. С помощью команды `AT+SETBT=ON` включите функционал Bluetooth.
16. Подключитесь к Билли через Bluetooth, используя ранее введённое вами имя ведомого устройства.
17. Попробуйте передавать устройству команды (например, `AT+SETLOAD=TOGGLE`) через Bluetooth-соединение. Удостоверьтесь, что Билли вас понимает и слушается.

### Полный список команд
см. файл `config_ASCII_cmd_check.h`.

### Билли как TCP-клиент и управление через Интернет
TCP-сервер, к которому обращается Билли (в том числе через Интернет), должен уметь:
- в ответ на направляемый Билли запрос (например, строку `"UPD_REQ"`) направлять ему сообщение (тоже строку), содержащее одну из валидных команд.
- изменять сообщение, заготовленное в качестве ответа на запрос Билли, согласно передаваемым самому серверу дистанционным командам.

Например, сервер, получив с вашей рабочей станции дистанционную команду "включить нагрузку", готовится в ответ на следующий запрос Билли отправить ему команду `AT+SETLOAD=ON`. Получив команду "выключить нагрузку", он заготавливает уже другую команду - `AT+SETLOAD=OFF`. Билли в роли TCP-клиента принимает команду и обрабатывает её так же, как полученную по любому другому протоколу.

Чтобы задать для Билли настройки обращения к удалённому серверу, используйте команды `AT+SETIOTIP=<значение>`, `AT+SETIOTPORT=<значение>`, `AT+SETIOTREQMSG=<значение>`, `AT+SETIOTREQPERIOD=<значение>`, `AT+SETIOT=ON` и `AT+SETIOT=OFF`.

[Здесь вы можете найти готовый код работающего таким образом сервера для Linux](https://github.com/ErlingSigurdson/server0451/tree/main), написанный на языке С. Он рассчитан как раз на взаимодействие с Билли и ему подобными устройствами.

# Общие примечания к коду
### Разделение проекта на файлы
Разделение исходного кода самого скетча на основной файл `.ino` и локальные файлы `.h` и `.cpp` не очень популярно (чаще используется вариант с несколькими файлами `.ino`, которые перед компиляцией конкатенируются средой разработки, либо с упаковкой в библиотеки), но я нахожу его удобным, поскольку он позволяет определять порядок включений и делает структуру кода более прозрачной.

### Глобальные переменные
Дополнительные глобальные переменные нарочито не вводятся, поскольку "общим пространством" для различных функций скетча служит встроенный накопитель устройства. Исключение - некоторые флаги и экземпляры классов (таких как `WiFiServer`, `BluetoothSerial` и т. д.).

### Модули и функции-врапперы
Вызовы методов в основном обёрнуты в функции-врапперы, объявленные в локальных модулях (файлах `.h` и `.cpp`). Это удобно, поскольку каждому вызову метода, как правило, сопутствуют связанные с ним дополнительные строки кода.
Модули не ссылаются друг на друга, вместо этого относящиеся к ним функции-врапперы комбинируются в файле `.ino`, благодаря чему упрощается допиливание скетча под конкретные нужды.

# Справедливые вопросы
### Почему не MQTT, ведь он отлично подходит для "Интернета вещей"?
Хотелось обеспечить больше гибкости и не хотелось привязываться к определённому протоколу 7-го уровня модели OSI. Из "голых" TCP-сообщений опытный пользователь может слепить практически что угодно, например, HTTP-запросы.

### Почему "Билли"?
В честь попугая-программиста, героя [мемных видео](https://www.youtube.com/watch?v=0MhVkKHYUAY&list=PLkdGijFCNuVmu35l6EJxdvsvf7xj4EQVf&index=21) от [Mr. P Solver](https://www.youtube.com/c/mrpsolver).

### Что с безопасностью?
В рамках вашей домашней сети Wi-Fi ваша лучшая защита - это пароль точки доступа. Вы можете даже развернуть на вашем роутере дополнительную сеть Wi-Fi, которая будет использоваться только для Билли и прочих устройств IoT.
Что касается Bluetooth, вы можете изменить синтаксис команд в `config_ASCII_cmd_handler.h`, чтобы рабочие команды имели вид типа `AT+` `MYPSWD_SETLOAD=TOGGLE`.

[^1]: Не работает для ESP8266 и чипов ESP32 без поддержки Bluetooth Classic (например, ESP32-S2 и ESP32-C3).
[^2]: Если среда разработки сообщает о недостатке у ESP32 памяти для загрузки скетча, попробуйте изменить опцию `partition scheme` на "Huge APP".
