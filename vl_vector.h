
#ifndef EX6__VL_VECTOR_H_
#define EX6__VL_VECTOR_H_

#include <cstddef>
#include <iterator>
#include <cmath>
#include <stdexcept>
#include <cstring>
#define INIT_STATIC_CAP 16
#define DEFAULT_SIZE 0
#define INDEX_OUT_OF_RANGE_MSG "Index out of range error."

template<class T, size_t C = INIT_STATIC_CAP>
class VLVector {

  // ----- Iterator Section (public) ----- //
 public:

  // ----- Iterator typedefs ----- //
  typedef T value_type;
  typedef const T const_value_type;
  typedef T &reference;
  typedef const T &const_reference;
  typedef T *iterator;
  typedef const T *const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  // ----- Iterator indexes ----- //
  /**
   * @return Iterator pointing to the beginning of vector.
   */
  iterator begin() {
	return &container_[0];
  }

  /**
   * @return Const iterator pointing to the beginning of vector.
   */
  const_iterator begin() const {
	return &container_[0];
  }

  /**
   * @return Const iterator pointing to the beginning of vector.
   */
  const_iterator cbegin() const {
	return &container_[0];
  }

  /**
   * @return Reverse iterator pointing to the beginning of vector. (from end)
   */
  reverse_iterator rbegin() {
	return reverse_iterator(end());
  }

  /**
   * @return Const reverse iterator pointing to the beginning of vector. (from
   * end)
   */
  const_reverse_iterator rbegin() const {
	return const_reverse_iterator(end());
  }

  /**
   * @return Const reverse iterator pointing to the beginning of vector. (from
   * end)
   */
  const_reverse_iterator crbegin() const {
	return const_reverse_iterator(cend());
  }

  /**
   * @return Iterator pointing to the end of vector.
   */
  iterator end() {
	return container_ + size_;
  }

  /**
   * @return Const iterator pointing to the end of vector.
   */
  const_iterator end() const {
	return (&container_[0]) + size_;
  }

  /**
   * @return Const iterator pointing to the end of vector.
   */
  const_iterator cend() const {
	return (&container_[0]) + size_;
  }

  /**
   * @return Reverse iterator pointing to the end of vector. (from end)
   */
  reverse_iterator rend() {
	return reverse_iterator(begin());
  }

  /**
   * @return Const Reverse iterator pointing to the end of vector. (from end)
   */
  const_reverse_iterator rend() const {
	return const_reverse_iterator(cbegin());
  }

  /**
   * @return Const Reverse iterator pointing to the end of vector. (from end)
   */
  const_reverse_iterator crend() const {
	return const_reverse_iterator(cbegin());
  }


  // ----- Private section ----- //
 private:
  // ----- container states ----- //
  /**
   * Enumerate to determinate for every moment the state of container
   * kDynamic for using a dynamic array (heap based) or static array (stack
   * based).
   */
  enum ArrayType {
	kDynamic,
	kStatic
  };

  // ----- Data members ----- //
  size_t size_;
  size_t capacity_;
  T stack_container_[C];
  T *container_;
  ArrayType array_type_;

  // ----- Private methods ----- //

  /**
   * Copies all elements from given array ot another.
   * @param array_to_fill array to fill with elements from other array.
   * @param origin_array other array to provide elements.
   * @param size size of arrays (should be identical)
   */
  void CopyElements(iterator array_to_fill, iterator origin_array, size_t
  size) noexcept(false) {
	for (size_t i = 0; i < size; ++i) {
	  array_to_fill[i] = origin_array[i];
	}
  }

  /**
   * Seeks for an index in vector, according to given element.
   * @param position element to find index according to it's presence.
   * @return index of element if found - index of last element otherwise.
   */
  size_t FindIndex(const_iterator position) const noexcept {
	size_t i = 0;
	for (auto it = begin(); it!=end(); ++it) {
	  if (it==position) {
		return i;
	  }
	  ++i;
	}
	return size_;
  }

  /**
   * Recalculates capacity of vector under specific circumstances according
   * to given formula.
   * @param k amount of elements wished to be added, used as a part of formula.
   * @return new capacity.
   */
  size_t CapC(size_t k) const noexcept(false) {
	if (size_ + k > C) {
	  return floor((3*(size_ + k))/2);
	}
	return C;
  }

