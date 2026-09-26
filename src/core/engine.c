#include "engine.h"

#include <windows.h>

/* Состояние движка */
typedef struct
{
    BOOL running;
    DWORD startTime;
    DWORD elapsed;
} EngineState;

/*
 * Инициализация движка.
 */
static void EngineInit(EngineState* state)
{
    state->running = TRUE;
    state->startTime = GetTickCount();
    state->elapsed = 0;
}

/*
 * Обновление времени.
 */
static void EngineUpdate(EngineState* state)
{
    DWORD now = GetTickCount();

    state->elapsed = now - state->startTime;
}

/*
 * Здесь позже появится система событий.
 */
static void EngineProcessEvents(
    EngineState* state
)
{
    /*
     * Пока движок просто работает.
     *
     * Позже здесь будет примерно:
     *
     * 0s   -> первое событие
     * 5s   -> второе событие
     * 12s  -> третье событие
     * ...
     */
    (void)state;
}

/*
 * Главный цикл.
 */
BOOL EngineRun(HINSTANCE hInstance)
{
    (void)hInstance;

    EngineState state;

    EngineInit(&state);

    while (state.running)
    {
        EngineUpdate(&state);

        EngineProcessEvents(&state);

        /*
         * Пока это просто тестовый цикл.
         * Позже здесь будет отрисовка и обработка
         * событий Windows.
         */
        if (state.elapsed >= 3000)
        {
            state.running = FALSE;
        }

        Sleep(16);
    }

    return TRUE;
}