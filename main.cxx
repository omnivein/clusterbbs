#include "main.hxx"

int main()
{
  using namespace std;
  PseudoR rnd(123432, { 0.0, 100050.1 });

  vector<double> test_array;
  size_t c = 0;
  int clusters_count = 5;

  // Fill the array with random data
  while (c < 100)
  {
    test_array.push_back(rnd.gen_real());
    ++c;
  }

  // Start clusterization sorting
  clusterbs(test_array, clusters_count);

  return 0;
}
