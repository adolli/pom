
#ifndef _POM_RANDOMGENERATOR_H_
#define _POM_RANDOMGENERATOR_H_

#include <cstdlib>
#include <ctime>
#include <vector>

using std::time;
using std::rand;
using std::srand;
using std::vector;

/**
 * @brief   随机分布产生
 *          使用vector<float>设定好分布量，在此分布量中产生序号为[0, n)的项目
 */
class RandomGenerator
{
public:
    RandomGenerator()
    {
        srand((unsigned int)time(NULL));
    }
    
    void setDistribution(const vector<float>& dist)
    {
        float sum = 0;
        for (auto p : dist)
        {
            sum += p;
        }
        for (auto p : dist)
        {
            distribute_.push_back(p / sum);
        }
    }

    int generate()
    {
        int r = rand() % 10000;
        CCLOG("[RamdomGenerator] [generate] [I] number generated: %d", r);
        int i = 0;
        float psum = 0.0;
        for (auto p : distribute_)
        {
            psum += p;
            if (r <= psum * 10000)
            {
                break;
            }
            ++i;
        }
        CCLOG("[RandomGenerator] [generator] [I] sum of probability: %f, item=%d", psum, i);
        return i;
    }

    unsigned int getCategories() const
    {
        return distribute_.size();
    }

private:
    vector<float> distribute_;
};

#endif  // _POM_RANDOMGENERATOR_H_

