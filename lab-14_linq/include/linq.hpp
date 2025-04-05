#ifndef LINQ_H_
#define LINQ_H_

#include <utility>
#include <vector>

namespace linq {
    namespace impl {

        template<typename T, typename Iter>
        class range_enumerator;

        template<typename T>
        class drop_enumerator;

        template<typename T, typename U, typename F>
        class select_enumerator;

        template<typename T, typename F>
        class until_enumerator;

        template<typename T, typename F>
        class where_enumerator;

        template<typename T>
        class take_enumerator;

        template<typename T>
        class enumerator {
        public:
            virtual const T& operator*() = 0; // Получает текущий элемент.
            virtual void operator++() = 0;  // Переход к следующему элементу
            virtual operator bool() = 0;  // Возвращает true, если есть текущий элемент

            auto drop(int count) {
                return drop_enumerator<T>(*this, count);
            }

            template<typename U = T, typename F>
            auto select(F func) {
                return select_enumerator<U, T, F>(*this, std::move(func));
            }

            template<typename F>
            auto until(F func) {
                return until_enumerator<T, F>(*this, std::move(func));
            }

            auto until_eq(T val) {
                const auto func = [val](const T& x) { return x == val; };
                return until_enumerator<T, decltype(func)>(*this, func);
            }

            template<typename F>
            auto where(F func) {
                return where_enumerator<T, F>(*this, std::move(func));
            }

            auto where_neq(T val) {
                const auto func = [val](const T& x) { return x != val; };
                return where_enumerator<T, decltype(func)>(*this, func);
            }

            auto take(int k) {
                return take_enumerator<T>(*this, k);
            }

            std::vector<T> to_vector() {
                std::vector<T> result;
                while(operator bool()){
                    result.push_back(std::move(operator*()));
                    operator++();
                };
                return result;
            }

            template<typename Iter>
            void copy_to(Iter it) {
                while(operator bool()){
                    *it = std::move(operator*());
                    operator++();
                    ++it;
                };
            }
        };

        template<typename T, typename Iter>
        class range_enumerator : public enumerator<T> {
        public:
            range_enumerator(Iter begin, Iter end) : begin_(begin), end_(end) {}
            const T& operator*() override {
                return *begin_;
            }
            void operator++() override {
                ++begin_;
            }
            operator bool() override {
                return begin_ != end_;
            }
        private:
            Iter begin_, end_;
        };

        template<typename T>
        class drop_enumerator : public enumerator<T> {
        public:
            drop_enumerator(enumerator<T> &parent, int count) : parent_(parent) {
                for (int i = 0; i < count; ++i) {
                    if (static_cast<bool>(parent_)) {
                        ++parent_;
                    }
                }
            }
            const T& operator*() override {
                return *parent_;
            }
            void operator++() override {
                ++parent_;
            }
            operator bool() override {
                return static_cast<bool>(parent_);
            }
        private:
            enumerator<T> &parent_;
        };

        template<typename T>
        class take_enumerator : public enumerator<T> {
        public:
            take_enumerator(enumerator<T> &parent, int count) : parent_(parent), count_(count) {}
            const T& operator*() override {
                return *parent_;
            }
            void operator++() override {
                ++parent_;
                --count_;
            }
            operator bool() override {
                return static_cast<bool>(parent_) && count_ > 0;
            }
        private:
            enumerator<T> &parent_;
            int count_;
        };

        template<typename T, typename U, typename F>
        class select_enumerator : public enumerator<T> {
        public:
            select_enumerator(enumerator<U> &parent, F func) : parent_(parent), func_(std::move(func)) {}
            const T& operator*() override {
                t = func_(*parent_);
                return t;
            }
            void operator++() override {
                ++parent_;
            }
            operator bool() override {
                return static_cast<bool>(parent_);
            }

        private:
            enumerator<U> &parent_;
            F func_;
            T t;
        };

        template<typename T, typename F>
        class until_enumerator : public enumerator<T> {
        public:
            until_enumerator(enumerator<T> &parent, F predicate) : parent_(parent), predicate_(std::move(predicate)) {}
            const T& operator*() override {
                return *parent_;
            }
            void operator++() override {
                ++parent_;
            }
            operator bool() override {
                if (static_cast<bool>(parent_) && predicate_(*parent_)) {
                    return false;
                }

                return static_cast<bool>(parent_);
            }
        private:
            enumerator<T> &parent_;
            F predicate_;
        };

        template<typename T, typename F>
        class where_enumerator : public enumerator<T> {
        public:
            where_enumerator(enumerator<T> &parent, F predicate) : parent_(parent), predicate_(std::move(predicate)) {
                while (static_cast<bool>(parent_) && !predicate_(*parent_)) {
                    ++parent_;
                }
            }
            const T& operator*() override {
                return *parent_;
            }
            void operator++() override {
                if (static_cast<bool>(parent_)) {
                    ++parent_;
                    while (static_cast<bool>(parent_) && !predicate_(*parent_)) {
                        ++parent_;
                    }
                }
            }
            operator bool() override {
                return static_cast<bool>(parent_);
            }
        private:
            enumerator<T> &parent_;
            F predicate_;
        };
    } // namespace impl

    template<typename T>
    auto from(T begin, T end) {
        return impl::range_enumerator< typename std::iterator_traits<T>::value_type, T>(begin, end);
    }

} // namespace linq

#endif
