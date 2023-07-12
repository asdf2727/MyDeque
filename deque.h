#pragma once

// === DECLARATIONS ===

typedef long long int sizeT;

template <typename T>
class MyDeque {
private:
	T *start;
	T *first, *last;
	sizeT now_size, max_size;

	MyDeque <T> recast(sizeT newSize) const;

public:
	MyDeque ();
	MyDeque (sizeT max_size);
	MyDeque (sizeT max_size, const T &elem);
	MyDeque (const MyDeque <T> &to_copy);

	MyDeque <T> &operator= (const MyDeque <T> &to_copy);
	MyDeque <T> &operator= (MyDeque <T> &&to_copy) noexcept;

	sizeT size() const;
	sizeT tot_size() const;

	bool operator== (const MyDeque <T> &to_comp) const;
	inline bool operator!= (const MyDeque <T> &to_comp) const;

	inline T &back() const;
	void push_back(const T &elem);
	void pop_back();

	inline T &front() const;
	void push_front(const T &elem);
	void pop_front();

private:
	class base_iter {
	protected:
		T *pter;
		T *start;
		sizeT max_size;

	public:
		base_iter(const MyDeque <T> &to_init, sizeT pos);
		base_iter(const base_iter &to_copy) = default;
		base_iter &operator=(const base_iter &to_copy) = default;
		base_iter &operator=(base_iter &&to_copy) noexcept;

		bool operator== (const base_iter &rhs) const;
		bool operator!= (const base_iter &rhs) const;

		base_iter &operator++ ();
		const base_iter operator++ (int);
		base_iter operator+ (sizeT dist);
		base_iter &operator+= (sizeT dist);

		base_iter &operator-- ();
		const base_iter operator-- (int);
		base_iter operator- (sizeT dist);
		base_iter &operator-= (sizeT dist);

		sizeT operator- (const base_iter &rhs) const;
	};

public:
	class iter : public base_iter {
	public:
		iter(const MyDeque <T> &to_init, sizeT pos) : base_iter(to_init, pos) { }

		T *operator-> () const;
		T &operator* () const;
	};
	class ct_iter : public base_iter {
	public:
		ct_iter(const MyDeque <T> &to_init, sizeT pos) : base_iter(to_init, pos) { }

		const T *operator-> () const;
		const T &operator* () const;
	};

	iter begin() const;
	iter end() const;

	ct_iter cbegin() const;
	ct_iter cend() const;
};

// === DEQUE DEFINITIONS ===

sizeT GetPow2(sizeT num) {
	return (num < 2 ? 0 : 63 - __builtin_clzll(num));
}

template <typename T> MyDeque <T> MyDeque <T>::recast(sizeT new_size) const {
	MyDeque <T> ret(new_size);
	ret.now_size = now_size;
	MyDeque <T>::iter copy_pos(*this, 0), end_pos(*this, now_size);
	while(copy_pos != end_pos) {
		*ret.last = *copy_pos;
		ret.last++;
		copy_pos++;
	}
	return ret;
}

template <typename T> MyDeque <T>::MyDeque () {
	max_size = 1;
	start = (T *)calloc(1, sizeof(T));
	first = last = start;
	now_size = 0;
}
template <typename T> MyDeque <T>::MyDeque (sizeT size) {
	max_size = GetPow2(size);
	start = (T *)calloc(max_size, sizeof(T));
	first = last = start;
	now_size = 0;
	while(now_size < size) {
		*last = new T();
		last++;
		now_size++;
	}
}
template <typename T> MyDeque <T>::MyDeque (sizeT size, const T &elem) {
	max_size = GetPow2(size);
	start =(T *)calloc(max_size, sizeof(T));
	first = last = start;
	now_size = 0;
	while(now_size < size) {
		*last = elem;
		last++;
		now_size++;
	}
}
template <typename T> MyDeque <T>::MyDeque (const MyDeque <T> &to_copy) {
	*this = to_copy.move_data(to_copy.now_size);
}

template <typename T> MyDeque <T> &MyDeque <T>::operator= (const MyDeque <T> &to_copy) {
	*this = to_copy.move_data(to_copy.now_size);
}
template <typename T> MyDeque <T> &MyDeque <T>::operator= (MyDeque <T> &&to_copy) noexcept {
	free(start);
	start = to_copy.start;
	now_size = to_copy.now_size;
	max_size = to_copy.max_size;
	first = to_copy.first;
	last = to_copy.last;
	to_copy.first = nullptr;
}

// TODO add rolling hash
template <typename T> bool MyDeque <T>::operator== (const MyDeque <T> &to_comp) const {
	if (max_size != to_comp.size) {
		return false;
	}
	for(T *val1 = first, *val2 = to_comp.first; val1 != last; val1++, val2++) {
		if (*val1 != *val2) {
			return false;
		}
	}
	return true;
}
template <typename T> bool MyDeque <T>::operator!= (const MyDeque <T> &to_comp) const {
	return !operator==(to_comp);
}

