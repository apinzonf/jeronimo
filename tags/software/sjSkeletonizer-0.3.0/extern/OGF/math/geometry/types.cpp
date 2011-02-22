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
 
#include <OGF/math/geometry/types.h>
#include <OGF/math/geometry/shewchuk.h>

namespace OGF {

//_________________________________________________________


    namespace Geom {
        
        double triangle_beauty(
            const Point2d& p1, const Point2d& p2, const Point2d& p3
        ) {
            Vector2d p1p2 = p2 - p1 ;
            Vector2d p2p3 = p3 - p2 ;
            Vector2d p3p1 = p1 - p3 ;
            double l12 = ::sqrt(p1p2 * p1p2) ;
            double l23 = ::sqrt(p2p3 * p2p3) ;
            double l31 = ::sqrt(p3p1 * p3p1) ;
            double d = 0.5 * (l12 + l23 + l31) ;
            return 8.0 * (d - l12) * (d - l23) * (d - l31) / (l12 * l23 * l31) ;
        }

        double triangle_beauty(
            const Point3d& p1, const Point3d& p2, const Point3d& p3
        ) {
            Vector3d p1p2 = p2 - p1 ;
            Vector3d p2p3 = p3 - p2 ;
            Vector3d p3p1 = p1 - p3 ;
            double l12 = ::sqrt(p1p2 * p1p2) ;
            double l23 = ::sqrt(p2p3 * p2p3) ;
            double l31 = ::sqrt(p3p1 * p3p1) ;
            double d = 0.5 * (l12 + l23 + l31) ;
            return 
                8.0 * (d - l12) * (d - l23) * (d - l31) / (l12 * l23 * l31) ;
        }


        static inline void perp(const Point2d& p1, const Point2d& p2, Point2d& p, Vector2d& v) {
            v = p2 - p1 ;
            v = Vector2d(-v.y(), v.x()) ;
            p = barycenter(p1,p2) ;
        }

        Point2d triangle_circum_center(
            const Point2d& q1, const Point2d& q2, const Point2d& q3
        ) {
            Point2d p1,p2 ;
            Vector2d v1,v2 ;
            perp(q1,q2,p1,v1) ;
            perp(q1,q3,p2,v2) ;
            return segments_intersection(p1,v1,p2,v2) ;
        }

        Sign orient(
            const Point2d& p1_in, const Point2d& p2_in, const Point2d& p3_in
        ) {
            double p1[2] ;
            double p2[2] ;
            double p3[2] ;

            p1[0] = p1_in.x() ; p1[1] = p1_in.y() ;
            p2[0] = p2_in.x() ; p2[1] = p2_in.y() ;
            p3[0] = p3_in.x() ; p3[1] = p3_in.y() ;

            return ogf_sgn(::orient2d(p1, p2, p3)) ;
        }
    
        bool segments_intersect(
            const Point2d& p1, const Point2d& p2,
            const Point2d& p3, const Point2d& p4
        ) {
            if(orient(p1,p2,p3) == orient(p1,p2,p4)) {
                return false ;
            }

            if(orient(p3,p4,p1) == orient(p3,p4,p2)) {
                return false ;
            }

            return true ;
        }    

        Vector3d perpendicular(const Vector3d& V) {
            int min_index = 0 ;
            double c = ::fabs(V[0]) ;
            double cur = ::fabs(V[1]) ;
            if(cur < c) {
                min_index = 1 ;
                c = cur ;
            }
            cur = ::fabs(V[2]) ;
            if(cur < c) {
                min_index = 2 ;
            }
            Vector3d result ;
            switch(min_index) {
            case 0:
                result = Vector3d(0, -V.z(), V.y()) ;
                break ;
            case 1:
                result = Vector3d(V.z(), 0, -V.x()) ;
                break ;
            case 2:
                result = Vector3d(-V.y(), V.x(), 0) ;
                break ;
            }
            return result ;
        }

        inline double det3x3(
            double a00,  double a01,  double a02,
            double a10,  double a11,  double a12,
            double a20,  double a21,  double a22
        ) {
            double m01 = a00*a11 - a10*a01;
            double m02 = a00*a21 - a20*a01;
            double m12 = a10*a21 - a20*a11;
            return m01*a22 - m02*a12 + m12*a02;
        }

        Point3d tetra_circum_center(
            const Point3d& p, const Point3d& q, 
            const Point3d& r, const Point3d& s
        ) {
            Vector3d qp = q-p ;
            double qp2 = qp.norm2() ;
            Vector3d rp = r-p ;
            double rp2 = rp.norm2() ;
            Vector3d sp = s-p ;
            double sp2 = sp.norm2() ;
            
            double num_x = det3x3(
                qp.y(),qp.z(),qp2,
                rp.y(),rp.z(),rp2,
                sp.y(),sp.z(),sp2
            );

            double num_y = det3x3(
                qp.x(),qp.z(),qp2,
                rp.x(),rp.z(),rp2,
                sp.x(),sp.z(),sp2
            ) ;

            double num_z = det3x3(
                qp.x(),qp.y(),qp2,
                rp.x(),rp.y(),rp2,
                sp.x(),sp.y(),sp2
            ) ;

            double den = det3x3(
                qp.x(),qp.y(),qp.z(),
                rp.x(),rp.y(),rp.z(),
                sp.x(),sp.y(),sp.z()
            ) ;

            ogf_assert(::fabs(den) > 1e-30) ;

            den *= 2.0 ;
            
            return Point3d(
                p.x() + num_x / den,
                p.y() - num_y / den,
                p.z() + num_z / den 
            ) ;

        }


    }

//_________________________________________________________

}

