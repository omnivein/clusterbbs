#ifndef __CLUSTERBS_H__
#define __CLUSTERBS_H__

  #include <iostream>
  #include <vector>
  #include <thread>

  #include "clustering.hxx"

  void clusterbs(
    std::vector<double> &unsorted_list,
    int clusters_count
  );
  void printArray(std::vector<double> &list);
  void threadClusterSort(std::vector<Cluster> &clusters);
  void threadBBSort(std::vector<double> &pointlist);

#endif // __CLUSTERBS_H__