template <typename T> T &MyDeque <T>::back() const {
	if (last == start) {
		return *(start + max_size - 1);
	}
	else {
		return *(last - 1);
	}
}
template <typename T> void MyDeque <T>::push_back(const T &elem) {
	if (now_size >= max_size) {
		*this = recast(max_size << 1);
	}
	*last = elem;
	last++;
	now_size++;
	if (last - start >= max_size) {
		last -= max_size;
	}
}
template <typename T> void MyDeque <T>::pop_back() {
	last--;
	now_size--;
	if(last < start) {
		last += max_size;
	}
	if (now_size <= max_size >> 2) {
		*this = recast(max_size >> 1);
	}
}

template <typename T> T &MyDeque <T>::front() const {
	return *first;
}
template <typename T> void MyDeque <T>::push_front(const T &elem) {
	if (now_size >= max_size) {
		*this = recast(max_size << 1);
	}
	first--;
	if (first < start) {
		first += max_size;
	}
	*first = elem;
	now_size++;

}
template <typename T> void MyDeque <T>::pop_front() {
	first++;
	now_size--;
	if (first - start >= max_size) {
		first -= max_size;
	}
	if (now_size <= max_size >> 2) {
		*this = recast(max_size >> 1);
	}
}

// === ITERATOR DEFINITIONS ===

template <typename T> MyDeque <T>::base_iter::base_iter(const MyDeque <T> &to_init, sizeT pos) {
	start = to_init.start;
	max_size = to_init.max_size;
	pter = to_init.first + pos % max_size;
	if (pter - start > max_size) {
		pter -= max_size;
	}
}
template <typename T> typename MyDeque <T>::base_iter &MyDeque <T>::base_iter::operator=(MyDeque <T>::base_iter &&to_copy) noexcept {
	start = to_copy.start;
	max_size = to_copy.max_size;
	pter = to_copy.pter;
	to_copy.pter = to_copy.start = nullptr;
}

template <typename T> bool MyDeque <T>::base_iter::operator==(const MyDeque <T>::base_iter &rhs) const {
	return pter == rhs.pter;
}
template <typename T> bool MyDeque <T>::base_iter::operator!=(const MyDeque <T>::base_iter &rhs) const {
	return pter != rhs.pter;
}

template <typename T> typename MyDeque <T>::base_iter &MyDeque <T>::base_iter::operator++() {
	pter++;
	if (pter - start >= max_size) {
		pter -= max_size;
	}
	return *this;
}
template <typename T> const typename MyDeque <T>::base_iter MyDeque <T>::base_iter::operator++(int) {
	MyDeque <T>::base_iter ret(*this);
	pter++;
	if (pter - start >= max_size) {
		pter -= max_size;
	}
	return ret;
}
template <typename T> typename MyDeque <T>::base_iter MyDeque <T>::base_iter::operator+(sizeT dist) {
	MyDeque <T>::base_iter ret(*this);
	ret.pter += dist % max_size;
	if (ret.pter - start >= max_size) {
		ret.pter -= max_size;
	}
	return ret;
}
template <typename T> typename MyDeque <T>::base_iter &MyDeque <T>::base_iter::operator+=(sizeT dist) {
	pter += dist % max_size;
	if (pter - start >= max_size) {
		pter -= max_size;
	}
	return *this;
}

template <typename T> typename MyDeque <T>::base_iter &MyDeque <T>::base_iter::operator--() {
	pter--;
	if (start > pter) {
		pter += max_size;
	}
	return *this;
}
template <typename T> const typename MyDeque <T>::base_iter MyDeque <T>::base_iter::operator--(int) {
	MyDeque <T>::base_iter ret(*this);
	pter--;
	if (start > pter) {
		pter += max_size;
	}
	return ret;
}
template <typename T> typename MyDeque <T>::base_iter MyDeque <T>::base_iter::operator-(sizeT dist) {
	MyDeque <T>::base_iter ret(*this);
	ret.pter -= dist % max_size;
	if (start > ret.pter) {
		ret.pter += max_size;
	}
	return ret;
}
template <typename T> typename MyDeque <T>::base_iter &MyDeque <T>::base_iter::operator-=(sizeT dist) {
	pter -= dist % max_size;
	if (start > pter) {
		pter += max_size;
	}
	return *this;
}

template <typename T> sizeT MyDeque <T>::base_iter::operator-(const MyDeque <T>::base_iter &rhs) const {
	return (sizeT)(pter - rhs.pter);
}


template <typename T> T *MyDeque <T>::iter::operator->() const {
	return this->pter;
}
template <typename T> T &MyDeque <T>::iter::operator*() const {
	return *this->pter;
}

template <typename T> const T *MyDeque <T>::ct_iter::operator->() const {
	return this->pter;
}
template <typename T> const T &MyDeque <T>::ct_iter::operator*() const {
	return *this->pter;
}

template <typename T> typename MyDeque <T>::iter MyDeque <T>::begin() const {
	iter ret(*this, 0);
	return ret;
}
template <typename T> typename MyDeque <T>::iter MyDeque <T>::end() const {
	iter ret(*this, now_size);
	return ret;
}

template <typename T> typename MyDeque <T>::ct_iter MyDeque <T>::cbegin() const {
	ct_iter ret(*this, 0);
	return ret;
}
template <typename T> typename MyDeque <T>::ct_iter MyDeque <T>::cend() const {
	ct_iter ret(*this, now_size);
	return ret;
}