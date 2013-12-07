#pragma once

#include <gmpxx.h>

#include <cg/primitives/point.h>
#include <cg/triangulation/cell.h>

#include <boost/numeric/interval.hpp>
#include <boost/optional.hpp>

#include <array>

namespace cg
{

   enum configuration_t
   {
      CG_IN, CG_ON, CG_OUT
   };

   struct in_circle_d
   {
      boost::optional<configuration_t> operator() (point_2 const& a, point_2 const& b,
                                                   point_2 const& c, point_2 const& d) const
      {
         double a11 = a.x - d.x, a12 = a.y - d.y, a13 = (a.x * a.x - d.x * d.x) + (a.y * a.y - d.y * d.y);
         double a21 = b.x - d.x, a22 = b.y - d.y, a23 = (b.x * b.x - d.x * d.x) + (b.y * b.y - d.y * d.y);
         double a31 = c.x - d.x, a32 = c.y - d.y, a33 = (c.x * c.x - d.x * d.x) + (c.y * c.y - d.y * d.y);
         double d1 = a11 * a22 * a33 - a11 * a23 * a32;
         double d2 = a12 * a21 * a33 - a12 * a23 * a31;
         double d3 = a13 * a21 * a32 - a13 * a22 * a31;
         double res = d1 - d2 + d3;
         double eps = (fabs(d1) + fabs(d2) + fabs(d3)) * 16 * std::numeric_limits<double>::epsilon();

         if (res > eps)
         {
            return CG_IN;
         }

         if (res < -eps)
         {
            return CG_OUT;
         }

         return boost::none;
      }
   };

   struct in_circle_i
   {
      boost::optional<configuration_t> operator() (point_2 const& a, point_2 const& b,
                                                   point_2 const& c, point_2 const& d) const
      {
         typedef boost::numeric::interval_lib::unprotect<boost::numeric::interval<double> >::type interval;

         boost::numeric::interval<double>::traits_type::rounding _;
         interval a11 = interval(a.x) - d.x, a12 = interval(a.y) - d.y,
               a13 = (interval(a.x) * a.x - interval(d.x) * d.x) + (interval(a.y) * a.y - interval(d.y) * d.y);
         interval a21 = interval(b.x) - d.x, a22 = interval(b.y) - d.y,
               a23 = (interval(b.x) * b.x - interval(d.x) * d.x) + (interval(b.y) * b.y - interval(d.y) * d.y);
         interval a31 = interval(c.x) - d.x, a32 = interval(c.y) - d.y,
               a33 = (interval(c.x) * c.x - interval(d.x) * d.x) + (interval(c.y) * c.y - interval(d.y) * d.y);
         interval d1 = a11 * a22 * a33 - a11 * a23 * a32;
         interval d2 = a12 * a21 * a33 - a12 * a23 * a31;
         interval d3 = a13 * a21 * a32 - a13 * a22 * a31;
         interval res = d1 - d2 + d3;

         if (res.lower() > 0)
         {
            return CG_IN;
         }

         if (res.upper() < 0)
         {
            return CG_OUT;
         }

         if (res.upper() == res.lower())
         {
            return CG_ON; // ?
         }

         return boost::none;
      }
   };

   struct in_circle_r
   {
      boost::optional<configuration_t> operator() (point_2 const& a, point_2 const& b,
                                                   point_2 const& c, point_2 const& d) const
      {
         mpq_class a11 = mpq_class(a.x) - d.x, a12 = mpq_class(a.y) - d.y,
               a13 = (mpq_class(a.x) * a.x - mpq_class(d.x) * d.x) + (mpq_class(a.y) * a.y - mpq_class(d.y) * d.y);
         mpq_class a21 = mpq_class(b.x) - d.x, a22 = mpq_class(b.y) - d.y,
               a23 = (mpq_class(b.x) * b.x - mpq_class(d.x) * d.x) + (mpq_class(b.y) * b.y - mpq_class(d.y) * d.y);
         mpq_class a31 = mpq_class(c.x) - d.x, a32 = mpq_class(c.y) - d.y,
               a33 = (mpq_class(c.x) * c.x - mpq_class(d.x) * d.x) + (mpq_class(c.y) * c.y - mpq_class(d.y) * d.y);
         mpq_class d1 = a11 * a22 * a33 - a11 * a23 * a32;
         mpq_class d2 = a12 * a21 * a33 - a12 * a23 * a31;
         mpq_class d3 = a13 * a21 * a32 - a13 * a22 * a31;
         mpq_class res = d1 - d2 + d3;

         int cres = cmp(res, 0);

         if (cres > 0)
         {
            return CG_IN;
         }
         else if (cres < 0)
         {
            return CG_OUT;
         }

         return CG_ON;
      }
   };

   inline configuration_t in_circle(point_2 const& a, point_2 const& b, point_2 const& c, point_2 const& d)
   {
      /*if (boost::optional<bool> v = in_circle_d()(a, b, c, d))
      {
         return *v;
      }*/

      if (boost::optional<configuration_t > v = in_circle_i()(a, b, c, d))
      {
         return *v;
      }

      return *in_circle_r()(a, b, c, d);

   }

   template <class Scalar>
   inline configuration_t in_circle(vertex_p<Scalar> a, vertex_p<Scalar> b,
                                    vertex_p<Scalar> c, vertex_p<Scalar> d)
   {
      if (d->inf)
         return CG_OUT;

      std::array<vertex_p<Scalar>, 3> f( {{a, b, c}} );
      bool inf = false;
      for (int i = 0; i < 3; i++)
      {
         if (!f[i]->inf)
            continue;
         inf = true;
         if (orientation(f[(i + 1) % 3]->p, f[(i + 3 - 1) % 3]->p, d->p) == CG_LEFT)
            return CG_IN;
      }
      if (inf)
         return CG_OUT;

      point_2 ap = a->p, bp = b->p, cp = c->p, dp = d->p;
      return in_circle(ap, bp, cp, dp);

   }

   template <class Scalar>
   bool non_delaunay(edge_p<Scalar> e)
   {
      vertex_p<Scalar> a = e->begin, b = e->end(), c = e->next->end(), d = e->twin->next->end();
      return in_circle(a, b, c, d) == CG_IN || in_circle(b, a, d, c) == CG_IN;
   }

}
