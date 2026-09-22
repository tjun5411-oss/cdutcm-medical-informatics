#include "key.h"
#include "delay.h"

Key key_scan() {
    static pdata uint8_t switch_cnt;
    static pdata uint8_t switch_cntSize;
    static bit switch_longPress;
    static bit switch_first;
    static pdata uint8_t pre_cnt;
    static pdata uint8_t pre_cntSize;
    static bit pre_longPress;
    static bit pre_first;
    static pdata uint8_t nex_cnt;
    static pdata uint8_t nex_cntSize;
    static bit nex_longPress;
    static bit nex_first;
    static pdata uint8_t enter_cnt;
    static pdata uint8_t enter_cntSize;
    static bit enter_longPress;
    static bit enter_first;
    static Key key;
    key = NoKey;
    if (key_Switch == 0 || key_Pre == 0 || key_Nex == 0 || key_Enter == 0) {
        delay_ms(5);
        if (key_Switch == 0) {
            if (switch_longPress == 0) {
                key |= Switch;
                switch_longPress = 1;
                switch_first = 1;
            }
            else {
                switch_cntSize = switch_first ? FIRST_CNT : PERIOD_CNT;
                if (++switch_cnt == switch_cntSize) {
                    switch_first = 0;
                    switch_cnt = 0;
                    key |= Switch;
                }
            }
        }
        else {
            switch_cnt = 0;
            switch_longPress = 0;
            switch_first = 1;
        }
        if (key_Pre == 0) {
            if (pre_longPress == 0) {
                key |= Pre;
                pre_longPress = 1;
                pre_first = 1;
            }
            else {
                pre_cntSize = pre_first ? FIRST_CNT : PERIOD_CNT;
                if (++pre_cnt == pre_cntSize) {
                    pre_first = 0;
                    pre_cnt = 0;
                    key |= Pre;
                }
            }
        }
        else {
            pre_cnt = 0;
            pre_longPress = 0;
            pre_first = 1;
        }
        if (key_Nex == 0) {
            if (nex_longPress == 0) {
                key |= Nex;
                nex_longPress = 1;
                nex_first = 1;
            }
            else {
                nex_cntSize = nex_first ? FIRST_CNT : PERIOD_CNT;
                if (++nex_cnt == nex_cntSize) {
                    nex_first = 0;
                    nex_cnt = 0;
                    key |= Nex;
                }
            }
        }
        else {
            nex_cnt = 0;
            nex_longPress = 0;
            nex_first = 1;
        }
        if (key_Enter == 0) {
            if (enter_longPress == 0) {
                key |= Enter;
                enter_longPress = 1;
                enter_first = 1;
            }
            else {
                enter_cntSize = enter_first ? FIRST_CNT : PERIOD_CNT;
                if (++enter_cnt == enter_cntSize) {
                    enter_first = 0;
                    enter_cnt = 0;
                    key |= Enter;
                }
            }
        }
        else {
            enter_cnt = 0;
            enter_longPress = 0;
            enter_first = 1;
        }
    }
    return key;
}
