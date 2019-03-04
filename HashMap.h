#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <list>

namespace aisdi
{
template <typename KeyType, typename ValueType>
class HashMap
{
public:

  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;


  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
  static const size_type SIZE = 100;
  std::list<value_type>* HashTable;

void init()
{
 HashTable = new std::list<value_type>[SIZE]; 
} 

size_type getHash(const key_type& key) const
  {
    return key%SIZE;
  }
    
 public:
  HashMap() 
  {
    init();
  }
  
  HashMap(std::initializer_list<value_type> list)
  {

    init();

    for (auto it=list.begin(); it != list.end(); ++it)
      HashTable[getHash(it->first)].push_back(*it);
  }

  HashMap(const HashMap& other)
  {
    init();

    for(size_type i=0; i<SIZE; ++i)
      HashTable[i].clear();

    for (auto it=other.begin(); it != other.end(); ++it)
        HashTable[getHash(it->first)].push_back(*it);
  }

  HashMap(HashMap&& other) noexcept
  {
    init();
    for (size_type i = 0; i < SIZE;i ++)
      {
        HashTable[i] = std::move(other.HashTable[i]);
        other.HashTable[i].clear();
      }
  }

  ~HashMap()
  {
    for(size_type i=0; i<SIZE; ++i)
      HashTable[i].clear();
      
    delete[] HashTable;
  }

  HashMap& operator=(const HashMap& other)
  {
    if(this == &other)
      return *this;

      for(size_type i=0; i<SIZE; ++i)
         HashTable[i].clear();

      delete[] HashTable;

      init();
      
      for (auto it=other.begin(); it != other.end(); ++it)
        HashTable[getHash(it->first)].push_back(*it);
    
    return *this;
  }

  HashMap& operator=(HashMap&& other) noexcept
  {
    if (*this == other)
    {
      for (size_type i = 0; i < SIZE; i++)  //if its the same clear other and return
        other.HashTable[i].clear();
      return *this;
    }
   for (size_type i = 0; i < SIZE;i ++)
      {
        HashTable[i] = std::move(other.HashTable[i]); //if its not move from other and clear the other one
        other.HashTable[i].clear();
      }
    return *this;

  }

  bool isEmpty() const
  {
    for (size_type i=0; i<SIZE; i++)
      if (!HashTable[i].empty())
        return false;
   
    return true;
  }

  mapped_type& operator[](const key_type& key)
  {
    size_type Ind = getHash(key);
    for (auto it=HashTable[Ind].begin(); it!=HashTable[Ind].end(); ++it)  //search in the given table for the key
      if (it->first==key)
        return it->second;
    //if not found then return created one
    value_type newVal = std::make_pair(key,mapped_type{});
    HashTable[Ind].push_back(newVal);

    auto it = HashTable[Ind].end();

    return (--it)->second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    size_type Ind = getHash(key);
    for (auto it=HashTable[Ind].begin(); it!=HashTable[Ind].end(); ++it)
      if (it->first==key)
        return it->second;
    throw std::out_of_range("not found in valueof const");
  }

  mapped_type& valueOf(const key_type& key)
  {
    size_type Ind = getHash(key);
    for (auto it=HashTable[Ind].begin(); it!=HashTable[Ind].end(); ++it)
      if (it->first==key)
        return it->second;
    throw std::out_of_range("not found in value of");
  }


  const_iterator find(const key_type& key) const
  {
    size_type Ind = getHash(key);

    for (auto it=HashTable[Ind].begin(); it!=HashTable[Ind].end(); ++it)
      if (it->first==key)
        return ConstIterator(this, it, Ind);
        
    return end();
  }

  iterator find(const key_type& key)
  {
    size_type Ind = getHash(key);
    for (auto it=HashTable[Ind].begin(); it!=HashTable[Ind].end(); ++it)
      if (it->first==key)
        return Iterator(this, it, Ind);
    return end();
  }

