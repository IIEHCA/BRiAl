// -*- c++ -*-
//*****************************************************************************
/** @file 
 *
 * @author Alexander Dreyer
 * @date 2006-03-06
 *
 * This file contains elementary tests for getting thwe leading term
 *
 * @par Copyright:
 *   (c) 2006 by
 *   Dep. of Mathematics, Kaiserslautern University of Technology and @n
 *   Fraunhofer Institute for Industrial Mathematics (ITWM)
 *   D-67663 Kaiserslautern, Germany
 *
 * @internal 
 * @version \$Id$
 *
 * @par History:
 * @verbatim
 * $Log$
 * Revision 1.10  2006/06/07 08:37:50  dreyer
 * ADD CCuddLastIter and BooleSet::lastLexicographicalTerm()
 *
 * Revision 1.9  2006/04/25 09:30:42  dreyer
 * FIX end of CTermIterm for constants, CHANGE consistent functional names
 *
 * Revision 1.8  2006/04/24 08:40:46  dreyer
 * FIX CTermIter<> treats o/1 Monomials correctly.
 *
 * Revision 1.7  2006/04/21 16:17:12  dreyer
 * ADD template class CTermIter<>
 *
 * Revision 1.6  2006/04/19 15:55:53  dreyer
 * ADD BooleMonomial, BoolePolynomial::fetchTerms() and ::terms()
 *
 * Revision 1.5  2006/04/13 07:53:19  dreyer
 * CHANGE BoolePolynomial::print() and deg() produces more useful results
 *
 * Revision 1.4  2006/04/10 14:38:39  dreyer
 * FIX operator*= works for nontrivial lhs
 *
 * Revision 1.3  2006/04/07 16:32:08  dreyer
 * ADD dd_transform and resp. examples
 *
 * Revision 1.2  2006/04/06 15:54:50  dreyer
 * CHANGE testsuite revised
 *
 * Revision 1.1  2006/04/06 13:05:54  dreyer
 * CHANGE more suitable names for CCuddNavigator functions
 *
 * Revision 1.3  2006/03/30 11:57:11  dreyer
 * CHANGE: Made use of 0/1 constants and the sets {}, {{}} consistent
 *
 * Revision 1.2  2006/03/30 08:52:57  dreyer
 * CHANGE: testsuite revised
 *
 * Revision 1.1  2006/03/28 07:34:03  dreyer
 * + Initial Version
 *
 * @endverbatim
**/
//*****************************************************************************

// load standard iostream capapilities
#include <iostream>
#include <list>
#include <iterator>
#include <sstream>
#include <vector>

// load polybori header file
# include "polybori.h"

#include "pbori_algo.h"

#include "pbori_func.h"

#include "CTermIter.h"

#include "CCuddLastIter.h"

USING_NAMESPACE_PBORI


template <class NaviType, class TermType, class ListType>
void
find_paths(NaviType navi, const TermType& currentPath, ListType& theList ) {

  if (navi.isConstant()) {      // Reached end of path
    if (navi.terminalValue())   // Case of a valid path
      theList.push_back(currentPath);
  }
  else {
    find_paths(navi.thenBranch(), currentPath.change_idx(*navi), theList);
    find_paths(navi.elseBranch(), currentPath, theList);
  }
}


template <class IteratorType>
void print1D(IteratorType first, IteratorType last) {

  while (first != last) {
    std::cout << *first <<", ";
    ++first;
  }
}

template<class IteratorType>
void print2D(IteratorType first, IteratorType last) {

  while (first != last) {
    print1D( (*first).begin(), (*first).end());
    std::cout << std::endl;
    ++first;
  }
}


// dummy type for storing monomial data
class variables_list :
  public std::list<int> {
public:
  
  typedef std::list<int> base;
  variables_list() : base() {}
};

std::ostream& 
operator<<(std::ostream& os, const variables_list& varlist) {

   std::copy(varlist.begin(), varlist.end(), 
             std::ostream_iterator<variables_list::value_type>(os, " ") );

  return os;
}


