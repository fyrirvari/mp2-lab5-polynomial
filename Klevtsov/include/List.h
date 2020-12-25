#pragma once

template<class T>
class List
{
private:
	class Node
	{
	public:
		Node(T val = T(), Node* next = nullptr, Node* prev = nullptr) : val(val), next(next), prev(prev) {}
		Node(Node* next, Node* prev) : next(next), prev(prev) {}
		~Node() {}

		T val;
		Node* next;
		Node* prev;
	};
public:
	class iterator
	{
	private:
		Node* node;
	public:
		iterator() : node(nullptr) {}
		iterator(Node* node) : node(node) {}
		iterator(const iterator& it) : node(it.node) {}

		iterator& operator=(const iterator& it)
		{
			node = it.node;
			return *this;
		}

		bool operator==(const iterator& it) const
		{
			return node == it.node;
		}

		bool operator!=(const iterator& it) const
		{
			return !(node == it.node);
		}

		iterator& operator++()
		{
			if (node == nullptr)
				throw "Increment an empty iterator";
			if (node->next == nullptr)
				throw "Increment end iterator";

			node = node->next;
			return *this;
		}

		iterator& operator--()
		{
			if (node == nullptr)
				throw "Decrement an empty iterator";
			if (node->prev == nullptr)
				throw "Decrement begin iterator";

			node = node->prev;
			return *this;
		}
		
		iterator operator++(int)
		{
			iterator it = *this;
			++(*this);
			return it;
		}

		iterator operator--(int)
		{
			iterator it = *this;
			--(*this);
			return it;
		}

		iterator operator+(int index) const
		{
			iterator it = *this;
			for (int i = 0; i < index; ++i)
				++it;
			return it;
		}
		
		iterator operator-(int index) const
		{
			iterator it = *this;
			for (int i = 0; i < index; ++i)
				--it;
			return it;
		}

		T& operator*() const
		{
			if (node == nullptr)
				throw "Dereference an empty iterator";

			return node->val;
		}

		friend class List;
	};
private:
	iterator _begin;
	iterator _end;
	int _size;
public:
	List() : _begin(new Node), _end(_begin), _size(0) {}
	List(const List& l) : _begin(new Node), _end(_begin), _size(0)
	{
		for (T val : l)
			push(val);
	}
	~List()
	{
		clear();
		delete _begin.node;
		_end = _begin = nullptr;
	}

	inline iterator begin() const { return _begin; }
	inline iterator end() const { return _end; }

	inline T& front() const { return *_begin; }
	inline T& back() const { return *(_end - 1); }

	inline bool empty() const { return _begin == _end; }
	inline int size() const { return _size; }

	void clear()
	{
		while (!empty())
		{
			--_end;
			delete _end.node->next;
		}
		_size = 0;
	}

	void push(const T& val)
	{
		*_end = val;
		_end.node->next = new Node(nullptr, _end.node);
		++_end;
		++_size;
	}

	T pop()
	{
		--_end;
		delete _end.node->next;
		_end.node->next = nullptr;
		--_size;
		return *_end;
	}

	void insert(const T& val, int index)
	{
		if (index < 0 || _size < index)
			throw "Illegal index";
		
		if (index == _size)
		{
			push(val);
			--_size;
		}
		else if (index == 0)
		{
			_begin.node->prev = new Node(val, _begin.node, nullptr);
			--_begin;
		}
		else
		{
			iterator it = _begin + index;
			Node* node = new Node(val, it.node, it.node->prev);
			(it - 1).node->next = node;
			it.node->prev = node;
		}
		++_size;
	}

	void emplace(const T& val, int index)
	{
		if (index < 0 || _size < index)
			throw "Illegal index";
		
		*(_begin + index) = val;
	}

	T erase(int index)
	{
		if (index < 0 || _size <= index)
			throw "Illegal index";

		T val;
		if (index == 0)
		{
			val = *_begin;
			++_begin;
			delete _begin.node->prev;
			_begin.node->prev = nullptr;
		}
		else
		{
			iterator it = _begin + index;
			(it - 1).node->next = it.node->next;
			(it + 1).node->prev = it.node->prev;
			val = *it;
			delete it.node;
		}
		--_size;
		return val;
	}

	List& operator=(const List& l)
	{
		if (this != &l)
		{
			clear();
			for (T val : l)
				push(val);
		}
		return *this;
	}

	bool operator==(const List& l) const
	{
		if (this != &l)
		{
			if (_size != l._size)
				return false;
			for (iterator it = _begin, it2 = l._begin; it != _end; ++it, ++it2)
				if (*it != *it2)
					return false;
		}
		return true;
	}

	bool operator!=(const List& l) const
	{
		return !(*this == l);
	}

	T& operator[](int index)
	{
		if (index < 0 || _size <= index)
			throw "Illegal index";

		return *(_begin + index);
	}
	
	const T& operator[](int index) const
	{
		if (index < 0 || _size <= index)
			throw "Illegal index";
		
		return *(_begin + index);
	}

	friend std::ostream& operator<<(std::ostream& out, const List& l)
	{
		for (T val : l)
			out << val;
		return out;
	}
};