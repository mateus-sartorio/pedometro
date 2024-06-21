#include "../include/button.h"

#include "../include/gpio.h"

#define BUTTON_INT_LEVEL 3

typedef struct {
    uint32_t mask;
    uint32_t counter;
    uint32_t status;
} Button_t;

static Button_t button_table[] = {
    {BUTTON1, 0, 0},
    {BUTTON2, 0, 0},
    {0, 0, 0},
};

static uint32_t pressed = 0;
static uint32_t released = 0;
static uint32_t changed = 0;
static void (*callback)(uint32_t param) = NULL;

void Button_Processing(void) {
    uint32_t button = GPIOB->DIN;

    Button_t* p = button_table;
    while(p->mask) {
        switch(p->status) {
            case 0:
                if(button & p->mask) {
                    if(p->counter == 10) {
                        p->status = 1;
                        pressed |= p->mask;
                        released &= ~p->mask;
                        changed |= p->mask;
                    } else {
                        p->counter++;
                        changed &= ~p->mask;
                    }
                } else {
                    p->counter = 0;
                    changed &= ~p->mask;
                }
            case 1:
                if(button & p->mask) {
                    p->counter = 0;
                    changed &= ~p->mask;
                } else {
                    if(p->counter == 10) {
                        p->status = 0;
                        pressed &= ~p->mask;
                        released |= p->mask;
                        changed |= p->mask;
                    } else {
                        p->counter++;
                    }
                }
            default:
                break;
        }

        p++;
    }

    changed = pressed | released;

    if(changed) {
        callback(changed);
    }
}

uint32_t Button_Status(void) {
    uint32_t button = 0;

    Button_t* p = button_table;
    while(p->mask) {
        if(p->status) {
            button |= p->mask;
        }

        p++;
    }

    return button;
}

void Button_Init(uint32_t buttons) {
    if(buttons & BUTTON1) {
        GPIOB->MODEH &= ~_GPIO_P_MODEL_MODE1_MASK;
        GPIOB->MODEH |= GPIO_P_MODEL_MODE1_INPUT;
    }

    if(buttons & BUTTON2) {
        GPIOB->MODEH &= ~_GPIO_P_MODEL_MODE2_MASK;
        GPIOB->MODEH |= GPIO_P_MODEL_MODE2_INPUT;
    }
}

uint32_t Button_Read(void) { return Button_Status(); }

uint32_t Button_ReadChanges(void) {
    uint32_t c = changed;
    changed = 0;
    return c;
}

uint32_t Button_ReadPressed(void) {
    uint32_t p = pressed;
    pressed = 0;
    return p;
}

uint32_t Button_ReadReleased(void) {
    uint32_t r = released;
    released = 0;
    return r;
}

void Button_SetCallback(void (*proc)(uint32_t param)) { callback = proc; }