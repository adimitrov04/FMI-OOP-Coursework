// ---- LIFECYCLE ----

template <typename T>
Vector<T>::Vector() noexcept
: fcapacity(0), fsize(0), arr(nullptr)
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
    return fsize;
}

template <typename T>
const uint64_t Vector<T>::capacity () const noexcept
{
    return fcapacity;
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

    return arr + (fsize - 1);
}

template <typename T>
T* Vector<T>::basic_search (const T& search_arg) const noexcept
{
    for (uint64_t i = 0; i < fsize; i++)
    {
        if (arr[i] == search_arg)
            return (arr + i);
    }

    return nullptr;
}

/**
 * @param search_arg The element to look for, passed by value
 * 
 * @warning Function works properly ONLY when the vector is sorted. Undefined behaviour if not sorted.
 * @warning May invoke undefined behaviour if the template type's comparison operators
 * compare on multiple criteria at the same time. Use with caution.
 * 
 * @return a pointer to an element inside the vertor
 * @return nullptr if no such element exists
 */
template <typename T>
T* Vector<T>::binary_search (const T& search_arg) const
{
    if ( arr == nullptr || fsize == 0)
        return nullptr;

    if (fsize == 1)
        return (*arr == search_arg) ? arr : nullptr;

    uint64_t lowLim = 0;
    uint64_t upLim = (fsize - 1);

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
uint64_t Vector<T>::count_repetitions (const T& element) const
{
    uint64_t count(0);

    for (uint64_t i = 0; i < fsize; i++)
    {
        if (arr[i] == element)
            count++;
    }

    return count;
}

template <typename T>
bool Vector<T>::is_sorted () const noexcept
{
    if (arr == nullptr || fsize <= 1)
        return true;

    return sort::is_sorted(arr, fsize);
}

template <typename T>
void Vector<T>::push_back (const T& element)
{
    if (capacity() == 0)
        reserve(DEFAULT_STARTING_CAPACITY);

    if (size() == capacity())
        reserve(fcapacity * 2);
    
    arr[fsize] = element;
    fsize++;
}

template <typename T>
void Vector<T>::push_sorted (const T& element)
{
    if (is_sorted() == false)
        throw std::logic_error("Vector.push_sorted: Current array needs to be sorted to perform this action.");

    push_back(element);
    for (uint64_t i = fsize - 1; i > 0; i--)
    {
        if (arr[i - 1] <= element)
        {
            for (uint64_t j = (fsize - 1); j > i; j--)
                arr[j] = arr[j - 1];

            arr[i] = element;
            break;
        }
    }
}

template <typename T>
void Vector<T>::pop_back ()
{
    fsize--;
}

template <typename T>
void Vector<T>::pop (const uint64_t index)
{
    for (uint64_t i = index; i < (fsize - 1); i++)
        arr[i] = arr[i + 1];

    fsize--;
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
    fcapacity = capacity;
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
    sorter(arr, fsize);
}

template <typename T>
void Vector<T>::clear () noexcept
{
    delete[] arr;
    arr = nullptr;
    fcapacity = 0;
    fsize = 0;
}

// ---- PRIVATE METHODS ----

template <typename T>
void Vector<T>::copy (const Vector<T> &other)
{
    Vector<T> buffer;
    buffer.reserve(other.capacity());
    
    for (uint64_t i = 0; i < other.fsize; i++)
        buffer.push_back(other[i]);

    this->clear();
    this->arr = buffer.arr;
    buffer.arr = nullptr;

    fcapacity = buffer.capacity();
    fsize = buffer.size();
}

template <typename T>
void Vector<T>::move (Vector<T> &&other) noexcept
{
    this->arr = other.arr;
    other.arr = nullptr;

    fcapacity = other.fcapacity;
    other.fcapacity = 0;

    fsize = other.fsize;
    other.fsize = 0;
}