#pragma once
#include <ktu/template.hpp>
#include <tuple>
namespace ktu {
    class variadic {
        private:
            struct impl {
                template <size_t N, typename... Ts>
                static constexpr auto getType() {
                    if constexpr (N >= sizeof...(Ts)) {
                        return empty{};
                    } else {
                        typename ::std::tuple_element<N, ::std::tuple<Ts...>>::type var;
                        return var;
                    }
                }
            };
        public:

            template <size_t N, typename... Ts>
            struct getType {
                using type = decltype(impl::getType<N, Ts...>());
            };

            template <typename... Ts>
            struct types {
                template <size_t N>
                using get = typename ::std::tuple_element<N, ::std::tuple<Ts...>>;

                template <typename T, size_t N>
                using is_same = typename ::std::is_same<typename get<N>::type, T>;
                
                
                static constexpr size_t size = sizeof...(Ts);

                
                template <template <typename A, typename B> class comparer, size_t Start, typename... Second>
                struct compare_at {
                    private:
                        template<size_t... TIndicies>
                        static constexpr bool comparison(::std::index_sequence<TIndicies...>) {
                            //static_assert(!(comparer<typename getType<0,Ts...>::type, typename getType<0,Second...>::type>::value), "Hello");
                            return (comparer<typename getType<TIndicies + Start,Ts...>::type,typename getType<TIndicies,Second...>::type>::value && ...);
                        }
                        static constexpr bool check() {
                            if constexpr (size >= sizeof...(Second) && size-Start >= sizeof...(Second)) {
                                return comparison(::std::make_index_sequence<sizeof...(Second)>{});
                            } else {
                                return false;
                            }
                        };
                    public:
                        static constexpr bool value = check();
                };
                template <template <typename A, typename B> class comparer, typename... Second>
                struct compare : compare_at<comparer, 0, Second...> {};
                
                template <size_t Start, typename... Second>
                struct same_at : compare_at<::std::is_same, Start, Second...> {};

                template <typename... Second>
                struct starts_with : same_at<0, Second...> {};
                template <typename... Second>
                struct ends_with : same_at<size-sizeof...(Second), Second...> {};

                template <typename... Second>
                struct same_as {
                    private:
                        static constexpr bool check() {
                            if constexpr (size == sizeof...(Second)) {
                                return (::std::is_same<Ts, Second>::value && ...);
                            } else {
                                return false;
                            }
                        }
                    public:
                        static constexpr bool value = check();
                };
                
            };
            template <size_t I, class... Ts>
            static inline constexpr decltype(auto) get(Ts&&... ts) {
                if constexpr (I >= sizeof...(Ts)) {
                    return empty{};
                } else {
                    return ::std::get<I>(::std::forward_as_tuple(ts...));
                }
            }


            template <typename T, typename ...Args>
            struct first {
                using type = T;
            };
    };
};