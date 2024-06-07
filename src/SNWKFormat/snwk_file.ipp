
// ---- LIFECYCLE ----

template <class ObjectType>
snwk::SNWKFile<ObjectType>::SNWKFile () noexcept
: metadata(ObjectType::TYPE_FCC)
{}

template <class ObjectType>
snwk::SNWKFile<ObjectType>::SNWKFile (const char* file_path)
: SNWKFile()
{
    open(file_path);
}

template <class ObjectType>
snwk::SNWKFile<ObjectType>::~SNWKFile () noexcept
{
    close();
}

// ---- OPERATORS ----

template <class ObjectType>
bool snwk::SNWKFile<ObjectType>::operator! () noexcept
{
    return !is_good();
}

// ---- METHODS ----

template <class ObjectType>
void snwk::SNWKFile<ObjectType>::create (const char* file_path)
{
    file_stream.open(file_path, std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::binary);
    if (!file_stream)
        throw std::runtime_error("SNWKFile.create: File could not be created.");

    metadata.set_state(state::good);
    metadata.serialize(file_stream);
}

template <class ObjectType>
void snwk::SNWKFile<ObjectType>::open (const char* file_path)
{
    file_stream.open(file_path, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    if (!file_stream)
        throw std::runtime_error("SNWKFile.ctor: File could not be opened or does not exist.");

    try
    {
        metadata.validate(file_stream);
    }
    catch (...)
    {
        file_stream.close();
        throw;
    }

    file_stream.seekg(metadata.get_size(), std::ios_base::beg);
}

template <class ObjectType>
void snwk::SNWKFile<ObjectType>::close () noexcept
{
    if (get_state() != state::bad)
        metadata.write_state(file_stream, state::closed);
    
    file_stream.close();
    metadata.clear();
}

template <class ObjectType>
void snwk::SNWKFile<ObjectType>::read_object (ObjectType& outObj)
{
    if (file_stream.eof() || file_stream.peek() == EOF)
        throw std::runtime_error("SNWKFile.read_object: Tried to read past end of file.");
    
    outObj.Deserialize(file_stream);
}

template <class ObjectType>
void snwk::SNWKFile<ObjectType>::write_object (const ObjectType& inObj)
{
    file_stream.seekp(0, std::ios_base::end);
    inObj.Serialize(file_stream);
    metadata.sign_new_record(file_stream);
}

template <class ObjectType>
bool snwk::SNWKFile<ObjectType>::is_good () noexcept
{
    return file_stream.good() && (get_state() == state::good) && file_stream.peek() != EOF;
}

template <class ObjectType>
snwk::state snwk::SNWKFile<ObjectType>::get_state () const noexcept
{
    return metadata.get_state();
}

template <class ObjectType>
uint64_t snwk::SNWKFile<ObjectType>::get_object_count () const noexcept
{
    return metadata.get_count();
}