int
main(){

  std::cout << "Testing navigating through decision diagrams" <<std::endl;   

  try {
    BoolePolyRing the_ring(5);

    BooleMonomial x = BooleVariable(0);
    BooleMonomial y = BooleVariable(1);

    BooleMonomial z = BooleVariable(2);

    BooleMonomial v = BooleVariable(3);
    BooleMonomial w = BooleVariable(4);

    BoolePolynomial poly = (z + v*w) * (x*z) + y;

    std::cout << "poly:  "<<std::endl;
    std::cout << poly <<std::endl;

    std::cout << "Navigation: "<<std::endl;

    BoolePolynomial::navigator navi = poly.navigation();

    std::list<CDDInterface<ZDD> > theList;

    dd_transform( navi, BoolePolyRing::ringOne(),  
                  std::back_inserter(theList),
                  change_idx<CDDInterface<ZDD> >() );

    std::copy(theList.begin(), theList.end(), 
              std::ostream_iterator<BoolePolynomial>(std::cout, "\n") );


    std::cout <<std::endl<< "Printing via dd_transform: "<<std::endl;
    dd_transform( navi, BoolePolyRing::ringOne(), 
                  std::ostream_iterator<BoolePolynomial>(std::cout, ""),
                  change_idx<CDDInterface<ZDD>, int> () );

    std::cout <<std::endl<< "Storing via dd_transform: "<<std::endl;

    std::list<std::list<int>  > allLists;

    dd_transform( navi, std::list<int>(),
                  std::back_inserter(allLists),
                  push_back<std::list<int> >() );

    print2D(allLists.begin(), allLists.end());

    std::cout <<std::endl<< "pure print dd_transform: "<<std::endl;

    dd_transform( navi, variables_list(),
                  std::ostream_iterator<variables_list>(std::cout, "\n"),
                  push_back<variables_list>() );
 
    std::vector<variables_list > monomvec( poly.length() );


    dd_transform( navi,  variables_list(),
                  monomvec.begin(),
                  push_back<variables_list>() );

    print1D(monomvec.begin(), monomvec.end());

    std::cout <<std::endl<<std::endl<< "Get terms: "<<std::endl;
    BoolePolynomial::termlist_type theTerms = poly.terms();
    
    print1D(theTerms.begin(), theTerms.end());

    std::cout <<std::endl <<std::endl<<"Test Term iteration "<<std::endl;

    typedef CTermIter<BooleMonomial, CCuddNavigator, 
      change_assign<BooleMonomial>, change_assign<BooleMonomial> >
      term_iterator;

       poly += x*z*w;
    std::cout << poly <<std::endl;
    term_iterator iter(poly.navigation());

    std::cout << *iter <<std::endl;

    ++iter;

    std::cout << *iter <<std::endl;

    ++iter;

    std::cout << *iter <<std::endl;

    ++iter;

    std::cout << *iter <<std::endl;
    ++iter;
    std::cout << "  last: "<< std::endl;
    std::cout << *iter <<std::endl;

    std::cout << "  next: "<< std::endl;
    poly += 1;
    std::cout << poly <<std::endl;
    iter = poly.navigation();
    ++iter;    ++iter;    ++iter;   ++iter;
    std::cout << *iter <<std::endl;
    ++iter;

    std::cout << *iter <<std::endl;

    std::cout << "  next: "<< std::endl;

    poly = 1;
    std::cout << poly <<std::endl;
    iter = poly.navigation();

    std::cout << *iter <<std::endl;
    ++iter;

    std::cout << *iter <<std::endl;

    std::cout << "  next: "<< std::endl;
    poly = 0;
    std::cout << poly <<std::endl;
    iter = poly.navigation();

    std::cout << *iter <<std::endl;
    ++iter;

    std::cout << *iter <<std::endl;
    std::cout << std::endl<< "Testing last term" <<std::endl;
    poly = x*y + x*w;

    std::cout << "poly "<<poly <<std::endl;

    CCuddLastIter start(poly.diagram().lastBegin() ), finish;

    std::cout << "last term ";
    copy(start, finish, 
         std::ostream_iterator<BoolePolynomial::idx_type>(std::cout, ", "));

    std::cout <<std::endl<<  std::endl<< "Testing last term" <<std::endl;
    poly = x*y + y*w;

    std::cout << "poly "<<poly <<std::endl;

     start=(poly.diagram().lastBegin() );

    std::cout << "last term ";
    copy(start, finish, 
         std::ostream_iterator<BoolePolynomial::idx_type>(std::cout, ", "));

    std::cout << std::endl<< std::endl<< "Testing last term" <<std::endl;
    poly = x*y + z*w;

    std::cout << "poly "<<poly <<std::endl;

     start=(poly.diagram().lastBegin() );

    std::cout << "last term ";
    copy(start, finish, 
         std::ostream_iterator<BoolePolynomial::idx_type>(std::cout, ", "));
  
    std::cout << std::endl<< std::endl<< "Testing last term" <<std::endl;
    poly = x*y*z*v*w + z*v*w + z*w;

    std::cout << "poly "<<poly <<std::endl;

     start=(poly.diagram().lastBegin() );

    std::cout << "last term ";
    copy(start, finish, 
         std::ostream_iterator<BoolePolynomial::idx_type>(std::cout, ", "));

  
    std::cout << std::endl<< std::endl<< "Testing last term" <<std::endl;
    poly = x*y*z*v*w + y*z*v*w +z*v*w + z*w;

    std::cout << "poly "<<poly <<std::endl;

     start=(poly.diagram().lastBegin() );

    std::cout << "last term ";
    copy(start, finish, 
         std::ostream_iterator<BoolePolynomial::idx_type>(std::cout, ", "));

    BooleSet bset(poly.diagram());

    std::cout << "last term via BoolSet: "<< 
      bset.lastLexicographicalTerm()<<std::endl;;

    std::cout <<std::endl<< "Finished."<<std::endl;

  }
  catch (PBoRiError& err) {
    std::cout << "  Caught error # "<< err.code() <<std::endl;   
    std::cout << "    which says: "<< err.text() <<std::endl;  
 }

  return 0;
}
