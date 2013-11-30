#pragma once

#include <cg/primitives/point.h>
#include <cg/operations/has_intersection/segment_segment.h>
#include <cg/operations/distance.h>

#include <vector>
#include <queue>
#include <map>
#include <algorithm>

namespace cg
{

template <class Scalar>
struct edge
{
   int vert_idx;
   Scalar dist;

   edge(int idx, Scalar dist)
      : vert_idx(idx)
      , dist(dist)
   {}
};

template <class Scalar>
void insert(point_2t<Scalar> p, std::map< point_2t<Scalar>, int > & indices,
            std::vector< point_2t<Scalar> > & points)
{
   if (indices.count(p))
      return;
   indices.insert( std::make_pair(p, points.size()) );
   points.push_back(p);
}


template <class Scalar>
std::vector< point_2t<Scalar> > shortest_path(const point_2t<Scalar> & s, const point_2t<Scalar> & f,
                                              const std::vector< segment_2t<Scalar> > & edges)
{
   // construct graph
   std::map< point_2t<Scalar>, int > indices;
   std::vector< point_2t<Scalar> > points;
   insert(s, indices, points);
   insert(f, indices, points);
   for (auto i = edges.begin(); i != edges.end(); i++)
   {
      const segment_2t<Scalar> & e = *i;
      insert(e[0], indices, points);
      insert(e[1], indices, points);
   }

   int size = points.size();
   std::vector< std::vector< edge<Scalar> > > graph(size);
   for (auto i = edges.begin(); i != edges.end(); i++)
   {
      const segment_2t<Scalar> & e = *i;
      Scalar dist = cg::distance(e[0], e[1]);
      int from = indices[e[0]], to = indices[e[1]];
      graph[from].push_back( edge<Scalar>(to, dist) );
      graph[to].push_back( edge<Scalar>(from, dist) );
   }

   int start = indices[s], finish = indices[f];


   // dijkstra
   std::vector<Scalar> d(size, INFINITY);
   d[start] = 0;
   std::vector<int> parent(size, -1);

   std::priority_queue< std::pair<Scalar, int> > queue;
   queue.push( std::make_pair(d[start], start) );
   while (!queue.empty())
   {
      int v = queue.top().second;
      Scalar v_dist = queue.top().first;
      queue.pop();
      if (-v_dist > d[v])
         continue;

      for (auto e : graph[v])
      {
         int to = e.vert_idx;
         Scalar dist = e.dist;
         if (d[v] + dist < d[to])
         {
            d[to] = d[v] + dist;
            parent[to] = v;
            queue.push(std::make_pair(-d[to], to));
         }
      }
   }

   // constructing path
   std::vector< point_2t<Scalar> > path;
   path.push_back(f);
   for (int v = finish; parent[v] != -1; v = parent[v])
   {
      path.push_back( points[parent[v]] );
   }
   std::reverse(path.begin(), path.end());
   return std::move(path);
}

}
