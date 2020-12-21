#ifndef UTILITIES_NULLABLE_DEF_HXX
#define UTILITIES_NULLABLE_DEF_HXX

/*
The idea to implement this class was my own. Taking my knowledge of C#.
The code to define this was taking from:
https://stackoverflow.com/questions/2537942/nullable-values-in-c
*/

template <typename T>
class Nullable final{
private:

public:
    Nullable();
    Nullable(const T &value);
    Nullable(nullptr_t nullpointer);
    const Nullable<T> & operator=(const Nullable<T> &value);
    const Nullable<T> & operator=(const T &value);
    const Nullable<T> & operator=(nullptr_t nullpointer);
    bool HasValue() const;
    const T & GetValueOrDefault() const;
    const T & GetValueOrDefault(const T &default) const;
    bool TryGetValue(T &value) const;

public:
    class NullableValue final{
    public:
        friend class Nullable;

    private:
        NullableValue();
        NullableValue(const T &value);

    public:
        NullableValue & operator=(const NullableValue &) = delete;
        operator const T &() const;
        const T & operator*() const;
        const T * operator&() const;

        // https://stackoverflow.com/questions/42183631/inability-to-overload-dot-operator-in-c
        const T * operator->() const;

    public:
        template <typename T2>
        friend bool operator==(const Nullable<T2> &op1, const Nullable<T2> &op2);

        template <typename T2>
        friend bool operator==(const Nullable<T2> &op, const T2 &value);

        template <typename T2>
        friend bool operator==(const T2 &value, const Nullable<T2> &op);

        template <typename T2>
        friend bool operator==(const Nullable<T2> &op, nullptr_t nullpointer);

        template <typename T2>
        friend bool operator!=(const Nullable<T2> &op1, const Nullable<T2> &op2);

        template <typename T2>
        friend bool operator!=(const Nullable<T2> &op, const T2 &value);

        template <typename T2>
        friend bool operator!=(const T2 &value, const Nullable<T2> &op);

        template <typename T2>
        friend bool operator==(nullptr_t nullpointer, const Nullable<T2> &op);

        template <typename T2>
        friend bool operator!=(const Nullable<T2> &op, nullptr_t nullpointer);

        template <typename T2>
        friend bool operator!=(nullptr_t nullpointer, const Nullable<T2> &op);

    private:
        void checkHasValue() const;

    private:
        bool m_hasValue;
        T m_value;
    };

public:
    NullableValue Value;
};

#endif
