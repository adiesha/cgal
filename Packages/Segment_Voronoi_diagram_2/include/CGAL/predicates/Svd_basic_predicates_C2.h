// ======================================================================
//
// Copyright (c) 2003 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : 
// release_date  : 
//
// file          : include/CGAL/predicates/Svd_basic_predicates_C2.h
// package       : Segment_Voronoi_diagram_2
// source        : $RCSfile$
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>
//
// coordinator   :
//
// ======================================================================



#ifndef CGAL_SEGMENT_VORONOI_DIAGRAM_BASIC_PREDICATES_C2_H
#define CGAL_SEGMENT_VORONOI_DIAGRAM_BASIC_PREDICATES_C2_H


#include <CGAL/enum.h>
#include <CGAL/Number_type_traits.h>
#include <CGAL/predicates/Square_root_1.h>
#include <CGAL/predicates/Square_root_2.h>



CGAL_BEGIN_NAMESPACE


template<class K>
struct Svd_basic_predicates_C2
{
public:
  //-------------------------------------------------------------------
  // TYPES
  //-------------------------------------------------------------------

  typedef typename K::RT         RT;
  typedef typename K::FT         FT;
  typedef typename K::Point_2    Point_2;
  typedef typename K::Segment_2  Segment_2;
  typedef typename K::Site_2     Site_2;

  typedef CGAL::Square_root_1<RT>       Sqrt_1;
  typedef CGAL::Square_root_2<RT>       Sqrt_2;
  typedef CGAL::Square_root_2<Sqrt_1>   Sqrt_3;

  typedef typename Number_type_traits<RT>::Has_sqrt  RT_Has_sqrt;
  typedef typename Number_type_traits<FT>::Has_sqrt  FT_Has_sqrt;

  static RT_Has_sqrt rt_has_sqrt;
  static FT_Has_sqrt ft_has_sqrt;

  class Line_2
  {
  private:
    RT a_, b_, c_;

  public:
    Line_2() : a_(1), b_(0), c_(0) {}
    Line_2(const RT& a, const RT& b, const RT& c)
      : a_(a), b_(b), c_(c) {}

    RT a() const { return a_; }
    RT b() const { return b_; }
    RT c() const { return c_; }

  };

  class Homogeneous_point_2
  {
  private:
    RT hx_, hy_, hw_;

  public:
    Homogeneous_point_2() : hx_(0), hy_(0), hw_(1) {}
    Homogeneous_point_2(const RT& hx, const RT& hy, const RT& hw)
      :  hx_(hx), hy_(hy), hw_(hw)
    {
      CGAL_precondition( !(CGAL::is_zero(hw_)) );
    }

    Homogeneous_point_2(const Point_2& p)
      : hx_(p.x()), hy_(p.y()), hw_(1) {}

    Homogeneous_point_2(const Homogeneous_point_2& other)
      : hx_(other.hx_), hy_(other.hy_), hw_(other.hw_) {}

    RT hx() const { return hx_; }
    RT hy() const { return hy_; }
    RT hw() const { return hw_; }

    FT x() const { return hx_ / hw_; }
    FT y() const { return hy_ / hw_; }
  };

public:
  //-------------------------------------------------------------------
  // CONVERSIONS
  //-------------------------------------------------------------------

  static FT compute_sqrt(const FT& x, Tag_true)
  {
    return CGAL::sqrt( x );
  }

  static FT compute_sqrt(const FT& x, Tag_false)
  {
    return FT(  CGAL::sqrt( CGAL::to_double(x) )  );
  }

  static
  FT to_ft(const Sqrt_1& x)
  {
    //    FT sqrt_c = CGAL::sqrt(x.c());
    FT sqrt_c = compute_sqrt( x.c(), ft_has_sqrt );
    return x.a() + x.b() * sqrt_c;
  }

  static
  FT to_ft(const Sqrt_3& x)
  {
    //    FT sqrt_e = CGAL::sqrt( to_ft(x.e()) );
    //    FT sqrt_f = CGAL::sqrt( to_ft(x.f()) );
    FT sqrt_e = compute_sqrt( to_ft(x.e()), ft_has_sqrt );
    FT sqrt_f = compute_sqrt( to_ft(x.f()), ft_has_sqrt );
    FT sqrt_ef = sqrt_e * sqrt_f;
    return to_ft(x.a()) + to_ft(x.b()) * sqrt_e
      + to_ft(x.c()) * sqrt_f + to_ft(x.d()) * sqrt_ef;
  }

public:
  //-------------------------------------------------------------------
  // BASIC CONSTRUCTIONS
  //-------------------------------------------------------------------

  static
  Line_2 compute_supporting_line(const Segment_2& s)
  {
#if 1
    RT a, b, c;
    compute_supporting_line(s, a, b, c);
#else
    RT a = s.source().y() - s.target().y();
    RT b = s.target().x() - s.source().x();
    RT c = s.source().x() * s.target().y()
      - s.target().x() * s.source().y();
#endif
    return Line_2(a, b, c);
  }

  static
  void compute_supporting_line(const Segment_2& s,
			       RT& a, RT& b, RT& c)
  {
    a = s.source().y() - s.target().y();
    b = s.target().x() - s.source().x();
    c = s.source().x() * s.target().y() - s.target().x() * s.source().y();
  }

  static
  Homogeneous_point_2
  compute_projection(const Line_2& l, const Point_2& p)
  {
    Homogeneous_point_2 hp;

    RT ab = l.a() * l.b();

    RT hx = CGAL::square(l.b()) * p.x()
      - ab * p.y() - l.a() * l.c();
    RT hy = CGAL::square(l.a()) * p.y()
      - ab * p.x() - l.b() * l.c();
    RT hw = CGAL::square(l.a()) + CGAL::square(l.b());

    return Homogeneous_point_2(hx, hy, hw);
  }