  /**
   * Switches current container into static container, with it's memory saved
   * on stack.
   */
  void SwitchToStackArr() noexcept(false) {
	CopyElements(stack_container_, container_, size_);
	delete[] container_;
	capacity_ = C;
	array_type_ = kStatic;
	container_ = stack_container_;
  }

  /**
   * Re sizes vector's container according to
   * formula mentioned in "CapC" function.
   * @param amount amount of elements planned to be added to vector.
   */
  void ReSizeContainer(size_t amount) noexcept(false) {
	size_t new_capacity = CapC(amount);
	T *new_array = new T[new_capacity];
	CopyElements(new_array, container_, size_);
	if (array_type_==kDynamic) {
	  delete[] container_;
	}
	array_type_ = kDynamic;
	container_ = new_array;
	capacity_ = new_capacity;
  }

  // ----- Public section ----- //
 public:

  // ----- Constructors ----- //

  /**
   * Default constructor, initiates all data members with default values.
   */
  VLVector() : size_(DEFAULT_SIZE), capacity_(C), container_
	  (stack_container_), array_type_(kStatic) {}

  VLVector(size_t count, const_reference elem) : size_(count), capacity_(C) {
	if (count > C) {
	  array_type_ = kDynamic;
	  container_ = new T[capacity_ = CapC(count - C)];
	} else {
	  array_type_ = kStatic;
	  container_ = stack_container_;
	}
	for (size_t i = 0; i < size_; ++i) {
	  container_[i] = elem;
	}
  }
  /**
   * Copy constructor, receives another VLVector object with same generic
   * types and creates a new one according to it's data.
   * @param origin VLVector to create a new one according to.
   */
  VLVector(const VLVector<T, C> &origin) : size_(origin.size_), capacity_
	  (origin.capacity_), array_type_(origin.array_type_) {
	if (array_type_==kStatic) {
	  container_ = stack_container_;
	} else if (array_type_==kDynamic) {
	  container_ = new T[capacity_];
	}
	CopyElements(container_, origin.container_, size_);
  }

  /**
   * Iterator based constructor, creates a vector and fills it with content
   * in range of 2 given iterators.
   * @tparam InputIterator Input iterator type - under user's responsibility
   * to implement with right objects.
   * @param first first iterator pointing to element to be added.
   * @param last last iterator signing the end of adding elements process.
   */
  template<class InputIterator>
  VLVector(InputIterator first, InputIterator last): VLVector() {
	Insert(begin(), first, last);
  }

  /**
   * Destructor for VLVector object.
   */
  ~VLVector() {
	if (array_type_==kDynamic) {
	  delete[] container_;
	}
  }

  // ----- Public Methods ----- //

  /**
   * @return Capacity of vector.
   */
  size_t Capacity() const noexcept {
	return capacity_;
  }

  /**
   * @return True if vector is empty, false otherwise
   */
  bool Empty() const noexcept {
	return size_==0;
  }

  /**
   * @return size of vector.
   */
  size_t Size() const noexcept {
	return size_;
  }

  /**
   * Returns copy of value of vector at given index. (won't access to actual
   * object and allow modification).
   * @param i index of element wished to be referred to.
   * @return element at index.
   */
  value_type At(const size_t i) const noexcept(false) {
	if (i >= size_) {
	  throw std::out_of_range(INDEX_OUT_OF_RANGE_MSG);
	}
	return container_[i];
  }

  /**
   * Returns reference of value of vector at given index.
   * @param i index of element wished to be referred to.
   * @return element at index.
   */
  reference At(const size_t i) noexcept(false) {
	if (i >= size_) {
	  throw std::out_of_range(INDEX_OUT_OF_RANGE_MSG);
	}
	return container_[i];
  }

  /**
   * @return Data container of vector.
   */
  iterator Data() {
	return container_;
  }

  /**
   * @return Data container of vector. (const version)
   */
  const_iterator Data() const {
	return container_;
  }

