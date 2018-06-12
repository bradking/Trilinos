// @HEADER
// ************************************************************************
//
//               Rapid Optimization Library (ROL) Package
//                 Copyright (2014) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact lead developers:
//              Drew Kouri   (dpkouri@sandia.gov) and
//              Denis Ridzal (dridzal@sandia.gov)
//
// ************************************************************************
// @HEADER

#include "Teuchos_GlobalMPISession.hpp"
#include "ROL_Stream.hpp"
#include "ROL_DynamicConstraintCheck.hpp"

#include "VdP_DynamicConstraint.hpp"


using RealT = double;

int main( int argc, char* argv[] ) {

  using ROL::Ptr;
  using ROL::makePtr;

  using vector = std::vector<RealT>;
  using SV     = ROL::StdVector<RealT>;

  Teuchos::GlobalMPISession mpiSession(&argc, &argv);  

  // This little trick lets us print to std::cout only if a (dummy) command-line argument is provided.
  auto outStream = ROL::makeStreamPtr( std::cout, argc > 1 );    
  int errorFlag  = 0;

  auto uo = makePtr<SV>( makePtr<vector>(2) );
  auto un = makePtr<SV>( makePtr<vector>(2) );
  auto z  = makePtr<SV>( makePtr<vector>(1) );
    
  ROL::RandomizeVector<RealT>(*uo);
  ROL::RandomizeVector<RealT>(*un);
  ROL::RandomizeVector<RealT>(*z);

  VdP::DynamicConstraint<RealT> dyn_con;

  ROL::ValidateFunction<RealT> validator( 1, 13, 20, 11, true, *outStream);

  ROL::DynamicConstraintCheck<RealT>::check( dyn_con, validator, *uo, *un, *z, 
    { "applyJacobian_uo",
      "applyJacobian_un",
      "applyJacobian_z",
      "applyAdjointJacobian_uo",
      "applyAdjointJacobian_un",
      "applyAdjointJacobian_z",
      "applyInverseJacobian_un",
      "applyInverseAdjointJacobian_un",
      "applyAdjointHessian_uo_uo",
      "applyAdjointHessian_un_un"} );

  if (errorFlag != 0) std::cout << "End Result: TEST FAILED\n";
  else                std::cout << "End Result: TEST PASSED\n";

  return 0;
}
