#ifndef __PSEUDOR_H__
#define __PSEUDOR_H__

#include <random>

/** Class: PseudoR
 * --------------------------------------------------
 * > Implements pseudo-random generator's "functionality"
 * -------------------------------------------------- */
class PseudoR
{
private:
  std::uniform_int_distribution<int> int_distrib;
  std::uniform_real_distribution<double> real_distrib;
  std::mt19937 m_gen;

  void setDefaults();

public:

  PseudoR();
  PseudoR(int seed);
  PseudoR(int, const int(&)[2]);
  PseudoR(int, const double(&)[2]);

  void setSeed(int seed);
  void setIntRange(const int(&)[2]);
  void setRealRange(const double(&)[2]);

  int    gen_int();
  double gen_real();
};

#endif