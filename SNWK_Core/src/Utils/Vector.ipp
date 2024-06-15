// ---- LIFECYCLE ----

template <typename T>
Vector<T>::Vector() noexcept
: f_capacity(0), f_size(0), arr(nullptr)
{}

template <typename T>
Vector<T>::Vector(const int64_t starting_capacity)
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
    delete[] arr;
}

// ---- OPERATORS ----

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other)
{
    if (this != other)
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
T& Vector<T>::operator[] (const int64_t index)
{
    return arr[index];
}

template <typename T>
const T& Vector<T>::operator[] (const int64_t index) const
{
    return arr[index];
}

// ---- METHODS ----

template <typename T>
const int64_t Vector<T>::size () const noexcept
{
    return f_size;
}

template <typename T>
const int64_t Vector<T>::capacity () const noexcept
{
    return f_capacity;
}

template <typename T>
T& Vector<T>::at (const int64_t index)
{
    if (size() == 0 || arr == nullptr)
        throw std::logic_error("Vector.at: Array is empty.");
    
    if (index < 0 || index >= size())
        throw std::invalid_argument("Vector.at: Given index outside of vector.");

    return arr[index];
}

template <typename T>
const T& Vector<T>::at (const int64_t index) const
{
    if (size() == 0 || arr == nullptr)
        throw std::logic_error("Vector.at: Array is empty.");
    
    if (index < 0 || index >= size())
        throw std::invalid_argument("Vector.at: Given index outside of vector.");

    return arr[index];
}

template <typename T>
int64_t Vector<T>::binary_search (const T& search_arg) const noexcept
{
    int64_t lowLim = 0;
    int64_t upLim = f_size - 1;

    while (lowLim <= upLim)
    {
        int64_t middle = lowLim + ((upLim - lowLim) / 2);
        if (arr[middle] == search_arg)
            return middle;

        if (search_arg > arr[middle])
            lowLim = middle + 1;

        if (search_arg < arr[middle])
            upLim = middle - 1;
    }

    return -1;
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
void Vector<T>::pop_back ()
{
    f_size--;
}

template <typename T>
void Vector<T>::reserve (const int64_t capacity)
{
    if (capacity <= this->capacity())
        return;

    T* buffer = new T[capacity];

    try
    {
        for (int64_t i = 0; i < size(); i++)
            buffer[i] = arr[i];
    }
    catch (...)
    {
        delete[] buffer;
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

    for (int64_t i = 0; i < other.size(); i++)
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
    f_capacity = 0;
    f_size = 0;
}

// ---- PRIVATE METHODS ----

template <typename T>
void Vector<T>::copy (const Vector<T> &other)
{
    Vector<T> buffer;
    buffer.reserve(other.capacity());
    
    for (int64_t i = 0; i < other.f_size; i++)
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