#ifndef OPENSIM_SCAPULOTHORACIC_JOINT_H_ 
#define OPENSIM_SCAPULOTHORACIC_JOINT_H_
/* -------------------------------------------------------------------------- *
 *                     OpenSim:  ScapulothoracicJoint.h                       *
 * -------------------------------------------------------------------------- *
 * ScapulothoracicJoint is offered as an addition to the OpenSim API          *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
* Copyright (c) 2005-2017 Stanford University and the Authors                *
 * Author(s): Ajay Seth                                                       *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

// INCLUDE
#include "osimPluginDLL.h"
#include <OpenSim/Simulation/SimbodyEngine/Joint.h>

namespace OpenSim {

//=============================================================================
//=============================================================================
/**
 * A class implementing a 4dof ScapulothoracicJoint.
 * Motion of the scapula is described by an ellipsoid surface fixed
 * to the thorax upon which the joint frame of scapul rides. The motion on
 * the surface is governed by 2 dofs: up-down and medio-lateral described
 * by latitude and longitudinal angles. Scapula rotation about the normal to
 * the ellipsoid surface is the 3rd dof. The 4th dof is a rotation about a 
 * "winging" axis defined by a point and axis direction in the scapula frame.
 *
 * @author Ajay Seth
 */
class OSIMPLUGIN_API ScapulothoracicJoint : public Joint  {
OpenSim_DECLARE_CONCRETE_OBJECT(ScapulothoracicJoint, Joint);

public:
//=============================================================================
// PROPERTIES
//=============================================================================
    OpenSim_DECLARE_PROPERTY(thoracic_ellipsoid_radii_x_y_z, SimTK::Vec3,
        "Radii of the thoracic surface ellipsoid a Vec3(rX, rY, rZ).");

    OpenSim_DECLARE_LIST_PROPERTY_SIZE(scapula_winging_axis_origin, double, 2,
        "Winging axis origin (x,y coordinates) in the scapula plane "
        "(tangent to the thoracic surface).");

    OpenSim_DECLARE_PROPERTY(scapula_winging_axis_direction, double, 
        "Winging axis orientation (in radians) in the scapula plane.");
    /** Indices of Coordinates for use as arguments to getCoordinate() and
    updCoordinate().

    <b>C++ example</b>
    \code{.cpp}
    const auto& r1 = scapuloThoracicJoint.getCoordinate(
                            ScapuloThoracicJoint::Coord::Abduction);
    \endcode
    */
    enum class Coord : unsigned {
        Abduction,
        Elevation,
        UpwardRotation,
        Winging
    };

private:
    /** Specify the Coordinates of the BallJoint. */
    CoordinateIndex rx{ constructCoordinate(Coordinate::MotionType::Rotational,
        static_cast<unsigned>(Coord::Abduction)) };
    CoordinateIndex ry{ constructCoordinate(Coordinate::MotionType::Rotational,
        static_cast<unsigned>(Coord::Elevation)) };
    CoordinateIndex rz{ constructCoordinate(Coordinate::MotionType::Rotational,
        static_cast<unsigned>(Coord::UpwardRotation)) };
    CoordinateIndex ryp{ constructCoordinate(Coordinate::MotionType::Rotational,
        static_cast<unsigned>(Coord::Winging)) };

public:
//=============================================================================
// METHODS
//=============================================================================
    // CONSTRUCTION
    /** Default contructor */
    ScapulothoracicJoint();
    /** Convenience Joint like Constructor */
    ScapulothoracicJoint(const std::string& name,
        const PhysicalFrame& parent,
        const PhysicalFrame& child,
        const SimTK::Vec3& ellipsoidRadii,
        SimTK::Vec2 wingingOrigin,
        double wingingDirection);

    /** Convenience constructor */
    ScapulothoracicJoint(const std::string& name,
        const PhysicalFrame& parent,
        const SimTK::Vec3& locationInParent,
        const SimTK::Vec3& orientationInParent,
        const PhysicalFrame& child,
        const SimTK::Vec3& locationInChild,
        const SimTK::Vec3& orientationInChild,
        const SimTK::Vec3& ellipsoidRadii,
        SimTK::Vec2 wingingOrigin,
        double wingingDirection);

    // default destructor, copy constructor, copy assignment

    /** Exposes getCoordinate() method defined in base class (overloaded below).
    @see Joint */
    using Joint::getCoordinate;

    /** Exposes updCoordinate() method defined in base class (overloaded below).
    @see Joint */
    using Joint::updCoordinate;

    /** Get a const reference to a Coordinate associated with this Joint.
    @see Coord */
    const Coordinate& getCoordinate(Coord idx) const {
        return get_coordinates(static_cast<unsigned>(idx));
    }

    /** Get a writable reference to a Coordinate associated with this Joint.
    @see Coord */
    Coordinate& updCoordinate(Coord idx) {
        return upd_coordinates(static_cast<unsigned>(idx));
    }

    // SCALE
    void extendScale(const SimTK::State& s, const ScaleSet& scaleSet) override;



protected:
    /** ModelComponent Interface */
    void extendAddToSystem(SimTK::MultibodySystem& system) const override;

    // Visual support in SimTK visualizer
/*    void generateDecorations(
        bool fixed,
        const ModelDisplayHints&                    hints,
        const SimTK::State&                         state,
        SimTK::Array_<SimTK::DecorativeGeometry>&   geometryArray) const override;
*/
private:
    void constructProperties();

//=============================================================================
};	// END of class ScapulothoracicJoint
//=============================================================================
//=============================================================================

} // end of namespace OpenSim

#endif // OPENSIM_SCAPULOTHORACIC_JOINT_H_