  static
  Homogeneous_point_2
  projection_on_line(const Line_2& l, const Point_2& p)
  {
    RT ab = l.a() * l.b();

    RT hx = CGAL::square(l.b()) * p.x()
      - ab * p.y() - l.a() * l.c();
    RT hy = CGAL::square(l.a()) * p.y()
      - ab * p.x() - l.b() * l.c();
    RT hw = CGAL::square(l.a()) + CGAL::square(l.b());

    return Homogeneous_point_2(hx, hy, hw);
  }


  static
  Homogeneous_point_2
  midpoint(const Point_2& p1, const Point_2& p2)
  {
    RT hx = p1.x() + p2.x();
    RT hy = p1.y() + p2.y();
    RT hw = RT(2);

    return Homogeneous_point_2(hx, hy, hw);
  }

  static
  Homogeneous_point_2
  midpoint(const Homogeneous_point_2& p1,
           const Homogeneous_point_2& p2)
  {
    RT hx = p1.hx() * p2.hw() + p2.hx() * p1.hw();
    RT hy = p1.hy() * p2.hw() + p2.hy() * p1.hw();
    RT hw = RT(2) * p1.hw() * p2.hw();

    return Homogeneous_point_2(hx, hy, hw);
  }

  static
  Line_2 compute_perpendicular(const Line_2& l, const Point_2& p)
  {
    RT a, b, c;
    a = -l.b();
    b = l.a();
    c = l.b() * p.x() - l.a() * p.y();
    return Line_2(a, b, c);
  }

  static
  Line_2 opposite_line(const Line_2& l)
  {
    return Line_2(-l.a(), -l.b(), -l.c());
  }

  static
  RT compute_squared_distance(const Point_2& p, const Point_2& q)
  {
    return CGAL::square(p.x() - q.x()) + CGAL::square(p.y() - q.y());
  }

  static
  std::pair<RT,RT>
  compute_squared_distance(const Point_2& p, const Line_2& l)
  {
    RT d2 = CGAL::square(l.a() * p.x() + l.b() * p.y() + l.c());
    RT n2 = CGAL::square(l.a()) + CGAL::square(l.b());
    return std::pair<RT,RT>(d2, n2);
  }

public:
  //-------------------------------------------------------------------
  // BASIC PREDICATES
  //-------------------------------------------------------------------
  static
  Comparison_result
  compare_squared_distances_to_line(const Line_2& l, const Point_2& p,
                                    const Point_2& q)
  {
    RT d2_lp = CGAL::square(l.a() * p.x() + l.b() * p.y() + l.c());
    RT d2_lq = CGAL::square(l.a() * q.x() + l.b() * q.y() + l.c());

    return CGAL::compare(d2_lp, d2_lq);
  }

  static
  Comparison_result
  compare_squared_distances_to_lines(const Point_2& p,
				     const Line_2& l1,
                                     const Line_2& l2)
  {
    RT d2_l1 = CGAL::square(l1.a() * p.x() + l1.b() * p.y() + l1.c());

    RT d2_l2 = CGAL::square(l2.a() * p.x() + l2.b() * p.y() + l2.c());

    RT n1 = CGAL::square(l1.a()) + CGAL::square(l1.b());
    RT n2 = CGAL::square(l2.a()) + CGAL::square(l2.b());

    return CGAL::compare(d2_l1 * n2, d2_l2 * n1);
  }

  static
  Oriented_side
  oriented_side_of_line(const Line_2& l, const Point_2& p)
  {
    Sign s = CGAL::sign(l.a() * p.x() + l.b() * p.y() + l.c());
    if ( s == ZERO ) { return ON_ORIENTED_BOUNDARY; }
    return ( s == POSITIVE ) ? ON_POSITIVE_SIDE : ON_NEGATIVE_SIDE;
  }

  static
  Oriented_side
  oriented_side_of_line(const Line_2& l, const Homogeneous_point_2& p)
  {
    Sign s1 =
      CGAL::sign(l.a() * p.hx() + l.b() * p.hy() + l.c() * p.hw());
    Sign s_hw = CGAL::sign(p.hw());

    Sign s = Sign(s1 * s_hw);

    if ( s == ZERO ) { return ON_ORIENTED_BOUNDARY; }
    return ( s == POSITIVE ) ? ON_POSITIVE_SIDE : ON_NEGATIVE_SIDE;
  }


  static
  bool is_on_positive_halfspace(const Line_2& l, const Segment_2& s)
  {
    Oriented_side os1, os2;

    os1 = oriented_side_of_line(l, s.source());
    os2 = oriented_side_of_line(l, s.target());

    return ( (os1 == ON_POSITIVE_SIDE && os2 != ON_NEGATIVE_SIDE) ||
	     (os1 != ON_NEGATIVE_SIDE && os2 == ON_POSITIVE_SIDE) );
  }

};

template<class K>
typename Svd_basic_predicates_C2<K>::RT_Has_sqrt
Svd_basic_predicates_C2<K>::rt_has_sqrt =
Svd_basic_predicates_C2<K>::RT_Has_sqrt();

template<class K>
typename Svd_basic_predicates_C2<K>::FT_Has_sqrt
Svd_basic_predicates_C2<K>::ft_has_sqrt =
Svd_basic_predicates_C2<K>::FT_Has_sqrt();

CGAL_END_NAMESPACE


#endif // CGAL_SEGMENT_VORONOI_DIAGRAM_BASIC_PREDICATES_C2_H
