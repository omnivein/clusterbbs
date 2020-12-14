#include "clusterbs.hxx"
#include <thread>
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void clusterbs(
  std::vector<double> &unsorted_list,
  int clusters_count
) {
  using namespace std;

  size_t counter = 0;
  vector<Cluster> cluster_list;
  vector<thread> thread_list;

  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   * > PRINT INITIAL DATA
   * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
  printHeader("INITIAL ARRAY");
  printArray(unsorted_list);


  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   * > CLUSTERING
   * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
  cluster_list = clusterize(unsorted_list, 5);



  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   * > SORTING
   * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
  while (counter < clusters_count)
  {
    thread_list.push_back(
      thread( threadBBSort, ref(cluster_list[counter].points) )
    );
    ++counter;
  }

  // Join threads
  counter = 0;
  while (counter < clusters_count)
  {
    thread_list[counter].join();
    ++counter;
  }

  // Sort clusters by centroids in separate thread
  thread(threadClusterSort, ref(cluster_list)).join();

  printHeader("SORTED CLUSTERS");
  printClusters(cluster_list);



  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   * > SAVE AND PRINT SORTED DATA
   * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
  // Move sorted points from clusters to the initial array
  printHeader("SORTED ARRAY");
  unsorted_list.clear(); // recycle unsorted initial array
  counter = 0;
  while (counter < clusters_count)
  {
    size_t
      pc = 0,
      pointsCount = cluster_list[counter].points.size();

    while (pc < pointsCount)
    {
      unsorted_list.push_back( cluster_list[counter].points[pc] );
      ++pc;
    }
    ++counter;
  }

  // Print sorted list altogether
  counter = 0;
  printArray(unsorted_list);
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void printArray(std::vector<double> &list)
{
  using namespace std; 

  size_t counter = 0;
  while (counter < list.size())
  {
    cout << list[counter] << " | ";
    ++counter;
    if (counter != 0 && counter % 10 == 0)
      cout << endl;
  }
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void threadClusterSort(std::vector<Cluster>& clusters)
{
  int arraySize = clusters.size() - 1,
    arrayCounter = 0;
  bool
    isSorted = 0,
    lastIteration = 0;

  while (!isSorted || !lastIteration)
  {
    if (arrayCounter == arraySize)
    {
      arrayCounter = 0;
      if (isSorted)
        lastIteration = 1;
      isSorted = 1;
    }

    Cluster
      * curr = &clusters[arrayCounter],
      * next = &clusters[arrayCounter + 1];

    bool needsExchange = [](double& d1, double& d2)
    {
      return d1 > d2;
    }(curr->ctrd, next->ctrd);

    if (needsExchange)
    {
      isSorted = 0;
      Cluster tmp = *curr;
      *curr = *next;
      *next = tmp;
    }
    ++arrayCounter;
  }
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void threadBBSort(std::vector<double>& pointlist)
{
  int arraySize = pointlist.size() - 1,
    arrayCounter = 0;
  bool
    isSorted = 0,
    lastIteration = 0;

  while (!isSorted || !lastIteration)
  {
    if (arrayCounter == arraySize)
    {
      arrayCounter = 0;
      if (isSorted)
        lastIteration = 1;
      isSorted = 1;
    }

    double
      * curr = &pointlist[arrayCounter],
      * next = &pointlist[arrayCounter + 1];

    bool needsExchange = [](double& d1, double& d2)
    {
      return d1 > d2;
    }(*curr, *next);

    if (needsExchange)
    {
      isSorted = 0;
      double tmp = *curr;
      *curr = *next;
      *next = tmp;
    }
    ++arrayCounter;
  }
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */