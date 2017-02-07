// Copyright (c) 2012 INRIA Sophia-Antipolis (France).
// Copyright (c) 2017 GeometryFactory Sarl (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Simon Giraudot, Florent Lafarge

#ifndef CGAL_CLASSIFICATION_ATTRIBUTE_DISTANCE_TO_PLANE_H
#define CGAL_CLASSIFICATION_ATTRIBUTE_DISTANCE_TO_PLANE_H

#include <vector>

#include <CGAL/Classifier.h>

namespace CGAL {

namespace Classification {

namespace Attribute {

  /*!
    \ingroup PkgClassificationAttributes

    %Attribute based on local distance to a fitted
    plane. Characterizing a level of non-planarity can help to
    identify noisy parts of the input such as vegetation. This
    attribute computes the distance of a point to a locally fitted
    plane.
    
    \tparam Geom_traits model of \cgal Kernel.
    \tparam PointRange model of `ConstRange`. Its iterator type
    is `RandomAccessIterator`.
    \tparam PointMap model of `ReadablePropertyMap` whose key
    type is the value type of the iterator of `PointRange` and value type
    is `Geom_traits::Point_3`.
    \tparam DiagonalizeTraits model of `DiagonalizeTraits` used
    for matrix diagonalization.
  */
template <typename Geom_traits, typename PointRange, typename PointMap,
          typename DiagonalizeTraits = CGAL::Default_diagonalize_traits<double,3> >
class Distance_to_plane : public Attribute_base
{
  typedef Classification::Local_eigen_analysis<Geom_traits, PointRange,
                                               PointMap, DiagonalizeTraits> Local_eigen_analysis;
#ifdef CGAL_CLASSIFICATION_PRECOMPUTE_ATTRIBUTES
  std::vector<double> distance_to_plane_attribute;
#else
  const PointRange& input;
  PointMap point_map;
  const Local_eigen_analysis& eigen;
#endif
  
public:
  /*!
    \brief Constructs the attribute.

    \param input input range.
    \param point_map property map to access the input points.
    \param eigen class with precomputed eigenvectors and eigenvalues.
  */
  Distance_to_plane (const PointRange& input,
                     PointMap point_map,
                     const Local_eigen_analysis& eigen)
#ifndef CGAL_CLASSIFICATION_PRECOMPUTE_ATTRIBUTES
    : input(input), point_map(point_map), eigen(eigen)
#endif
  {
    this->set_weight(1.);
#ifndef CGAL_CLASSIFICATION_PRECOMPUTE_ATTRIBUTES
    std::vector<double> distance_to_plane_attribute;
#endif
    
    for(std::size_t i = 0; i < input.size(); i++)
      distance_to_plane_attribute.push_back
        (CGAL::sqrt (CGAL::squared_distance (get(point_map, *(input.begin()+i)), eigen.plane(i))));
    
    this->compute_mean_max (distance_to_plane_attribute, this->mean, this->max);
    //    max *= 2;
  }

  /// \cond SKIP_IN_MANUAL
  virtual double value (std::size_t pt_index)
  {
#ifdef CGAL_CLASSIFICATION_PRECOMPUTE_ATTRIBUTES
    return distance_to_plane_attribute[pt_index];
#else
    return CGAL::sqrt (CGAL::squared_distance
                       (get(point_map, *(input.begin()+pt_index)), eigen.plane(pt_index)));
#endif
  }

  virtual std::string name() { return "distance_to_plane"; }
  /// \endcond
};

} // namespace Attribute

} // namespace Classification
  
} // namespace CGAL

#endif // CGAL_CLASSIFICATION_ATTRIBUTE_DISTANCE_TO_PLANE_H
