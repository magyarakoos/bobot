#include "proximity_sensor.h"

ProximitySensor::ProximitySensor(uint pin, void (*fn)()) : pin(pin), handler(fn) {}

ProximitySensor::~ProximitySensor() {}
