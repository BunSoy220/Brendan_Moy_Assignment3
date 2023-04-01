#ifndef DOUBLE_HASHING_H
#define DOUBLE_HASHING_H

#include <vector>
#include <algorithm>
#include <functional>


namespace {

//Internal method to test if a positive number is prime.
bool DoubleIsPrime(size_t n) {
  if( n == 2 || n == 3 )
    return true;
  
  if( n == 1 || n % 2 == 0 )
    return false;
  
  for( int i = 3; i * i <= n; i += 2 )
    if( n % i == 0 )
      return false;
  
  return true;
}


// Internal method to return a prime number at least as large as n.
int DoubleNextPrime(size_t n) {
  if (n % 2 == 0)
    ++n;  
  while (!DoubleIsPrime(n)) n += 2;  
  return n;
}

}  // namespace


// Quadratic probing implementation.
template <typename HashedObj>
class HashTableDouble {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTableDouble(size_t size = 101) : array_(DoubleNextPrime(size))
    { MakeEmpty(); }
  
  bool Contains(const HashedObj & x) {
    return IsActive(FindPos(x));
  }
  
  void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
    collisions_ = 0;
  }

  bool Insert(const HashedObj & x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;
    
    // Rehash; see Section 5.5, if table is half full resize
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

    // Rehash; see Section 5.5 if hash table is half full resize
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    //--current_size_;
    return true;
  }

  size_t Elements(){
      return current_size_;
  }

  size_t Size(){
    return array_.size();
  }

  float LoadFactor(){
    return float(current_size_)/float(array_.size());
  }

  int Collisions(){
    return collisions_;
  }

  void ResetCollisions(){
    collisions_ = 0;
  }


  int GetProbes(){
    return probes_;
  }

  void ResetProbes(){
    probes_ = 0;
  }


  void SetR(int r){
    r_ = r;
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
  int collisions_;
  int r_;
  int probes_;
  //check if index is taken
  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }
  
  //find open new position or new position for x
  size_t FindPos(const HashedObj & x){
    size_t offset = 1;
    size_t current_pos = InternalHash(x);
    ++probes_;
    while (array_[current_pos].info_ != EMPTY && array_[current_pos].element_ != x) { 
      // current_pos += offset*DoubleHash(x);  // Compute ith probe.
      // ++offset;
      // ++collisions_;
      // ++probes_;
      // current_pos = current_pos%array_.size();

      //linear
      current_pos += offset;  // Compute ith probe.
      offset += 1;
      ++collisions_;
      ++probes_;
      current_pos = current_pos%array_.size();
    }
    return current_pos;
  }

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(DoubleNextPrime(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	  Insert(std::move(entry.element_));
  }
  //second function R - (x%R)
  size_t DoubleHash(const HashedObj & x){
    return size_t(r_-(int(InternalHash(x))%r_));
  }

  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size(); //return hash results
  }
};

#endif  // DOUBLE_HASHING_H