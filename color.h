#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
    float r;
    float g;
    float b;
    inline Color(float _r,float _g,float _b) :r(_r),g(_g),b(_b) {}
    inline Color() {}
};

#endif // COLOR_H
