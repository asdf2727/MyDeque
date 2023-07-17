#pragma once

// === DECLARATIONS ===

typedef long long int sizeT;

template <typename T>
class MyDeque {
private:
	T *start = nullptr;
	T *first = nullptr, *last = nullptr;
	sizeT now_size = 0, max_size = 0;

	inline T *GetNext(T *pter, sizeT add = 1) const {
		return start + (((pter - start) + add) & (max_size - 1));
	}
	inline T *GetPrev(T *pter, sizeT dec = 1) const {
		return start + (((pter - start) - dec) & (max_size - 1));
	}

	sizeT GetBufferSize(sizeT new_size) const {
		return 1 << (64 - __builtin_clzll(new_size + 1));
	}

	void Realloc(sizeT new_size) {
		if (now_size > new_size) {
			now_size = new_size;
		}
		T *new_start = static_cast <T *>(calloc(new_size, sizeof(T)));
		for (sizeT i = 0; i < now_size; i++) {
			*(new_start + i) = *GetNext(first, i);
		}
		free(start);
		first = start = new_start;
		last = new_start + now_size;
		max_size = new_size;
	}

public:
	MyDeque () {
		clear();
	}
	MyDeque (sizeT size) {
		Realloc(GetBufferSize(size));
		now_size = size;
		last = first + size;
		T init_val;
		while(first != last) {
			*first = init_val;
			first++;
		}
		first = start;
	}
	MyDeque (sizeT size, const T &init_val) {
		Realloc(GetBufferSize(size));
		now_size = size;
		last = first + size;
		while(first != last) {
			*first = init_val;
			first++;
		}
		first = start;
	}
	MyDeque (const MyDeque <T> &to_copy) {
		Realloc(to_copy.max_size);
		for (int i = 0; i < now_size; i++) {
			*(start + i) = *to_copy.GetNext(to_copy.first + i);
		}
		now_size = to_copy.now_size;
		last = start + now_size;
	}

	MyDeque <T> &operator= (const MyDeque <T> &to_copy) {
		now_size = 0;
		max_size = 0;
		Realloc(to_copy.max_size);
		for (int i = 0; i < now_size; i++) {
			*(start + i) = *to_copy.GetNext(to_copy.first + i);
		}
		now_size = to_copy.now_size;
		last = start + now_size;
	}
	MyDeque <T> &operator= (MyDeque <T> &&to_copy) noexcept {
		free(start);
		start = to_copy.start;
		now_size = to_copy.now_size;
		max_size = to_copy.max_size;
		first = to_copy.first;
		return *this;
	}

	void clear() {
		free(start);
		max_size = 1;
		start = static_cast <T *> (calloc(1, sizeof(T)));
		first = last = start;
		now_size = 0;
	}
	void resize(sizeT new_size) {
		T init_val;
		resize(new_size, init_val);
	}
	void resize(sizeT new_size, T &init_val) {
		if (GetBufferSize(new_size) != max_size) {
			Realloc(GetBufferSize(new_size));
		}
		if (new_size < now_size) {
			last = GetNext(first + new_size);
		}
		else {
			while (now_size < new_size) {
				*last = init_val;
				last++;
				now_size++;
			}
		}
	}
	void reserve(sizeT future_size) {
		if (GetBufferSize(future_size) > max_size) {
			Realloc(future_size);
		}
	}

	sizeT size() const {
		return now_size;
	}
	sizeT tot_size() const {
		return max_size;
	}

