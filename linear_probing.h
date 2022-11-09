//
// Created by bbsm2 on 11/8/2022.
//

#ifndef LINEAR_PROBING_H
#define LINEAR_PROBING_H
#include <vector>
#include <algorithm>
#include <functional>
#include <cstring>
#include "quadratic_probing.h"






// Quadratic probing implementation.
template <typename HashedObj>
class LinearHashTable {
public:
    enum EntryType {ACTIVE, EMPTY, DELETED};

    explicit LinearHashTable(size_t size = 101) : array_(NextPrime(size))
    { MakeEmpty(); }

    bool Contains(const HashedObj & x) {
        return IsActive(FindPos(x));
    }

    const int getProbes(const HashedObj & x){
        Contains(x);
        return probes;
    }

    void MakeEmpty() {
        current_size_ = 0;
        collision = 0;
        for (auto &entry : array_)
            entry.info_ = EMPTY;
    }

    bool Insert(const HashedObj & x) {
        // Insert x as active
        size_t current_pos = FindPos(x);
        if (IsActive(current_pos))
            return false;

        array_[current_pos].element_ = x;
        array_[current_pos].info_ = ACTIVE;

        // Rehash; see Section 5.5
        if (++current_size_ > array_.size() / 2)
            Rehash();
        return true;
    }

    bool Insert(HashedObj && x) {
        // Insert x as active
        size_t current_pos = FindPos(x);
        if (IsActive(current_pos))
            return false;

        array_[current_pos] = std::move(x);
        array_[current_pos].info_ = ACTIVE;

        // Rehash; see Section 5.5
        if (++current_size_ > array_.size() / 2)
            Rehash();
        return true;
    }
    const int& getCollision(){//getter for collision
        return collision;
    }
    const auto size(){
        return array_.size();
    }
    const size_t& getElements(){
        return current_size_;
    }

    bool Remove(const HashedObj & x) {
        size_t current_pos = FindPos(x);
        if (!IsActive(current_pos))
            return false;

        array_[current_pos].info_ = DELETED;
        return true;
    }

private:
    struct HashEntry {
        HashedObj element_;
        EntryType info_;

        HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
                :element_{e}, info_{i} { }

        HashEntry(HashedObj && e, EntryType i = EMPTY)
                :element_{std::move(e)}, info_{ i } {}
    };


    std::vector<HashEntry> array_;
    size_t current_size_;
    int collision;
    int probes;

    bool IsActive(size_t current_pos) const
    { return array_[current_pos].info_ == ACTIVE; }

    size_t FindPos(const HashedObj & x) {
        size_t offset = 1; //offset == 1
        size_t current_pos = InternalHash(x); //get data from hash
        probes = 1;
        while (array_[current_pos].info_ != EMPTY && array_[current_pos].element_ != x){
            probes++;
            collision++;//track collision count
            current_pos += offset;// Compute ith probe.
            offset += 1;;// square offset

            if (current_pos >= array_.size())
                current_pos -= array_.size();
        }//end while
        return current_pos;
    }


    void Rehash() {
        std::vector<HashEntry> old_array = array_;

        // Create new double-sized, empty table.
        array_.resize(NextPrime(2 * old_array.size()));
        for (auto & entry : array_)
            entry.info_ = EMPTY;

        // Copy table over.
        current_size_ = 0;
        for (auto & entry :old_array)
            if (entry.info_ == ACTIVE)
                Insert(std::move(entry.element_));
    }

    size_t InternalHash(const HashedObj & x) const {
        static std::hash<HashedObj> hf;
        return hf(x) % array_.size( );
    }
};

#endif
