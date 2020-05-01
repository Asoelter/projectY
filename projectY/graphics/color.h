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

    static constexpr auto White() { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
    static constexpr auto Black() { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
    static constexpr auto Red() { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
    static constexpr auto Green() { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
    static constexpr auto Blue() { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
    static constexpr auto LightGrey() { return Color(0.2f, 0.3f, 0.4f); }
};

#endif //COLOR_H