  void remove(const key_type& key)
  {
    if (isEmpty())
      throw std::out_of_range("cant remove from empty map");
    size_type Ind = getHash(key);
    for (auto it=HashTable[Ind].begin(); it!=HashTable[Ind].end(); ++it)
      if (it->first==key)
      {
        HashTable[Ind].erase(it); //using list erase  to erase an element in the given Index
        return;
      }
    throw std::out_of_range("couldnt find given  key");
  }

  void remove(const const_iterator& it)
  {
    if (it==end())
      throw std::out_of_range("Cant remove the end");
    if(HashTable[it.index].empty())
      return;
    HashTable[(it.index)].erase(it.iter);
  }

  size_type getSize() const
  {
    size_type count=0;

    for (size_type i=0; i<SIZE; ++i)  //simply sum counts from lists
      count = count + HashTable[i].size();
    
    return count;
  }

  bool operator==(const HashMap& other) const
  {
    for(auto it1 = begin(), it2 = other.begin() ; it1!=end() && it2!=other.end(); ++it1, ++it2) 
    {
      if(*it1 != *it2)  //if at least one is different than another return false
        return false;
    }

    return true;
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    for(size_type i=0; i<SIZE; ++i)
      if(!HashTable[i].empty()) //search for the first non-empty list and give the first element of it
        return Iterator(this,HashTable[i].begin(),i);

    return end();
  }

  iterator end()
  {
    return Iterator(this,HashTable[SIZE].end(),SIZE); 
  }

  const_iterator cbegin() const
  {
    for(size_type i=0; i<SIZE; ++i) //same as begin but const
      if(!HashTable[i].empty())
        return ConstIterator(this,HashTable[i].begin(),i);

    return cend();
  }

  const_iterator cend() const
  {
    return ConstIterator(this,HashTable[SIZE].end(),SIZE);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;
  using liter = typename std::list<value_type>::const_iterator;

private:
    const HashMap *map;
    liter iter;
    size_type index;
    
    friend class HashMap<KeyType, ValueType>;
    
public:
  explicit ConstIterator(const HashMap* m, liter it, size_type i)
  {
    map = m;
    iter = it;
    index = i;
  }

  ConstIterator(const ConstIterator& other)
  {
    map = other.map;
    iter = other.iter;
    index = other.index;
  }

  ConstIterator& operator++()
  {
    if (*this == map->end())
            throw std::out_of_range("cant increment the end operator++()");
    
    iter ++;  
    if (iter == map->HashTable[index].end())
    {
      for (size_type i=index+1; i!=map->SIZE; ++i)  //if it reached the end of list move to another one that has at least 1 oelement
        if(!map->HashTable[i].empty())
        {
          iter=map->HashTable[i].begin();
          index=i;
          return *this;
        }
        *this = map->end();
        return *this;
    }
    return *this;
  }

  ConstIterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  ConstIterator& operator--()
  {
    if (*this == map->begin())
            throw std::out_of_range("cant decrement the end operator--()");
    
    if (*this == map->end())  // search from the back for a list with an element
      for (size_type i=SIZE-1; i>=0; --i)
        if(!map->HashTable[i].empty())
        {
          iter=map->HashTable[i].end();
          index=i;
          iter--;
          return *this;
        }
        
    if (iter == map->HashTable[index].begin())  //if its the begin of a list then move to a previous one with at least one element
    {
      for (size_type i=index-1; i>=0; --i)
        if(!map->HashTable[i].empty())
        {
          iter=map->HashTable[i].end();
          index=i;
          --iter;
          return *this;
        }
    }
    --iter;
    return *this;
            
  }

  ConstIterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  reference operator*() const
  {
    if (index==SIZE && iter==map->HashTable[SIZE].end())
      throw std::out_of_range("Cant reference from the end");
    return *iter;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return map == other.map && iter == other.iter && index==other.index;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;
  using liter = typename std::list<value_type>::iterator;
    
public:
  explicit Iterator(const HashMap* my, liter it, size_type in) : ConstIterator(my,it,in)
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