  /**
   * Inserts all elements in range between first (inclusive) and last
   * (exclusive) iterators.
   * @tparam InputIterator Input iterator type - under user's responsibility
   * to implement with right objects.
   * @param position position of element we wish to add elements before.
   * @param first first element's iterator.
   * @param last iterator after last element we wish to add.
   * @return iterator to first element added to vector.
   */
  template<class InputIterator>
  iterator Insert(const_iterator position, InputIterator first, InputIterator
  last) noexcept(false) {
	size_t index = FindIndex(position);
	size_t amount_to_add = std::distance(first, last);
	if (size_ + amount_to_add > capacity_) {
	  ReSizeContainer(amount_to_add);
	}
	for (reverse_iterator it = rbegin(); it!=rend() - index; ++it) {
	  *(it - amount_to_add) = *it;
	}
	iterator it = &container_[index];
	while (first!=last) {
	  *it = *first;
	  ++it;
	  ++first;
	}
	size_ += amount_to_add;
	return &container_[index];
  }

  /**
   * Inserts a single element to vector.
   * @param position position of element we wish to add element before.
   * @param elem_to_add element to add.
   * @return iterator newly added element in vector.
   */
  iterator Insert(const_iterator position, const_reference elem_to_add)
  noexcept(false) {
	return Insert(position, &elem_to_add, &elem_to_add + 1);
  }

  /**
   * Erases given element from vector.
   * @param elem_to_erase element to erase.
   * @return iterator to position of element after
   */
  iterator Erase(const_iterator elem_to_erase) noexcept {
	return Erase(elem_to_erase, elem_to_erase + 1);
  }

  /**
   * Erases all elements from first (included) element pointed by given
   * iterator - to last (excluded).
   * @param first iterator to first element wished to be removed from vector.
   * @param last iterator to end of iteration of elements to remove.
   * @return iterator preceding to the first element to be erased.
   */
  iterator Erase(const_iterator first, const_iterator last) noexcept {
	size_t first_index = FindIndex(first);
	size_t amount_to_erase = std::distance(first, last);
	if (first_index==size_ || first >= last) {
	  return end();
	}
	if (size_ - amount_to_erase <= C && array_type_==kDynamic) {
	  SwitchToStackArr();
	}
	for (size_t i = first_index; i < size_; ++i) {
	  container_[i] = container_[i + amount_to_erase];
	}
	size_ -= amount_to_erase;
	return &container_[first_index];
  }

  /**
   * Inserts given element to end of vector.
   * @param elem_to_add element to add to end of vector.
   */
  void PushBack(const_reference elem_to_add) noexcept(false) {
	if (size_==capacity_) {
	  ReSizeContainer(1);
	}
	container_[size_++] = elem_to_add;
  }

  /**
   * Removes last element from vector.
   */
  void PopBack() {
	if (size_==0) {
	  return;
	}
	if (--size_ <= C && array_type_==kDynamic) {
	  SwitchToStackArr();
	}
  }

  void Clear() {
	if (array_type_==kDynamic) {
	  delete[] container_;
	  container_ = stack_container_;
	  array_type_ = kStatic;
	}
	size_ = 0;
	capacity_ = C;
  }

  // ----- Operator functions ----- //

  reference operator[](const size_t i) {
	return container_[i];
  }

  value_type operator[](const size_t i) const {
	return container_[i];
  }

  bool operator==(const VLVector<T, C> &other) {
	if (size_!=other.size_) {
	  return false;
	}
	for (size_t i = 0; i < size_; ++i) {
	  if (container_[i]!=other.container_[i]) {
		return false;
	  }
	}
	return true;
  }

  bool operator!=(const VLVector<T, C> &other) {
	return !(*this==other);
  }

  VLVector<T, C> &operator=(const VLVector<T, C> &origin) {
	if (*this==origin) {
	  return *this;
	}
	if (array_type_==kDynamic) {
	  delete[] container_;
	}
	if (origin.array_type_==kStatic) {
	  array_type_ = kStatic;
	  container_ = stack_container_;
	} else {
	  container_ = new T[origin.capacity_];
	  array_type_ = kDynamic;
	}
	capacity_ = origin.capacity_;
	size_ = origin.size_;
	CopyElements(container_, origin.container_, size_);
	return *this;
  }

};

#endif //EX6__VL_VECTOR_H_
