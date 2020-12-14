#include "pseudor.hxx"

/* >> Class: PseudoR >> */
/* ========================================================================= */
// PRIVATE
void PseudoR::setDefaults()
{
  this->setSeed(599572); // Default seed
  this->setIntRange({ 0, 10 }); // Default int range
  this->setRealRange({ 0.0, 10.0 }); // Default real range
}

// PUBLIC
PseudoR::PseudoR()
{
  this->setDefaults();
}

PseudoR::PseudoR(int seed)
{
  this->setSeed(seed);
};

PseudoR::PseudoR(int seed, const int(&range)[2])
{
  this->setSeed(seed);
  this->setIntRange(range);
}

PseudoR::PseudoR(int seed, const double(&range)[2])
{
  this->setSeed(seed);
  this->setRealRange(range);
}

void PseudoR::setSeed(int seed) { this->m_gen.seed(seed); }
void PseudoR::setIntRange(const int(&range)[2])
{
  this->int_distrib.param(
    std::uniform_int_distribution<int>(range[0], range[1]).param()
  );
}

void PseudoR::setRealRange(const double(&range)[2])
{
  this->real_distrib.param(
    std::uniform_real_distribution<double>(range[0], range[1]).param()
  );
}

int    PseudoR::gen_int() { return this->int_distrib(this->m_gen); }
double PseudoR::gen_real() { return this->real_distrib(this->m_gen); }
