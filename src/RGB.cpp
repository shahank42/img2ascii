#include "RGB.h"


RGB::RGB(double _gray)
    : r { _gray }, g { _gray }, b { _gray }
{

}

RGB::RGB(int _r, int _g, int _b)
    :   r { static_cast<double>(_r) }, g { static_cast<double>(_g) }, b { static_cast<double>(_b) }
{

}

RGB::RGB(double _r, double _g, double _b)
    :   r { _r }, g { _g }, b { _b }
{

}
