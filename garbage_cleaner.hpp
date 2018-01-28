#if !defined(SMILE_GARBAGE_CLEANER_HPP__)
#define SMILE_GARBAGE_CLEANER_HPP__

#include <functional>
#include <stack>
#include <mutex>

namespace smile
{

class garbage_cleaner
{
public:
    typedef std::function<void()> cleaner_type;

    static garbage_cleaner& get();

    ~garbage_cleaner();

    void add(cleaner_type cln);

private:
    std::stack<cleaner_type> cleaners_;
    std::mutex guard_;
};

}

#endif
