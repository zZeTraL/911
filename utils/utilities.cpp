/**
 * Re-maps a number from one range to another.
 *
 * @param x
 * @param in_min
 * @param in_max
 * @param out_min
 * @param out_max
 * @return number
 */
long mathMap(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}