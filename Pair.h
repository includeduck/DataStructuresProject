#ifndef PAIR_H
#define PAIR_H

template<typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;

    Pair() : first(), second() {}
    Pair(const T1& f, const T2& s) : first(f), second(s) {}

    bool operator<(const Pair& other) const {
        if (first == other.first) return second < other.second;
        return first < other.first;
    }

    bool operator==(const Pair& other) const {
        return first == other.first && second == other.second;
    }
};

#endif