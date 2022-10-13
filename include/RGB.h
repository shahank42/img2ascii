#ifndef RGB_H
#define RGB_H

struct RGB
{
    double r { };
    double g { };
    double b { };

    RGB(double _gray);
    RGB(int _r, int _g, int _b);
    RGB(double _r, double _g, double _b);
};

#endif // RGB_H
