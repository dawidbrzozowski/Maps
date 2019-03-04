#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include <stdio.h>
#include <ctime>

#include "mpacTreeMap.h"
#include "mpacHashMap.h"
#define N 1000

namespace
{

template <typename K, typename V>
using HashMap = aisdi::HashMap<K, V>;

template <typename K, typename V>
using TreeMap = aisdi::TreeMap<K, V>;

void perfomTest()
{
 // Map<int, std::string> map;
 // map[1] = "TODO";
}

} // namespace


void add_element_test(int test_size, TreeMap <int,int> tree, HashMap <int,int> hash)
{
    clock_t hash_endTime;
    clock_t hash_begTime;
    long double hash_time;
    clock_t tree_endTime;
    clock_t tree_begTime;
    long double tree_time;


    for (int i = 0; i < test_size; i++)
    {
        tree[rand()%test_size] = rand()%1000;
        hash[rand()%test_size] = rand()%1000;
    }
    hash_begTime = clock();
    hash [rand()%test_size] = rand()%1000;
    hash_endTime = clock();
    tree_begTime = clock();
    tree [rand()%test_size] = rand()%1000;
    tree_endTime = clock();
    tree_time = (tree_endTime - tree_begTime)/(CLOCKS_PER_SEC/1000000);
    hash_time = (hash_endTime - hash_begTime)/(CLOCKS_PER_SEC/1000000);
    std::cout<< "Dla "<<test_size<<" elementow, dodanie elementu dla hashmapy wykonal sie w czasie: "<<hash_time<<"ns"<<std::endl;
    std::cout<< "Dla "<<test_size<<" elementow, dodanie elementu dla treemapy wykonalo sie w czasie: "<<tree_time<<"ns"<<std::endl;

}

void remove_element_test(int test_size, TreeMap <int,int> tree, HashMap <int,int> hash)
{
    clock_t hash_endTime;
    clock_t hash_begTime;
    long double hash_time;
    clock_t tree_endTime;
    clock_t tree_begTime;
    long double tree_time;


    for (int i = 0; i < test_size; i++)
    {
        tree[rand()%test_size] = rand()%1000;
        hash[rand()%test_size] = rand()%1000;
    }
        tree[test_size/2] = rand()%1000;
        hash[test_size/2] = rand()%1000;
    hash_begTime = clock();
    hash.remove(test_size/2);
    hash_endTime = clock();
    tree_begTime = clock();
    tree.remove(test_size/2);
    tree_endTime = clock();
    tree_time = (tree_endTime - tree_begTime)/(CLOCKS_PER_SEC/1000000);
    hash_time = (hash_endTime - hash_begTime)/(CLOCKS_PER_SEC/1000000);
    std::cout<< "Dla "<<test_size<<" elementow, usuniecie elementu dla hashmapy wykonal sie w czasie: "<<hash_time<<"ns"<<std::endl;
    std::cout<< "Dla "<<test_size<<" elementow, usuniecie elementu dla treemapy wykonalo sie w czasie: "<<tree_time<<"ns"<<std::endl;

}



int main(int argc, char** argv)
{
  
	TreeMap <int,int> treemap;
  HashMap<int,int> hashmap;

  for (int i = 10000; i <= 100000; i+=10000)
  {
    add_element_test(i, treemap, hashmap);
  }
  
  for (int i = 10000; i <= 100000; i+=10000)
  {
    remove_element_test(i, treemap, hashmap);
  }
  
  //const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  //for (std::size_t i = 0; i < repeatCount; ++i)
  //  perfomTest();
  
  return 0;
}
