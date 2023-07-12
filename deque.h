#pragma once

// === DECLARATIONS ===

typedef long long int sizeT;

template <typename T>
class MyDeque {
private:
	T *start;
	T *first, *last;
	sizeT now_size, max_size;

	inline T *GetNext(T *pter, sizeT add = 1) {
		return start + (((pter - start) + add) & (max_size - 1));
	}
	inline T *GetPrev(T *pter, sizeT dec = 1) {
		return start + (((pter - start) - dec) & (max_size - 1));
	}

	sizeT GetPow2(sizeT num) const {
		return (num < 2 ? 0 : 63 - __builtin_clzll(num));
	}
	void Realloc(sizeT new_size) {
		T *new_start = static_cast <T *>(calloc(new_size, sizeof(T)));
		last = new_start;
		for (int i = 0; i < now_size; i++) {
			*last = *first;
			last++;
			first = GetNext(first);
		}
		free(start);
		first = start = new_start;
		max_size = new_size;
	}

public:
	MyDeque () {
		max_size = 1;
		start = static_cast <T *> (malloc(1 * sizeof(T)));
		first = last = start;
		now_size = 0;
	}
	MyDeque (sizeT size) {
		max_size = 1 << GetPow2(size);
		start = static_cast <T *> (malloc(max_size * sizeof(T)));
		first = last = start;
		T elem;
		for (now_size = 0; now_size < size; now_size++) {
			*last = elem;
			last++;
		}
	}
	MyDeque (sizeT size, const T &elem) {
		max_size = 1 << GetPow2(size);
		start = static_cast <T *> (malloc(max_size * sizeof(T)));;
		first = last = start;
		for (now_size = 0; now_size < size; now_size++) {
			*last = elem;
			last++;
		}
	}
	MyDeque (const MyDeque <T> &to_copy) {
		max_size = to_copy.max_size;
		start = static_cast <T *> (malloc(max_size * sizeof(T)));;
		first = last = start;
		T *loc = to_copy.first;
		for (now_size = 0; now_size < to_copy.size; now_size++) {
			*last = *loc;
			last++;
			loc = to_copy.GetNext(loc);
		}
	}

	MyDeque <T> &operator= (const MyDeque <T> &to_copy) {
		max_size = to_copy.max_size;
		free(start);
		start = static_cast <T *> (malloc(max_size * sizeof(T)));;
		first = last = start;
		T *loc = to_copy.first;
		for (now_size = 0; now_size < to_copy.size; now_size++) {
			*last = *loc;
			last++;
			loc = to_copy.GetNext(loc);
		}
	}
	MyDeque <T> &operator= (MyDeque <T> &&to_copy) noexcept {
		free(start);
		start = to_copy.start;
		now_size = to_copy.now_size;
		max_size = to_copy.max_size;
		first = to_copy.first;
		return *this;
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