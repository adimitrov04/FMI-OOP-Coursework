// ---- LIFECYCLE ----

template <typename T>
Vector<T>::Vector() noexcept
: f_capacity(0), f_size(0), arr(nullptr)
{}

template <typename T>
Vector<T>::Vector(const uint64_t starting_capacity)
: Vector<T>()
{
    reserve(starting_capacity);
}

template <typename T>
Vector<T>::Vector(Vector<T> &&other) noexcept
: Vector<T>()
{
    move(std::move(other));
}

template <typename T>
Vector<T>::Vector(const Vector<T> &other)
: Vector<T>()
{
    copy(other);
}

template <typename T>
Vector<T>::~Vector() noexcept
{
    clear(); // i am losing my mind
}

// ---- OPERATORS ----

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other)
{
    if (this != &other)
        copy(other);

    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
{
    if (this != &other)
    {
        move(std::move(other));
    }

    return *this;
}

template <typename T>
T& Vector<T>::operator[] (const uint64_t index)
{
    return arr[index];
}

template <typename T>
const T& Vector<T>::operator[] (const uint64_t index) const
{
    return arr[index];
}

// ---- METHODS ----

template <typename T>
const uint64_t Vector<T>::size () const noexcept
{
    return f_size;
}

template <typename T>
const uint64_t Vector<T>::capacity () const noexcept
{
    return f_capacity;
}

template <typename T>
T& Vector<T>::at (const uint64_t index)
{
    if (size() == 0 || arr == nullptr)
        throw std::logic_error("Vector.at: Array is empty.");
    
    if (index < 0 || index >= size())
        throw std::invalid_argument("Vector.at: Given index outside of vector.");

    return arr[index];
}

template <typename T>
const T& Vector<T>::at (const uint64_t index) const
{
    if (size() == 0 || arr == nullptr)
        throw std::logic_error("Vector.at: Array is empty.");
    
    if (index < 0 || index >= size())
        throw std::invalid_argument("Vector.at: Given index outside of vector.");

    return arr[index];
}

template <typename T>
T* Vector<T>::front () const
{
    if (!arr)
        throw std::domain_error("Vector.front: Cannot get element since vector is empty.");

    return arr;
}

template <typename T>
T* Vector<T>::back () const
{
    if (!arr)
        throw std::domain_error("Vector.back: Cannot get element since vector is empty.");

    return arr + (f_size - 1);
}

/**
 * @param search_arg The element to look for, passed by value
 * 
 * @return a pointer to an element inside the vertor
 * @return nullptr if no such element exists
 */
template <typename T>
T* Vector<T>::binary_search (T& search_arg) const noexcept
{
    if (f_size == 0 || arr == nullptr)
        return nullptr;

    if (f_size == 1)
        return (*arr == search_arg) ? arr : nullptr;

    uint64_t lowLim = 0;
    uint64_t upLim = (f_size - 1);

    while (lowLim <= upLim)
    {
        uint64_t middle = lowLim + ((upLim - lowLim) / 2);
        if (arr[middle] == search_arg)
            return (arr + middle);

        if (search_arg > arr[middle])
            lowLim = middle + 1;

        if (search_arg < arr[middle])
            upLim = middle - 1;
    }

    return nullptr;
}

template <typename T>
bool Vector<T>::is_sorted () const noexcept
{
    return sort::is_sorted(arr, f_size);
}

template <typename T>
void Vector<T>::push_back (const T& element)
{
    if (capacity() == 0)
        reserve(DEFAULT_STARTING_CAPACITY);

    if (size() == capacity())
        reserve(f_capacity * 2);
    
    arr[f_size] = element;
    f_size++;
}

template <typename T>
void Vector<T>::push_sorted (const T& element)
{
    if (is_sorted() == false)
        throw std::logic_error("Vector.push_sorted: Current array needs to be sorted to perform this action.");

    push_back(element);
    for (uint64_t i = f_size - 1; i > 0; i--)
    {
        if (arr[i - 1] <= element)
        {
            for (uint64_t j = (f_size - 1); j > i; j--)
                arr[j] = arr[j - 1];

            arr[i] = element;
            break;
        }
    }
}

template <typename T>
void Vector<T>::pop_back ()
{
    f_size--;
}

template <typename T>
void Vector<T>::pop (const uint64_t index)
{
    for (uint64_t i = index; i < (f_size - 1); i++)
        arr[i] = arr[i + 1];

    f_size--;
}

template <typename T>
void Vector<T>::reserve (const uint64_t capacity)
{
    if (capacity <= this->capacity())
        return;

    T* buffer = new T[capacity];

    try
    {
        for (uint64_t i = 0; i < size(); i++)
            buffer[i] = arr[i];
    }
    catch (...)
    {
        delete[] buffer;
        buffer = nullptr;
        throw;
    }

    delete[] arr;
    arr = buffer;
    f_capacity = capacity;
}

template <typename T>
void Vector<T>::append (const Vector<T> &other)
{
    if (other.size() == 0)
        return;
    
    if (size() == 0)
    {
        copy(other);
        return;
    }

    reserve(this->size() + other.size());

    for (uint64_t i = 0; i < other.size(); i++)
    {
        push_back(other[i]);
    }
}

template <typename T>
template <typename SortAlgorithm>
void Vector<T>::sort (const SortAlgorithm& sorter)
{
    sorter(arr, f_size);
}

template <typename T>
void Vector<T>::clear () noexcept
{
    delete[] arr;
    arr = nullptr;
    f_capacity = 0;
    f_size = 0;
}

// ---- PRIVATE METHODS ----

template <typename T>
void Vector<T>::copy (const Vector<T> &other)
{
    Vector<T> buffer;
    buffer.reserve(other.capacity());
    
    for (uint64_t i = 0; i < other.f_size; i++)
        buffer.push_back(other[i]);

    this->clear();
    this->arr = buffer.arr;
    buffer.arr = nullptr;

    f_capacity = buffer.capacity();
    f_size = buffer.size();
}

template <typename T>
void Vector<T>::move (Vector<T> &&other) noexcept
{
    this->arr = other.arr;
    other.arr = nullptr;

    f_capacity = other.f_capacity;
    other.f_capacity = 0;

    f_size = other.f_size;
    other.f_size = 0;
}