#include "Filter.h"

Filter::Filter(uint8_t max_items)
    : max_items(max_items)
{
    clear();
}

void Filter::clear() {
    items = 0;
    sum = 0;
};

uint32_t Filter::add(uint32_t value) {
    if (items < max_items) {
        sum += value;
        items++;
    } else {
        sum = sum - (sum / max_items) + value;
    }

    return sum / items;
}

uint32_t Filter::getAvg() {
    return sum / items;
}

bool Filter::isFull() {
    return items == max_items;
}

uint8_t Filter::count() {
    return items;
}

