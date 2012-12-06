#include "planet.h"

Planet::Planet(triangle_t* low,int lowsize, triangle_t* mid,int midsize, triangle_t* high, int highsize)
{
    std::copy(low, low[lowsize], m_low);
    std::copy(mid, mid[midsize], m_mid);
    std::copy(high, high[highsize], m_high);

}


Planet::~Planet(){
    if (m_low){
        delete[] m_low;
    }
    if (m_mid){
        delete[] m_mid;
    }
    if (m_high){
        delete[] m_high;
    }
}

void Planet::displace_surface(){

}

triangle_t* Planet ::get_tessellation(int tessellation){

}
