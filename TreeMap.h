#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iostream>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
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
  
  class Node
  {
		public:
		value_type* value;
		Node* left;
		Node* right;
		Node* parent;
		Node()
		{
			value = nullptr;
			left = nullptr;
			right = nullptr;
			parent = nullptr;
		}
		Node(value_type val, Node* p)
		{
			value=new value_type(val);
			left = nullptr;
			right = nullptr;
			parent = p;
		}
		~Node()
		{
			if (value)
				delete value;
		}
		mapped_type& getValue()
		{
			return value->second;
		}

		key_type getKey()
		{
			return value->first;
		}
		
		value_type& getPair()
		{
			return *value;
		}

		Node& operator=(const Node& other)
		{
			value=other.value;
			left=other.left;
			right=other.right;
			parent=other.parent;
		}
		bool operator==(const Node& other)
		{
			if (value==other->value && left==other->left && right==other->right && parent==other->parent)
				return true;
			return false;
		}
		bool operator!=(const Node& other)
		{
			return !((*this)==other);
		}
		
	};
  
  Node* root;
  size_type size;
	void init()
	{
		root = nullptr;
		size = 0;
	}

  TreeMap()
  {
		init();
	}

  ~TreeMap()
  {
		size_type SIZE = size;
		for(size_type i=0; i<SIZE; ++i)
			remove(root->getKey());
	}
	
  TreeMap(std::initializer_list<value_type> list)
  {
		init();
    for (auto it = list.begin(); it!=list.end(); ++it)
        this->operator[](it->first) = it->second;
  }

  TreeMap(const TreeMap& other)
  {
		init();
    for (auto it = other.cbegin(); it != other.cend(); ++it)
        this->operator[](it->first) = it->second;
  }

  TreeMap(TreeMap&& other)
  {
		init();
    root=other.root;
    size=other.size;
    other.root=nullptr;
    other.size=0;
  }
	
  TreeMap& operator=(const TreeMap& other) 
  {
    if(this==&other)
			return *this;	
		
		removeAll();

		for (auto it = other.cbegin(); it != other.cend(); ++it)
        this->operator[](it->first) = it->second;
        
    return *this;
  }

  TreeMap& operator=(TreeMap&& other)
  {
    root=other.root;
    size=other.size;
    other.root=nullptr;
    other.size=0;
    return *this;
  }

  bool isEmpty() const
  {
    if (size == 0)
			return true;
		return false;
  }

  mapped_type& operator[](const key_type& key)
  {
    Node* curr=root;
    Node* currentParent=nullptr;
    
    while(curr!=nullptr)
    {
			currentParent=curr;
			if(key==curr->getKey())
				return curr->getValue();
			else if(key>curr->getKey())
				curr=curr->right;
			else if (key<curr->getKey())
				curr=curr->left;
		}	//if not found then create new node
		
		Node* newNode = new Node(std::make_pair(key,mapped_type{}),currentParent);
		++size;
		
		if (currentParent!=nullptr) // means if it is not root
		{
			if (key >= currentParent->getKey())
				currentParent->right=newNode;
			else
				currentParent->left=newNode;
		}
		else
			root=newNode;		
		
		return newNode->getValue();
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    Node* curr=root;
    
    while(curr!=nullptr)
    {
			if(key==curr->getKey())
				return curr->getValue();
			else if(key>curr->getKey())
				curr=curr->right;
			else if (key<curr->getKey())
				curr=curr->left;
		}
		
		throw std::out_of_range("value not found in valueof const");
  }

  mapped_type& valueOf(const key_type& key)
  {
    Node* curr=root;
    
    while(curr!=nullptr)
    {
			if(key==curr->getKey())
				return curr->getValue();
			else if(key>curr->getKey())
				curr=curr->right;
			else if (key<curr->getKey())
				curr=curr->left;
		}
		
		throw std::out_of_range("value not found in valueof");
  }

  const_iterator find(const key_type& key) const
  {
    Node* curr=root;
    
    while(curr!=nullptr)
    {
			if(key==curr->getKey())
				return ConstIterator(curr,this);
		
			if(key>curr->getKey())
				curr=curr->right;
		
			else if (key<curr->getKey())
				curr=curr->left;
		}
		return cend();
  }

  iterator find(const key_type& key)
  {
    Node* curr=root;
    
    while(curr!=nullptr)
    {
			if(key==curr->getKey())
				return Iterator(curr,this);
		
			if(key>curr->getKey())
				curr=curr->right;
		
			else if (key<curr->getKey())
				curr=curr->left;
		}
		return end();
  }

	void removeAll()
	{
		size_type SIZE = size;
		for(size_type i=0; i<SIZE; ++i)
			remove(root->getKey());
	}

  void remove(const key_type& key)
  {
		key_type k=key;
    remove(find(k));
  }

  void remove(const const_iterator& it)
  {
		remove(it.node);	//since it's easier to operate on single nodes than iterators
	}
	
  void remove(Node* curr)
  { 
		if (root == nullptr)
			throw std::out_of_range("Can't remove from an empty map.");
    if (curr == nullptr)
			throw std::out_of_range("Node to remove is null");
			
		//if there are no children node to remove is "lisc"
		if(curr->left == nullptr && curr->right == nullptr)
		{
			if (curr->parent == nullptr)
				root=nullptr;
			else
				if (curr == curr->parent->left)
					curr->parent->left=nullptr;
				else if (curr == curr->parent->right)
					curr->parent->right=nullptr;
		}
		//if removed node has only left child
		else if (curr->left!=nullptr and curr->right == nullptr)
		{
			if (curr->parent == nullptr)	//if it is root to remove
			{
				root=curr->left;
				curr->left->parent=nullptr;
			}
			else
				if (curr == curr->parent->left)
				{
					curr->parent->left=curr->left;
					curr->left->parent=curr->parent;
				}
				else if (curr == curr->parent->right)
				{
					curr->parent->right=curr->left;
					curr->left->parent=curr->parent;
				}
		}
		else if (curr->left==nullptr and curr->right != nullptr) //only right child
		{
			if (curr->parent == nullptr)	//if it is root to remove
			{
				root=curr->right;
				curr->right->parent=nullptr;
			}
			else
				if (curr == curr->parent->left)
				{
					curr->parent->left=curr->right;
					curr->right->parent=curr->parent;
				}
				else if (curr == curr->parent->right)
				{
					curr->parent->right=curr->right;
					curr->right->parent=curr->parent;
				}
		}
		else //node to remove has 2 children
		{
			Node* replacement = curr->right;
			
			while(replacement->left != nullptr)	//which is like operator ++
				replacement=replacement->left;
				
			value_type* val;
			val = new value_type(replacement->getKey(),replacement->getValue());
			
			delete curr->value;	//delete current value and replace it with the value from the next (operator ++) node
			curr->value=val;
			remove(replacement);	//and delete the one that has just jumped in the position of the removed one
			return;
		}		
		size--;
		delete curr;
  }

  size_type getSize() const
  {
    return size;
  }

  bool operator==(const TreeMap& other) const
  {
		if (size!=other.size)
			return false;
			
		for (auto it1=begin(), it2=other.begin(); it1!=end() || it2!=other.end(); ++it1, ++it2)
			if (it1->first != it2->first || it1->second != it2->second)
				return false;
		
		return true;
  }

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    Node* temp = root;
    if (isEmpty())
			return end();
		else
			while (temp->left != nullptr)
				temp=temp->left;
				
		return Iterator(temp, this);
  }

  iterator end()
  {
    return Iterator(nullptr, this);
  }

  const_iterator cbegin() const
  {
    Node* temp = root;
    if (isEmpty())
			return end();
		else
			while (temp->left != nullptr)
				temp=temp->left;
				
		return ConstIterator(temp, this);
  }

  const_iterator cend() const
  {
    return ConstIterator(nullptr, this);
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
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;
  
  const TreeMap *map;
  Node *node;

  explicit ConstIterator() : map(nullptr), node(nullptr)
  {}

	ConstIterator(Node* n, const TreeMap* m) : map(m), node(n)
	{}
	
  ConstIterator(const ConstIterator& other) : map(other.map), node(other.node)
  {}

  ConstIterator& operator++()
  {
    if (node==nullptr)
			throw std::out_of_range("Cant increase end in operator ++");
		
		if(node->right != nullptr)
		{
			node=node->right;
			while(node->left != nullptr)
				node=node->left;
		}
		else
		{
			while(node->parent != nullptr && node->parent->right==node)
				node=node->parent;
			if(node->parent != nullptr)	//if it is not root
				node=node->parent;
			else	//if it is root
				node=nullptr;
		}
		return *this;
  }

  ConstIterator operator++(int)
  {
    auto result=*this;
    operator++();
    return result;
  }

  ConstIterator& operator--()
  {
    if (*this==map->begin())
			throw std::out_of_range("Cant decrease from begin of the map");
		if (map->size <= 0)
			throw std::out_of_range("Cant decrease from an empty map");
		if (node == nullptr)
		{
			Node* help=map->root;
			
			while(help->right!=nullptr)
				help=help->right;
			
			node=help;
			return *this;
		}
		else if (node->left != nullptr)
		{
			node=node->left;
			while(node->right!=nullptr)
				node=node->right;
		}
		else
		{
			while(node->parent!=nullptr && node->parent->left == node)
				node=node->parent;
				
			if (node->parent != nullptr)
				node=node->parent;
			else
				node=nullptr;
		}
		return *this;
  }

  ConstIterator operator--(int)
  {
    auto result=*this;
    operator--();
    return result;
  }

  reference operator*() const
  {
		if (*this==map->end())	
			throw std::out_of_range("cant derefrence from nullpointer");
    return node->getPair();
  }

  pointer operator->() const
  {
		if (*this==map->end())	
			throw std::out_of_range("Cant point to the end of the map");
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return node==other.node && map==other.map;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

  explicit Iterator() : ConstIterator()
  {}

	Iterator(Node* n, const TreeMap* m) : ConstIterator(n,m)
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

#endif //AISDI_MAPS_MAP_H 
