#ifndef __CLUSTERING_H__
#define __CLUSTERING_H__

  #include <iostream>
  #include <vector>
  #include <iterator>
  #include <cstdlib>
  #include <string>

  #include "pseudor.hxx"



/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

  /* >> CLASS: Cluster
   * ----------------------------------------------------------
   * > Represents cluster which have centroid - center of the cluster
   * > and contains points list
   */
  class Cluster
  {
  public:
    double ctrd; // Centroid
    std::vector<double> points;

    Cluster(double ctr);
  };

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

  void printHeader(std::string text);

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

  void printClusters(std::vector<Cluster>& clist);

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

  double reckon_centroid(const Cluster& cluster);

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

  double absdiff(double& d1, double& d2);

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

  /* >> FUNCTION: clusterize
   * ----------------------------------------------------------
   * > Calls all needed functions to conduct clusterizing datalist
   * ----------------------------------------------------------
   */
  std::vector<Cluster> clusterize(std::vector<double>& points, int k);

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

  /* >> FUNCTION: initialize_clusters
   * ----------------------------------------------------------
   * > Finds minimal and maximal values in a given array
   * > Initializez clusters with random means within given min and max
   * > Assigns points to the cluster with nearest centroid
   * ----------------------------------------------------------
   * @param dataList - array to clusterize
   * @param seed     - seed combination for random generator to use
   * @param k        - amount of clusters to create
   * ----------------------------------------------------------
   * @returns vector<Clusters>
   */
  std::vector<Cluster> initialize_clusters
  (
    std::vector<double>& dataList,
    const int& seed,
    const int& k
  );

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
  void adjust_clusters(std::vector<Cluster>& clusters);

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

#endif // __CLUSTERING_H__