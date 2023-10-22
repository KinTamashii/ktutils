#pragma once
#include <random>


namespace ktu {
    template <class Engine, class Seeder = std::random_device>
    class random_base {
        private:
            static Engine &engine() {
                thread_local Engine engine{Seeder{}()};
                return engine;
            }
        public:

            template <class Distribution>
            static auto use(Distribution &&distribution) {
                return distribution(engine());
            }

            template <std::integral IntType = int>
            static IntType integer(IntType __a = 0, IntType __b = std::numeric_limits<IntType>::max()) {
                return use(std::uniform_int_distribution<IntType>(__a, __b));
            }

            template <std::floating_point RealType = double>
            static RealType floating(RealType __a = 0.0, RealType __b = 1.0) {
                return use(std::uniform_real_distribution<RealType>(__a, __b));
            }

            static bool boolean(double __p = 0.5) {
                return use(std::bernoulli_distribution(__p));
            }

            template <std::integral IntType = int>
            static IntType binomial(IntType __t, double __p = 0.5) {
                return use(std::binomial_distribution<IntType>(__t, __p));
            }

            template <std::integral IntType = int>
            static IntType geometric(double __p = 0.5) {
                return use(std::geometric_distribution<IntType>(__p));
            }

            template <std::integral IntType = int>
            static IntType negative_binomial(IntType __t = 1, double __p = 0.5) {
                return use(std::negative_binomial_distribution<IntType>(__t, __p));
            }

            template <std::integral IntType = int>
            static IntType poisson(double __mean = 1.0) {
                return use(std::poisson_distribution<IntType>(__mean));
            }

            template <std::floating_point RealType = double>
            static RealType exponential(RealType __lambda = 1.0) {
                return use(std::exponential_distribution(__lambda));
            }

            template <std::floating_point RealType = double>
            static RealType gamma(RealType __alpha = 1.0, RealType __beta = 1.0) {
                return use(std::gamma_distribution(__alpha, __beta));
            }

            template <std::floating_point RealType = double>
            static RealType weibull(RealType __a = 1.0, RealType __b = 1.0) {
                return use(std::weibull_distribution(__a, __b));
            }

            template <std::floating_point RealType = double>
            static RealType extreme_value(RealType __a = 0.0, RealType __b = 1.0) {
                return use(std::extreme_value_distribution<RealType>(__a, __b));
            }

            template <std::floating_point RealType = double>
            static RealType normal(RealType __mean = 0.0, RealType __stddev = 1.0) {
                return use(std::normal_distribution<RealType>(__mean, __stddev));
            }

            template <std::floating_point RealType = double>
            static RealType lognormal(RealType __m = 0.0, RealType __s = 1.0) {
                return use(std::lognormal_distribution<RealType>(__m, __s));
            }

            template <std::floating_point RealType = double>
            static RealType chi_squared(RealType __n = 1.0) {
                return use(std::chi_squared_distribution<RealType>(__n));
            }

            template <std::floating_point RealType = double>
            static RealType cauchy(RealType __a = 0.0, RealType __b = 1.0) {
                return use(std::cauchy_distribution<RealType>(__a, __b));
            }

            template <std::floating_point RealType = double>
            static RealType fisher_f(RealType __m = 0.0, RealType __n = 1.0) {
                return use(std::fisher_f_distribution<RealType>(__m, __n));
            }
            template <std::floating_point RealType = double>
            static RealType student_t(RealType __n = 1.0) {
                return use(std::student_t_distribution<RealType>(__n));
            }
            
            template <std::integral IntType = int>
            static IntType discrete() {
                return use(std::discrete_distribution<IntType>());
            }
            template <typename IntType = void, class InputIterator>
            requires (std::integral<IntType> || std::is_same<IntType, void>::value)
            static auto discrete(InputIterator __first, InputIterator __last) {
                return use(
                    std::discrete_distribution<
                        typename std::conditional<std::is_same<IntType, void>::value,
                            decltype(*__first),
                            IntType>::type
                    >(__first, __last)
                );
            }
            template <std::integral IntType = int>
            static IntType discrete(std::initializer_list<double> __il) {
                return use(std::discrete_distribution<IntType>(__il));
            }
            template <std::integral IntType = int, class UnaryOperation>
            static IntType discrete(size_t __nw, double __xmin, double __xmax, UnaryOperation __fw) {
                return use(std::discrete_distribution<IntType>(__nw, __xmin, __xmax, __fw));
            }

