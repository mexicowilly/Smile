#if !defined(SMILE_DUPLICABLE_HPP__)
#define SMILE_DUPLICABLE_HPP__

namespace smile
{

/**
 * A type of object that can produce a copy of itself. Duplicable
 * objects are typically grouped together by a shared common
 * ancestor. For example, all exception classes inherit from
 * Duplicable<Exception>.
 *
 * @headerfile smile/Duplicable.hpp
 */
template <class T>
class Duplicable
{
public:
    /**
     * @name Constructors and destructors
     */
    //@{
    /**
     * Virtual destructor that does nothing.
     */
    virtual ~Duplicable();
    //@}

    /**
     * Return a duplicate of this object.
     * @note The returned pointer is owned by the caller and must
     * be deleted.
     *
     * @return a fresh copy of this object
     */
    virtual T* duplicate() const = 0;
};

template <class T>
Duplicable<T>::~Duplicable()
{
}

}

#endif
