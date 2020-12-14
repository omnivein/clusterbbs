#include "clustering.hxx"



/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

Cluster::Cluster(double ctr)
{
  this->ctrd = ctr;
};

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void printHeader(std::string text)
{
  using namespace std;

  char inline_decor_sym = '+';
  char underline_decor_sym = '>';
  int side_decor_size = 4;
  int inline_space_count = 2;
  int text_size = text.size();

  if (text_size < 60)
    text_size = 60;

  int complete_size = (side_decor_size * 2) + inline_space_count + text_size;
  int c = 0;

  cout << "\n\n";
  while (c++ < side_decor_size)
  {
    cout << inline_decor_sym;
  }
  cout << " " << text << " "; // inline_space_count = 2
  
  c = 0;
  while (c++ < side_decor_size)
  {
    cout << inline_decor_sym;
  }
  cout << endl;

  c = 0;
  while (c++ < complete_size)
  {
    cout << underline_decor_sym;
  }
  cout << "\n\n" << endl;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

void printClusters(std::vector<Cluster> &clist)
{
  using namespace std;

  size_t
    clustersCount = clist.size(),
    pointsCount   = 0,
    ccounter      = 0, // Cluster counter
    pcounter      = 0; // Point counter

  while (ccounter < clustersCount)
  {
    cout
      << "------------------------------------------------\n"
      << "* CLUSTER #" << ccounter 
      << ": (CENT--- " << clist[ccounter].ctrd << " ---ROID)"
      << "\n------------------------------------------------"
      << endl;

    pointsCount = clist[ccounter].points.size();
    pcounter = 0;
    
    while (pcounter < pointsCount)
    {
      cout << " | " << clist[ccounter].points[pcounter];
      ++pcounter;
    }
    cout << "\n" << endl;
    ++ccounter;
  }
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

std::vector<Cluster> clusterize(std::vector<double>& points, int k)
{
  using namespace std;

  const int randomSeed = 32432;

  // Initialize clusters' centroids and assign points from data set
  std::vector<Cluster> clist = initialize_clusters(points, randomSeed, k);
  
  // Recompute centroids and reassign points
  adjust_clusters(clist);
  
  // Print
  printHeader("REASSIGNED CLUSTERS");
  printClusters(clist);
  
  return clist;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* >> FUNCTION: absdiff
  * ----------------------------------------------------------
  * > Calculates absolute difference between two real numbers
  * ----------------------------------------------------------
  * @param &d1 - real number
  * @param &d2 - real number
  * ----------------------------------------------------------
  * @returns { double } - calculated absolute difference
  */
double absdiff(double& d1, double& d2)
{
  return abs(abs(d1) - abs(d2));
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* >> FUNCTION: reckon_centroid
  * ----------------------------------------------------------
  * > Recomputes the centroid based on points contained within cluster
  * > the centroid then takes the average value of all points
  * ----------------------------------------------------------
  * @param &cluster - cluster to recalculate centroid in
  * ----------------------------------------------------------
  * @returns { double } - calculated centroid
  */
double reckon_centroid(const Cluster& cluster)
{
  using namespace std;

  size_t size = cluster.points.size();
  size_t c = 0;
  double sed = 0;

  while (c < size)
  {
    sed += cluster.points[c];
    ++c;
  }

  return sed / size;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

/* >> FUNCTION: initialize_clusters
 * ----------------------------------------------------------
 * > Finds minimal and maximal values in a given array
 * > Initializez clusters with random means within given min and max
 * > Assigns points to the cluster with nearest centroid
 * ----------------------------------------------------------
 * @param arr - array to search edge values in
 * ----------------------------------------------------------
 * @returns struct array_edges
 */
std::vector<Cluster> initialize_clusters(
  std::vector<double>& dataList,
  const int& seed,
  const int& k
) {
  using namespace std;

  vector<Cluster> *c_ptr = new vector<Cluster>;
  vector<Cluster> &clusters = *c_ptr;
  
  double
    pointMin, pointMax,
    currExtracted;
  size_t
    c = 0, // counter
    pointsCount = dataList.size();


  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   * Search points with maximum and minimum values 
   * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
  printHeader("SEARCHING MAX and MIN POINT VALUES");

  pointMin = pointMax = dataList[0];
  while (++c < pointsCount)
  {
    currExtracted = dataList[c];

    if (pointMin > currExtracted) { pointMin = currExtracted; }
    else
    if (pointMax < currExtracted) { pointMax = currExtracted; }
  }

  cout << "MIN : " << pointMin << "\n"
       << "MAX : " << pointMax << "\n" << endl;



  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 
   * Initialize clusters with random means within min-max range 
   * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
  printHeader("ASSIGNING RANDOM CENTROIDS TO EACH CLUSTER");

  PseudoR rnd(seed, { pointMin, pointMax });
  const double MIN_CTRD_DIST = 3;
  double currentRandom = 0;
  c = -1;
  while (++c < k)
  {
    currentRandom = rnd.gen_real();
    if (c &&
        absdiff(clusters.at(c - 1).ctrd, currentRandom) < MIN_CTRD_DIST)
    {
      --c;
      continue;
    }
    clusters.emplace_back(currentRandom);
    cout << "CLUSTER #" << c << " : " << currentRandom << "\n";
  }
  cout << endl;



  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   * Assign points to each cluster
   * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
  printHeader("ASSIGNING EACH POINT TO NEAREST CLUSTER");

  size_t cc = 0; // Clusters counter
  c = 0;
  double
    lastDifference = DBL_MAX,
    currDifference = 0;

  int assignToPos = 0;
  while (c < pointsCount)
  {
    if (cc == k)
    {
      clusters[assignToPos].points.push_back(dataList[c]);
      cc = 0;
      lastDifference = DBL_MAX;
      ++c;
      continue;
    }
    currDifference = absdiff(dataList[c], clusters[cc].ctrd);
    if (lastDifference > currDifference)
    {
      assignToPos = cc;
      lastDifference = currDifference;
    }
    ++cc;
  }
  printClusters(clusters);

  return clusters;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

 /* >> FUNCTION: adjust_clusters
  * ----------------------------------------------------------
  * > Recalculates centroids for each cluster
  * > Reassigns each point to the closest cluster
  * ----------------------------------------------------------
  * @param &clusters - reference to the clusters array
  * ----------------------------------------------------------
  * @returns void
  */
void adjust_clusters(std::vector<Cluster>& clusters)
{
  using namespace std;

  size_t
    cc = 0, // cluster counter
    clustersCount = clusters.size(); 

  // Points to whether the point was reassigned to another cluster
  bool wasReassigned = 1,
       distanceChanged = 0;

  
  /* [ 1 ] >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   * > Reassign clusters
   * > Loop until there's no point reassigned 
   * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
  while (wasReassigned)
  {
    wasReassigned = 0;
    
    // Reckon centroids
    while (cc < clustersCount)
    {
      clusters[cc].ctrd = reckon_centroid(clusters[cc]);
      ++cc;
    }

    cc = 0;
    
    /* [ 2 ] >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     * Reassingn points to the closest cluster or leave unchanged
     * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
    while (cc < clustersCount)
    {
      size_t
        // Points amount inside of each cluster
        pointsCount = clusters[cc].points.size(),
        // points counter
        pc = 0,
        // Counter for nearest to the current point cluster
        suitableClusterCounter = 0,
        // Position of the found nearest cluster
        suitableClusterPos = cc;

      double
        // Current calculated distance from point to centroid
        curDistance,
        // Last nearest distance from point to centroid
        suitableDistance = absdiff(clusters[cc].points[0], clusters[cc].ctrd);

      
      /* [ 3 ] >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
       * Reassingn points in respect to the cluster with closest centroid
       * >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
      while (pc < pointsCount)
      {
        curDistance = absdiff(
          clusters[cc].points[pc],
          clusters[suitableClusterCounter].ctrd
        );
        
        /* Check if there is closer centroid and
         * remember last distance & cluster position */
        if (curDistance < suitableDistance)
        {
          suitableDistance = curDistance;
          suitableClusterPos = suitableClusterCounter;
          distanceChanged = 1;
        }
        
        ++suitableClusterCounter;
        
        // When all of the clusters are checked
        if (suitableClusterCounter == clustersCount)
        {
          if (distanceChanged && suitableClusterPos != cc)
          {
            // Move point to the cluster with nearest centroid
            vector<double> :: iterator point = clusters[cc].points.begin() + pc;
            clusters[suitableClusterPos].points.push_back(*point);
            clusters[cc].points.erase(point);
            --pointsCount;
             wasReassigned = 1;
          }
          distanceChanged = 0;
          suitableClusterPos = 0;
          suitableClusterCounter = 0;
          ++pc;
        }
      } 
      // [ 3 ] >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
      
      ++cc;
    }
    // [ 2 ] >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  }
  // [ 1 ] >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}
