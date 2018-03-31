#if !defined(SMILE_DELETEPOINTER_HPP__)
#define SMILE_DELETEPOINTER_HPP__

#include <functional>

namespace smile
{

template <class T>
class DeletePointer : public std::unary_function<T, void>
{
public:
    void operator() (T*& ptr);
};

template <class T>
void DeletePointer<T>::operator() (T*& ptr)
{
    delete ptr;
}

}

#endif
