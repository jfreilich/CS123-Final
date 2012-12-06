#include "planet.h"

Planet::Planet(Triangle** mid, int numMid)
{
    m_mid = mid;
    m_numMid = numMid;


    /*std::copy(low, low[lowsize], m_low);
    std::copy(mid, mid[midsize], m_mid);
    std::copy(high, high[highsize], m_high);*/

}

Triangle** Planet::getMid(int &num) {

    num = m_numMid;
    return m_mid;

}

Planet::~Planet(){
//    if (m_low){
//        delete[] m_low;
//    }
//    if (m_mid){
//        delete[] m_mid;
//    }
//    if (m_high){
//        delete[] m_high;
//    }
}