	bool operator== (const MyDeque <T> &to_comp) const {
		// TODO add rolling hash
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
	inline bool operator!= (const MyDeque <T> &to_comp) const {
		return !operator==(to_comp);
	}

	void push_back(const T &elem) {
		if (now_size + 1 >= max_size) {
			Realloc(max_size << 1);
		}
		*last = elem;
		last = GetNext(last);
		now_size++;
	}
	void pop_back() {
		last = GetPrev(last);
		now_size--;
		if (now_size < max_size >> 2) {
			Realloc(max_size >> 1);
		}
	}
	void push_front(const T &elem) {
		if (now_size + 1 >= max_size) {
			Realloc(max_size << 1);
		}
		first = GetPrev(first);
		*first = elem;
		now_size++;
	}
	void pop_front() {
		first = GetNext(first);
		now_size--;
		if (now_size < max_size >> 2) {
			Realloc(max_size >> 1);
		}
	}

	inline T &back() const {
		return *GetPrev(last);
	}
	inline T &front() const {
		return *first;
	}
	inline const T &operator[] (sizeT pos) const {
		return *GetNext(first, pos);
	}
	inline T &operator[] (sizeT pos) {
		return *GetNext(first, pos);
	}

private:
	class base_iter {
	protected:
		T *pter;
		T *start;
		sizeT max_size;

		inline T *GetNext(sizeT add = 1) {
			return start + (((pter - start) + add) & (max_size - 1));
		}
		inline T *GetPrev(sizeT dec = 1) {
			return start + (((pter - start) - dec) & (max_size - 1));
		}

	public:
		base_iter(const MyDeque <T> &to_init, sizeT pos) {
			start = to_init.start;
			max_size = to_init.max_size;
			pter = to_init.first + pos % max_size;
		}
		base_iter(const base_iter &to_copy) = default;
		base_iter &operator=(const base_iter &to_copy) = default;
		base_iter &operator=(base_iter &&to_copy) noexcept {
			start = to_copy.start;
			max_size = to_copy.max_size;
			pter = to_copy.pter;
			to_copy.pter = to_copy.start = nullptr;
		}

		bool operator== (const base_iter &rhs) const {
			return pter == rhs.pter;
		}
		bool operator!= (const base_iter &rhs) const {
			return pter != rhs.pter;
		}

		base_iter &operator++ () {
			pter = GetNext();
			return *this;
		}
		const base_iter operator++ (int) {
			base_iter ret = *this;
			pter = GetNext();
			return ret;
		}
		base_iter operator+ (sizeT dist) const {
			base_iter ret;
			ret.start = start;
			ret.pter = GetNext(dist);
			ret.max_size = max_size;
			return ret;
		}
		base_iter &operator+= (sizeT dist) {
			pter = GetNext(dist);
			return *this;
		}

		base_iter &operator-- () {
			pter = GetPrev();
			return *this;
		}
		const base_iter operator-- (int) {
			base_iter ret = *this;
			pter = GetPrev();
			return ret;
		}
		base_iter operator- (sizeT dist) const {
			base_iter ret;
			ret.start = start;
			ret.pter = GetPrev(dist);
			ret.max_size = max_size;
			return ret;
		}
		base_iter &operator-= (sizeT dist) {
			pter = GetPrev(dist);
			return *this;
		}

		sizeT operator- (const base_iter &rhs) const {
			return pter - rhs.pter;
		}
	};

public:
	class iter : public base_iter {
	public:
		iter(const MyDeque <T> &to_init, sizeT pos) : base_iter(to_init, pos) { }

		iter operator+ (sizeT dist) {
			base_iter::operator+(dist);
			return *this;
		}
		iter operator- (sizeT dist) {
			base_iter::operator-(dist);
			return *this;
		}

		T *operator-> () const {
			return this->pter;
		}
		T &operator* () const {
			return *this->pter;
		}
	};
	class ct_iter : public base_iter {
	public:
		ct_iter(const MyDeque <T> &to_init, sizeT pos) : base_iter(to_init, pos) { }

		ct_iter operator+ (sizeT dist) {
			base_iter::operator+(dist);
			return *this;
		}
		ct_iter operator- (sizeT dist) {
			base_iter::operator-(dist);
			return *this;
		}

		const T *operator-> () const {
			return this->pter;
		}
		const T &operator* () const {
			return *this->pter;
		}
	};

	inline iter begin() const {
		iter ret(*this, 0);
		return ret;
	}
	inline iter end() const {
		iter ret(*this, now_size);
		return ret;
	}

	inline ct_iter cbegin() const {
		ct_iter ret(*this, 0);
	}
	inline ct_iter cend() const {
		ct_iter ret(*this, now_size);
		return ret;
	}
};