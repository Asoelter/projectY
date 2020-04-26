#ifndef COLOR_H
#define COLOR_H

struct Color
{
    constexpr Color(float r, float g, float b, float a = 1.0f)
        : red(r)
        , green(g)
        , blue(b)
        , alpha(a)
    {

    }

    union
    {
        struct
        {
            float red;
            float green;
            float blue;
            float alpha;
        };

        float data[4];
    };
};

#endif //COLOR_H