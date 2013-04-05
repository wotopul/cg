#include <random>
#include <string>

namespace util
{
    template<class Int>
    class RandomInt
    {
        std::random_device generator;
        std::uniform_int_distribution<Int> d;
    public:
        RandomInt(Int min, Int max) : d(min, max)
        {}

        RandomInt() : RandomInt(std::numeric_limits<Int>::min(),
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

        RandomInt& operator>> (Int& rhs)
        {
            rhs = (*this)();
            return *this;
        }
    };

    template<class Real>
    class RandomReal
    {
        std::random_device generator;
        std::uniform_real_distribution<Real> d;
    public:
        RandomReal(Real min, Real max): d(min, max)
        {}

        RandomReal(): d(std::numeric_limits<Real>::min(),
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


        RandomReal& operator>> (Real& rhs)
        {
            rhs = (*this)();
            return *this;
        }
    };

    inline std::string randomString(int length)
    {
        RandomInt<char> rand('a', 'z');
        std::string result;
        result.reserve(length);

        for (int i = 0; i < length; ++i)
        {
            result += rand();
        }

        return result;
    }
}

