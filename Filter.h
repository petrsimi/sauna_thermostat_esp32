#pragma once

#include <Arduino.h>

class Filter {
    public:
        Filter(uint8_t max_items);

        void clear();

        uint32_t add(uint32_t value);
 
        uint32_t getAvg();

        bool isFull();

        uint8_t count();
 
    private:
        uint8_t max_items;
        uint8_t items;
 
        uint32_t sum;
};