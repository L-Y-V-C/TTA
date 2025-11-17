#ifndef MPAIR_HH
#define MPAIR_HH

template <typename T1, typename T2>
class Mpair{
public:
    Mpair() : first(T1()), second(T2()){}
    Mpair(const T1& inFirst, const T2& inSecond) :
        first(inFirst), second(inSecond){}

    T1 first;
    T2 second;
};

#endif