            template <std::floating_point RealType = double>
            static RealType piecewise_constant() {
                return use(std::piecewise_constant_distribution());
            }
            template <typename RealType = void, class InputIteratorB, class InputIteratorW>
            requires (std::floating_point<RealType> || std::is_same<RealType, void>::value)
            static auto piecewise_constant(InputIteratorB __firstB, InputIteratorB __lastB, InputIteratorW __firstW) {
                return use(std::piecewise_constant_distribution<
                    typename std::conditional<std::is_same<RealType, void>::value,
                        decltype(*__firstB),
                        RealType>::type
                >(__firstB, __lastB, __firstW));
            }
            template <std::floating_point RealType = double, class UnaryOperation>
            static RealType piecewise_constant(std::initializer_list<double> __il, UnaryOperation __fw) {
                return use(std::piecewise_constant_distribution<RealType>(__il, __fw));
            }
            template <std::floating_point RealType = double, class UnaryOperation>
            static RealType piecewise_constant(size_t __nw, double __xmin, double __xmax, UnaryOperation __fw) {
                return use(std::piecewise_constant_distribution<RealType>(__nw, __xmin, __xmax, __fw));
            }

            template <std::floating_point RealType = double>
            static RealType piecewise_linear() {
                return use(std::piecewise_linear_distribution());
            }
            template <typename RealType = void, class InputIteratorB, class InputIteratorW>
            requires (std::floating_point<RealType> || std::is_same<RealType, void>::value)
            static auto piecewise_linear(InputIteratorB __firstB, InputIteratorB __lastB, InputIteratorW __firstW) {
                return use(std::piecewise_linear_distribution<
                    typename std::conditional<std::is_same<RealType, void>::value,
                        decltype(*__firstB),
                        RealType>::type
                >(__firstB, __lastB, __firstW));
            }
            template <std::floating_point RealType = double, class UnaryOperation>
            static RealType piecewise_linear(std::initializer_list<double> __il, UnaryOperation __fw) {
                return use(std::piecewise_linear_distribution<RealType>(__il, __fw));
            }
            template <std::floating_point RealType = double, class UnaryOperation>
            static RealType piecewise_linear(size_t __nw, double __xmin, double __xmax, UnaryOperation __fw) {
                return use(std::piecewise_linear_distribution<RealType>(__nw, __xmin, __xmax, __fw));
            }


            static constexpr typename Engine::result_type (min)() {
                return (Engine::min)();
            }

            static constexpr typename Engine::result_type (max)() {
                return (Engine::max)();
            }

            static void discard(const unsigned long long __z) {
                engine().discard(__z);
            }

            static void reseed() {
                Seeder seeder;
                seed(seeder());
            }
            
            static void seed(const typename Engine::result_type __value = Engine::default_seed) {
                engine().seed(__value);
            }

            template <class SeedSequence>
            static void seed(SeedSequence &__sequence) {
                engine().seed(__sequence);
            }

            static typename Engine::result_type get() {
                return engine()();
            }

            static bool equivalent(const Engine &other) {
                return engine() == other;
            }

            template<typename CharT, typename Traits>
            static void serialize(std::basic_ostream<CharT, Traits>& ost) {
                ost << engine();
            }

            template<typename CharT, typename Traits>
            static void deserialize(std::basic_istream<CharT, Traits>& ist) {
                ist >> engine();
            }

            static Engine &reference() {
                return engine();
            }
            static Engine copy() {
                return engine();
            }

            template <std::random_access_iterator RandomIt>
            static void shuffle(RandomIt __first, RandomIt __last) {
                std::shuffle(__first, __last, engine());
            }

            template <class Container>
            static void shuffle(Container &container) {
                std::shuffle(std::begin(container), std::end(container), engine());
            }

            template <class InputIt>
            static InputIt position(InputIt __first, InputIt __last) {
                return std::next(__first, integer((ptrdiff_t)0, std::distance(__first, __last)-1));
            }
            template <class Container>
            static auto position(Container &container) {
                return position(std::begin(container), std::end(container));
            }
            template <class InputIt>
            static auto &choice(InputIt __first, InputIt __last) {
                return *position(__first, __last);
            }
            template <class Container>
            static auto &choice(Container &container) {
                return *position(container);
            }
    };


    template <class Seeder = std::random_device>
    using minstd_rand = ktu::random_base<std::minstd_rand, Seeder>;

    template <class Seeder = std::random_device>
    using minstd_rand0 = ktu::random_base<std::minstd_rand0, Seeder>;

    template <class Seeder = std::random_device>
    using mt19937 = ktu::random_base<std::mt19937, Seeder>;

    template <class Seeder = std::random_device>
    using mt19937_64 = ktu::random_base<std::mt19937_64, Seeder>;

    template <class Seeder = std::random_device>
    using mt19937_size = typename std::conditional<sizeof(size_t) == 8, mt19937_64<Seeder>, mt19937<Seeder>>::type;

    template <class Seeder = std::random_device>
    using ranlux24_base = ktu::random_base<std::ranlux24_base, Seeder>;

    template <class Seeder = std::random_device>
    using ranlux48_base = ktu::random_base<std::ranlux48_base, Seeder>;

    template <class Seeder = std::random_device>
    using ranlux24 = ktu::random_base<std::ranlux24, Seeder>;

    template <class Seeder = std::random_device>
    using ranlux48 = ktu::random_base<std::ranlux48, Seeder>;

    template <class Seeder = std::random_device>
    using knuth_b = ktu::random_base<std::knuth_b, Seeder>;
};
