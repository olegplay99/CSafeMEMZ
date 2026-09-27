# CSafeMEMZ 

Большая безопасная визуальная симуляция MEMZ-подобного хаоса на C/WinAPI/GDI.

## Что здесь есть

- Рендер непосредственно в desktop DC, без полноэкранного overlay-окна.
- 20+ процедурных эффектов GDI/BitBlt/StretchBlt.
- Сценарный таймлайн примерно на 105 секунд.
- Фейковые Блокнот, Chrome, CMD, Проводник, error/crash-окна.
- Окна появляются по ходу процесса, живут ограниченное время и в финале начинают двигаться хаотичнее.
- Финальный каскад эффектов.
- ESC — аварийный выход.

## Безопасность

Это именно визуальная симуляция.

Программа НЕ:

- удаляет файлы;
- форматирует диски;
- меняет MBR/bootloader;
- меняет реестр для автозапуска;
- устанавливает persistence;
- запускает реальные системные ошибки;
- шифрует или повреждает данные.

Фейковые окна являются окнами самой программы и не запускают Notepad/Chrome/CMD/Explorer.

## Сборка MinGW-w64

Откройте `build.bat` в Windows с установленным MinGW-w64.

После успешной сборки:

`build\\CSafeMEMZ.exe`

## Visual Studio / CMake

Можно открыть `CSafeMEMZ.sln` в Visual Studio 2022 и собрать конфигурацию `Release | x64`.

Также можно открыть проект через CMake:

`cmake -S . -B build`

`cmake --build build --config Release`

## Управление

- ESC — немедленно завершить симуляцию.

## Структура

```text
CSafeMEMZ/
├── CMakeLists.txt
├── build.bat
├── run.bat
├── README.md
├── include/
│   └── config.h
└── src/
    ├── main.c
    ├── core/
    │   ├── engine.c
    │   ├── engine.h
    │   ├── event.c
    │   ├── event.h
    │   ├── timeline.c
    │   └── timeline.h
    ├── effects/
    │   ├── effects.c
    │   └── effects.h
    └── ui/
        ├── fake_windows.c
        ├── fake_windows.h
        ├── intro.c
        ├── intro.h
        ├── outro.c
        └── outro.h
```
