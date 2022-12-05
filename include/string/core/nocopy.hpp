namespace String
{
template<class T>
class NonCopyable
{
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
public:
    NonCopyable(const NonCopyable&) = delete;
    T& operator=(const T&) = delete;
};
} // namespace String
