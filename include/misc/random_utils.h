#include <random>
#include <string>

namespace util
{
    template<class Int>
    class uniform_random_int
    {
        std::random_device generator;
        std::uniform_int_distribution<Int> d;
    public:
        uniform_random_int(Int min, Int max) : d(min, max)
        {}

        uniform_random_int() : uniform_random_int(std::numeric_limits<Int>::min(),
                                    std::numeric_limits<Int>::max())
        {}

        Int operator() ()
        {
            return d(generator);
        }

        void reset(Int min, Int max)
        {
            d = std::uniform_int_distribution<Int>(min, max);
        }

        uniform_random_int& operator>> (Int& rhs)
        {
            rhs = (*this)();
            return *this;
        }
    };

    template<class Real>
    class uniform_random_real
    {
        std::random_device generator;
        std::uniform_real_distribution<Real> d;
    public:
        uniform_random_real(Real min, Real max): d(min, max)
        {}

        uniform_random_real(): d(std::numeric_limits<Real>::min(),
                            std::numeric_limits<Real>::max())
        {}

        void reset(Real min, Real max)
        {
            d = std::uniform_real_distribution<double>(min, max);
        }

        double operator() ()
        {
            return d(generator);
        }


        uniform_random_real& operator>> (Real& rhs)
        {
            rhs = (*this)();
            return *this;
        }
    };

    inline std::string randomString(int length)
    {
        uniform_random_int<char> rand('a', 'z');
        std::string result;
        result.reserve(length);

        for (int i = 0; i < length; ++i)
        {
            result += rand();
        }

        return result;
    }
}

