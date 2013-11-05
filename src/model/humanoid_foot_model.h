////////////////////////////////////////////////////////////////////////////////
///
///\file humanoid_foot_model.h
///\brief Implement of a foot model
///\author de Gourcuff Martin
///\date 11/07/13
///
////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef MPC_WALKGEN_HUMANOID_FOOT_MODEL_H
#define MPC_WALKGEN_HUMANOID_FOOT_MODEL_H

#include "../tools.h"

namespace MPCWalkgen{
  class HumanoidFootModel
  {
    public:

      /// \brief Structure to store every kinematic limit the foot is
      ///        constraint to.
      struct  KinematicLimits
      {
          KinematicLimits();
          Scalar hipYawUpperBound_;
          Scalar hipYawSpeedUpperBound_;
          Scalar hipYawAccelerationUpperBound_;
          Scalar hipYawLowerBound_;
          Scalar maxHeight_;

          ConvexPolygon kinematicConvexPolygon_;
          ConvexPolygon kinematicConvexPolygonInWorldFrame_;
      };

      HumanoidFootModel(int nbSamples,
                        Scalar samplingPeriod);
      HumanoidFootModel();


      ~HumanoidFootModel();

      /// \brief Set the number of samples for this dynamic
      void setNbSamples(int nbSamples);

      /// \brief Get the number of samples for this dynamic
      inline int getNbSamples() const
      {return nbSamples_;}

      /// \brief Set the sampling period
      void setSamplingPeriod(Scalar samplingPeriod);

      /// \brief Get the sampling period
      inline Scalar getSamplingPeriod() const
      {return samplingPeriod_;}

      //TODO: doc weird
      /// \brief Get the simple support convex polygon in which the CoP
      ///        must remain in the foot frame
      inline const ConvexPolygon& getCopConvexPolygon() const
      {return CopConvexPolygon_;}

      /// \brief Set the simple support convex polygon in the foot frame
      inline void setCopConvexPolygon(const ConvexPolygon& CopConvexPolygon)
      {
        CopConvexPolygon_=CopConvexPolygon;
      }

      /// \brief Set the state of the Foot along the X coordinate
      ///        It is a vector of size 3:
      ///        (Position, Velocity, Acceleration)
      inline void setStateX(const VectorX& state)
      {
        stateX_=state;
      }

      /// \brief Get the state of the Foot along the X coordinate
      inline const VectorX& getStateX() const
      {return stateX_;}

      /// \brief Get the state of the Foot along the Y coordinate
      ///        It is a vector of size 3:
      ///        (Position, Velocity, Acceleration)
      inline void setStateY(const VectorX& state)
      {
        stateY_=state;
      }

      /// \brief Get the state of the Foot along the Y coordinate
      inline const VectorX& getStateY() const
      {return stateY_;}

      /// \brief Get the state of the Foot along the Z coordinate
      ///        It is a vector of size 3:
      ///        (Position, Velocity, Acceleration)
      inline void setStateZ(const VectorX& state)
      {
        stateZ_=state;
      }

      /// \brief Get the state of the Foot along the Z coordinate
      inline const VectorX& getStateZ() const
      {return stateZ_;}

      /// \brief Get the state of the Foot around Yaw axis
      ///        It is a vector of size 3:
      ///        (Position, Velocity, Acceleration)
      inline void setStateYaw(const VectorX& state)
      {
        stateYaw_=state;
      }

      /// \brief Get the state of the Foot around Yaw axis
      inline const VectorX& getStateYaw() const
      {return stateYaw_;}


      /// \brief Each time an update function is called, a 3-polynom spline
      ///        is interpolated between the current state along axis X, Y or Z
      ///        (depending of the update function that was called) and obj. The
      ///        interpolation interval is T, and the update value is computed at t.

      /// \brief Update the state of the Foot along the X coordinate.
      void updateStateX(const Vector3& obj,
                        Scalar T,
                        Scalar t);

      /// \brief Update the state of the Foot along the X coordinate
      void updateStateY(const Vector3& obj,
                        Scalar T,
                        Scalar t);

      /// \brief Update the state of the Foot along the X coordinate
      void updateStateZ(const Vector3& obj,
                        Scalar T,
                        Scalar t);

      /// \brief True if the foot touch the ground at the ith sample
      inline bool isInContact(int nbSample) const
      {
        assert(nbSample>=0 && nbSample<nbSamples_);
        return isInContact_[nbSample];
      }


      /// \brief Setters for the kinematic limits of the foot
      void setHipYawUpperBound(Scalar hipYawUpperBound);
      void setHipYawSpeedUpperBound(Scalar hipYawSpeedUpperBound);
      void setHipYawAccelerationUpperBound(
          Scalar hipYawAccelerationUpperBound);
      void setHipYawLowerBound(Scalar hipYawLowerBound);
      void setMaxHeight(Scalar maxHeight);

      inline Scalar getMaxHeight()
      {return kinematicLimits_.maxHeight_;}

      /// \brief Set the convex polygon of the reachable positions by the foot
      void setKinematicConvexPolygon(const ConvexPolygon& kinematicConvexPolygon);
      /// \brief Get the convex polygon of the reachable positions by the foot
      inline const ConvexPolygon& getKinematicConvexPolygon() const
      {return kinematicLimits_.kinematicConvexPolygon_;}

    private:
      /// \brief Constructors initialization instruction
      void init();
      /// \brief Interpolate trajectory between  currentState and objstate, using interpolator_.
      ///        Then update currentState
      void interpolateTrajectory(VectorX &currentState,
                                 const Vector3& objState,
                                 Scalar T,
                                 Scalar t);

    private:
      int nbSamples_;
      Scalar samplingPeriod_;

      VectorX stateX_;
      VectorX stateY_;
      VectorX stateZ_;
      VectorX stateYaw_;

      KinematicLimits kinematicLimits_;

      ConvexPolygon CopConvexPolygon_;
      ConvexPolygon CopConvexPolygonInWorldFrame_;

      /// \brief Vector of size nbSamples_
      std::vector<bool> isInContact_;

      /// \brief parameters related to foot trajectory interpolation
      Interpolator interpolator_;
      VectorX factor_;
      Vector4 subFactor_;
  };
}

#endif // MPC_WALKGEN_HUMANOID_FOOT_MODEL_H