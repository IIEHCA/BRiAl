// -*- c++ -*-
//*****************************************************************************
/** @file CIdxPath.h
 * 
 * @author Alexander Dreyer
 * @date 2006-12-04
 *
 * This file contains the definition of a template for the storage type for
 * indices.
 *
 * @par Copyright:
 *   (c) 2006 by The PolyBoRi Team
**/
//*****************************************************************************

// get std::vector functionality
#include <vector>

// get std::iterator functionality
#include <iterator>

// include basic definitions
#include "polybori/pbori_defs.h"

// get functionals and algorithms
#include <polybori/routines/pbori_func.h>
#include <polybori/routines/pbori_algo.h>

#include <polybori/literals/CStringLiteral.h>
#include "polybori/CPrintOperation.h"

#include "polybori/CIdxVariable.h"

#ifndef CIdxPath_h_
#define CIdxPath_h_

/** @class CIdxPath
 * @brief This template class defines a storage type for monomial indices
 * and customizable "pretty" printing.
 **/
BEGIN_NAMESPACE_PBORI

template <class IdxType = CIdxVariable<CTypes::idx_type>, 
          class SeparatorType = CStringLiteral<CLiteralCodes::list_separator> >
class CIdxPath:
  public std::vector<IdxType> {

public:
  /// @name Adopt global type definitions
  //@{
  typedef IdxType idx_type;
  typedef CTypes::ostream_type ostream_type;
  //@}

  /// Type of base class
  typedef std::vector<idx_type> base;

  /// Type used to generate a string for separating elements
  typedef SeparatorType separator_type;

  /// String-like type for separator
  // typedef typename separator_type::result_type sep_value_type ;

  /// Type of *this
  typedef CIdxPath<idx_type, separator_type> self;

  /// Type for sizes
  typedef typename base::size_type size_type;

  /// Default constructor
  CIdxPath(): base() {}

  /// Construct storage for nlen indices
  CIdxPath(size_type nlen): base(nlen) {};

  /// Copy constructor
  CIdxPath(const self& rhs): base(rhs) {};

  /// Destructor
  ~CIdxPath() {};

  /// Print to out-stream
  ostream_type& print(ostream_type& os) const {

    if (base::begin() == base::end()) {
      os << 1;
    }

    special_first_transform( base::begin(), base::end(), 
                             std::ostream_iterator<idx_type>(os),
                             CPrintOperation<idx_type, separator_type>(os),
                             project_ith<1>() );

    return os;
  }

};

/// Stream output operator
template <class IdxType, class SeparatorType>
inline typename CIdxPath<IdxType, SeparatorType>::ostream_type& 
operator<<(typename CIdxPath<IdxType, SeparatorType>::ostream_type& os, 
           const CIdxPath<IdxType, SeparatorType>& storage){

  return storage.print(os);
}

END_NAMESPACE_PBORI

#endif