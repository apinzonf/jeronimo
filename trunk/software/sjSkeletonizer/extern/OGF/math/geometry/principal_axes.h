/*
 *  OGF/Graphite: Geometry and Graphics Programming Library + Utilities
 *  Copyright (C) 2000 Bruno Levy
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  If you modify this software, you should include a notice giving the
 *  name of the person performing the modification, the date of modification,
 *  and the reason for such modification.
 *
 *  Contact: Bruno Levy
 *
 *     levy@loria.fr
 *
 *     ISA Project
 *     LORIA, INRIA Lorraine, 
 *     Campus Scientifique, BP 239
 *     54506 VANDOEUVRE LES NANCY CEDEX 
 *     FRANCE
 *
 *  Note that the GNU General Public License does not permit incorporating
 *  the Software into proprietary programs. 
 */
 

#ifndef __MESH_TOOLS_MATH_PPAL_AXIS__
#define __MESH_TOOLS_MATH_PPAL_AXIS__

#include <OGF/math/common/common.h>
#include <OGF/math/geometry/types.h>

namespace OGF {

//_________________________________________________________

    /**
     * PrincipalAxes3d enables the center and inertia axes of
     * a cloud of 3d points to be computed.
     */
    class MATH_API PrincipalAxes3d {
    public:
        PrincipalAxes3d() ;
        void begin_points() ;
        void end_points() ;
        void point(const Point3d& p, double weight = 1.0) ;
        
        Point3d center() const ;
        const Vector3d& axis(int i) const ;
        double eigen_value(int i) const ; 
        
        
    private:
        double center_[3] ;
        Vector3d axis_[3] ;
        double eigen_value_[3] ;
        
        double M_[6] ;
        int nb_points_ ;
        double sum_weights_ ;
    } ;
    
//_________________________________________________________

    /**
     * PrincipalAxes2d enables the center and inertia axes of
     * a cloud of 2d points to be computed.
     */
    class MATH_API PrincipalAxes2d {
    public:
        PrincipalAxes2d() ;
        void begin_points() ;
        void end_points() ;
        void point(const Point2d& p, double weight = 1.0) ;
        
        Point2d center() const ;
        const Vector2d& axis(int i) const ;
        double eigen_value(int i) const ; 
        
        
    private:
        double center_[2] ;
        Vector2d axis_[2] ;
        double eigen_value_[2] ;
        
        double M_[3] ;
        int nb_points_ ;
        double sum_weights_ ;
    } ;

//_________________________________________________________

    inline Point3d PrincipalAxes3d::center() const {
        return Point3d(center_[0], center_[1], center_[2]) ;
    }

    inline const Vector3d& PrincipalAxes3d::axis(int i) const {
        ogf_assert(i >= 0 && i < 3) ;
        return axis_[i] ;
    }

    inline double PrincipalAxes3d::eigen_value(int i) const {
        ogf_assert(i >= 0 && i < 3) ;
        return eigen_value_[i] ;
    }

    //______________________________________________________

    inline Point2d PrincipalAxes2d::center() const {
        return Point2d(center_[0], center_[1]) ;
    }

    inline const Vector2d& PrincipalAxes2d::axis(int i) const {
        ogf_assert(i >= 0 && i < 2) ;
        return axis_[i] ;
    }

    inline double PrincipalAxes2d::eigen_value(int i) const {
        ogf_assert(i >= 0 && i < 2) ;
        return eigen_value_[i] ;
    }

//___________________________________________________
}

#endif

